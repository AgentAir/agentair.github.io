Shin infra lab の記事として、以下の会話内容をもとに技術記事を作成してください。

テーマ：
Azure VM以外のシステムで Microsoft Sentinel を使う場合、Azure Arc は本当に必須なのか

記事の目的：
「Azure VM 以外は必ず Azure Arc が必要」と単純に覚えるのではなく、
どのログ収集方式で Sentinel に連携するかによって Azure Arc の要否が変わることを整理する。

今回の学びの流れ：
1. 最初は「Azure VM 以外のシステムで Sentinel を使うには Azure Arc が必須」と理解していた。
2. しかし、Sentinel はログ分析基盤であり、ログを入れる方法は複数あるため、Arc が不要なケースもあるのではないかと疑問を持った。
3. 調べた結果、正確には「非 Azure サーバーを AMA + DCR 方式で Sentinel に直接収集対象として扱う場合は Azure Arc が必要」と理解した。
4. 一方で、Syslog / CEF 転送、Logstash、API、SaaS コネクタ、Microsoft 365 / Defender 系コネクタなど、Arc が不要な方式もある。
5. 最終的に、「Sentinel を使うために Azure Arc が必須」ではなく、「Azure 外のサーバーを Azure 管理リソースとして扱い、AMA + DCR の対象にするために Azure Arc が必要」と理解した。

記事に必ず含める内容：
- Microsoft Sentinel、Log Analytics Workspace、AMA、DCR、Azure Arc の関係
- AMA = Azure Monitor Agent
- DCR = Data Collection Rule
- Azure VM では Azure VM 拡張機能として AMA を導入できるため Arc 不要
- オンプレ、VMware、他クラウド上のサーバーでは Arc で Azure に登録してから AMA + DCR を使う
- Arc が不要なログ連携方式もある

図解：
Azure VM の場合：
Azure VM
  → Azure VM 拡張機能として AMA を導入
  → DCR を関連付け
  → Log Analytics Workspace
  → Microsoft Sentinel
  → Azure Arc は不要

オンプレ / VMware / 他クラウドのサーバーの場合：
オンプレ / VMware / 他クラウドのサーバー
  → Azure Arc で Azure に登録
  → Azure Arc 対応サーバーとして Azure 上にリソース化
  → AMA を導入
  → DCR を関連付け
  → Log Analytics Workspace
  → Microsoft Sentinel

Arc が不要な例：
- Azure VM からの AMA 収集
- SaaS / Microsoft 365 / Defender 系コネクタ
- ネットワーク機器が Syslog / CEF をログフォワーダへ送る構成
- Logstash や Logs Ingestion API を使う構成
- API で直接 Log Analytics Workspace に送信する構成

Arc が必要になりやすい例：
- オンプレ Windows Server のイベントログを AMA + DCR で収集する場合
- VMware 上の Linux / Windows Server を AMA + DCR で収集する場合
- AWS / GCP 上の VM を Azure Monitor Agent で管理する場合
- 非 Azure の Linux サーバーを Syslog / CEF ログフォワーダとして使い、そのフォワーダに AMA を導入する場合

記事のトーン：
- 初学者にもわかるように整理する
- 「最初の誤解 → 疑問 → 調査 → 理解の修正 → 最終的な整理」という学習プロセスを書く
- 単なるナレッジ集ではなく、理解が深まった過程が伝わるようにする
- Shin infra lab の既存記事の雰囲気に合わせる

想定タイトル案：
「Azure VM以外でMicrosoft Sentinelを使うにはAzure Arcが必須なのか整理した」
または
「SentinelとAzure Arcの関係を整理：Arcが必要なケースと不要なケース」

最終結論：
Microsoft Sentinel を使うこと自体に Azure Arc が必須なわけではない。
Azure Arc が必要になるのは、主に Azure 外のサーバーを Azure の管理対象リソースとして登録し、AMA + DCR によってログ収集する場合である。
