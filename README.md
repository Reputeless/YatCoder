# YatCoder
競技プログラミング（プログラミングコンテスト）のコードを、Siv3D スタイルで書けるライブラリ

**対応コンパイラ:** C++14 (GCC 5.4.1)　**ライセンス:** CC0 1.0 Universal

# 1. インクルードされるヘッダ

`# include <bits/stdc++.h>`

# 2. 数値型

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


# 3. 文字列型

## 3.1 String 型

| 名前     | 説明   |
|--------|------|
| String | 文字列型 |

```C++
int main()
{
	String s = "YatCoder";
}
```

## 3.2 String 型のメンバ関数

### `size_t String::count(char c);`
文字列中に含まれる文字 'c' の個数を返します。
```C++
int main()
{
	String s1 = "abaab";
	Print << s1.count('a');

	String s2 = "ABC";
	Print << s2.count('a');
}
```
出力
```
3
0
```


🚧🚧🚧


# 4. 動的配列型

## 4.1 Array 型

| 名前    | 説明    |
|-------|-------|
| Array | 動的配列型 |

## 4.2 Array 型のメンバ関数

🚧🚧🚧


# 5. 文字列フォーマット

## 5.1 値から文字列への変換

**フォーマット可能** な型の値は、`Format()` で文字列に変換できます。

| 型                        | Format 結果の例     |
|--------------------------|-----------------|
| int8                     | -100            |
| int16                    | -100            |
| int32                    | -100            |
| int64                    | -100            |
| int128                   | -100            |
| uint8                    | 100             |
| uint16                   | 100             |
| uint32                   | 100             |
| uint64                   | 100             |
| uint128                  | 100             |
| float32                  | 12.5000000000   |
| float64                  | 12.5000000000   |
| float128                 | 12.5000000000   |
| bool                     | true            |
| char                     | a               |
| const char*              | abc             |
| int[3]                   | {11, 22, 33}    |
| String                   | abc             |
| `std::array<int, 3>`     | {11, 22, 33}    |
| `std::pair<int, String>` | {10, aaa}       |
| `Array<int>`             | {11, 22, 33}    |
| `Array<String>`          | {aaa, bbb, ccc} |

```C++
int main()
{
	String s1 = Format(12345); // 12345
	String s2 = Format(10, "abc", 5); // 10abc5
	String s3 = Format(123.45678); // 123.4567800000
	String s4 = Format(int128(1) << 100)); // 1267650600228229401496703205376
	String s5 = Format(10 < 20); // true
	String s6 = Format(Array<int>{ 11, 22, 33 }); // {11, 22, 33}
}
```

## 5.2 小数点以下の桁数指定

`Format()` に `N_dp` (N は整数) を渡すと、それ以降の浮動小数点数型の引数に対して、小数点以下 N 桁まで変換します。デフォルトでは 10 です。小数点以下 (N+1) 桁目の数は四捨五入されます (例: `Format(2_dp, 1.76543)` では 5 が四捨五入されて `1.77`)

```C++
int main()
{
	String s1 = Format(123.456789);			// 123.4567890000
	String s2 = Format(0_dp, 123.456789);	// 123
	String s3 = Format(1_dp, 123.456789);	// 123.5
	String s4 = Format(3_dp, 123.456789);	// 123.457
	String s5 = Format(5_dp, 123.456789);	// 123.45679
	String s6 = Format(10_dp, 123.456789);	// 123.4567890000

	// 途中で変更することも可能
	String s7 = Format(0_dp, 1.11, ' ', 1_dp, 2.22); // 1 2.2

	// _dp の効力はその Format 内なので、デフォルトの桁数に戻る
	String s8 = Format(123.456789);			// 123.4567890000
}
```


# 6. 標準出力

## 6.1 Print

**フォーマット可能** な型の値を `Print` に `<<` 演算子で送ると、`Format()` で文字列化されたのち標準出力されます。`<<` でつながった一連の `Print` の最後で自動的に改行が挿入されます。

```C++
int main()
{
	Print << 12345;
	Print << 10 << "abc" << 5;
	Print << 123.45678;
	Print << (int128(1) << 100);
	Print << (10 < 20);
	Print << Array<int>{ 11, 22, 33 };
	Print << 0_dp << 1.11 << ' ' << 1_dp << 2.22;
	Print << 123.45678;
}
```
出力
```
12345
10abc5
123.4567800000
1267650600228229401496703205376
true
{11, 22, 33}
1 2.2
123.4567890000
```


# 7. 標準入力

## 7.1 数値の読み込み
スペースや改行で区切られた標準入力文字列から、数値を読み込みます。

| 読み込む値    | 関数           |
|----------|--------------|
| int8     | ReadInt8     |
| int16    | ReadInt16    |
| int32    | ReadInt32    |
| int64    | ReadInt64    |
| uint8    | ReadUint8    |
| uint16   | ReadUint16   |
| uint32   | ReadUint32   |
| uint64   | ReadUint64   |
| float32  | ReadFloat32  |
| float64  | ReadFloat64  |
| float128 | ReadFloat128 |

| 読み込む値   | 関数         |
|---------|------------|
| int32   | ReadInt    |
| uint32  | ReadUint   |
| float32 | ReadFloat  |
| float64 | ReadDouble |

```C++
int main()
{
	const int32 a = ReadInt();
	const int32 b = ReadInt(), c = ReadInt();
	Print << a;
	Print << b + c;
	Print << b - c;
}
```
入力
```
10
5 3
```
出力
```
10
8
2
```

## 7.2 数値の読み込み + 入力の終わりの判定
入力の終わりを判定したい場合は、戻り値が `bool` で参照引数から結果を受け取るオーバーロード関数を使います。

```C++
int main()
{
	int32 n;

	while (ReadInt(n)) // 入力の終わりに達していた場合 false を返す
	{
		Print << n * 2;
	}
}
```
入力
```
1
3
5
```
出力
```
2
6
10
```

## 7.3 文字の読み込み（空白を含まない）

### `char ReadChar();`
### `bool ReadChar(char& c);`
標準入力から文字を 1 つ読み込み、その文字を返します。空白や改行の場合、無視して次の空白や改行でない文字を読み込みます。

```C++
int main()
{
	const char a = ReadChar();
	const char b = ReadChar();
	const char c = ReadChar();
	Print << a;
	Print << b;
	Print << c;
}
```
入力
```
N Y Y
```
出力
```
N
Y
Y
```

## 7.4 文字の読み込み（空白を含む）

### `char ReadCodePoint();`
### `bool ReadCodePoint(char& c);`
標準入力から文字を 1 つ読み込み、その文字を返します。空白や改行も 1 文字として扱います。

```C++
int main()
{
	const char a = ReadCodePoint();
	const char b = ReadCodePoint();
	const char c = ReadCodePoint();
	Print << a;
	Print << b;
	Print << c;
}
```
入力
```
# #
```
出力
```
#

#
```

## 7.5 文字列の読み込み（空白を含まない）

### `String ReadWord();`
### `bool ReadWord(String& s);`
標準入力から空白や改行で区切られた一連の文字列を読み込み、その文字列を返します。

```C++
int main()
{
	const String s1 = ReadWord();
	const String s2 = ReadWord();
	Print << s1;
	Print << s2;
}
```
入力
```
hello world
```
出力
```
hello
world
```

## 7.6 文字列の読み込み（空白を含む 1 行全部）

### `String ReadLine();`
### `bool ReadLine(String& s);`
標準入力から 1 行読み込み、その文字列を返します。空白行はスキップし、次の空白でない行を読み込みます。

```C++
int main()
{
	const String s1 = ReadLine();
	const String s2 = ReadLine();
	Print << s1;
	Print << s2;
}
```
入力
```
hello world
hello yatcoder
```
出力
```
hello world
hello yatcoder
```

## 7.7 配列の読み込み

🚧🚧🚧

# 8. ループ

```C++
int main()
{
	for (auto i : step(5))
	{
		Print << i;
	}
}
```
出力
```
0
1
2
3
4
```

```C++
int main()
{
	for (auto i : Range(1, 4))
	{
		Print << i;
	}
}
```
出力
```
1
2
3
4
```

```C++
int main()
{
	for (auto i : Range(5, 0, -1))
	{
		Print << i;
	}
}
```
出力
```
5
4
3
2
1
0
```

🚧🚧🚧


# 9. 便利関数・関数オブジェクト

## 9.1 範囲内にあるかの判定

🚧🚧🚧

## 9.2 偶数・奇数判定

### `IsOdd`
`()` 演算子で渡した整数が奇数なら `true` を返します。

```C++
int main()
{
	Print << IsOdd(3);
	Print << IsOdd(-3);
	Print << IsOdd(24);
}
```
出力
```
true
true
false
```

### `IsEven`
`()` 演算子で渡した整数が偶数なら `true` を返します。

```C++
int main()
{
	Print << IsEven(2);
	Print << IsEven(-2);
	Print << IsEven(25);
}
```
出力
```
true
true
false
```

🚧🚧🚧


# AtCoder の解答例

### [PracticeA - Welcome to AtCoder](https://atcoder.jp/contests/abs/tasks/practice_1)
```C++
int main()
{
	const int32 a = ReadInt();
	const int32 b = ReadInt(), c = ReadInt();
	const String s = ReadLine();
	Print << (a + b + c) << ' ' << s;
}
```

### [ABC086A - Product](https://atcoder.jp/contests/abs/tasks/abc086_a)
```C++
int main()
{
	const int32 a = ReadInt(), b = ReadInt();
	Print << (IsEven(a * b) ? "Even" : "Odd");
}
```

### [ABC081A - Placing Marbles](https://atcoder.jp/contests/abs/tasks/abc081_a)
```C++
int main()
{
	Print << ReadLine().count('1');
}
```

### [ABC081B - Shift only](https://atcoder.jp/contests/abs/tasks/abc081_b)
```C++
int main()
{
	const int32 N = ReadInt();
	Array<int32> as = ReadIntArray(N);
	int32 result = 0;

	while (as.all(IsEven))
	{
		as.each([](int32& a) { a /= 2; });
		++result;
	}

	Print << result;
}
```

### [ABC087B - Coins](https://atcoder.jp/contests/abs/tasks/abc087_b)
```C++
int main()
{
	const int32 N500 = ReadInt(), N100 = ReadInt(), N50 = ReadInt();
	const int32 X = ReadInt();
	int32 result = 0;

	for (auto n500 : Range(0, N500))
	{
		for (auto n100 : Range(0, N100))
		{
			for (auto n50 : Range(0, N50))
			{
				const int32 sum = (n500 * 500) + (n100 * 100) + (n50 * 50);
				result += (sum == X);
			}
		}
	}

	Print << result;
}
```

### [ABC083B - Some Sums](https://atcoder.jp/contests/abs/tasks/abc083_b)
```C++
int main()
{
	const int32 N = ReadInt();
	const int32 A = ReadInt(), B = ReadInt();
	int32 result = 0;

	for (auto n : Range(1, N))
	{
		int32 t = 0;

		for (auto i : Range(0, 4))
		{
			t += GetDigit(n, i);
		}

		if (InRange(t, A, B))
		{
			result += n;
		}
	}

	Print << result;
}
```

### [ABC088B - Card Game for Two](https://atcoder.jp/contests/abs/tasks/abc088_b)
```C++
int main()
{
	const int32 N = ReadInt();
	const Array<int32> as_rsorted = ReadIntArray(N).rsort();
	int32 alice = 0, bob = 0;

	for (auto i : step(as_rsorted.size()))
	{
		(IsEven(i) ? alice : bob) += as_rsorted[i];
	}

	Print << (alice - bob);
}
```

### [ABC085B - Kagami Mochi](https://atcoder.jp/contests/abs/tasks/abc085_b)
```C++
int main()
{
	const int32 N = ReadInt();
	Print << ReadIntArray(N).unique().size();
}
```

### [ABC085C - Otoshidama](https://atcoder.jp/contests/abs/tasks/abc085_c)
```C++
int main()
{
	const int32 N = ReadInt();
	const int32 Y = ReadInt();

	for (auto n10000 : Range(0, N))
	{
		for (auto n5000 : Range(0, N - n10000))
		{
			const int32 n1000 = N - n10000 - n5000;
			const int32 sum = (n10000 * 10000) + (n5000 * 5000) + (n1000 * 1000);

			if (sum == Y)
			{
				Print << n10000 << ' ' << n5000 << ' ' << n1000;
				return 0;
			}
		}
	}

	Print << "-1 -1 -1";
}
```

### [ABC049C - 白昼夢 / Daydream](https://atcoder.jp/contests/abs/tasks/arc065_a)
```C++
int main()
{
	String S = ReadWord();

	while (S)
	{
		bool removed = false;

		for (StringView keyword : { "eraser", "erase", "dreamer", "dream" })
		{
			if (S.ends_with(keyword))
			{
				S.erase(S.end() - keyword.size(), S.end());
				removed = true;
				break;
			}
		}

		if (!removed)
		{
			break;
		}
	}

	Print << YESNO(S.isEmpty());
}
```

