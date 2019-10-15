////////////////////////////////////////////////////////////////
//
//	YatCoder Library
//	https://github.com/Reputeless/YatCoder
//

# define YAT_WITH_FEATURE(X) YAT_WITH_FEATURE_PRIVATE_DEFINITION_##X()
# define YAT_WITH_FEATURE_PRIVATE_DEFINITION_INT128() 0
# if defined(__SIZEOF_INT128__) // 組み込み 128-bit 整数型の有無
#	undef YAT_WITH_FEATURE_PRIVATE_DEFINITION_INT128
#	define YAT_WITH_FEATURE_PRIVATE_DEFINITION_INT128() 1
# endif
# if defined(_WIN32) // MSVC での警告抑制
#	pragma warning(disable : 26444)
# endif

////////////////////////////////
//
//	1. インクルードされるヘッダ
//
////////////////////////////////

# include <bits/stdc++.h>

namespace yat
{
	////////////////////////////////
	//
	//	2. 数値型
	//
	////////////////////////////////

	//	数値型
	using int8 = std::int8_t;
	using int16 = std::int16_t;
	using int32 = std::int32_t;
	using int64 = std::int64_t;
	using uint8 = std::uint8_t;
	using uint16 = std::uint16_t;
	using uint32 = std::uint32_t;
	using uint64 = std::uint64_t;
# if YAT_WITH_FEATURE(INT128)
	using int128  = __int128;
	using uint128 = unsigned __int128;
# endif
	using float32  = float;
	using float64  = double;
	using float128 = long double;


	////////////////////////////////
	//
	//	3. 文字列型
	//
	////////////////////////////////

	//	文字列型
	using String = std::string;


	////////////////////////////////
	//
	//	4. 動的配列型
	//
	////////////////////////////////

	//	動的配列型
	template <class Type> using Array = std::vector<Type>;


	////////////////////////////////
	//
	//	5. 文字列フォーマット
	//
	////////////////////////////////

	//	文字列フォーマット用内部データ
	struct FormatData
	{
		String string; //	文字列バッファ
		struct DecimalPlaces
		{
			explicit constexpr DecimalPlaces(int32 v = 10) : value(v) {}
			int32 value; //	小数点以下の桁数
		} decimalPlaces;
	};

	//	小数点数以下の桁数指定
	//	* Format や Print に渡す
	inline constexpr FormatData::DecimalPlaces operator ""_dp(unsigned long long width)
	{
		return FormatData::DecimalPlaces(static_cast<int32>(width));
	}

	namespace detail
	{
		struct Format_impl
		{
		private:
			static void format(const FormatData&) {}
			template <class Type, class... Args>
			static void format(FormatData& f, const Type& x, const Args&... args) { Formatter(f, x); format(f, args...); }
		public:
			template <class... Args>
			String operator ()(const Args&... args) const
			{
				FormatData formatData;
				format(formatData, args...);
				return std::move(formatData.string);
			}
			String operator ()(const char c) const { return String(1, c); }
			String operator ()(const char* text) const { return String(text); }
			const String& operator ()(const String& text) const { return text; }
			String operator ()(String&& text) const noexcept { return std::move(text); }
		};

		template <class Float>
		inline void FloatFormatter_impl(FormatData& f, Float x)
		{
			std::ostringstream os;
			os.precision(f.decimalPlaces.value);
			os << std::fixed << x;
			f.string.append(os.str());
		}
	}

# if YAT_WITH_FEATURE(INT128)
	// 128-bit 整数のフォーマット
	namespace detail
	{
		class Int128Formatter
		{
		private:
			static constexpr size_t BufferSize = 40;
			char m_buffer[BufferSize];
			char* m_str;
			char* format(uint128 value)
			{
				char* bufferEnd = m_buffer + BufferSize;
				while (value)
				{
					*(--bufferEnd) = static_cast<char>('0' + (value % 10));
					value /= 10;
				}
				return bufferEnd;
			}
		public:
			explicit Int128Formatter(int128 value)
			{
				uint128 absValue = static_cast<uint128>(value);
				const bool negative = (value < 0);
				if (negative) absValue = (0 - absValue);
				m_str = format(absValue);
				if (negative) *--m_str = '-';
			}
			explicit Int128Formatter(uint128 value) : m_str(format(value)) {}
			const char* data() const { return m_str; }
			size_t size() const { return m_buffer - m_str + BufferSize; }
		};
	}
# endif

	inline void Formatter(FormatData& f, const FormatData::DecimalPlaces x) { f.decimalPlaces = x; }
	inline void Formatter(FormatData& f, int8 x)  { f.string.append(std::to_string(x)); }
	inline void Formatter(FormatData& f, int16 x) { f.string.append(std::to_string(x)); }
	inline void Formatter(FormatData& f, int32 x) { f.string.append(std::to_string(x)); }
	inline void Formatter(FormatData& f, int64 x) { f.string.append(std::to_string(x)); }
	inline void Formatter(FormatData& f, uint8 x)  { f.string.append(std::to_string(x)); }
	inline void Formatter(FormatData& f, uint16 x) { f.string.append(std::to_string(x)); }
	inline void Formatter(FormatData& f, uint32 x) { f.string.append(std::to_string(x)); }
	inline void Formatter(FormatData& f, uint64 x) { f.string.append(std::to_string(x)); }
# if YAT_WITH_FEATURE(INT128)
	inline void Formatter(FormatData& f, int128 x)
	{
		detail::Int128Formatter fmt(x);
		f.string.append(fmt.data(), fmt.size());
	}
	inline void Formatter(FormatData& f, uint128 x)
	{
		detail::Int128Formatter fmt(x);
		f.string.append(fmt.data(), fmt.size());
	}
# endif
	inline void Formatter(FormatData& f, float32 x)  { detail::FloatFormatter_impl(f, x); }
	inline void Formatter(FormatData& f, float64 x)  { detail::FloatFormatter_impl(f, x); }
	inline void Formatter(FormatData& f, float128 x) { detail::FloatFormatter_impl(f, x); }
	inline void Formatter(FormatData& f, bool b) { f.string.append(b ? "true" : "false"); }
	inline void Formatter(FormatData& f, char c) { f.string.push_back(c); }
	inline void Formatter(FormatData& f, const char* text)   { f.string.append(text); }
	inline void Formatter(FormatData& f, const String& text) { f.string.append(text); }

	template <class Iterator>
	inline void Formatter(FormatData& f, Iterator begin, Iterator end)
	{
		f.string.push_back('{');
		for (bool isFirst = true; begin != end; ++begin)
		{
			if (isFirst) isFirst = false;
			else f.string.append(", ");
			Formatter(f, *begin);		
		}
		f.string.push_back('}');
	}

	template <class Type> inline void Formatter(FormatData& f, const Array<Type>& v) { Formatter(f, v.begin(), v.end()); }

	//	フォーマット可能な型の値を文字列に変換
	//	Format(...)
	constexpr auto Format = detail::Format_impl();


	////////////////////////////////
	//
	//	6. 標準出力
	//
	////////////////////////////////

	//	標準出力
	namespace detail
	{
		struct PrintBuffer
		{
			std::unique_ptr<FormatData> formatData;
			PrintBuffer() : formatData(std::make_unique<FormatData>()) {}
			PrintBuffer(PrintBuffer&& o) noexcept : formatData(std::move(o.formatData)) {}
			~PrintBuffer() { if (formatData) std::cout << formatData->string << '\n'; }
			template <class Type> PrintBuffer& operator <<(const Type& x) { Formatter(*formatData, x); return *this; }
		};

		struct Print_impl
		{
			template <class Type, class = decltype(Formatter(std::declval<FormatData&>(), std::declval<Type>()))>
			PrintBuffer operator <<(const Type& x) const { PrintBuffer b; Formatter(*b.formatData, x); return b; }
		};
	}

	constexpr auto Print = detail::Print_impl();


	////////////////////////////////
	//
	//	7. 標準入力
	//
	////////////////////////////////

	//	標準入力
	template <class Type> inline Type Read_impl() { Type t; std::cin >> t; return t; }
	template <class Type> inline bool Read_impl(Type& t) { return !!(std::cin >> t); }

	//	標準入力から数値を 1 つ読み込む
	inline int8  ReadInt8()  { return Read_impl<int8>();  }
	inline int16 ReadInt16() { return Read_impl<int16>(); }
	inline int32 ReadInt32() { return Read_impl<int32>(); }
	inline int64 ReadInt64() { return Read_impl<int64>(); }
	inline int8  ReadUint8()  { return Read_impl<uint8>();  }
	inline int16 ReadUint16() { return Read_impl<uint16>(); }
	inline int32 ReadUint32() { return Read_impl<uint32>(); }
	inline int64 ReadUint64() { return Read_impl<uint64>(); }
# if YAT_WITH_FEATURE(INT128)
	// [ToDo] inline int128 ReadInt128() { ... } 
	// [ToDo] inline uint128 ReadUint128() { ... }
# endif
	inline float32  ReadFloat32()  { return Read_impl<float32>();  }
	inline float64  ReadFloat64()  { return Read_impl<float64>();  }
	inline float128 ReadFloat128() { return Read_impl<float128>(); }
	inline int32  ReadInt()  { return ReadInt32(); }
	inline uint32 ReadUint() { return ReadUint32(); }
	inline float32 ReadFloat()  { return ReadFloat32();}
	inline float64 ReadDouble() { return ReadFloat64(); }

	//	標準入力から数値を 1 つ読み込む
	//	* 入力の終わりに達していた場合 `false` を返す
	inline bool ReadInt8(int8& x)   { return Read_impl<int8>(x);  }
	inline bool ReadInt16(int16& x) { return Read_impl<int16>(x); }
	inline bool ReadInt32(int32& x) { return Read_impl<int32>(x); }
	inline bool ReadInt64(int64& x) { return Read_impl<int64>(x); }
	inline bool ReadUint8(uint8& x)   { return Read_impl<uint8>(x);  }
	inline bool ReadUint16(uint16& x) { return Read_impl<uint16>(x); }
	inline bool ReadUint32(uint32& x) { return Read_impl<uint32>(x); }
	inline bool ReadUint64(uint64& x) { return Read_impl<uint64>(x); }
# if YAT_WITH_FEATURE(INT128)
	// [ToDo] inline bool ReadInt128(int128& x) { ... } 
	// [ToDo] inline bool ReadUint128(uint128& x) { ... }
# endif
	inline bool ReadFloat32(float32& x)   { return Read_impl<float32>(x);  }
	inline bool ReadFloat64(float64& x)   { return Read_impl<float64>(x);  }
	inline bool ReadFloat128(float128& x) { return Read_impl<float128>(x); }
	inline bool ReadInt(int32& x)   { return ReadInt32(x); }
	inline bool ReadUint(uint32& x) { return ReadUint32(x); }
	inline bool ReadFloat(float32& x)  { return Read_impl<float32>(x); }
	inline bool ReadDouble(float64& x) { return Read_impl<float64>(x); }

	//	標準入力から、文字を 1 つ読み込む
	//	* 空白や改行の場合は無視して次の空白や改行でない文字を読み込む
	inline char ReadChar() { return Read_impl<char>(); }

	//	標準入力から、文字を 1 つ読み込む
	//	* 空白や改行の場合は無視して次の空白や改行でない文字を読み込む
	//	* 入力の終わりに達していた場合 `false` を返す
	inline bool ReadChar(char& c) { return Read_impl<char>(c); }

	//	標準入力から、空白や改行を含む文字を 1 つ読み込む
	inline char ReadCodePoint() { return static_cast<char>(std::getchar()); }

	//	標準入力から、空白や改行を含む文字を 1 つ読み込む
	//	* 入力の終わりに達していた場合 `false` を返す
	inline bool ReadCodePoint(char& c) { c = static_cast<char>(std::getchar()); return (c != EOF); }

	//	標準入力から、空白を含まない 1 単語を読み込む
	inline String ReadWord() { String s; std::cin >> s; return s; }
	
	//	標準入力から、空白も含んで 1 行を読み込む
	//	* 空白行の場合は無視して次の空白でない行を読み込む
	inline String ReadLine() { String s; do { std::getline(std::cin, s); } while (s.empty()); return s; }

	//	標準入力から、空白を含まない 1 単語を読み込む
	//	* 入力の終わりに達していた場合 `false` を返す
	inline bool ReadWord(String& s) { return !!(std::cin >> s); }

	//	標準入力から、空白も含んで 1 行を読み込む
	//	* 空白行の場合は無視して次の空白でない行を読み込む
	//	* 入力の終わりに達していた場合 `false` を返す
	inline bool ReadLine(String& s) { do { std::getline(std::cin, s); if (!std::cin) return false; } while (s.empty()); return true; }


	////////////////////////////////
	//
	//	8. 便利関数
	//
	////////////////////////////////

	namespace detail
	{
		struct Odd_impl
		{
			template <class Type> constexpr bool operator()(const Type& x) const { return (x % 2) != 0; }
		};

		struct Even_impl
		{
			template <class Type> constexpr bool operator()(const Type& x) const { return (x % 2) == 0; }
		};
	}

	// 奇数の場合に `true` を返す
	constexpr auto IsOdd = detail::Odd_impl();

	// 偶数の場合に `true` を返す
	constexpr auto IsEven = detail::Even_impl();
}

using namespace std;
using namespace yat;
//
////////////////////////////////////////////////////////////////
