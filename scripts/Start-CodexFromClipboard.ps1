# C:\Users\shin\Documents\GitHub\agentair.github.io\scripts\Start-CodexFromClipboard.ps1

$ErrorActionPreference = "Stop"

# =========================
# 設定
# =========================
$RepoPath = "$env:USERPROFILE\Documents\GitHub\agentair.github.io"
$InboxDir = Join-Path $RepoPath "codex-inbox"
$LogDir = Join-Path $RepoPath "codex-logs"

$Today = Get-Date -Format "yyyy-MM-dd"
$Now = Get-Date -Format "yyyy-MM-dd-HHmmss"

$PromptFile = Join-Path $InboxDir "$Now-codex-request.md"
$LogFile = Join-Path $LogDir "$Now-codex-run.log"

$BranchName = "feature/article-$Today"

try {
    # =========================
    # フォルダ作成
    # =========================
    if (!(Test-Path $RepoPath)) {
        throw "リポジトリが見つかりません: $RepoPath"
    }

    if (!(Test-Path $InboxDir)) {
        New-Item -ItemType Directory -Path $InboxDir | Out-Null
    }

    if (!(Test-Path $LogDir)) {
        New-Item -ItemType Directory -Path $LogDir | Out-Null
    }

    Start-Transcript -Path $LogFile -Append | Out-Null

    Write-Host "=== Codex自動記事化スクリプト開始 ==="
    Write-Host "RepoPath: $RepoPath"
    Write-Host "PromptFile: $PromptFile"
    Write-Host "LogFile: $LogFile"
    Write-Host ""

    # =========================
    # コマンド存在確認
    # =========================
    Write-Host "=== コマンド存在確認 ==="

    if (!(Get-Command git -ErrorAction SilentlyContinue)) {
        throw "git コマンドが見つかりません。"
    }

    if (!(Get-Command codex -ErrorAction SilentlyContinue)) {
        throw "codex コマンドが見つかりません。Codex CLIがインストールされているか確認してください。"
    }

    if (!(Get-Command code -ErrorAction SilentlyContinue)) {
        Write-Host "警告: code コマンドが見つかりません。最後にVS Codeを開く処理はスキップします。"
        $CanOpenVSCode = $false
    } else {
        $CanOpenVSCode = $true
    }

    git --version
    codex --version
    Write-Host ""

    # =========================
    # クリップボードからCodex依頼文を取得
    # =========================
    Write-Host "=== クリップボード確認 ==="

    $prompt = Get-Clipboard -Raw

    if ([string]::IsNullOrWhiteSpace($prompt)) {
        throw "クリップボードが空です。ChatGPTで作成したCodex依頼文をコピーしてから再実行してください。"
    }

    if ($prompt -notmatch "Codex|Shin infra Lab|記事|依頼|作業ルール") {
        Write-Host "クリップボードの内容がCodex依頼文ではなさそうです。"
        Write-Host "先頭200文字:"
        Write-Host $prompt.Substring(0, [Math]::Min(200, $prompt.Length))
        throw "Codex依頼文チェックに失敗しました。"
    }

    Set-Content -Path $PromptFile -Value $prompt -Encoding UTF8

    Write-Host "Codex依頼文を保存しました。"
    Write-Host $PromptFile
    Write-Host ""

    # =========================
    # Git準備
    # =========================
    cd $RepoPath

    Write-Host "=== 作業前 git status ==="
    git status

    $gitStatus = git status --porcelain
    if (![string]::IsNullOrWhiteSpace($gitStatus)) {
        Write-Host ""
        Write-Host "未コミット変更があります。安全のため停止します。"
        Write-Host $gitStatus
        throw "未コミット変更があるため停止しました。"
    }

    Write-Host ""
    Write-Host "=== mainを最新化 ==="
    git checkout main
    git pull

    Write-Host ""
    Write-Host "=== 作業ブランチ作成 ==="

    $branchExists = git branch --list $BranchName
    if ($branchExists) {
        git checkout $BranchName
    } else {
        git checkout -b $BranchName
    }

    # =========================
    # Codexに渡す最終指示
    # =========================
    $codexInstruction = @"
以下のCodex依頼文に従って、Shin infra Labの記事を作成してください。

重要ルール:
- commit はしない
- push はしない
- 個人情報、サブスクリプションID、テナントID、IPアドレス、トークン、SAS URL、秘密鍵は記事に含めない
- 不明点は推測せず TODO として残す
- 既存のHTML/CSS構成に合わせる
- articles.html に記事リンクを追加する
- 技術的な結論だけでなく、理解の変化も記事に含める
- 最後に変更点と人間が確認すべき点を要約する

保存済み依頼文:
$PromptFile

--- Codex依頼文ここから ---

$prompt

--- Codex依頼文ここまで ---
"@

    # =========================
    # Codex実行
    # =========================
    Write-Host ""
    Write-Host "=== Codex実行開始 ==="

    codex exec --sandbox workspace-write $codexInstruction

    Write-Host ""
    Write-Host "=== Codex実行完了 ==="

    # =========================
    # 結果確認
    # =========================
    Write-Host ""
    Write-Host "=== 変更状況 ==="
    git status
    git diff --stat

    Write-Host ""
    Write-Host "=== 秘密情報っぽい文字列チェック ==="

    $secretResult = Select-String -Path .\articles\*.html, .\*.html, .\assets\js\*.js -Pattern `
    "subscription|tenant|client_secret|password|passwd|token|apikey|api_key|secret|Bearer|sas|sig=|SharedAccessSignature|BEGIN PRIVATE KEY|BEGIN RSA PRIVATE KEY|tenantId|subscriptionId" `
    -CaseSensitive:$false `
    -ErrorAction SilentlyContinue

    if ($secretResult) {
        $secretResult
    } else {
        Write-Host "簡易チェックでは該当なし"
    }

    Write-Host ""
    Write-Host "=== 次に人間がやること ==="
    Write-Host "1. git diff を確認"
    Write-Host "2. 秘密情報がないか確認"
    Write-Host "3. 問題なければ git add / commit / push"
    Write-Host ""
    Write-Host "保存したCodex依頼文:"
    Write-Host $PromptFile

    if ($CanOpenVSCode) {
        code $RepoPath
    }
}
catch {
    Write-Host ""
    Write-Host "=== エラーで停止しました ===" -ForegroundColor Red
    Write-Host $_.Exception.Message -ForegroundColor Red
    Write-Host ""
    Write-Host "ログファイル:"
    Write-Host $LogFile
}
finally {
    try {
        Stop-Transcript | Out-Null
    } catch {
    }

    Write-Host ""
    Write-Host "Enterキーを押すまで閉じません。"
    Read-Host
}