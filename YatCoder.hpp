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

	class String;

	class StringView
	{
	public:
		using traits_type				= std::char_traits<char>;
		using value_type				= char;
		using pointer					= const char*;
		using const_pointer				= const char*;
		using reference					= const char&;
		using const_reference			= const char&;
		using const_iterator			= pointer;
		using iterator					= const_iterator;
		using const_reverse_iterator	= std::reverse_iterator<const_iterator>;
		using reverse_iterator			= const_reverse_iterator;
		using size_type					= size_t;
		using difference_type			= ptrdiff_t;
	private:
		const char* m_ptr = nullptr;
		size_t m_length = 0;
	public:
		static constexpr size_type npos = size_type{ static_cast<size_type>(-1) };
		StringView() = default;
		StringView(const StringView&) = default;
		StringView(StringView&&) = default;
		StringView(const std::string& str) noexcept : m_ptr(str.data()), m_length(str.length()) {}
		constexpr StringView(const value_type* text, size_type count) : m_ptr(text), m_length(count) {}
		constexpr StringView(const value_type* text) : m_ptr(text), m_length(text ? traits_type::length(text) : 0) {}
		StringView& operator =(const StringView&) = default;
		StringView& operator =(StringView&&) = default;
		constexpr const_iterator begin() const noexcept { return m_ptr; }
		constexpr const_iterator end() const noexcept { return m_ptr + m_length; }
		constexpr const_iterator cbegin() const noexcept { return m_ptr; }
		constexpr const_iterator cend() const noexcept { return m_ptr + m_length; }
		const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
		const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
		const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
		const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }
		constexpr const_reference operator[](size_type offset) const { return m_ptr[offset]; }
		const_reference at(size_t offset) const { if (offset >= m_length) throw std::out_of_range("StringView::at"); return m_ptr[offset]; }
		constexpr const_reference front() const { return m_ptr[0]; }
		constexpr const_reference back()  const { return m_ptr[m_length - 1]; }
		constexpr const_pointer data()  const { return m_ptr; }
		constexpr size_type size() const noexcept { return m_length; }
		constexpr size_type size_bytes() const noexcept { return m_length * sizeof(value_type); }
		constexpr size_type length() const noexcept { return m_length; }
		constexpr size_type max_size() const noexcept { return m_length; }
		constexpr bool empty() const noexcept { return m_length == 0; }
		constexpr bool isEmpty() const noexcept { return m_length == 0; }
		constexpr explicit operator bool() const noexcept { return (!isEmpty()); }
		constexpr void remove_prefix(size_type n) noexcept
		{
			if (n > m_length) n = m_length;
			m_ptr += n;
			m_length -= n;
		}
		constexpr void remove_suffix(size_type n) noexcept
		{
			if (n > m_length) n = m_length;
			m_length -= n;
		}
		void swap(StringView& other) noexcept { std::swap(m_length, other.m_length); std::swap(m_ptr, other.m_ptr); }
		constexpr void clear() noexcept { *this = StringView(); }
		size_type copy(value_type* dst, const size_type n, const size_type pos = 0) const
		{
			if (pos > m_length)
				throw std::out_of_range("StringView::copy");
			const size_type rlen = std::min(n, m_length - pos);
			for (auto it = m_ptr + pos, end = it + rlen; it != end;)
				*dst++ = *it++;
			return rlen;
		}
		StringView substr(const size_type pos = 0, const size_type n = npos) const
		{
			if (pos > size())
				throw std::out_of_range("StringView::substr");
			return StringView(m_ptr + pos, std::min(n, m_length - pos));
		}
		int compare(StringView str) const
		{
			const int cmp = traits_type::compare(m_ptr, str.m_ptr, std::min(m_length, str.m_length));
			return cmp != 0 ? cmp : (m_length == str.m_length ? 0 : m_length < str.m_length ? -1 : 1);
		}
	};
	inline bool operator ==(const StringView x, const StringView y) noexcept { return x.compare(y) == 0; }
	inline bool operator !=(const StringView x, const StringView y) noexcept { return !(x == y); }
	inline bool operator <(const StringView x, const StringView y) noexcept { return x.compare(y) < 0; }
	inline bool operator >(const StringView x, const StringView y) noexcept { return x.compare(y) > 0; }
	inline bool operator <=(const StringView x, const StringView y) noexcept { return x.compare(y) <= 0; }
	inline bool operator >=(const StringView x, const StringView y) noexcept { return x.compare(y) >= 0; }

	//	文字列型
	class String
	{
	public:		
		using string_type				= std::string;
		using traits_type				= typename string_type::traits_type;
		using allocator_type			= typename string_type::allocator_type;
		using value_type				= typename string_type::value_type;
		using size_type					= typename string_type::size_type;
		using difference_type			= typename string_type::difference_type;
		using pointer					= typename string_type::pointer;
		using const_pointer				= typename string_type::const_pointer;
		using reference					= typename string_type::reference;
		using const_reference			= typename string_type::const_reference;
		using iterator					= typename string_type::iterator;
		using const_iterator			= typename string_type::const_iterator;
		using reverse_iterator			= typename string_type::reverse_iterator;
		using const_reverse_iterator	= typename string_type::const_reverse_iterator;
	private:
		string_type m_string;
	public:
		template <class StringViewIsh>
		using IsStringViewIsh = std::enable_if_t<
			std::is_convertible<const StringViewIsh&, StringView>::value &&
			!std::is_convertible<const StringViewIsh&, const char*>::value>;
		static constexpr size_type npos = size_type{ static_cast<size_type>(-1) };
		String() = default;
		String(const String& text) = default;
		String(String&& text) = default;
		template <class StringViewIsh, class = IsStringViewIsh<StringViewIsh>> explicit String(const StringViewIsh& viewish) : m_string(viewish.data(), viewish.size()) {}
		operator StringView() const noexcept { return StringView(m_string.data(), m_string.size()); }
		String(const string_type& text) : m_string(text) {}
		String(string_type&& text) noexcept : m_string(std::move(text)) {}
		String(const String& text, size_type pos) : m_string(text.m_string, pos) {}
		String(const String& text, size_type pos, size_type count) : m_string(text.m_string, pos, count) {}
		String(const value_type* text) : m_string(text) {}
		String(const value_type* text, size_type count) : m_string(text, count) {}
		String(std::initializer_list<value_type> ilist) : m_string(ilist) {}
		String(size_t count, value_type ch) : m_string(count, ch) {}
		String& operator =(const String& text) = default;
		String& operator =(String&& text) = default;
		String& operator =(const string_type& text) { return assign(text); }
		String& operator =(string_type&& text) noexcept { return assign(std::move(text)); }
		String& operator =(const value_type* text) { return assign(text); }
		String& operator =(std::initializer_list<value_type> ilist) { return assign(ilist); }
		template <class StringViewIsh, class = IsStringViewIsh<StringViewIsh>> String& operator =(const StringViewIsh& viewish) { return assign(viewish); }
		String& assign(const String& text) { m_string.assign(text.m_string); return *this; }
		String& assign(String&& text) { m_string.assign(std::move(text.m_string)); return *this; }
		String& assign(const string_type& text) { m_string.assign(text); return *this; }
		String& assign(string_type&& text) { m_string.assign(std::move(text)); return *this; }
		String& assign(const value_type* text) { m_string.assign(text); return *this; }
		String& assign(std::initializer_list<value_type> ilist) { m_string.assign(ilist); return *this; }
		template <class StringViewIsh, class = IsStringViewIsh<StringViewIsh>> String& assign(const StringViewIsh& viewish) { m_string.assign(viewish.data(), viewish.size()); return *this; }
		template <class Iterator> String& assign(Iterator first, Iterator last) { m_string.assign(first, last); return *this; }
		String& operator +=(const String& text) { return append(text); }
		String& operator +=(const string_type& text) { return append(text); }
		String& operator +=(value_type ch) { return append(ch); }
		String& operator +=(const value_type* text) { return append(text); }
		String& operator +=(std::initializer_list<value_type> ilist) { return append(ilist); }
		String& operator <<(const value_type ch) { return append(ch); }
		template <class StringViewIsh, class = IsStringViewIsh<StringViewIsh>> String& operator +=(const StringViewIsh& viewish) { return append(viewish); }
		String& append(const String& text) { m_string.append(text.m_string); return *this; }
		String& append(const string_type& text) { m_string.append(text); return *this; }
		String& append(value_type ch) { m_string.push_back(ch); return *this; }
		String& append(const value_type* text) { m_string.append(text); return *this; }
		String& append(const value_type* text, size_t count) { m_string.append(text, count); return *this; }
		String& append(std::initializer_list<value_type> ilist) { m_string.append(ilist); return *this; }
		String& append(size_t count, value_type ch) { m_string.append(count, ch); return *this; }
		template <class StringViewIsh, class = IsStringViewIsh<StringViewIsh>> String& append(const StringViewIsh& viewish) { m_string.append(viewish.data(), viewish.size()); return *this; }
		template <class Iterator> String& append(Iterator first, Iterator last) { m_string.append(first, last); return *this; }
		String& insert(size_t offset, const String& str) { m_string.insert(offset, str.m_string); return *this; }
		String& insert(size_t offset, std::initializer_list<value_type> ilist) { m_string.insert(offset, ilist); return *this; }
		String& insert(size_t offset, const value_type* str) { m_string.insert(offset, str); return *this; }
		template <class StringViewIsh, class = String::IsStringViewIsh<StringViewIsh>> String& insert(size_t offset, const StringViewIsh& text) { m_string.insert(offset, text.data(), text.size()); return *this; }
		String& insert(const size_t offset, size_t count, value_type ch) { m_string.insert(offset, count, ch); return *this; }
		iterator insert(const_iterator where, value_type ch) { return insert(where, 1, ch); }
		iterator insert(const_iterator where, size_t count, value_type ch)
		{
			const size_type off = std::distance(cbegin(), where);
			m_string.insert(off, count, ch);
			return begin() + static_cast<difference_type>(off);
		}
		template <class Iterator> iterator insert(const_iterator where, Iterator first, Iterator last) { return m_string.insert(where, first, last); }
		template <class Iterator> String& insert(const_iterator first1, const_iterator last1, Iterator first2, Iterator last2) { m_string.insert(first1, last1, first2, last2); return *this; }
		String& erase(size_t offset, size_t count = npos) { m_string.erase(offset, count); return *this; }
		iterator erase(const_iterator where) { return erase(where, where + 1); }
		iterator erase(const_iterator first, const_iterator last)
		{
			const size_type off = std::distance(cbegin(), first);
			erase(off, static_cast<size_type>(last - first));
			return begin() + static_cast<difference_type>(off);
		}
		void clear() noexcept { m_string.clear(); }
		iterator begin() noexcept { return m_string.begin(); }
		const_iterator begin() const noexcept { return m_string.begin(); }
		const_iterator cbegin() const noexcept { return m_string.cbegin(); }
		iterator end() noexcept { return m_string.end(); }
		const_iterator end() const noexcept { return m_string.end(); }
		const_iterator cend() const noexcept { return m_string.cend(); }
		reverse_iterator rbegin() noexcept { return m_string.rbegin(); }
		const_reverse_iterator rbegin() const noexcept { return m_string.rbegin(); }
		const_reverse_iterator crbegin() const noexcept { return m_string.crbegin(); }
		reverse_iterator rend() noexcept { return m_string.rend(); }
		const_reverse_iterator rend() const noexcept { return m_string.rend(); }
		const_reverse_iterator crend() const noexcept { return m_string.crend(); }
		void shrink_to_fit() { m_string.shrink_to_fit(); }
		void release() { clear(); shrink_to_fit(); }
		value_type& at(size_t offset)& { return m_string.at(offset); }
		const value_type& at(size_t offset) const& { return m_string.at(offset); }
		value_type at(size_t offset)&& { return m_string.at(offset); }
		value_type& operator[](size_t offset)& { return m_string[offset]; }
		const value_type& operator[](size_t offset) const& { return m_string[offset]; }
		value_type operator[](size_t offset)&& { return std::move(m_string[offset]); }
		void push_front(value_type ch) { insert(begin(), ch); }
		void push_back(value_type ch) { m_string.push_back(ch); }
		void pop_front() { m_string.erase(m_string.begin()); }
		void pop_back() { m_string.pop_back(); }
		value_type& front() { return m_string.front(); }
		const value_type& front() const { return m_string.front(); }
		value_type& back() { return m_string.back(); }
		const value_type& back() const { return m_string.back(); }
		const value_type* c_str() const noexcept { return m_string.c_str(); }
		const value_type* data() const noexcept { return m_string.data(); }
		value_type* data() noexcept { return &m_string[0]; }
		string_type& str() { return m_string; }
		const string_type& str() const noexcept { return m_string; }
		size_t length() const noexcept { return m_string.length(); }
		size_t size() const noexcept { return m_string.size(); }
		size_t size_bytes() const noexcept { return m_string.size() * sizeof(value_type); }
		bool empty() const noexcept { return m_string.empty(); }
		bool isEmpty() const noexcept { return m_string.empty(); }
		explicit operator bool() const noexcept { return !m_string.empty(); }
		size_t maxSize() const noexcept { return m_string.max_size(); }
		size_t capacity() const noexcept { return m_string.capacity(); }
		void resize(size_t newSize) { m_string.resize(newSize); }
		void resize(size_t newSize, value_type ch) { m_string.resize(newSize, ch); }
		void reserve(size_t newCapacity) { m_string.reserve(newCapacity); }
		void swap(String& text) noexcept { m_string.swap(text.m_string); }
		String substr(size_t offset = 0, size_t count = npos) const { return m_string.substr(offset, count); }
		//size_t indexOf(const String& text, size_t offset = 0) const noexcept;
		//size_t indexOf(const value_type* text, size_t offset = 0) const;
		//size_t indexOf(value_type ch, size_t offset = 0) const;
		//size_t indexOfNot(value_type ch, size_t offset = 0) const;
		//size_t lastIndexOf(const String& text, size_t offset = npos) const;
		//size_t lastIndexOf(const value_type* text, size_t offset = npos) const;
		//size_t lastIndexOf(value_type ch, size_t offset = npos) const;
		//size_t lastIndexNotOf(value_type ch, size_t offset = npos) const;
		//size_t indexOfAny(const String& anyof, size_t offset = 0) const;
		//size_t indexOfAny(const value_type* anyof, size_t offset = 0) const;
		//size_t lastIndexOfAny(const String& anyof, size_t offset = npos) const;
		//size_t lastIndexOfAny(const value_type* anyof, size_t offset = npos) const;
		//size_t indexNotOfAny(const String& anyof, size_t offset = 0) const;
		//size_t indexNotOfAny(const value_type* anyof, size_t offset = 0) const;
		//size_t lastIndexNotOfAny(const String& anyof, size_t offset = npos) const;
		//size_t lastIndexNotOfAny(const value_type* anyof, size_t offset = npos) const;
		int32 compare(const String& text) const noexcept { return m_string.compare(text.m_string); }
		int32 compare(const value_type* text) const
		{
			return m_string.compare(text);
		}
	};
	template <class StringViewIsh, class = String::IsStringViewIsh<StringViewIsh>>
	inline String operator +(const String::value_type lhs, const StringViewIsh& rhs)
	{
		String result;
		result.reserve(1 + rhs.size());
		result.append(lhs);
		result.append(rhs);
		return result;
	}
	String operator +(const String::value_type lhs, const String& rhs)
	{
		String result;
		result.reserve(1 + rhs.size());
		result.append(lhs);
		result.append(rhs);
		return result;
	}
	String operator +(const String::value_type lhs, String&& rhs) { rhs.push_front(lhs); return std::move(rhs); }
	template <class StringViewIsh, class = String::IsStringViewIsh<StringViewIsh>>
	inline String operator +(const String::value_type* lhs, const StringViewIsh& rhs)
	{
		const size_t len = std::char_traits<String::value_type>::length(lhs);
		String result;
		result.reserve(len + rhs.size());
		result.append(lhs, len);
		result.append(rhs);
		return result;
	}
	String operator +(const String::value_type* lhs, const String& rhs)
	{
		const size_t len = std::char_traits<String::value_type>::length(lhs);
		String result;
		result.reserve(len + rhs.size());
		result.append(lhs, len);
		result.append(rhs);
		return result;
	}
	String operator +(const String::value_type* lhs, String&& rhs) { return std::move(rhs.insert(0, lhs)); }
	template <class StringViewIsh, class = String::IsStringViewIsh<StringViewIsh>>
	inline String operator +(const StringViewIsh& lhs, const String::value_type rhs)
	{
		String result;
		result.reserve(lhs.size() + 1);
		result.append(lhs);
		result.append(rhs);
		return result;
	}
	template <class StringViewIsh, class = String::IsStringViewIsh<StringViewIsh>>
	inline String operator +(const StringViewIsh& lhs, const String::value_type* rhs)
	{
		const size_t len = std::char_traits<String::value_type>::length(rhs);
		String result;
		result.reserve(lhs.size() + len);
		result.append(lhs);
		result.append(rhs, len);
		return result;
	}
	template <class StringViewIshT, class StringViewIshU, class = String::IsStringViewIsh<StringViewIshT>, class = String::IsStringViewIsh<StringViewIshU>>
	inline String operator +(const StringViewIshT& lhs, const StringViewIshU& rhs)
	{
		String result;
		result.reserve(lhs.size() + rhs.size());
		result.append(lhs);
		result.append(rhs);
		return result;
	}
	template <class StringViewIsh, class = String::IsStringViewIsh<StringViewIsh>>
	inline String operator +(const StringViewIsh& lhs, const String& rhs)
	{
		String result;
		result.reserve(lhs.size() + rhs.size());
		result.append(lhs);
		result.append(rhs);
		return result;
	}
	template <class StringViewIsh, class = String::IsStringViewIsh<StringViewIsh>> inline String operator +(const StringViewIsh& lhs, String&& rhs) { return std::move(rhs.insert(0, lhs)); }
	String operator +(const String& lhs, const String::value_type rhs)
	{
		String result;
		result.reserve(lhs.size() + 1);
		result.append(lhs);
		result.append(rhs);
		return result;
	}
	String operator +(const String& lhs, const String::value_type* rhs)
	{
		const size_t len = std::char_traits<String::value_type>::length(rhs);
		String result;
		result.reserve(lhs.size() + len);
		result.append(lhs);
		result.append(rhs, len);
		return result;
	}
	template <class StringViewIsh, class = String::IsStringViewIsh<StringViewIsh>>
	inline String operator +(const String& lhs, const StringViewIsh& rhs)
	{
		String result;
		result.reserve(lhs.size() + rhs.size());
		result.append(lhs);
		result.append(rhs);
		return result;
	}
	String operator +(const String& lhs, const String& rhs)
	{
		String result;
		result.reserve(lhs.size() + rhs.size());
		result.append(lhs);
		result.append(rhs);
		return result;
	}
	String operator +(const String& lhs, String&& rhs) { return std::move(rhs.insert(0, lhs)); }
	String operator +(String&& lhs, const String::value_type rhs) { return std::move(lhs << rhs); }
	String operator +(String&& lhs, const String::value_type* rhs) { return std::move(lhs.append(rhs)); }
	template <class StringViewIsh, class = String::IsStringViewIsh<StringViewIsh>> inline String operator +(String&& lhs, const StringViewIsh& rhs) { return std::move(lhs.append(rhs)); }
	String operator +(String&& lhs, const String& rhs) { return std::move(lhs.append(rhs)); }
	String operator +(String&& lhs, String&& rhs)
	{
		if (rhs.size() <= lhs.capacity() - lhs.size() || rhs.capacity() - rhs.size() < lhs.size()) return std::move(lhs.append(rhs));
		else return std::move(rhs.insert(0, lhs));
	}
	bool operator ==(const String::value_type* lhs, const String& rhs) { return lhs == rhs.str(); }
	bool operator ==(const String& lhs, const String::value_type* rhs) { return lhs.str() == rhs; }
	bool operator !=(const String::value_type* lhs, const String& rhs) { return lhs != rhs.str(); }
	bool operator !=(const String& lhs, const String::value_type* rhs) { return lhs.str() != rhs; }
	bool operator <(const String::value_type* lhs, const String& rhs) { return lhs < rhs.str(); }
	bool operator <(const String& lhs, const String::value_type* rhs) { return lhs.str() < rhs; }
	bool operator >(const String::value_type* lhs, const String& rhs) { return lhs > rhs.str(); }
	bool operator >(const String& lhs, const String::value_type* rhs) { return lhs.str() > rhs; }
	bool operator <=(const String::value_type* lhs, const String& rhs) { return lhs <= rhs.str(); }
	bool operator <=(const String& lhs, const String::value_type* rhs) { return lhs.str() <= rhs; }
	bool operator >=(const String::value_type* lhs, const String& rhs) { return lhs >= rhs.str(); }
	bool operator >=(const String& lhs, const String::value_type* rhs) { return lhs.str() >= rhs; }
	std::ostream& operator <<(std::ostream& os, const String& x) { return os << x.str(); }
	std::istream& operator >>(std::istream& is, String& x) { return is >> x.str(); }


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
	inline String ReadLine() { String s; do { std::getline(std::cin, s.str()); } while (s.empty()); return s; }

	//	標準入力から、空白を含まない 1 単語を読み込む
	//	* 入力の終わりに達していた場合 `false` を返す
	inline bool ReadWord(String& s) { return !!(std::cin >> s); }

	//	標準入力から、空白も含んで 1 行を読み込む
	//	* 空白行の場合は無視して次の空白でない行を読み込む
	//	* 入力の終わりに達していた場合 `false` を返す
	inline bool ReadLine(String& s) { do { std::getline(std::cin, s.str()); if (!std::cin) return false; } while (s.empty()); return true; }


	////////////////////////////////
	//
	//	8. 便利関数
	//
	////////////////////////////////

	namespace detail
	{
		struct Odd_impl  { template <class Type> constexpr bool operator()(const Type& x) const { return (x % 2) != 0; } };
		struct Even_impl { template <class Type> constexpr bool operator()(const Type& x) const { return (x % 2) == 0; } };
	}

	// 奇数の場合に `true` を返す
	constexpr auto IsOdd = detail::Odd_impl();

	// 偶数の場合に `true` を返す
	constexpr auto IsEven = detail::Even_impl();
}

namespace std
{
	inline void swap(yat::StringView& a, yat::StringView& b) noexcept { a.swap(b); }
	inline void swap(yat::String& a, yat::String& b) noexcept { a.swap(b); }
}

using namespace std;
using namespace yat;
//
////////////////////////////////////////////////////////////////
