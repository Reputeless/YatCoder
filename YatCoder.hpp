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
}

using namespace std;
using namespace yat;
//
////////////////////////////////////////////////////////////////
