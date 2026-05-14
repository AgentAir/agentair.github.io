$ErrorActionPreference = "Stop"

$RepoPath = "C:\Users\shin\Documents\GitHub\agentair.github.io"
$CodexScript = Join-Path $RepoPath "scripts\Start-CodexFromClipboard.ps1"

Set-Location $RepoPath

$start = Get-Date
$status = "成功"
$message = ""

try {
    Write-Host "Codexを開始します..."
    & $CodexScript

    if ($LASTEXITCODE -ne $null -and $LASTEXITCODE -ne 0) {
        $status = "失敗"
        $message = "Codex が終了コード $LASTEXITCODE で終了しました。"
    } else {
        $message = "Codex の実行が完了しました。"
    }
}
catch {
    $status = "失敗"
    $message = $_.Exception.Message
}
finally {
    $end = Get-Date
    $duration = New-TimeSpan -Start $start -End $end

    $title = "Codex実行完了: $status"
    $body = @"
リポジトリ: agentair.github.io
結果: $status
開始: $($start.ToString("yyyy/MM/dd HH:mm:ss"))
終了: $($end.ToString("yyyy/MM/dd HH:mm:ss"))
所要時間: $($duration.ToString("hh\:mm\:ss"))

$message
"@

    Write-Host ""
    Write-Host "==== Codex 実行結果 ===="
    Write-Host $body

    try {
        [console]::beep(880, 500)
        Start-Sleep -Milliseconds 200
        [console]::beep(660, 500)
    } catch {}

    try {
        $wshell = New-Object -ComObject WScript.Shell
        $null = $wshell.Popup($body, 20, $title, 64)
    } catch {
        Write-Host "Windowsポップアップ通知に失敗しました。"
    }

    if ($status -eq "失敗") {
        exit 1
    }
}
