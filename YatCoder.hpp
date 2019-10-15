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
}

using namespace std;
using namespace yat;
//
////////////////////////////////////////////////////////////////
