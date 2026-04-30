[1mdiff --git a/articles.html b/articles.html[m
[1mindex 50b7e77..5b252fa 100644[m
[1m--- a/articles.html[m
[1m+++ b/articles.html[m
[36m@@ -21,6 +21,22 @@[m
   <main class="container">[m
     <section>[m
       <h2>記事一覧</h2>[m
[32m+[m[32m      <article class="card">[m
[32m+[m[32m        <p>[m
[32m+[m[32m          <span class="tag">Terraform</span>[m
[32m+[m[32m          <span class="tag">Azure</span>[m
[32m+[m[32m          <span class="tag">VNet</span>[m
[32m+[m[32m        </p>[m
[32m+[m[32m        <h3>[m
[32m+[m[32m          <a href="articles/2026-04-terraform-azure-vnet-before-build.html">[m
[32m+[m[32m            TerraformでAzure VNetを作る前に理解したいこと[m
[32m+[m[32m          </a>[m
[32m+[m[32m        </h3>[m
[32m+[m[32m        <p>[m
[32m+[m[32m          TerraformでAzure VNetを作成する前に、Resource Group、Address Space、Subnet、NSG、Route Tableなど、設計時に理解しておきたい要素を整理します。[m
[32m+[m[32m        </p>[m
[32m+[m[32m      </article>[m
[32m+[m
       <article class="card">[m
         <p>[m
           <span class="tag">Terraform</span>[m
[1mdiff --git a/articles/2026-04-terraform-azure-vnet-before-build.html b/articles/2026-04-terraform-azure-vnet-before-build.html[m
[1mnew file mode 100644[m
[1mindex 0000000..5221483[m
[1m--- /dev/null[m
[1m+++ b/articles/2026-04-terraform-azure-vnet-before-build.html[m
[36m@@ -0,0 +1,241 @@[m
[32m+[m[32m<!DOCTYPE html>[m
[32m+[m[32m<html lang="ja">[m
[32m+[m[32m<head>[m
[32m+[m[32m  <meta charset="UTF-8">[m
[32m+[m[32m  <meta name="viewport" content="width=device-width, initial-scale=1.0">[m
[32m+[m[32m  <title>TerraformでAzure VNetを作る前に理解したいこと - Shin Infra Lab</title>[m
[32m+[m[32m  <link rel="stylesheet" href="../css/style.css">[m
[32m+[m[32m</head>[m
[32m+[m[32m<body>[m
[32m+[m[32m  <header class="site-header">[m
[32m+[m[32m    <h1>TerraformでAzure VNetを作る前に理解したいこと</h1>[m
[32m+[m[32m    <p>2026-04-30 / Terraform / Azure / VNet</p>[m
[32m+[m[32m  </header>[m
[32m+[m
[32m+[m[32m  <nav class="nav">[m
[32m+[m[32m    <a href="../index.html">Home</a>[m
[32m+[m[32m    <a href="../articles.html">Articles</a>[m
[32m+[m[32m    <a href="../about.html">About</a>[m
[32m+[m[32m  </nav>[m
[32m+[m
[32m+[m[32m  <main class="container">[m
[32m+[m[32m    <section>[m
[32m+[m[32m      <h2>はじめに</h2>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        前回の記事「Terraformとは何かをネットワークエンジニア目線で整理する」では、[m
[32m+[m[32m        provider・resource・state・plan・apply という基本概念を整理しました。[m
[32m+[m[32m      </p>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        今回はその続きとして、TerraformでAzure VNetを実際に作る前に理解しておきたいことを整理します。[m
[32m+[m[32m        コードを書き始める前に、どんな要素を設計・把握しておくべきかを確認するのが目的です。[m
[32m+[m[32m      </p>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        本記事は公式ドキュメントの代替ではなく、ネットワークエンジニアとしての視点で整理した個人の学習メモです。[m
[32m+[m[32m      </p>[m
[32m+[m[32m    </section>[m
[32m+[m
[32m+[m[32m    <section>[m
[32m+[m[32m      <h2>TerraformでAzureを操作するにはProviderが必要</h2>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        TerraformがAzureのリソースを作成・変更・削除するには、<code>azurerm</code> プロバイダーが必要です。[m
[32m+[m[32m        プロバイダーはTerraformとAzureの間の接続口のような役割を担っており、[m
[32m+[m[32m        Azure Resource Manager APIを通じてリソースを操作します。[m
[32m+[m[32m      </p>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        ネットワーク機器の運用に例えると、SSHで機器に接続するための接続設定（IPアドレス、認証情報、プロトコル）を[m
[32m+[m[32m        事前に定義するのに近いイメージです。[m
[32m+[m[32m        Terraformでもプロバイダー設定をコードに書いておくことで、どのAzureテナント・サブスクリプションに対して操作を行うかを明示します。[m
[32m+[m[32m      </p>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        認証方法はいくつかあります（Azure CLIログイン、サービスプリンシパルなど）が、[m
[32m+[m[32m        まず個人検証環境で試す場合はAzure CLIで事前にログインしておく方法が手軽です。[m
[32m+[m[32m        認証情報やシークレットをコードに直接書くのは避け、環境変数や外部ファイルで管理するのが原則です。[m
[32m+[m[32m      </p>[m
[32m+[m[32m    </section>[m
[32m+[m
[32m+[m[32m    <section>[m
[32m+[m[32m      <h2>VNet作成時に考える要素</h2>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        TerraformでAzure VNetを作成する場合、コードに定義が必要な要素を事前に把握しておくことが重要です。[m
[32m+[m[32m        主な要素を整理します。[m
[32m+[m[32m      </p>[m
[32m+[m
[32m+[m[32m      <h3>Resource Group（リソースグループ）</h3>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        AzureのリソースはすべてResource Groupに属します。VNetを作る前に、Resource Groupが存在している必要があります。[m
[32m+[m[32m        Terraformでは、Resource GroupもVNetも同じコードで定義でき、作成順の依存関係は自動で解決されます。[m
[32m+[m[32m      </p>[m
[32m+[m
[32m+[m[32m      <h3>Location（リージョン）</h3>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        リソースをデプロイするAzureのリージョンです（例：Japan East）。[m
[32m+[m[32m        Resource GroupとVNetは同じLocationに配置するのが基本です。[m
[32m+[m[32m      </p>[m
[32m+[m
[32m+[m[32m      <h3>VNet Address Space（アドレス空間）</h3>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        VNet全体のIPアドレス範囲をCIDR形式で指定します。[m
[32m+[m[32m        オンプレミスや他のVNetと重複しないよう、事前にIPアドレス設計をしておく必要があります。[m
[32m+[m[32m      </p>[m
[32m+[m
[32m+[m[32m      <h3>Subnet Address Prefix（サブネット）</h3>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        VNet内をさらに分割した単位です。用途（アプリケーション、管理、Bastion、Private Endpointなど）ごとにサブネットを分けるのが一般的です。[m
[32m+[m[32m        オンプレミスのVLAN分割の感覚に近い部分があります。[m
[32m+[m[32m      </p>[m
[32m+[m
[32m+[m[32m      <h3>NSG（Network Security Group）</h3>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        サブネットまたはNICに関連付けるアクセス制御リストです。[m
[32m+[m[32m        オンプレミスのACLに近い役割を持ちますが、優先度（Priority）と方向（Inbound/Outbound）をセットで定義する点が異なります。[m
[32m+[m[32m        Terraformでは、NSGの定義とサブネットへの関連付けをそれぞれ別のresourceブロックで書きます。[m
[32m+[m[32m      </p>[m
[32m+[m
[32m+[m[32m      <h3>Route Table（ルートテーブル）</h3>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        サブネット内のトラフィックのルーティングを制御します。[m
[32m+[m[32m        既定ではAzureのシステムルートが適用されますが、NVAやVPN Gateway経由のルーティングを強制したい場合に追加します。[m
[32m+[m[32m        オンプレミスのスタティックルートの設定に近い感覚です。[m
[32m+[m[32m      </p>[m
[32m+[m
[32m+[m[32m      <h3>Tags（タグ）</h3>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        リソースに付与するメタデータです。環境（dev/prod）、担当者、プロジェクト名などを記録しておくと、[m
[32m+[m[32m        リソースの棚卸しやコスト管理に役立ちます。[m
[32m+[m[32m        Terraformでは全リソースのタグをコードで一元管理できます。[m
[32m+[m[32m      </p>[m
[32m+[m[32m    </section>[m
[32m+[m
[32m+[m[32m    <section>[m
[32m+[m[32m      <h2>オンプレミスNWとの考え方の違い</h2>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        ネットワークエンジニアとしてAzureのVNetを理解するとき、オンプレミスとの類比は助けになりますが、[m
[32m+[m[32m        完全に同じではない部分もあります。[m
[32m+[m[32m      </p>[m
[32m+[m[32m      <ul>[m
[32m+[m[32m        <li>[m
[32m+[m[32m          <strong>物理機器への設定投入ではない：</strong>[m
[32m+[m[32m          Terraformで書くのはAzure上の論理リソースです。[m
[32m+[m[32m          スイッチやルーターのCLIにコマンドを投入するのではなく、クラウド上のAPIを通じてリソースを定義します。[m
[32m+[m[32m        </li>[m
[32m+[m[32m        <li>[m
[32m+[m[32m          <strong>VLANに近い感覚があるSubnetだが仕組みは異なる：</strong>[m
[32m+[m[32m          SubnetはVNet内の論理分割ですが、オンプレミスのVLANとは違い、物理NICや802.1Qタギングとは無関係です。[m
[32m+[m[32m          Subnetはセキュリティ（NSG）や通信制御（Route Table）の適用単位として機能します。[m
[32m+[m[32m        </li>[m
[32m+[m[32m        <li>[m
[32m+[m[32m          <strong>ACLとNSGは似ているが優先度ベース：</strong>[m
[32m+[m[32m          NSGはオンプレミスのACLに近い役割を持ちますが、許可・拒否のルールを優先度の数値（小さいほど優先）で管理します。[m
[32m+[m[32m          Denyルールが下位にある場合、Allowルールが先に適用されて通信が許可される点に注意が必要です。[m
[32m+[m[32m        </li>[m
[32m+[m[32m        <li>[m
[32m+[m[32m          <strong>VNet/Subnet/NSG/Route Tableはそれぞれ独立したリソース：</strong>[m
[32m+[m[32m          Terraformでは、これらの関連付けもコードで明示的に定義します。[m
[32m+[m[32m          NSGをSubnetに関連付けるには別途 <code>azurerm_subnet_network_security_group_association</code> というリソースが必要です。[m
[32m+[m[32m        </li>[m
[32m+[m[32m      </ul>[m
[32m+[m[32m    </section>[m
[32m+[m
[32m+[m[32m    <section>[m
[32m+[m[32m      <h2>Terraformで書く前に設計しておくべきこと</h2>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        コードを書き始める前に、以下の点を整理しておくと、修正の手戻りが減ります。[m
[32m+[m[32m      </p>[m
[32m+[m
[32m+[m[32m      <h3>IPアドレス設計</h3>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        VNetのアドレス空間と各サブネットのCIDRを事前に決めておきます。[m
[32m+[m[32m        後から変更するとリソースの再作成が必要になる場合があるため、最初にまとめて設計するのが望ましいです。[m
[32m+[m[32m      </p>[m
[32m+[m
[32m+[m[32m      <h3>サブネット分割</h3>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        用途ごとにサブネットを分けておくと、NSGやRoute Tableの適用範囲が明確になります。[m
[32m+[m[32m        例えば、Bastion専用サブネット（AzureBastionSubnet）は名前が固定されているなど、Azureのサービスによって制約がある場合もあります。[m
[32m+[m[32m      </p>[m
[32m+[m
[32m+[m[32m      <h3>通信制御の方針</h3>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        どのサブネット間の通信を許可・拒否するか、インターネットアクセスの要否、オンプレミスとの接続が必要かどうかを整理しておきます。[m
[32m+[m[32m        これがNSGのルール設計に直結します。[m
[32m+[m[32m      </p>[m
[32m+[m
[32m+[m[32m      <h3>命名規則</h3>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        Resource Group、VNet、Subnet、NSGなどのリソース名を統一しておきます。[m
[32m+[m[32m        後から命名を変えるとTerraformでリソースの再作成が発生する場合があるため、最初に決めておくことが重要です。[m
[32m+[m[32m      </p>[m
[32m+[m
[32m+[m[32m      <h3>タグ設計</h3>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        環境（dev/staging/prod）やプロジェクト名などのタグキーを事前に決めておくと、コード全体で統一したタグを付与できます。[m
[32m+[m[32m        Terraformでは変数や locals を使ってタグをまとめて管理できます。[m
[32m+[m[32m      </p>[m
[32m+[m
[32m+[m[32m      <h3>将来的な拡張性</h3>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        後からPrivate EndpointやVPN Gatewayを追加する可能性がある場合、それらに必要なサブネットのCIDRをあらかじめ確保しておくと、[m
[32m+[m[32m        後の設計変更を最小限にできます。[m
[32m+[m[32m      </p>[m
[32m+[m[32m    </section>[m
[32m+[m
[32m+[m[32m    <section>[m
[32m+[m[32m      <h2>plan / apply の重要性</h2>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        Terraformを使う大きなメリットの一つが、<code>terraform plan</code> によって変更内容を事前に確認できることです。[m
[32m+[m[32m      </p>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        <code>terraform plan</code> を実行すると、作成・変更・削除されるリソースの一覧が表示されます。[m
[32m+[m[32m        手作業でポータルから変更する場合と比べて、変更の意図と実際の差分を突き合わせやすくなります。[m
[32m+[m[32m        ネットワーク変更で言えば、コマンド投入前の「変更前後の比較確認」に相当するステップです。[m
[32m+[m[32m      </p>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        <code>terraform apply</code> は plan の内容を実際に適用します。[m
[32m+[m[32m        確認プロンプトで <code>yes</code> を入力するまで変更は走らないため、誤操作のリスクを減らせます。[m
[32m+[m[32m      </p>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        特に注意が必要なのは、変更内容によってはリソースの再作成（削除→新規作成）が発生する場合がある点です。[m
[32m+[m[32m        plan の出力で <code>-/+</code> と表示されるものはその対象です。[m
[32m+[m[32m        IPアドレスや名前など変更不可な属性を変えた場合に発生しやすいため、plan の確認を習慣化することが重要です。[m
[32m+[m[32m      </p>[m
[32m+[m[32m    </section>[m
[32m+[m
[32m+[m[32m    <section>[m
[32m+[m[32m      <h2>今後やりたいこと</h2>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        前提知識の整理ができたので、今後は実際に手を動かして検証を進める予定です。[m
[32m+[m[32m        ステップとして以下を想定しています。[m
[32m+[m[32m      </p>[m
[32m+[m[32m      <ul>[m
[32m+[m[32m        <li>TerraformでResource Groupを作成する</li>[m
[32m+[m[32m        <li>TerraformでVNetとSubnetを作成する</li>[m
[32m+[m[32m        <li>NSGを定義してSubnetに関連付ける</li>[m
[32m+[m[32m        <li>Route Tableを定義して通信経路を制御する</li>[m
[32m+[m[32m        <li>Azure BastionやPrivate Endpoint構成へ発展させる</li>[m
[32m+[m[32m      </ul>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        それぞれの検証結果は別記事としてまとめていきます。[m
[32m+[m[32m      </p>[m
[32m+[m[32m    </section>[m
[32m+[m
[32m+[m[32m    <section>[m
[32m+[m[32m      <h2>まとめ</h2>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        TerraformでAzure VNetを作成する前に、Resource Group、Location、Address Space、Subnet、NSG、Route Table、Tagsという[m
[32m+[m[32m        構成要素を整理しておくことが重要です。[m
[32m+[m[32m      </p>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        オンプレミスのネットワーク設計と考え方が近い部分はありますが、物理機器への設定投入とは異なるアプローチのため、[m
[32m+[m[32m        クラウド特有のリソースの定義方法や関連付けの仕組みを理解してからコードを書き始めるのがよいと感じました。[m
[32m+[m[32m      </p>[m
[32m+[m[32m      <p>[m
[32m+[m[32m        plan / apply の流れを活用しながら、実際に手を動かして検証を進めていきます。[m
[32m+[m[32m      </p>[m
[32m+[m[32m    </section>[m
[32m+[m[32m  </main>[m
[32m+[m
[32m+[m[32m  <footer class="site-footer">[m
[32m+[m[32m    <p>&copy; 2026 Shin Infra Lab</p>[m
[32m+[m[32m  </footer>[m
[32m+[m[32m</body>[m
[32m+[m[32m</html>[m
[1mdiff --git a/index.html b/index.html[m
[1mindex 6e9eb12..8faa0f5 100644[m
[1m--- a/index.html[m
[1m+++ b/index.html[m
[36m@@ -32,6 +32,22 @@[m
 [m
     <section>[m
       <h2>最新記事</h2>[m
[32m+[m[32m      <article class="card">[m
[32m+[m[32m        <p>[m
[32m+[m[32m          <span class="tag">Terraform</span>[m
[32m+[m[32m          <span class="tag">Azure</span>[m
[32m+[m[32m          <span class="tag">VNet</span>[m
[32m+[m[32m        </p>[m
[32m+[m[32m        <h3>[m
[32m+[m[32m          <a href="articles/2026-04-terraform-azure-vnet-before-build.html">[m
[32m+[m[32m            TerraformでAzure VNetを作る前に理解したいこと[m
[32m+[m[32m          </a>[m
[32m+[m[32m        </h3>[m
[32m+[m[32m        <p>[m
[32m+[m[32m          TerraformでAzure VNetを作成する前に、Resource Group、Address Space、Subnet、NSG、Route Tableなど、設計時に理解しておきたい要素を整理します。[m
[32m+[m[32m        </p>[m
[32m+[m[32m      </article>[m
[32m+[m
       <article class="card">[m
         <p>[m
           <span class="tag">Terraform</span>[m
