# CppPuyo

## 実行方法

### Makeコマンドを使用する
`$ make`

`$ ./p_cui`


### 手動
`$ g++ p_cui.cpp p_system.cpp -lncurses`

`$ ./a.out`

![demo](https://github.com/caprolactam2450/CppPuyo/blob/master/screenshots/chain.gif)

## 表示内容

![description](https://github.com/caprolactam2450/CppPuyo/blob/master/screenshots/chain.gif)

一番左に表示されているフィールドに対応して

操作中のぷよが移動できる範囲であるかどうか、それぞれのぷよが同じ色となん連結しているか

右に表示している。

移動可能範囲では移動できれば（ぷよがなければ）T, できなければ（ぷよがあれば）Fと表示される。