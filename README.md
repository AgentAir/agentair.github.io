# Shin Infra Lab

ネットワークエンジニアが、Azure、IaC、Network Automation、生成AI活用を学習・検証した内容を整理する個人ブログです。

## 公開URL

https://agentair.github.io/

## 主なテーマ

- Azure / Cloud Network
- Azure Arc / Sentinel / Bastion
- Terraform / IaC
- Network Automation
- ChatGPT / Claude Code / OneNote を使った学習・ナレッジ化

## サイト構成

```
agentair.github.io/
├── index.html        # トップページ
├── articles.html     # 記事一覧（カテゴリ別）
├── about.html        # ブログと運営者について
├── css/
│   └── style.css     # 共通スタイル
└── articles/
    └── *.html        # 記事ファイル
```

## 運用方針

- 実際に検証したこと、詰まったこと、後から見返したいことを記録する
- 公式ドキュメントの代替ではなく、個人の学習・検証ログとして運用する
- 会社情報、顧客情報、実IPアドレス、認証情報、SAS URL、試験問題本文は含めない
- ChatGPTやClaude Codeを活用するが、公開前に自分で内容を確認する

## Codex利用時の基本方針

- 既存のHTML/CSS構造を大きく変えず、手書きの静的サイトとして保守する
- 変更前に編集対象ファイルと変更方針を確認する
- 大規模なリファクタリングやサイト構成変更は行わない
- 記事本文は個人の検証ログ・学習メモとして一般化し、公開前に自分で最終確認する
- 技術的な断定が必要な内容は、公式ドキュメントなどで確認してから公開する

## 記事追加時に更新するファイル

- `articles/YYYY-MM-slug.html`: 新規記事本文
- `articles.html`: カテゴリ別の記事一覧
- `index.html`: 最新記事、必要に応じておすすめ記事
- `README.md`: 運用方針やサイト説明を変更する場合のみ
- `css/style.css`: 既存スタイルで足りない場合のみ最小限

## 公開禁止情報

- 会社情報
- 顧客情報
- 実IP
- 認証情報
- SAS URL
- 試験問題本文

## Codexに任せてよい作業

- 既存記事を参考にした新規記事HTMLの作成
- `articles.html` への記事カード追加
- `index.html` の最新記事・おすすめ記事の更新
- 相対リンク、CSSパス、ナビゲーションの確認
- 見出し構成、文章の読みやすさ、誤字脱字の確認
- 機密情報を含まない形への一般化
- 小さなCSS改善やHTML構文の確認

## Codexに任せない作業

- 公開してよい情報かどうかの最終判断
- 会社情報、顧客情報、実案件構成、実IP、認証情報、SAS URLを含む原稿の直接公開
- 試験問題本文や教材本文に近い内容の作成
- 公式確認が必要な技術情報の未確認の断定
- サイト全体の大規模リファクタリング
- Astro、Hugoなど別構成への移行

## commit前チェック項目

- 編集対象が意図したファイルだけになっていること
- 会社情報、顧客情報、実IP、認証情報、SAS URL、試験問題本文が含まれていないこと
- `index.html` と `articles.html` から新規記事へリンクできること
- 記事ページから `../css/style.css`、トップページ類から `css/style.css` を参照していること
- Home / Articles / About のナビゲーションが崩れていないこと
- ブラウザで主要ページを開き、表示崩れやリンク切れがないこと

## 今後の予定

- 記事の追加
- カテゴリ整理の改善
- 必要に応じてAstroやHugoなどへの移行を検討
