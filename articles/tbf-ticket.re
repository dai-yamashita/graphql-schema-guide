= リアルワールドスキーマ（ただの日記）

副題：技術書典Web 一般入場整理券販売システムその他の開発日記

技術書典Webで一般入場整理券販売の機能を追加開発を行う記録を残していきます。
別の言い方をすると、一般入場整理券の機能をこれからでっち上げないといけない事実があり、ドタバタとそれを開発するログを綴っていきます。

== 8月27日（火） 日記を書き始める

前回の技術書典6では事故なく終えることができ、また沢山の本が貰われていってよかったです。
他方改善点も多くあり、運営としては7でのよりスムーズな開催にむけてがんばっています。

今回は改善の一環として一般入場整理券販売について前売り方式へと変更を予定しています。
2フロアへ増床したことももちろん影響していますが、前回は参加者の皆様のやっていきが高まった結果、一般入場整理券販売開始前に自律的（なし崩し的）に列形成が始まってしまいました。
これにより会場外での一般入場整理券販売を行わざるをえなくなりました。
これは施設側担当者との事前の打ち合わせとは異なる形であったため、今回は列が膨らまない対策が求められます。

技術季報の販売なども考慮する必要がありますが、運営としては安全を最優先に考え、改善をすすめます。

そこで、今回は事前に一般入場整理券を販売することになります。
なりました。
そのためには、Webサイト上で一般入場整理券を販売するシステムを組む必要があります。
やっていきましょう。

なるべく早めに着手する必要がありますが、会場のレイアウトや導線の検討が必要→サークルの配置会議→Webサイト一般公開作業 に忙殺されていて作業が進むわけもなく…。
現時点でシステム面は未着手です。
進捗0%です！

幸い、どういう流れで当日を迎えるか、どういうシステムを組めば実現可能かの相談は終わっています。
あとは、実装していくだけです…。

要件メモ。
販売するのは3枠。
11時 Cホール枠、11時 Dホール枠、12時枠。
規定数を販売したら終了。

設計メモ。
Datastore上での同一Entityへのトランザクション処理はさほど速度が出ないと考えられる。
なので1整理券1Entityで事前にデータを用意し、購入したい人に対してそれを個別に割り当てていく方法で実現する。
PayPalとStripeで決済を行うプロセスが挟まるので、整理券をロック→決済→確定、もし決済されなかったら一定時間で解放という実装でよいはず。

作業メモ。
サーバAPI実装、Webフロントエンド実装、もしかしたらモバイルアプリ改修（作業担当者は別）、運営アプリ改修（これも作業担当者は別）が必要になる。
大変そうですねぇ。

さて、自分の原稿を諸般の事情により8月中に一定量書ききらないといけないので、続きはまた今度…。

== 8月28（水）番宣 サークル配置運営裏話

はい。
システム開発には全然入ってないんですが、技術季報用の収録をしたので宣伝します。
僕とsfujiwaraで技術書典のサークル配置の現状と展望についてお話しました。
席配置といってるけど、これは完全に要件定義から始まるシステム開発ですよ。
運営裏話ということで比較的面白そうなネタをしゃべることができたので、みんな買ってみてね！

…と、書いたはいいけどみなさんがこの本を買って帰って読んでくれたとして、今から会場に買いに戻るのは難しいわけですね…。
運良く買ってた人は読んでみてね！
そのうち電子版も売ると思うよ！たぶん！

== 8月30日（金）番宣 将来の展望 運営表話

はい。
システム開発には全然入ってないんですが、以下略。
僕とmhidakaで技術書典の将来の展望とか夢とかについてお話しました。
本当は羊とたかはしさんが話したほうがいいと思うんだけどさまざまな人間が無駄に忙しいのでこうなったんだと思います。
職場が同じというアドバンテージを最大限活用していく…！

買ってね！

== 9月1日（日）開発に着手する

9月に突入しました。
8月中に某所に何らかの原稿を送るという謎ミッションがあったのですが、@<chapref>{github}と@<chapref>{relay}あたりの原稿を送っておきました。
わりといい話になっているというか、他の人がしてないタイプの話を書けている気はするんですよね。
価値があるかはいまいちわかりませんが。

さて、そんなわけで9月に入ったので一般入場整理券システムの開発に着手しました。

GraphQL以前の開発では、DB周りのコード書く→ビジネスロジック的なところ書く→REST APIハンドラ周り書く→Webフロントエンド書く、みたいな手順でした。

GraphQL導入以降、GraphQLスキーマを書く→GraphQL APIハンドラ周り書く→DB周りのコードを書く→ビジネスロジック的なところ書く→Webフロントエンド書く、みたいな手順で作業するようになりました。

#@# prh:disable:良い
このやり方が良いか悪いかというと、軽微な改修の場合は良し、今回のような重たい改修の場合は悪し、という気がします。
やはりアプリケーションのキモはDB、つまりはデータ構造でしょう。
そして、データを駆動するのはビジネスロジックです。

最初から完璧に設計できることは期待できないので、やりながらエッジケースやユースケースを自分の中で育てていきます。
よね？
であれば、DB周りとビジネスロジックを最初に作り始めるのは理にかなっています。

一方、GraphQLのスキーマから入るとデータの構造とその関係性にどうしても目が行ってしまいます。
本来であれば、@<code>{EventTicket}は@<code>{EventID: String}を持ちますが、スキーマ上は@<code>{event: Event}と表現されます。
どうやら僕の場合、ここで微妙に脳の処理容量を持っていかれるようで、エッジケースの検出が甘くなる傾向がある気がします。
これが今後訓練次第で解決されるのか@<fn>{most-important-solution}は不明です。

//footnote[most-important-solution][プレッシャーの少ない環境でよく寝て時間をかけて設計しろという指摘がもっとも正しい]

とりあえず疲れて曖昧な気持ちでスキーマ書いてても自分の作ったlintルールが一貫性ぶっ壊れているところを指摘してくれる。
怒られたら直す。
平和守られる。
みんなもlintルール書け！

#@# prh:disable:一通り
そんなわけで、最初は@<code>{EventTicket}カインドを用意して、これに対する実装を一通り作成…したのですが、問題があると気が付きます。
8月27日に書いたとおり、@<code>{EventTicket}は1エンティティ=1一般入場整理券という構造です。
なので整理券の名称・価格・販売開始・終了日時の制御などを行うためのマスタデータ的なものが必要でした。

とりあえず気がついたので@<code>{EventTicketInfo}カインドを追加して再実装。
DB周りとビジネスロジックの仮組みを先にやっておけば手戻りが少なかったものを…！
反省点は完成する画面の脳内での作り込みが甘かったせいで、適切なデータ構造を導けなかった、というところかなぁ。
普段の僕だったら手を動かす前にその辺気がついててエッジケースとかもだいたい潰せてると思うんだけどなぁ…（自信過剰）。

それにしても@<code>{EventTicketInfo}という名前はよくない気がするなー…。
@<code>{EventTicketTemplate}とか@<code>{EventTicketCatalog}とかが妥当な気がする。
@<code>{EventTicketMaster}はちょっとやだな。
テーブル名として考えるとまぁ妥当なんだけど1エンティティを指す語として見ると適当ではない。
後日変えるかも。

とりあえず回り道したけど、GraphQLスキーマ作ってサーバ実装作って課金周りのコードも書いた。
テスト全然書いてないのでヤバそうな箇所を対象にテストもりもり書いたらサーバ側は終わり、あとフロントエンド。
多少余裕持って考えるとここからあと2人日くらいはかかってしまいそうな気がする。

全然関係ないけど git.apache.org が落ちてるようで、CIがまったく通らん。
おつらい…。

== 9月2日（月）テストを書くとバグが見つかるんだな

月曜日なので仕事です。
仕事をしている場合ではない気がするんだけど…？

git.apache.org が落ちっぱなしなので@<code>{go.mercari.io/datastore}@<fn>{mercari-datastore}の依存関係更新したりしてた。
Datastoreは一括Put500件、Get1000件の制限があり、GraphQLの場合一気に何件Getするかわからないのでたまにエラーになる。
これを回避するために自動的に処理を分割してくれるミドルウェアを書いて投入してみたんだけど、わりと便利かもしれない。
EventTicketを一気に4000件Putするような雑なテストを書いてもエラーにならなくなった。

//footnote[mercari-datastore][@<href>{https://godoc.org/go.mercari.io/datastore}]

昨日書いたコードにテストを書くと、それなりにバグが見つかる。
Go言語の@<code>{time.Time}の@<code>{Before}と@<code>{After}はどんなに気合入れて実装してもテスト書くと絶対間違ってる。

画面から逆算してスキーマを見直していったんAPI実装はこれで終わり！
あれやらこれやらが足りない気もするけど付属要素なので後からちょいちょいすればいいでしょ。

販売システムと当日運用システムはまた別なので悩ましいところだ。
まずは販売だけできるようにしなければ…。

== 9月3日（火）UIを作ろう！

今日はとりあえずUIを必要要素表示するだけでっちあげて、PayPalとStripeの繋ぎこみに着手した。
PayPalはとりあえずsandbox環境で疎通確認できた。
Stripeの動作確認は明日にしよう…。
それにしても課金系の動作確認は本当にめんどくさい…。

とりあえずAPIはmergeして適当にリリースしてみる。
しかし、現状のままだとUIはリリースと販売開始が同時になるな…。
それを解消するためには一般入場整理券情報に公開/非公開フラグをつけるべきだわ。

最初の設計時だと最小工数でできるように実装するので毎回後から建てましする形になる。
総工数さほど変わらないから別にいいんだけど。

見た目をいい感じに整えるが難しいのでCSS誰か書いてくれないかなー。

まぁとりあえず週末くらいに販売開始しようと思ったらできなくもない感じにはなった気がする。
疲れて帰ってくるとねこをふかふかしつつそのまま寝落ちしてしまうのがよくない。
洗濯物干すのと資源ごみの排出が忘れられている…！

== 9月4日（水）アプリ側対応の話

関係者各位に仕様を説明して開発をお願いするなどしてきた。
実に2週間ちょい前なのである…！

一般参加者用の整理券処理システム、サークル入場用と違ってそもそもの発行枚数が多い。
オフラインキャッシュを作るのすら時間がかかって大変そうなので、サークル用とは別方式でやる必要がありそうだ。
整理券データをJWT+JWKSでいい感じにできるようにしてやれば、入場受付アプリでは公開鍵だけ持っておけば検証可能になる。

データに適当に署名できるシステムは以前なんとなく作っておいた@<fn>{urabanashi}ので、そっちについては今から開発する必要はない。
適当に整理券データと繋いでJWTが出るようにした。
PayloadがでかくなってしまったのでQRコードのサイズがデカくなってしまった…。
が、まぁ支障なさそうなのでいったん気にしないことにした。

//footnote[urabanashi][当初は本を買った人が、僕本当に買ったよ！って証明できるデータを作れて第三者（サークル主とか）が検証できたら面白いのでは？と思って作った気がするけど忘れた]

この日記を読み返すと全体仕様について解説してるとこが全然ないな…。
まぁ日記だからそういうものか。

== 9月5日（木）動きそうな気がする

だいたい動く気がしてきたので運営メンバーに9日（月）には一般入場整理券を販売できる計画で動いていいよと伝える。
見栄えがどうなるかはわからんけどとりあえず動きはするでしょ…。

当日使う用の、券面的なものを見せる画面をざっくり実装する。
Twitterに 泣きながら "css img はみ出る" とかで検索してる とツイートしたら12ふぁぼもらった。
フロントエンドクソザコ開発者が開発してるんですよ！！

めんどくさかったのでQRコード画像の提供方法をGraphQL経由でdata URLにしちゃったけどいいよね…（デバッグがめんどいのであまりよくない）。
バックグラウンドとテキストの色を指定できたほうがよいのでAPIを拡張。
この辺もわかってたんだけど最初の実装では盛り込まなかったパートだ。
クライアント側でごまかす余地があるし、実装を追加するにしても互換性が壊れるわけではないからだ。
最初に決めておかないと死ぬタイプの決断と、後から気軽にやり直せる決断があり、これは後者なので判断を先送りにした。

@NkznとSlackで話していて、"9月1日（日）開発に着手する"で僕が感じたモデリングの問題点をアプリ開発者的なワードで説明してくれたのでここに引用しておく@<fn>{nkzn-said}。

//quote{
GraphQLのスキーマはAPIサーバーとしてのPresentation Modelだった。データベースのためのデータモデリングもやらないといけなかったので、一緒にやろうとしちゃったのが敗因。今日はそれぞれ別々にモデリングして、データベースをDataSource（Apollo語？）に隠蔽したら上手いこと回り始めた。
//}

//footnote[nkzn-said][@<href>{https://twitter.com/Nkzn/status/1169544768805752833}]

データ構造のモデリングについて、リレーションはオマケで最初に注力するべきポイントではないわけです。
GraphQLの場合、DBをまったく変更せずとも新しい関係性を定義して、resolverに実装をちょいと足してやればそれだけでリレーションは増やせます。
本質的なデータに注目するべきであり、補助係のデータ@<fn>{rdb-intermediate-table}は最初は俎上に載せる必要はないかもしれません。
ある程度構造が固まってから、GraphQLスキーマ上で関係性を定義し、resolverの中でだけ参照されるというのがよいかもしれません。

//footnote[rdb-intermediate-table][関連するデータのIDをもつフィールド。RDBでいうと中間テーブルとか。関係性のみ保持して実データを持たないもの]

いい話なので原稿に輸入しておこう。

== 9月6日（金）見た目を少し整える

送別会がある日だったのであまり作業してない。

Twitterに “div css margin 貫通 どうして” とかで泣きながら検索してる とツイートしたら28ふぁぼもらった。
CSSはもう不条理の塊だよぉ なんでそうなるんだよ…。
わからんてこんなもん…。

CSSわからん！

 1. 暗黙の挙動が多い（デフォルトの設定値とか）
 2. "こうしたら直った" の知見と書いたコードの相関がわからない
 3. 人の書いたコンポーネントの設計の意図がわからん（Angular Materialとか）
 4. そもそもあまり関心がない←致命的

ASTとかのほうがよっぽど簡単だ…

== 9月7日（土）今日は原稿書いてた

羊とかりんごがWebの見た目を頑張って整えてくれたっぽい。
すまねぇ…俺がCSSに勝てないばっかりに…！

あと、Nkznが後払いアプリの手数料2%のUI対応をやってくれたので準備が整ってきた…！

今日は真面目に1日原稿書いてました。
データベースの章書いた。
えらい。
間に合ってくれ頼む…！

といいつつ夜に入場整理券情報管理画面を作成した。
うーんこういうのもっとシュッと作れてほしい…辛い…ジーザス…。
どう考えてもAPI作るほうが簡単だし楽だ。

さらにこの後、入場整理券管理画面も作る必要があるんだぜ…ふふふ…。

== 9月8日（日）もくもく執筆会

運営スタッフとして参加。
みんながもくもく執筆しているであろう中、僕はシステム開発をしているのであった…。

一般入場整理券販売までに絶対やらないと困ることはだいぶ少なくなってきた。
今日は、整理券を購入した人に控え的なやつをメールで送るパートを実装する。

この前sinmetalが「当日Webにログインできないって言って詰む人が稀にいる」という話をしていた。
当日、スタッフ側で何らかの対応をするために、入場整理券のIDなどをメールで送付するとよいという話であった。
完全にわかりみが得られてしまったので今日はそれの実装をした。

僕はもう本当にeメールの運用というものを憎んでいて、使いたくないんだけど仕方ない…。
素直に届けばいいんだけど、やれ迷惑メールに分類されただの、アドレスの入力ミスだの、さまざまな要因で届かない。
相手の環境を勝手に覗けるわけでもないのでデバッグは困難だし、届かない！？知らんがな！！という気持ちになる。
あぁGoogle Waveが世界を制覇していてメールなんて滅ぼされていればよかったのに…。
と思わなくもないが、メールは送るのも受けるのも安いし特定のベンダに依存しないしだいたいみんな持ってるし結局メールってすごいな…ってなる。
つらい。

== 9月9日（月）最終デバッグ

明日一般入場整理券販売開始なので最後のデバッグ。
メール文面やら整理券名称やらの文言系をfixして投入。

ひととおり動かしてみたらメール文面の出力が結構バグってて修正に手間取る。
あとHTMLのinputの@<code>{type="datetime-local"}の値。
なんでDate突っ込んじゃダメなの… どう変換したらいいかわからなくて毎回結構調べるハメになる。
げせぬ。

== 9月10日（火）販売開始 at 10:00am！

昨日のうちにデータ設定とデータ投入を済ませておいたので見てるだけ。
10時にオープン、購入が開始され、最初の10分が山場だなーと思っていた。
一番最初に3桁人くらい購入トライしてくれたみたい。
Twitterとかで障害の報告も見つからず、無事に滑り出したっぽい。
各種設計やリミットの設定が正しかったと考えられるので、よかったですね。

一方、Stackdriver Loggingくんは間が悪く遅延@<fn>{stackdriver-delayed}していて、ログが見えないのであった。
10時時点でトラブってたらめちゃめちゃつらい気持ちになってたやつですね…。

//footnote[stackdriver-delayed][@<href>{https://status.cloud.google.com/incident/google-stackdriver/19006}]

GraphQL対応のログシステムをまだあまり作り込んでないので、当日までにはなんとかしておきたい所存。

あと、入場整理券管理画面もでっちあげた。
入場整理券情報管理画面とは別なのだ。
ドメイン言語の設計をミスった感じがあるけど仕方ないのだ…。

//comment{
== 9月11日（水）無

珍しく技術書典関連タスクをあまりやってない。
だからといって技術書典関連の作業が無かったわけではないのだが…。

はやく原稿を書かないとまずいので明日は有給にして原稿書くことにしようかな…。

== 9月12日（木）有給 and the day

有給とった。
原稿書いた。
やっとひととおりのトピックを書いたので、TODOの消化漏れないか見てセルフレビューして編集して誰かにレビューを頼むか…。
そんな時間ある？
あと10日で技術書典7ってマジ？
//}

== 9月13日（金）end of the week

11日、12日の日記は後で読み返したらマジでただの作業ログだったので闇に葬りました。
歯抜けの日があったらそういうことだと思ってください…。

さて、技術書典6までは後払い決済におけるPayPal、Stripeや銀行振込の手数料などは全額運営が負担していました。
今回は前々から告知しているとおり、後払い決済のサークル側手数料として、およそ半額にあたる2%を負担していただきます。
もちろん、システム側にこの改修を作り込まないといけません。

というわけでシステム改修と動作確認をするなど…。
初回（技術書典3だっけ？）のときに手数料の仕組みは作ってあったんだけど、一回も動かしてなかったので設定値変えて動作確認する。
動き的には問題ないんだけどDatastoreのデータをBigQueryに流し込んだ時の集計がやりにくいコードだったので各Entityがもつ（計算で導出可能な）データをちょっとリッチに。
既存データをマイグレーションしておかないとデータ参照時のデータ穴埋めコストがバカにならないのでマイグレーション作業も必要なのでめんどうである。

== 9月14日（土）マイグレーション作業着手

というわけで移行バッチを組んだので流してみる。
stg環境で流すも、テストのために不規則なデータが多くてバッチが結構途中で止まる…つらい…。
バッチを手直ししつつ適当にやっていく。

合間合間に支払い・請求データの生成ロジックを改善。
これもデータ形式をリッチにする方向だけど、既存データは料率0%固定でいいのでDBを参照せずに穴埋めできるので比較的ラク。

いい加減自分の原稿をfinishしてかかりたいんだけどなかなか手が付かない。
表紙を作るためにゆかりさんにかわいい表情して！って頼んでるんだけどおすましフェイスしててなかなか激プリティ写真が撮れない…。

== 9月15日（日）本番環境のマイグレーション作業

本番環境でマイグレーション作業を開始する。
処理速度が割と遅いのですんげー時間がかかる…。
こういう時はRDBのほうが楽でいいよなーという気持ち。

//comment{
本番環境で、絶対引っかからないだろうけど念の為に入れておいたチェックロジックにひっかかってバッチが落ちる…。
デバッグログをモリモリにしてチェックを細分化して分析していったら、Memcacheに乗っかってるデータがちょっと古いのが原因だった…。
準備処理をトランザクションなし（暗黙的にMemcacheが最初に参照される）で、本番処理をトランザクションあり（Memcacheは参照されない）と処理して両者を突合していた。
そこで、全体のデータはほぼ合致してたんだけど、UpdatedAtの値だけ異なって落ちてた。
…そんなことある？
数時間無駄にしたけどMemcacheを消し飛ばして動かしたらスムーズに流れはじめた。
頑張ってほしい…。
//}

#@# prh:disable:その様子
見本誌チェックアプリをsinmetal一味が開発しているんだけど、その様子を横目にみていた。
シチュエーションの組み合わせがアホみたいに多くて大変そうである。
Alloyかなんかを使ってパターンの網羅ができてるか検証してみたくなるが自分の作業も詰まっておる…。

頒布物の見本誌は電子的に事前提出必須なので、それをベースにオペを組める…といいんだけど、これを守ってくれない参加者さんの考慮が大変。
スムーズな運営はスムーズなデータから…！
みなさんご協力よろしくおねがいします。

//comment{
自分の本は電子書籍でのみ出すのでダウンロードカードを名刺屋さんに発注する。
毎回50部くらいしか出ないのだけど単価が安いので毎回200枚くらい刷っている。
配布用のURLはGoogle Driveにフォルダを作って公開設定にして決定しておく。
すると、フォルダの中身はあとからいくらでも入れ替え可能なのである！

自分の原稿を一回セルフレビュー通す。
すごい時間がかかる…。
今からレビュー依頼をしたいのだがいまさらレビュー依頼とかかけてしまって大丈夫なのだろうか？
//}

バッチ進捗見てたら完遂してた。
よかったね！

あ、あとJWTのサイズがでかくてQRコードが細かくなりすぎているという話があり、フィールドの圧縮を図った。
zip圧縮とかgzip圧縮を試したがbase64すると元よりデカくなってしまう。
エラー訂正のレベルをいじる程度かな…。

//comment{
== 9月16日（祝）なぜかものすごく忙しい

昨晩レビュー依頼を何名かにしたら快諾していただきすぐさま返してくださる方も…！ありがとうございます！
今気がついたんだけどこの本あとがきとか謝辞書くとこなくない…？

昼間はジムに行った。
体を動かすことに多少慣らしておかないと当日急に動き回ると死んでしまうので…。
最近体調があまり快調ではなかったんだけどジムいったらいい感じになった。
たまには運動は必要なんやなって…。

今日は運営タスクをもりもりやるのでレビューの修正はちょっとできなさそう。
無限にIssueを閉じたり無限にドキュメント書いたりレビューしたり無限にコード書いたりしてる。
今まで後払い決済の集計バッチの実行に8時間とかかかってたような気がする（ほんとか？）ので、処理方式を変えて並列処理するようにしたい…。

3連休って何？
休みってどこ？

== 9月17日（火）ブログを書くのは大変

細かく色々な運営便利機能を開発していく。
ダウンロード機能（β）周りの技術書典7対応が漏れてることに気がついたりして色々めんどかった…。

今回も後払い決済の説明ブログを書いた。
僕は図表を作ったりスクショ撮ったりとか本当にきらいで泣きながらやった。
手伝ってくれたNkznありがとう…。
//}

== 9月18日（水）動けバッチ 唸れ処理

後払い決済の集計バッチを書き直して並列処理っぽくできるようにした。
わりと動いてるっぽい気がするのであとは当日投入してみましょう…！という感じ。

わりとみんな反応がゆるふわになってきてるので疲れが溜まってそうですなぁ！
