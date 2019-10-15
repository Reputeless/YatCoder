# YatCoder
競技プログラミング（プログラミングコンテスト）のコードを、Siv3D スタイルで書けるライブラリ

**対応コンパイラ:** C++14 (GCC 5.4.1)

**ライセンス:** CC0 1.0 Universal

## 機能

### 1. インクルードされるヘッダ

`# include <bits/stdc++.h>`

### 2. 数値型

| 名前       | 説明              |
|----------|-----------------|
| int8     | 8-bit 符号付き整数型   |
| int16    | 16-bit 符号付き整数型  |
| int32    | 32-bit 符号付き整数型  |
| int64    | 64-bit 符号付き整数型  |
| int128   | 128-bit 符号付き整数型 |
| uint8    | 8-bit 符号なし整数型   |
| uint16   | 16-bit 符号なし整数型  |
| uint32   | 32-bit 符号なし整数型  |
| uint64   | 64-bit 符号なし整数型  |
| uint128  | 128-bit 符号なし整数型 |
| float32  | 単精度浮動小数点数型      |
| float64  | 倍精度浮動小数点数型      |
| float128 | 四倍精度浮動小数点数型     |

```C++
# include "YatCoder.hpp"

int main()
{
    uint8 a = 0b0101;
    int32 b = -200;
    uint64 c = 1234567890123;
    int128 d = int128(1) << 100; // 1267650600228229401496703205376
    uint128 e = uint128(-1); // 340282366920938463463374607431768211455
    float32 f = 3.14f;
    float64 g = 3.14;
    float128 h = 3.14L;
}
```


## 3. 文字列型

| 名前     | 説明   |
|--------|------|
| String | 文字列型 |

```C++
# include "YatCoder.hpp"

int main()
{
	String s = "YatCoder";
}
```

🚧🚧🚧


## 4. 動的配列型

| 名前    | 説明    |
|-------|-------|
| Array | 動的配列型 |

🚧🚧🚧


## 5. 文字列フォーマット
**フォーマット可能** な型の値は、`Format()` で文字列に変換できます。

| 型             | Format 結果の例     |
|---------------|-----------------|
| int8          | -100            |
| int16         | -100            |
| int32         | -100            |
| int64         | -100            |
| int128        | -100            |
| uint8         | 100             |
| uint16        | 100             |
| uint32        | 100             |
| uint64        | 100             |
| uint128       | 100             |
| float32       | 12.5000000000   |
| float64       | 12.5000000000   |
| float128      | 12.5000000000   |
| bool          | true            |
| char          | a               |
| const char*   | abc             |
| String        | abc             |
| Array<int>    | {11, 22, 33}    |
| Array<String> | {aaa, bbb, ccc} |

```C++
int main()
{
	String s1 = Format(12345); // 12345

	String s2 = Format(10, "abc", 5); // 10abc5

	String s3 = Format(123.45678); // 123.4567800000

	String s4 = Format(10 < 20); // true

	String s5 = Format(Array<int>{ 11, 22, 33 }); // {11, 22, 33}
}
```

🚧🚧🚧

