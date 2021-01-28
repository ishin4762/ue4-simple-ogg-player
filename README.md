# ue4-simple-ogg-player
Unreal Engine 4でイントロ付きoggを再生するプラグインです。

## 仕様

* ループ開始点とループ長を指定することで、なめらかなイントロループを実現
* v0.10現在、uassetファイルは未対応。直接ファイルパスを指定します
* v0.10現在、oggの組み込みタグによるループはできません

## 動作環境

UE4.25.4 (他は未検証です)

## 使い方

1. Plugins/SimpleOggPlayerをプロジェクトのPluginsフォルダに入れてください
1. .oggファイルをプロジェクトのContentフォルダの任意の場所に入れてください
1. Sound Cueを作成し、下記のようにOggPlayerというノードを追加します
![](images/image1.png)
1. パラメータを入力します
    * Ogg File Pathには、Contentフォルダからのoggファイルの相対パス
    * Loop Startはループ開始地点のサンプル数
    * Loop Lengthにはループするサンプル数
1. Cueを再生するとoggをループ付きで再生できます

## ライセンス
MIT

## 更新履歴
* 2021/01/29 v0.10公開

