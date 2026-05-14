Shin Infra Lab の記事として、今回の「Ubuntu を Microsoft Sentinel に連携し、Syslog を収集できるようにした検証」を記事化してください。

記事の目的は、単なる手順メモではなく、以下の学びの流れが読者に伝わるようにすることです。

# 記事タイトル案
Ubuntu の Syslog を Azure Arc / AMA / DCR 経由で Microsoft Sentinel に収集する手順

# 記事で必ず書くこと

## 1. 最初の理解
最初は、Ubuntu が Azure Arc に接続されていれば、そのまま Sentinel にログが送られると思っていた。
しかし実際には、Azure Arc に接続しただけでは不十分で、以下が必要だった。

- Azure Arc 対応サーバーとして Azure に登録されていること
- Azure Monitor Agent / AMA が導入されていること
- Data Collection Rule / DCR が対象の Ubuntu Arc マシンに関連付いていること
- DCR の Collect 設定で Syslog の Facility / Severity が設定されていること

## 2. 今回の構成
以下の流れでログを収集したことを説明してください。

Ubuntu
  ↓ Syslog / logger
rsyslog
  ↓
Azure Monitor Agent / AMA
  ↓ DCR
Log Analytics Workspace
  ↓
Microsoft Sentinel
  ↓
Syslog テーブルで確認

## 3. 実施した内容
以下の順番で説明してください。

1. Ubuntu が Azure Arc に接続されていることを確認
2. Sentinel の Data connectors から Syslog via AMA を選択
3. DCR を作成
4. DCR の Resources で Ubuntu の Azure Arc 対応サーバーを選択
5. Ubuntu 側で configchunks に DCR 設定が配布されたことを確認
6. Heartbeat テーブルで AMA の接続を確認
7. logger コマンドで Syslog テストログを生成
8. Syslog テーブルで sentinel-test が見えることを確認

## 4. 途中で詰まったところ
今回、最初に以下のコマンドを実行しても、DCR 設定ファイルが配布されていなかった。

sudo ls -l /etc/opt/microsoft/azuremonitoragent/config-cache/configchunks/

原因は、DCR の Resources に Ubuntu の Azure Arc 対応サーバーを正しく選択していなかったことだった。

その後、DCR の Resources で Azure Arc の Ubuntu マシンを選択したところ、configchunks 配下にファイルが増えた。
これにより、DCR が Ubuntu に配布されたことが確認できた。

## 5. 確認コマンド
以下のコマンドを記事に掲載してください。

sudo azcmagent show

systemctl status azuremonitoragent --no-pager

sudo ls -l /etc/opt/microsoft/azuremonitoragent/config-cache/configchunks/

sudo grep -R -i "syslog" /etc/opt/microsoft/azuremonitoragent/config-cache/configchunks/

sudo ls -l /etc/rsyslog.d/

sudo ls -l /etc/rsyslog.d/*azuremonitoragent*

logger -p authpriv.err "sentinel-test ubuntu authpriv err $(hostname) $(date -Is)"
logger -p daemon.warning "sentinel-test ubuntu daemon warning $(hostname) $(date -Is)"
logger -p user.info "sentinel-test ubuntu user info $(hostname) $(date -Is)"

## 6. KQL
以下の KQL を記事に掲載してください。

Heartbeat
| where TimeGenerated > ago(24h)
| where Category == "Azure Monitor Agent"
| project TimeGenerated, Computer, OSType, Category
| order by TimeGenerated desc

Syslog
| where TimeGenerated > ago(24h)
| where SyslogMessage contains "sentinel-test"
| project TimeGenerated, Computer, HostName, Facility, SeverityLevel, ProcessName, SyslogMessage
| order by TimeGenerated desc

Syslog
| where TimeGenerated > ago(24h)
| summarize count() by Computer, HostName, Facility, SeverityLevel
| order by count_ desc

## 7. 最終的に理解したこと
以下の理解に落ち着いたことを明記してください。

- Heartbeat が見える場合、AMA 自体は動作している
- configchunks にファイルがある場合、DCR は Ubuntu に配布されている
- Syslog テーブルに sentinel-test が出れば、Ubuntu の Syslog 収集は成功
- Syslog はすぐ表示されない場合があるため、数分待つ必要がある
- 初回確認では ago(1h) より ago(24h) のように広めに見る方がよい
- Azure Arc 接続だけでは Sentinel にログは送られず、AMA と DCR の関連付けが重要

## 8. 記事のトーン
初心者が同じ検証を再現できるように、以下の点を意識してください。

- Azure Arc、AMA、DCR、Sentinel の役割を分けて説明する
- 「なぜ configchunks を見るのか」を説明する
- 「なぜ Heartbeat は見えるのに Syslog が見えないことがあるのか」を説明する
- 今回の詰まりポイントを、学びとして丁寧に書く
- 単なるコマンド集ではなく、理解の変化が分かる記事にする

## 9. 出力先
Shin Infra Lab の記事として、既存の記事構成に合わせて HTML または Markdown を作成してください。
記事一覧ページがある場合は、今回の記事へのリンクも追加してください。
