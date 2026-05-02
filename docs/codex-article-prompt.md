# Codex記事作成依頼プロンプト

以下の内容に従って、GitHub Pagesブログの記事を1本作成してください。

## 前提

- このリポジトリは、GitHub Pagesで公開している手書きHTML/CSSの個人ブログです。
- 既存のHTML/CSS構造を大きく変えずに保守してください。
- 新規記事は `articles/template.html` をベースに作成してください。
- CSSは原則変更しないでください。
- 大規模な仕組み化やリファクタリングは行わないでください。

## 記事情報

- 記事タイトル: `{{TITLE}}`
- 公開日: `{{DATE}}`
- 作成ファイル名: `articles/{{YYYY-MM-SLUG}}.html`
- カテゴリ: `{{CATEGORY}}`
- タグ: `{{TAG_1}}`, `{{TAG_2}}`, `{{TAG_3}}`

## 記事の目的

{{ARTICLE_PURPOSE}}

## 入れたい内容

- {{CONTENT_ITEM_1}}
- {{CONTENT_ITEM_2}}
- {{CONTENT_ITEM_3}}
- {{CONTENT_ITEM_4}}
- {{CONTENT_ITEM_5}}

## 入れてはいけない情報

- 会社情報
- 顧客情報
- 実IP
- 認証情報
- SAS URL
- 試験問題本文
- 実案件の構成や固有名詞
- 公開してよいか判断できないログ、コマンド出力、エラー全文

## 編集してよいファイル

- `articles/{{YYYY-MM-SLUG}}.html`
- `articles.html`
- `index.html`

## 編集してはいけないファイル

- `css/style.css`
- `about.html`
- `README.md`
- `articles/template.html`
- その他、上記の「編集してよいファイル」に含まれないファイル

## 作業ルール

- `articles/template.html` の構造を流用して、新規記事HTMLを作成してください。
- 記事ページのCSS参照は `../css/style.css` にしてください。
- 記事ページのナビゲーションは `../index.html`、`../articles.html`、`../about.html` にしてください。
- `articles.html` の適切なカテゴリに記事カードを追加してください。
- `index.html` の「最新記事」に新規記事を追加してください。
- 最新記事の件数が増えすぎる場合は、既存の表示件数に合わせて一番古い最新記事カードを外してください。
- `おすすめ記事` は、明示的に依頼がない限り変更しないでください。
- 実データや機密情報は入れず、必要に応じて一般化してください。

## 作業後の確認項目

- 編集したファイルが「編集してよいファイル」だけであること
- 新規記事が `articles/{{YYYY-MM-SLUG}}.html` として作成されていること
- 新規記事の `<title>`、`<h1>`、一覧カードのタイトルが一致していること
- 新規記事のCSS参照が `../css/style.css` であること
- 新規記事のナビゲーションリンクが正しいこと
- `articles.html` から新規記事へリンクできること
- `index.html` の最新記事から新規記事へリンクできること
- 会社情報、顧客情報、実IP、認証情報、SAS URL、試験問題本文が含まれていないこと
- `git status` と `git diff` で差分を確認すること

## 作業後に報告してほしいこと

- 編集したファイル
- 追加した記事の概要
- `articles.html` と `index.html` の更新内容
- 公開禁止情報が含まれていないことの確認結果
- 未確認事項や、公開前に人間が確認すべき点
