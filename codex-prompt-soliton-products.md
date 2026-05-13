Shin infra Lab に、Soliton製品群の理解を整理する記事を追加してください。

今回の記事では、単なる製品紹介ではなく、ChatGPTとの対話を通じて理解が深まっていった過程を重視してください。
特に、最初に誤解・混同していた点、途中で整理し直した点、最終的に落ち着いた理解を、記事の流れとして丁寧に書いてください。

目的:
- FileZen S、Soliton SecureBrowser / SecureGateway、SmartOn、OneGate、NetAttest EPS の違いを、ネットワークエンジニア向けに分かりやすく整理する。
- 製品名を覚えるだけでなく、「何の役割を持つ製品なのか」「何と混同しやすいのか」「最終的にどう理解すればよいのか」を説明する。
- 自治体NW、インターネット分離、認証基盤、RADIUS、MFA、SSO、ファイル受け渡しの文脈で理解できる記事にする。
- 今回の対話で理解に時間がかかった部分は、深めに記載する。

記事タイトル案:
Soliton製品群の役割整理：FileZen S / SecureBrowser / SmartOn / OneGate / NetAttest EPS の違い

記事の書き方:
- 製品ごとの説明だけでなく、「自分は最初こう理解した」「途中でこう修正した」「最終的にはこの理解に落ち着いた」という学習ログ形式を入れる。
- Shin infra Labらしく、ネットワークエンジニアが自分の理解を整理する記事として書く。
- 難しすぎる製品カタログ風ではなく、「何に近いか」「何ではないか」を重視する。
- 必要に応じて簡単なテキスト構成図を入れる。
- 断定しすぎず、公式仕様は確認が必要という注意も入れる。

記事構成:

1. はじめに
- Soliton製品は認証、Web分離、ファイル受け渡し、ネットワーク認証など役割が分かれている。
- 名前だけ見ると混乱しやすいため、用途別に整理する。
- 今回は、最初の理解、途中の修正、最終理解の流れも含めてまとめる。

2. 全体像
- FileZen S: ファイル受け渡しゲートウェイ
- Soliton SecureBrowser: セキュアブラウザ、Webアクセス制御
- Soliton SecureGateway: SecureBrowser用のゲートウェイサーバ/アプライアンス
- SmartOn: WindowsログオンMFA
- OneGate: クラウド型のCA/MFA/SSO/ID認証基盤
- NetAttest EPS: Wi-Fi/VPN/有線LAN向けRADIUS/802.1X/証明書認証基盤

3. FileZen S：FTPサーバか、ファイルサーバか、ファイル受け渡しゲートウェイか
- 最初の理解: FTPサーバのようなものかと思った。
- 途中の理解: FTPよりは、社内公開ストレージやファイルサーバに高級な機能を追加したものに近いと考えた。
- 最終理解: 通常のファイルサーバでもなく、分離ネットワーク間で安全にファイルを受け渡すWeb型ゲートウェイ。
- 承認、ウイルスチェック、無害化連携、ログ、保存期間管理が重要。
- USBメモリ運用の代替として説明する。

4. Soliton SecureBrowser / SecureGateway：Webアプリか、専用ブラウザか、ゲートウェイは何か
- 最初の理解: SecureBrowserはWebアプリケーションのように見えた。
- 途中の理解: SecureGatewayはNW機器なのか、物理アプライアンスなのかを整理した。
- 最終理解: SecureBrowserは端末に入れる専用ブラウザアプリ。SecureGatewayはNW機器というより、DMZ等に置くゲートウェイサーバ/物理・仮想アプライアンス。
- 端末にデータを残さない、コピー/保存/ダウンロードを制御する。
- VDIや画面転送とは違い、端末側で専用ブラウザを起動する考え方。

5. SmartOn：RADIUSサーバではなくWindowsログオンMFA
- 最初の理解: 多要素認証を行うサーバ、つまりRADIUSサーバなのかと考えた。
- 途中の理解: RADIUSという大きな枠の中にNetAttest EPSがあり、その中にSmartOnがあるように誤解しそうになった。
- 最終理解: SmartOnはRADIUS系ではなく、Windows PCログオンに特化したMFA製品。RADIUS系はSolitonではNetAttest EPSが担う。
- ICカード、顔認証、指紋、指静脈などで本人確認する。
- サーバソフト、クライアントソフト、PC台数ライセンス、トークンライセンスなどで導入するイメージ。
- NetAttest EPSとは用途が違う。

6. OneGate：RADIUSサーバか、CA/SSO/MFA基盤か
- 最初の理解: OneGateはクラウド版でRADIUS系もMFA機能も備えた製品なのかと考えた。
- 途中の理解: RADIUSサーバそのものではなく、CA局やMFA、SSOなどユーザーがログインに使用する機能を持つものではないかと整理した。
- さらに、ADサーバのようなものかとも考えたが、ADそのものではなくAD/Entra IDと連携する認証基盤と理解し直した。
- 最終理解: OneGateはクラウド型のCA/MFA/SSO/ID認証基盤。RADIUSサーバそのものではない。
- Wi-Fi/VPN認証にも関係するが、RADIUSの現地受け口はEPS-edge等と連携する理解がよい。
- SmartOnとは機能が一部重なるが、SmartOnはWindowsログオン、OneGateはSaaS/クラウド/Webログインが主戦場。

7. SmartOnとOneGateはなぜ機能がかぶって見えるのか
- どちらも認証製品なので、MFA、証明書、ログイン制御などの用語が重なる。
- しかし中心領域が違う。
- SmartOn: PCを使い始めるときのWindowsログオン認証。
- OneGate: SaaS、クラウド、Webサービスにログインするときの認証基盤。
- 一部重なるが、主戦場が違うと説明する。

8. NetAttest EPS：基本的なRADIUS系を担当
- 最終理解: Wi-Fi、VPN、有線LANなどの基本的なRADIUS系を担当する製品。
- RADIUS、802.1X、EAP-TLS/EAP-PEAP、プライベートCA、証明書認証を扱う。
- AP、無線LANコントローラ、スイッチ、VPN装置からRADIUS認証要求を受ける。
- SmartOnやOneGateと違い、ネットワーク接続認証が主戦場。

9. まとめ表
- 製品名、分類、一言説明、最初に混同しやすいもの、最終理解を表でまとめる。

成果物:
- Shin infra Lab の記事として追加できるHTMLまたはMarkdownを作成する。
- 既存サイト構成に合わせて、記事一覧へのリンク追加も行う。
- 可能なら slug は soliton-products-auth-overview などにする。
- 本記事は、製品説明だけでなく、自分の理解がどう変化したかを残す学習ログとして仕上げる。
