$ErrorActionPreference = "Stop"

# =========================
# Settings
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
    if (!(Test-Path $RepoPath)) {
        throw "Repository not found: $RepoPath"
    }

    if (!(Test-Path $InboxDir)) {
        New-Item -ItemType Directory -Path $InboxDir | Out-Null
    }

    if (!(Test-Path $LogDir)) {
        New-Item -ItemType Directory -Path $LogDir | Out-Null
    }

    Start-Transcript -Path $LogFile -Append | Out-Null

    Write-Host "=== Codex article automation started ==="
    Write-Host "RepoPath: $RepoPath"
    Write-Host "PromptFile: $PromptFile"
    Write-Host "LogFile: $LogFile"
    Write-Host ""

    Write-Host "=== Checking commands ==="

    if (!(Get-Command git -ErrorAction SilentlyContinue)) {
        throw "git command was not found."
    }

    if (!(Get-Command codex -ErrorAction SilentlyContinue)) {
        throw "codex command was not found. Please check Codex CLI installation."
    }

    if (!(Get-Command code -ErrorAction SilentlyContinue)) {
        Write-Host "Warning: code command was not found. VS Code open step will be skipped."
        $CanOpenVSCode = $false
    } else {
        $CanOpenVSCode = $true
    }

    git --version
    codex --version
    Write-Host ""

    Write-Host "=== Reading clipboard ==="

    $prompt = Get-Clipboard -Raw

    if ([string]::IsNullOrWhiteSpace($prompt)) {
        throw "Clipboard is empty. Copy the Codex request text from ChatGPT and run again."
    }

    if ($prompt -notmatch "Codex|Shin infra Lab|記事|依頼|作業ルール") {
        Write-Host "Clipboard content does not look like a Codex request."
        Write-Host "First 200 characters:"
        Write-Host $prompt.Substring(0, [Math]::Min(200, $prompt.Length))
        throw "Codex request validation failed."
    }

    Set-Content -Path $PromptFile -Value $prompt -Encoding UTF8

    Write-Host "Saved Codex request:"
    Write-Host $PromptFile
    Write-Host ""

    cd $RepoPath

    Write-Host "=== Git status before work ==="
    git status

    $gitStatus = git status --porcelain
    if (![string]::IsNullOrWhiteSpace($gitStatus)) {
        Write-Host ""
        Write-Host "Uncommitted changes were found. Stopping for safety."
        Write-Host $gitStatus
        throw "Stopped because the working tree is not clean."
    }

    Write-Host ""
    Write-Host "=== Updating main ==="
    git checkout main
    git pull

    Write-Host ""
    Write-Host "=== Creating work branch ==="

    $branchExists = git branch --list $BranchName
    if ($branchExists) {
        git checkout $BranchName
    } else {
        git checkout -b $BranchName
    }

    $codexInstruction = @"
Please create a Shin infra Lab article based on the following Codex request.

Important rules:
- Do not commit.
- Do not push.
- Do not include personal information.
- Do not include subscription IDs, tenant IDs, IP addresses, tokens, SAS URLs, private keys, or secrets.
- If something is unclear, leave it as TODO instead of guessing.
- Follow the existing HTML/CSS structure.
- Add a link to articles.html.
- Include not only the technical conclusion, but also the learning process.
- Include initial misunderstanding, questions during the discussion, and final understanding.
- At the end, summarize changed files and points the human should review.

Saved request file:
$PromptFile

--- Codex request begins ---

$prompt

--- Codex request ends ---
"@

    Write-Host ""
    Write-Host "=== Running Codex ==="

    codex exec --sandbox workspace-write $codexInstruction

    Write-Host ""
    Write-Host "=== Codex finished ==="

    Write-Host ""
    Write-Host "=== Git changes ==="
    git status
    git diff --stat

    Write-Host ""
    Write-Host "=== Simple secret-like string check ==="

    $secretResult = Select-String -Path .\articles\*.html, .\*.html, .\assets\js\*.js -Pattern `
    "subscription|tenant|client_secret|password|passwd|token|apikey|api_key|secret|Bearer|sas|sig=|SharedAccessSignature|BEGIN PRIVATE KEY|BEGIN RSA PRIVATE KEY|tenantId|subscriptionId" `
    -CaseSensitive:$false `
    -ErrorAction SilentlyContinue

    if ($secretResult) {
        $secretResult
    } else {
        Write-Host "No matches found by simple check."
    }

    Write-Host ""
    Write-Host "=== Next manual steps ==="
    Write-Host "1. Run: git diff"
    Write-Host "2. Check article accuracy and secrets."
    Write-Host "3. If OK, run git add / commit / push."
    Write-Host ""
    Write-Host "Saved Codex request:"
    Write-Host $PromptFile

    if ($CanOpenVSCode) {
        code $RepoPath
    }
}
catch {
    Write-Host ""
    Write-Host "=== Stopped with error ===" -ForegroundColor Red
    Write-Host $_.Exception.Message -ForegroundColor Red
    Write-Host ""
    Write-Host "Log file:"
    Write-Host $LogFile
}
finally {
    try {
        Stop-Transcript | Out-Null
    } catch {
    }

    Write-Host ""
    Write-Host "Press Enter to close."
    Read-Host
}
