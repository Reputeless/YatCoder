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
	//	X. 内部実装
	//
	////////////////////////////////
	
	namespace detail
	{
		struct Id_impl{ template <class Type> constexpr decltype(auto) operator()(Type&& x) const noexcept { return std::forward<Type>(x); }};
	}
	constexpr auto Id = detail::Id_impl();

	template <class Type = void> inline constexpr auto Plus() noexcept { return std::plus<Type>(); }

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
	inline bool operator ==(const StringView a, const StringView b) noexcept { return a.compare(b) == 0; }
	inline bool operator !=(const StringView a, const StringView b) noexcept { return !(a == b); }
	inline bool operator <(const StringView a, const StringView b) noexcept { return a.compare(b) < 0; }
	inline bool operator >(const StringView a, const StringView b) noexcept { return a.compare(b) > 0; }
	inline bool operator <=(const StringView a, const StringView b) noexcept { return a.compare(b) <= 0; }
	inline bool operator >=(const StringView a, const StringView b) noexcept { return a.compare(b) >= 0; }

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
		int32 compare(const value_type* text) const { return m_string.compare(text); }
		bool operator ==(const String& text) const { return m_string == text.m_string; }
		bool operator !=(const String& text) const { return m_string != text.m_string; }
		bool operator <(const String& text) const { return m_string < text.m_string; }
		bool operator >(const String& text) const {	return m_string > text.m_string; }
		bool operator <=(const String& text) const { return m_string <= text.m_string; }
		bool operator >=(const String& text) const { return m_string >= text.m_string; }
		size_t count(value_type ch) const noexcept { return std::count(begin(), end(), ch); }
		size_t count(StringView view) const
		{
			size_t count = 0;
			for (auto it = begin();; ++it, ++count)
			{
				it = std::search(it, end(), view.begin(), view.end());
				if (it == end())
					return count;
			}
		}
	};
	template <class StringViewIsh, class = String::IsStringViewIsh<StringViewIsh>>
	inline String operator +(const String::value_type a, const StringViewIsh& b)
	{
		String result;
		result.reserve(1 + b.size());
		result.append(a);
		result.append(b);
		return result;
	}
	String operator +(const String::value_type a, const String& b)
	{
		String result;
		result.reserve(1 + b.size());
		result.append(a);
		result.append(b);
		return result;
	}
	String operator +(const String::value_type a, String&& b) { b.push_front(a); return std::move(b); }
	template <class StringViewIsh, class = String::IsStringViewIsh<StringViewIsh>>
	inline String operator +(const String::value_type* a, const StringViewIsh& b)
	{
		const size_t len = std::char_traits<String::value_type>::length(a);
		String result;
		result.reserve(len + b.size());
		result.append(a, len);
		result.append(b);
		return result;
	}
	String operator +(const String::value_type* a, const String& b)
	{
		const size_t len = std::char_traits<String::value_type>::length(a);
		String result;
		result.reserve(len + b.size());
		result.append(a, len);
		result.append(b);
		return result;
	}
	String operator +(const String::value_type* a, String&& b) { return std::move(b.insert(0, a)); }
	template <class StringViewIsh, class = String::IsStringViewIsh<StringViewIsh>>
	inline String operator +(const StringViewIsh& a, const String::value_type b)
	{
		String result;
		result.reserve(a.size() + 1);
		result.append(a);
		result.append(b);
		return result;
	}
	template <class StringViewIsh, class = String::IsStringViewIsh<StringViewIsh>>
	inline String operator +(const StringViewIsh& a, const String::value_type* b)
	{
		const size_t len = std::char_traits<String::value_type>::length(b);
		String result;
		result.reserve(a.size() + len);
		result.append(a);
		result.append(b, len);
		return result;
	}
	template <class StringViewIshT, class StringViewIshU, class = String::IsStringViewIsh<StringViewIshT>, class = String::IsStringViewIsh<StringViewIshU>>
	inline String operator +(const StringViewIshT& a, const StringViewIshU& b)
	{
		String result;
		result.reserve(a.size() + b.size());
		result.append(a);
		result.append(b);
		return result;
	}
	template <class StringViewIsh, class = String::IsStringViewIsh<StringViewIsh>>
	inline String operator +(const StringViewIsh& a, const String& b)
	{
		String result;
		result.reserve(a.size() + b.size());
		result.append(a);
		result.append(b);
		return result;
	}
	template <class StringViewIsh, class = String::IsStringViewIsh<StringViewIsh>> inline String operator +(const StringViewIsh& a, String&& b) { return std::move(b.insert(0, a)); }
	String operator +(const String& a, const String::value_type b)
	{
		String result;
		result.reserve(a.size() + 1);
		result.append(a);
		result.append(b);
		return result;
	}
	String operator +(const String& a, const String::value_type* b)
	{
		const size_t len = std::char_traits<String::value_type>::length(b);
		String result;
		result.reserve(a.size() + len);
		result.append(a);
		result.append(b, len);
		return result;
	}
	template <class StringViewIsh, class = String::IsStringViewIsh<StringViewIsh>>
	inline String operator +(const String& a, const StringViewIsh& b)
	{
		String result;
		result.reserve(a.size() + b.size());
		result.append(a);
		result.append(b);
		return result;
	}
	String operator +(const String& a, const String& b)
	{
		String result;
		result.reserve(a.size() + b.size());
		result.append(a);
		result.append(b);
		return result;
	}
	String operator +(const String& a, String&& b) { return std::move(b.insert(0, a)); }
	String operator +(String&& a, const String::value_type b) { return std::move(a << b); }
	String operator +(String&& a, const String::value_type* b) { return std::move(a.append(b)); }
	template <class StringViewIsh, class = String::IsStringViewIsh<StringViewIsh>> inline String operator +(String&& a, const StringViewIsh& b) { return std::move(a.append(b)); }
	String operator +(String&& a, const String& b) { return std::move(a.append(b)); }
	String operator +(String&& a, String&& b)
	{
		if (b.size() <= a.capacity() - a.size() || b.capacity() - b.size() < a.size()) return std::move(a.append(b));
		else return std::move(b.insert(0, a));
	}
	bool operator ==(const String::value_type* a, const String& b) { return a == b.str(); }
	bool operator ==(const String& a, const String::value_type* b) { return a.str() == b; }
	bool operator !=(const String::value_type* a, const String& b) { return a != b.str(); }
	bool operator !=(const String& a, const String::value_type* b) { return a.str() != b; }
	bool operator <(const String::value_type* a, const String& b) { return a < b.str(); }
	bool operator <(const String& a, const String::value_type* b) { return a.str() < b; }
	bool operator >(const String::value_type* a, const String& b) { return a > b.str(); }
	bool operator >(const String& a, const String::value_type* b) { return a.str() > b; }
	bool operator <=(const String::value_type* a, const String& b) { return a <= b.str(); }
	bool operator <=(const String& a, const String::value_type* b) { return a.str() <= b; }
	bool operator >=(const String::value_type* a, const String& b) { return a >= b.str(); }
	bool operator >=(const String& a, const String::value_type* b) { return a.str() >= b; }
	std::ostream& operator <<(std::ostream& os, const String& x) { return os << x.str(); }
	std::istream& operator >>(std::istream& is, String& x) { return is >> x.str(); }


	////////////////////////////////
	//
	//	4. 動的配列型
	//
	////////////////////////////////

	//	動的配列型
	template <class Type, class Allocator = std::allocator<Type>>
	class Array : protected std::vector<Type, Allocator>
	{
	private:
		using base_type = std::vector<Type, Allocator>;
	public:
		using typename base_type::value_type;
		using typename base_type::pointer;
		using typename base_type::const_pointer;
		using typename base_type::reference;
		using typename base_type::const_reference;
		using typename base_type::iterator;
		using typename base_type::const_iterator;
		using typename base_type::reverse_iterator;
		using typename base_type::const_reverse_iterator;
		using typename base_type::size_type;
		using typename base_type::difference_type;
		using typename base_type::allocator_type;
		using base_type::vector;
		using base_type::operator=;
		using base_type::assign;
		using base_type::get_allocator;
		using base_type::front;
		using base_type::back;
		using base_type::data;
		using base_type::begin;
		using base_type::cbegin;
		using base_type::end;
		using base_type::cend;
		using base_type::rbegin;
		using base_type::crbegin;
		using base_type::rend;
		using base_type::crend;
		using base_type::empty;
		using base_type::size;
		using base_type::max_size;
		using base_type::reserve;
		using base_type::capacity;
		using base_type::shrink_to_fit;
		using base_type::clear;
		using base_type::insert;
		using base_type::emplace;
		using base_type::erase;
		using base_type::push_back;
		using base_type::emplace_back;
		using base_type::pop_back;
		using base_type::resize;
		Array(): base_type() {}
		Array(const Array&) = default;
		Array(Array &&) = default;
		Array& operator =(const Array&) = default;
		Array& operator =(Array &&) = default;
		void swap(Array& other) noexcept { base_type::swap(other); }
		bool isEmpty() const noexcept { return empty(); }
		explicit operator bool() const noexcept { return !empty(); }
		void release() { clear(); shrink_to_fit(); }
		size_t size_bytes() const noexcept { static_assert(std::is_trivially_copyable<value_type>::value, "Array::size_bytes() value_type must be trivially copyable."); return size() * sizeof(value_type); }
		void push_front(const value_type& value) { insert(begin(), value); }
		void push_front(value_type&& value) { insert(begin(), std::move(value)); }
		void pop_front() { erase(begin()); }
		const value_type& operator[](size_t index) const& { return base_type::operator[](index); }
		value_type& operator[](size_t index)& { return base_type::operator[](index); }
		value_type operator[](size_t index)&& { return std::move(base_type::operator[](index)); }
		const value_type& at(size_t index) const& { return base_type::at(index); }
		value_type& at(size_t index)& { return base_type::at(index); }
		value_type at(size_t index)&& { return std::move(base_type::at(index)); }
		Array& operator <<(const value_type& value) { push_back(value); return *this; }
		Array& operator <<(value_type&& value) { push_back(std::forward<value_type>(value)); return *this; }
		template <class Fty = decltype(Id), std::enable_if_t<std::is_convertible<std::result_of_t<Fty(Type)>, bool>::value>* = nullptr>
		bool all(Fty f = Id) const { return std::all_of(begin(), end(), f); }
		template <class Fty = decltype(Id), std::enable_if_t<std::is_convertible<std::result_of_t<Fty(Type)>, bool>::value> * = nullptr>
		bool any(Fty f = Id) const { return std::any_of(begin(), end(), f); }
		Array& append(const Array& other_array) { insert(end(), other_array.begin(), other_array.end()); return *this; }
		Array<Array<value_type>> chunk(const size_t n) const
		{
			Array<Array<value_type>> result;

			if (n == 0)
			{
				return result;
			}

			for (size_t i = 0; i < (size() + n - 1) / n; ++i)
			{
				result.push_back(slice(i * n, n));
			}

			return result;
		}
		size_t count(const value_type& value) const
		{
			size_t result = 0;

			for (const auto& v : *this)
			{
				if (v == value)
				{
					++result;
				}
			}

			return result;
		}
		template <class Fty>
		size_t count_if(Fty f) const
		{
			size_t result = 0;

			for (const auto& v : *this)
			{
				if (f(v))
				{
					++result;
				}
			}

			return result;
		}
		Array& drop(size_t n)
		{
			erase(begin(), begin() + std::min(n, size()));

			return *this;
		}
		Array dropped(const size_t n) const
		{
			if (n >= size())
			{
				return Array();
			}

			return Array(begin() + n, end());
		}
		template <class Fty>
		Array dropped_while(Fty f) const
		{
			return Array(std::find_if_not(begin(), end(), f), end());
		}
		template <class Fty> Array& each(Fty f) { std::for_each(begin(), end(), f); return *this; }
		template <class Fty>
		const Array& each(Fty f) const
		{
			for (const auto& v : *this)
			{
				f(v);
			}

			return *this;
		}
		template <class Fty>
		Array& each_index(Fty f)
		{
			size_t i = 0;

			for (auto& v : *this)
			{
				f(i++, v);
			}

			return *this;
		}
		template <class Fty>
		const Array& each_index(Fty f) const
		{
			size_t i = 0;

			for (const auto& v : *this)
			{
				f(i++, v);
			}

			return *this;
		}
		const value_type& fetch(const size_t index, const value_type& defaultValue) const
		{
			if (index >= size())
			{
				return defaultValue;
			}

			return operator[](index);
		}
		Array& fill(const value_type& value)
		{
			std::fill(begin(), end(), value);

			return *this;
		}
		template <class Fty>
		Array filter(Fty f) const
		{
			Array new_array;

			for (const auto& v : *this)
			{
				if (f(v))
				{
					new_array.push_back(v);
				}
			}

			return new_array;
		}
		Array<Array<value_type>> in_groups(const size_t group) const
		{
			Array<Array<value_type>> result;

			if (group == 0)
			{
				return result;
			}

			const size_t div = size() / group;
			const size_t mod = size() % group;
			size_t index = 0;

			for (size_t i = 0; i < group; ++i)
			{
				const size_t length = div + (mod > 0 && mod > i);

				result.push_back(slice(index, length));

				index += length;
			}

			return result;
		}
		bool includes(const value_type& value) const
		{
			for (const auto& v : *this)
			{
				if (v == value)
				{
					return true;
				}
			}

			return false;
		}
		template <class Fty>
		bool includes_if(Fty f) const
		{
			return any(f);
		}
		template <class T = Type>
		bool isSorted() const
		{
			const size_t size_ = size();

			if (size_ <= 1)
			{
				return true;
			}

			const value_type* p = data();

			for (size_t i = 0; i < size_ - 1; ++i)
			{
				if (p[i] > p[i + 1])
				{
					return false;
				}
			}

			return true;
		}
		String join(StringView sep = ", ", StringView begin = "{", StringView end = "}") const
		{
			String s;

			s.append(begin);

			bool isFirst = true;

			for (const auto& v : *this)
			{
				if (isFirst)
				{
					isFirst = false;
				}
				else
				{
					s.append(sep);
				}

				s.append(Format(v));
			}

			s.append(end);

			return s;
		}
		template <class Fty>
		Array& keep_if(Fty f)
		{
			erase(std::remove_if(begin(), end(), std::not1(f)), end());

			return *this;
		}
		template <class Fty>
		auto map(Fty f) const
		{
			Array<std::decay_t<std::result_of_t<Fty(Type)>>> new_array;

			new_array.reserve(size());

			for (const auto& v : *this)
			{
				new_array.push_back(f(v));
			}

			return new_array;
		}
		template <class Fty = decltype(Id), std::enable_if_t<std::is_convertible<std::result_of_t<Fty(Type)>, bool>::value> * = nullptr>
		bool none(Fty f = Id) const { return std::none_of(begin(), end(), f); }
		template <class Fty, class R = std::decay_t<std::result_of_t<Fty(Type, Type)>>>
		auto reduce(Fty f, R init) const
		{
			auto value = init;

			for (const auto& v : *this)
			{
				value = f(value, v);
			}

			return value;
		}
		template <class Fty>
		auto reduce1(Fty f) const
		{
			if (empty())
			{
				throw std::out_of_range("Array::reduce1() reduce from empty Array");
			}

			auto it = begin();
			const auto itEnd = end();

			std::result_of_t<Fty(value_type, value_type)> value = *it++;

			while (it != itEnd)
			{
				value = f(value, *it++);
			}

			return value;
		}
		Array& remove(const value_type& value)
		{
			erase(std::remove(begin(), end(), value), end());

			return *this;
		}
		Array removed(const value_type& value) const&
		{
			Array new_array;

			for (const auto& v : *this)
			{
				if (v != value)
				{
					new_array.push_back(v);
				}
			}

			return new_array;
		}
		Array removed(const value_type& value)&&
		{
			erase(std::remove(begin(), end(), value), end());

			shrink_to_fit();

			return std::move(*this);
		}
		Array& remove_at(const size_t index)
		{
			if (index >= size())
			{
				throw std::out_of_range("Array::remove_at() index out of range");
			}

			erase(begin() + index);

			return *this;
		}
		Array removed_at(const size_t index) const
		{
			if (index >= size())
			{
				throw std::out_of_range("Array::removed_at() index out of range");
			}

			Array new_array;

			new_array.reserve(size() - 1);

			new_array.insert(new_array.end(), begin(), begin() + index);

			new_array.insert(new_array.end(), begin() + index + 1, end());

			return new_array;
		}
		template <class Fty>
		Array& remove_if(Fty f)
		{
			erase(std::remove_if(begin(), end(), f), end());

			return *this;
		}
		template <class Fty>
		Array removed_if(Fty f) const&
		{
			Array new_array;

			for (const auto& v : *this)
			{
				if (!f(v))
				{
					new_array.push_back(v);
				}
			}

			return new_array;
		}
		template <class Fty>
		Array removed_if(Fty f)&&
		{
			erase(std::remove_if(begin(), end(), f), end());

			shrink_to_fit();

			return std::move(*this);
		}
		Array& replace(const value_type& oldValue, const value_type& newValue)
		{
			for (auto& v : *this)
			{
				if (v == oldValue)
				{
					v = newValue;
				}
			}

			return *this;
		}
		Array replaced(const value_type& oldValue, const value_type& newValue) const&
		{
			Array new_array;

			new_array.reserve(size());

			for (const auto& v : *this)
			{
				if (v == oldValue)
				{
					new_array.push_back(newValue);
				}
				else
				{
					new_array.push_back(v);
				}
			}

			return new_array;
		}
		Array replaced(const value_type& oldValue, const value_type& newValue)&&
		{
			replace(oldValue, newValue);

			return std::move(*this);
		}
		template <class Fty>
		Array& replace_if(Fty f, const value_type& newValue)
		{
			for (auto& v : *this)
			{
				if (f(v))
				{
					v = newValue;
				}
			}

			return *this;
		}
		template <class Fty>
		Array replaced_if(Fty f, const value_type& newValue) const&
		{
			Array new_array;

			new_array.reserve(size());

			for (const auto& v : *this)
			{
				if (f(v))
				{
					new_array.push_back(newValue);
				}
				else
				{
					new_array.push_back(v);
				}
			}

			return new_array;
		}
		template <class Fty>
		Array replaced_if(Fty f, const value_type& newValue)&&
		{
			replace_if(f, newValue);

			return std::move(*this);
		}
		Array& reverse()
		{
			std::reverse(begin(), end());

			return *this;
		}
		Array reversed() const&
		{
			return Array(rbegin(), rend());
		}
		Array reversed()&&
		{
			reverse();

			return std::move(*this);
		}
		template <class Fty>
		Array& reverse_each(Fty f)
		{
			for (auto it = rbegin(); it != rend(); ++it)
			{
				f(*it);
			}

			return *this;
		}
		template <class Fty>
		const Array& reverse_each(Fty f) const
		{
			for (auto it = rbegin(); it != rend(); ++it)
			{
				f(*it);
			}

			return *this;
		}
		Array& rotate(std::ptrdiff_t count = 1)
		{
			if (empty())
			{
				;
			}
			else if (count > 0) // rotation to the left
			{
				if (static_cast<size_t>(count) > size())
				{
					count %= size();
				}

				std::rotate(begin(), begin() + count, end());
			}
			else if (count < 0) // rotation to the right
			{
				count = -count;

				if (static_cast<size_t>(count) > size())
				{
					count %= size();
				}

				std::rotate(rbegin(), rbegin() + count, rend());
			}

			return *this;
		}
		Array rotated(const std::ptrdiff_t count = 1) const&
		{
			return Array(*this).rotate(count);
		}
		Array rotated(const std::ptrdiff_t count = 1)&&
		{
			rotate(count);

			return std::move(*this);
		}
		template <class T = Type>
		Array& rsort()
		{
			std::sort(begin(), end(), std::greater<>());
			return *this;
		}
		template <class T = Type>
		Array rsorted() const&
		{
			return Array(*this).rsort();
		}
		template <class T = Type>
		Array rsorted()&&
		{
			rsort();

			return std::move(*this);
		}
		Array slice(const size_t index) const
		{
			if (index >= size())
			{
				return Array();
			}

			return Array(begin() + index, end());
		}
		Array slice(const size_t index, const size_t length) const
		{
			if (index >= size())
			{
				return Array();
			}

			return Array(begin() + index, begin() + std::min(index + length, size()));
		}
		template <class T = Type>
		Array& sort()
		{
			std::sort(begin(), end());

			return *this;
		}
		template <class T = Type>
		Array& stable_sort()
		{
			std::stable_sort(begin(), end());

			return *this;
		}
		template <class Fty>
		Array& sort_by(Fty f)
		{
			std::sort(begin(), end(), f);

			return *this;
		}
		template <class Fty>
		Array& stable_sort_by(Fty f)
		{
			std::stable_sort(begin(), end(), f);

			return *this;
		}
		template <class T = Type>
		Array sorted() const&
		{
			return Array(*this).sort();
		}
		template <class T = Type>
		Array stable_sorted() const&
		{
			return Array(*this).stable_sort();
		}
		template <class T = Type>
		Array sorted()&&
		{
			sort();

			return std::move(*this);
		}
		template <class T = Type>
		Array stable_sorted()&&
		{
			stable_sort();

			return std::move(*this);
		}
		template <class Fty>
		Array sorted_by(Fty f) const&
		{
			return Array(*this).sort_by(f);
		}
		template <class Fty>
		Array stable_sorted_by(Fty f) const&
		{
			return Array(*this).stable_sort_by(f);
		}
		template <class Fty>
		Array sorted_by(Fty f)&&
		{
			sort_by(f);

			return std::move(*this);
		}
		template <class Fty>
		Array stable_sorted_by(Fty f)&&
		{
			stable_sort_by(f);

			return std::move(*this);
		}
		template <class T = Type>
		auto sum() const
		{
			decltype(std::declval<T>() + std::declval<T>()) result{};

			for (const auto& v : *this)
			{
				result += v;
			}

			return result;
		}
		//template <class T = Type, std::enable_if_t<Meta::HasPlus_v<T> && !Meta::HasPlusAssign_v<T>> * = nullptr>
		//auto sum() const
		//{
		//	decltype(std::declval<T>() + std::declval<T>()) result{};

		//	for (const auto& v : *this)
		//	{
		//		result = result + v;
		//	}

		//	return result;
		//}
		template <class T = Type, std::enable_if_t<std::is_floating_point<T>::value> * = nullptr>
		auto sumF() const&
		{
			T s = 0.0;
			T err = 0.0;

			for (const auto& v : *this)
			{
				const T y = v - err;
				const T t = s + y;
				err = (t - s) - y;
				s = t;
			}

			return static_cast<T>(s);
		}
		template <class T = Type, std::enable_if_t<!std::is_floating_point<T>::value> * = nullptr>
		auto sumF() const& = delete;
		Array take(const size_t n) const
		{
			return Array(begin(), begin() + std::min(n, size()));
		}
		template <class Fty>
		Array take_while(Fty f) const
		{
			return Array(begin(), std::find_if_not(begin(), end(), f));
		}
		Array& unique()
		{
			sort();

			erase(std::unique(begin(), end()), end());

			return *this;
		}
		Array uniqued() const&
		{
			return Array(*this).unique();
		}
		Array uniqued()&&
		{
			sort();

			erase(std::unique(begin(), end()), end());

			shrink_to_fit();

			return std::move(*this);
		}
		Array values_at(std::initializer_list<size_t> indices) const
		{
			Array new_array;

			new_array.reserve(indices.size());

			for (auto index : indices)
			{
				if (index >= size())
				{
					throw std::out_of_range("Array::values_at() index out of range");
				}

				new_array.push_back(operator[](index));
			}

			return new_array;
		}
	};

	template <class Type, class Allocator>
	inline bool operator ==(const Array<Type, Allocator>& a, const Array<Type, Allocator>& b)
	{
		return ((a.size() == b.size()) && std::equal(a.begin(), a.end(), b.begin()));
	}

	template <class Type, class Allocator>
	inline bool operator !=(const Array<Type, Allocator>& a, const Array<Type, Allocator>& b)
	{
		return ((a.size() != b.size()) || !std::equal(a.begin(), a.end(), b.begin()));
	}

	template <class Type, class Allocator>
	inline bool operator <(const Array<Type, Allocator>& a, const Array<Type, Allocator>& b)
	{
		return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
	}

	template <class Type, class Allocator>
	inline bool operator >(const Array<Type, Allocator>& a, const Array<Type, Allocator>& b)
	{
		return b < a;
	}

	template <class Type, class Allocator>
	inline bool operator <=(const Array<Type, Allocator>& a, const Array<Type, Allocator>& b)
	{
		return !(b < a);
	}

	template <class Type, class Allocator>
	inline bool operator >=(const Array<Type, Allocator>& a, const Array<Type, Allocator>& b)
	{
		return !(a < b);
	}

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

	template <class Iterator> inline void Formatter(FormatData& f, Iterator begin, Iterator end)
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
	template <class Type, size_t N> inline void Formatter(FormatData& f, const Type(&v)[N]) { Formatter(f, std::begin(v), std::end(v)); }
	template <class Type, size_t N> inline void Formatter(FormatData& f, const std::array<Type, N>& v) { Formatter(f, v.begin(), v.end()); }
	template <class Type, class Allocator = std::allocator<Type>> inline void Formatter(FormatData& f, const std::vector<Type, Allocator>& v) { Formatter(f, v.begin(), v.end()); }
	template <class Type> inline void Formatter(FormatData& f, const std::initializer_list<Type>& ilist) { Formatter(f, ilist.begin(), ilist.end()); }
	template <class Fitrst, class Second> inline void Formatter(FormatData& f, const std::pair<Fitrst, Second>& pair)
	{
		f.string.push_back('{');
		Formatter(f, pair.first);
		f.string.append(", ");
		Formatter(f, pair.second);
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
			void operator()(const String& text) const { std::cout << text << '\n'; }
			template <class... Args> void operator()(const Args&... args) const { operator()(Format(args...)); }
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

	template <class Type> inline Array<Type> ReadArray_impl(size_t n)
	{
		Array<Type> as(n);
		for (auto& a : as)
			std::cin >> a;
		return as;
	}

	inline Array<int8>  ReadInt8Array(size_t n)  { return ReadArray_impl<int8>(n);  }
	inline Array<int16> ReadInt16Array(size_t n) { return ReadArray_impl<int16>(n); }
	inline Array<int32> ReadInt32Array(size_t n) { return ReadArray_impl<int32>(n); }
	inline Array<int64> ReadInt64Array(size_t n) { return ReadArray_impl<int64>(n); }
	inline Array<uint8>  ReadUint8Array(size_t n)  { return ReadArray_impl<uint8>(n);  }
	inline Array<uint16> ReadUint16Array(size_t n) { return ReadArray_impl<uint16>(n); }
	inline Array<uint32> ReadUint32Array(size_t n) { return ReadArray_impl<uint32>(n); }
	inline Array<uint64> ReadUint64Array(size_t n) { return ReadArray_impl<uint64>(n); }
# if YAT_WITH_FEATURE(INT128)
	// [ToDo] inline Array<int128 ReadInt128Array(size_t n)   { ... }
	// [ToDo] inline Array<uint128 ReadUint128Array(size_t n) { ... }
# endif
	inline Array<float32>  ReadFloat32Array(size_t n)  { return ReadArray_impl<float32>(n);  }
	inline Array<float64>  ReadFloat64Array(size_t n)  { return ReadArray_impl<float64>(n);  }
	inline Array<float128> ReadFloat128Array(size_t n) { return ReadArray_impl<float128>(n); }
	inline Array<int32>  ReadIntArray(size_t n)  { return ReadInt32Array(n);  }
	inline Array<uint32> ReadUintArray(size_t n) { return ReadUint32Array(n); }
	inline Array<float32> ReadFloatArray(size_t n)  { return ReadFloat32Array(n); }
	inline Array<float64> ReadDoubleArray(size_t n) { return ReadFloat64Array(n); }
	

	////////////////////////////////
	//
	//	8. ループ
	//
	////////////////////////////////

	template <class T, class N, class S>
	class Step
	{
	public:
		class Iterator
		{
		private:
			T m_currentValue;
			N m_count;
			S m_step;
		public:
			constexpr Iterator() noexcept : m_currentValue(T()), m_count(N()), m_step(S()) {}
			constexpr Iterator(T startValue, N count, S step) noexcept : m_currentValue(startValue), m_count(count), m_step(step) {}
			constexpr Iterator& operator ++() noexcept { --m_count; m_currentValue += m_step; return *this; }
			constexpr Iterator operator ++(int) noexcept { Iterator tmp = *this; --m_count; m_currentValue += m_step; return tmp; }
			constexpr const T& operator *() const noexcept { return m_currentValue; }
			constexpr const T* operator ->() const noexcept { return &m_currentValue; }
			constexpr bool operator ==(const Iterator& other) const noexcept { return m_count == other.m_count; }
			constexpr bool operator !=(const Iterator& other) const noexcept { return !(m_count == other.m_count); }
			constexpr T currentValue() const noexcept { return m_currentValue; }
			constexpr N size() const noexcept { return m_count; }
			constexpr S step() const noexcept { return m_step; }
		};
		using value_type = T;
		using iterator = Iterator;
		constexpr Step(T startValue, N count, S step) noexcept : m_start_iterator(startValue, count, step) {}
		constexpr iterator begin() const noexcept { return m_start_iterator; }
		constexpr iterator end() const noexcept { return m_end_iterator; }
		constexpr value_type startValue() const noexcept { return m_start_iterator.currentValue(); }
		constexpr N size() const noexcept { return m_start_iterator.size(); }
		constexpr S step() const noexcept { return m_start_iterator.step(); }
		constexpr bool isEmpty() const noexcept { return size() == 0; }
		operator Array<value_type>() const { return asArray(); }
		Array<value_type> asArray() const
		{
			Array<value_type> new_array;
			if (isEmpty())
				return new_array;
			new_array.reserve(static_cast<size_t>(size()));
			auto count_ = size();
			auto value = startValue();
			const auto step_ = step();
			for (;;)
			{
				new_array.push_back(value);
				if (--count_)
					value += step_;
				else
					break;
			}
			return new_array;
		}
		template <class Fty> constexpr N count_if(Fty f) const
		{
			if (isEmpty())
				return 0;
			N result = 0;
			auto count_ = size();
			auto value = startValue();
			const auto step_ = step();
			for (;;)
			{
				if (f(value))
					++result;
				if (--count_)
					value += step_;
				else
					break;
			}
			return result;
		}
		template <class Fty> void each(Fty f) const
		{
			if (isEmpty())
				return;
			auto count_ = size();
			auto value = startValue();
			const auto step_ = step();
			for (;;)
			{
				f(value);
				if (--count_)
					value += step_;
				else
					break;
			}
		}
		template <class Fty> void each_index(Fty f) const
		{
			if (isEmpty())
				return;
			auto count_ = size();
			auto value = startValue();
			const auto step_ = step();
			size_t index = 0;
			for (;;)
			{
				f(index++, value);
				if (--count_)
					value += step_;
				else
					break;
			}
		}
		template <class Fty> constexpr auto filter(Fty f) const;
		constexpr bool include(const value_type& x) const
		{
			if (isEmpty())
				return false;
			auto count_ = size();
			auto value = startValue();
			const auto step_ = step();
			for (;;)
			{
				if (x == value)
					return true;
				if (--count_)
					value += step_;
				else
					break;
			}
			return false;
		}
		template <class Fty> constexpr bool include_if(Fty f) const
		{
			if (isEmpty())
				return false;
			auto count_ = size();
			auto value = startValue();
			const auto step_ = step();
			for (;;)
			{
				if (f(value))
					return true;
				if (--count_)
					value += step_;
				else
					break;
			}
			return false;
		}
		String join(StringView sep = U", ", StringView begin = U"{", StringView end = U"}") const
		{
			String s;
			s.append(begin);
			if (isEmpty())
			{
				s.append(end);
				return s;
			}
			bool isFirst = true;
			auto count_ = size();
			auto value = startValue();
			const auto step_ = step();
			for (;;)
			{
				if (isFirst)
					isFirst = false;
				else
					s.append(sep);
				s.append(Format(value));
				if (--count_)
					value += step_;
				else
					break;
			}
			s.append(end);
			return s;
		}
		template <class Fty> constexpr auto map(Fty f) const;
		template <class Fty, class R = std::decay_t<std::result_of_t<Fty(T, T)>>> constexpr auto reduce(Fty f, R init) const
		{
			if (isEmpty())
				return init;
			auto count_ = size();
			auto value = startValue();
			const auto step_ = step();
			for (;;)
			{
				init = f(init, value);
				if (--count_)
					value += step_;
				else
					return init;
			}
		}
		template <class Fty> constexpr auto reduce1(Fty f) const
		{
			if (isEmpty())
				throw std::out_of_range("Step::reduce1() reduce from empty range");
			auto count_ = size();
			auto value = startValue();
			const auto step_ = step();
			std::result_of_t<Fty(value_type, value_type)> result = value;
			for (;;)
			{
				if (--count_)
					value += step_;
				else
					break;
				result = f(result, value);
			}
			return result;
		}
		constexpr auto sum() const
		{
			using result_type = decltype(std::declval<T>() + std::declval<S>());
			if (std::is_scalar<result_type>::value)
			{
				const auto n = size();
				const auto a = startValue();
				const auto d = step();
				return static_cast<result_type>(n * (2 * a + (n - 1) * d) / 2);
			}
			else
				return reduce(Plus<result_type>(), result_type{});
		}
		Array<value_type> take(size_t n) const
		{
			Array<value_type> new_array;
			if (isEmpty() || n == 0)
				return new_array;
			auto count_ = size();
			auto value = startValue();
			const auto step_ = step();
			for (;;)
			{
				new_array.push_back(value);
				if (--count_ && new_array.size() < n)
					value += step_;
				else
					break;
			}
			return new_array;
		}
		template <class Fty> Array<value_type> take_while(Fty f) const
		{
			Array<value_type> new_array;
			if (isEmpty())
				return new_array;
			auto count_ = size();
			auto value = startValue();
			const auto step_ = step();
			for (;;)
			{
				if (f(value))
					new_array.push_back(value);
				else
					break;
				if (--count_)
					value += step_;
				else
					break;
			}
			return new_array;
		}
	private:
		iterator m_end_iterator;
		iterator m_start_iterator;
	};

	template <class T, class N, class S = int32, std::enable_if_t<std::is_integral<N>::value> * = nullptr>
	inline constexpr auto step(T a, N n, S s = 1) { return Step<decltype(a + s), N, S>(a, n, s); }

	template <class N, std::enable_if_t<std::is_integral<N>::value> * = nullptr>
	inline constexpr auto step(N n) { return Step<N, N, int32>(N(0), n, 1); }

	template <class N, std::enable_if_t<std::is_integral<N>::value> * = nullptr>
	inline constexpr auto step_backward(N n) { return Step<N, N, int32>(n + int32(-1), n, int32(-1)); }

	template <class T, class U, class S = int32, class StartType = std::common_type_t<T, U>, class CounterType = std::common_type_t<std::size_t, StartType>, std::enable_if_t<std::is_integral<StartType>::value> * = nullptr>
	inline constexpr auto step_to(T a, U b, S s = 1)
	{
		CounterType  n = 0;
		using DiffType = std::common_type_t<int64, StartType>;
		if (s == 0 || (b != a && (b < a) != (s < 0)))
			n = 0;
		else
		{
			S abs_s = s > 0 ? s : -s;
			CounterType diff = b > a ? DiffType(b) - DiffType(a) : DiffType(a) - DiffType(b);
			if (abs_s == 1)
				n = diff;
			else
				n = diff / abs_s;
			n++;
		}
		return Step<StartType, CounterType, S>(a, n, s);
	}

	template<class T, class U, class S = int32, class StartType = std::common_type_t<T, U>, class CounterType = std::common_type_t<std::size_t, StartType>, std::enable_if_t<std::is_integral<StartType>::value> * = nullptr>
	inline constexpr auto step_until(T a, U b, S s = 1)
	{
		CounterType n;
		using DiffType = std::common_type_t<int64, StartType>;
		if (b == a || s == 0 || (b < a) != (s < 0))
			n = 0;
		else
		{
			S abs_s = s > 0 ? s : -s;
			CounterType diff = b > a ? DiffType(b) - DiffType(a) : DiffType(a) - DiffType(b);
			if (abs_s == 1)
				n = diff;
			else
				n = diff / abs_s;
			CounterType finish = a + n * s;
			if (finish != static_cast<CounterType>(b))
				n++;
		}
		return Step<StartType, CounterType, S>(a, n, s);
	}

	template<class T, class U, class S = int32, class StartType = std::common_type_t<T, U>, class CounterType = std::common_type_t<std::size_t, StartType>, std::enable_if_t<std::is_integral<StartType>::value> * = nullptr>
	inline constexpr auto Range(T beg, U end, S step = 1) { return step_to(beg, end, step); }

	namespace detail
	{
		template <class Fty> struct FilterFunction
		{
			using isMap = std::false_type;
			Fty function;
			template <class T> constexpr auto operator() (const T& value) const { return function(value); }
		};

		template <class Fty> struct MapFunction
		{
			using isMap = std::true_type;
			Fty function;
			template <class T> constexpr auto operator() (const T& value) const { return function(value); }
		};

		template <class F> struct IsMap : std::conditional_t<F::isMap::value, std::true_type, std::false_type> {};

		template <class Fty, class ValueType, size_t Index, class Tuple, class Next>
		constexpr void Apply_impl(Fty f, const ValueType& value, const Tuple& tuple)
		{
			if (IsMap<Next>::value)
			{
				if (Index + 1 == std::tuple_size<Tuple>::value)
					f(std::get<Index>(tuple)(value));
				else
					Apply_impl<Fty, decltype(std::get<Index>(tuple)(value)), Index + 1, Tuple, std::decay_t<decltype(std::get<Index + 1>(std::declval<Tuple>()))>>(f, std::get<Index>(tuple)(value), tuple);
			}
			else
			{
				if (Index + 1 == std::tuple_size<Tuple>::value)
				{
					if (std::get<Index>(tuple)(value))
						f(value);
				}
				else
				{
					if (std::get<Index>(tuple)(value))
						Apply_impl<Fty, ValueType, Index + 1, Tuple, std::decay_t<decltype(std::get<Index + 1>(std::declval<Tuple>()))>>(f, value, tuple);
				}
			}
		}

		template <class Fty, class ValueType, class Tuple>
		constexpr void Apply(Fty f, const ValueType& value, const Tuple& tuple)
		{
			Apply_impl<Fty, ValueType, 0, Tuple, std::decay_t<decltype(std::get<0>(std::declval<Tuple>()))>>(f, value, tuple);
		}

		template <class Fty, class ResultType, class ValueType, size_t Index, class Tuple, class Next, std::enable_if_t<IsMap<Next>::value && (Index + 1 == std::tuple_size<Tuple>::value)> * = nullptr>
		constexpr void Reduce_impl(Fty f, ResultType& result, const ValueType& value, const Tuple& tuple)
		{
			result = f(result, std::get<Index>(tuple)(value));
		}

		template <class Fty, class ResultType, class ValueType, size_t Index, class Tuple, class Next, std::enable_if_t<IsMap<Next>::value && (Index + 1 != std::tuple_size<Tuple>::value)> * = nullptr>
		constexpr void Reduce_impl(Fty f, ResultType& result, const ValueType& value, const Tuple& tuple)
		{
			Reduce_impl<Fty, ResultType, decltype(std::get<Index>(tuple)(value)), Index + 1, Tuple, std::decay_t<decltype(std::get<Index + 1>(std::declval<Tuple>()))>>(f, result, std::get<Index>(tuple)(value), tuple);
		}

		template <class Fty, class ResultType, class ValueType, size_t Index, class Tuple, class Next, std::enable_if_t<!IsMap<Next>::value>* = nullptr>
		constexpr void Reduce_impl(Fty f, ResultType& result, const ValueType& value, const Tuple& tuple)
		{
			if (Index + 1 == std::tuple_size<Tuple>::value)
			{
				if (std::get<Index>(tuple)(value))
					result = f(result, value);
			}
			else
			{
				if (std::get<Index>(tuple)(value))
					Reduce_impl<Fty, ResultType, ValueType, Index + 1, Tuple, std::decay_t<decltype(std::get<Index + 1>(std::declval<Tuple>()))>>(f, result, value, tuple);
			}
		}

		template <class Fty, class ResultType, class ValueType, class Tuple>
		constexpr void Reduce(Fty f, ResultType& result, const ValueType& value, const Tuple& tuple)
		{
			Reduce_impl<Fty, ResultType, ValueType, 0, Tuple, std::decay_t<decltype(std::get<0>(std::declval<Tuple>()))>>(f, result, value, tuple);
		}

		template <class StepClass, class ValueType, class Tuple>
		class F_Step
		{
		private:
			StepClass m_base;
			Tuple m_functions;
		public:
			using value_type = ValueType;
			using functions_type = Tuple;
			constexpr F_Step(StepClass stepClass, Tuple functions) : m_base(stepClass), m_functions(functions) {}
			operator Array<value_type>() const { return asArray(); }
			Array<value_type> asArray() const { Array<value_type> new_array; each([&new_array](const auto& value) { new_array.push_back(value); }); return new_array; }
			size_t size() const { size_t sum = 0; each([&sum](const auto) { ++sum; }); return sum; }
			template <class Fty>
			size_t count_if(Fty f) const { size_t sum = 0; each([&sum, f = f](const auto& value) { sum += f(value); }); return sum; }
			template <class Fty>
			void each(Fty f) const { m_base.each([f, functions = m_functions](const auto& value){ Apply(f, value, functions); }); }
			void evaluate() const { m_base.each([functions = m_functions](const auto& value){ Apply(Id, value, functions); }); }
			template <class Fty>
			constexpr auto filter(Fty f) const
			{
				const auto functions = std::tuple_cat(m_functions, std::make_tuple(FilterFunction<Fty>{ f }));
				return F_Step<StepClass, value_type, decltype(functions)>(m_base, functions);
			}
			bool include(const value_type& x) const
			{
				if (m_base.isEmpty())
					return false;
				bool hasValue = false;
				auto count_ = m_base.size();
				auto value = m_base.startValue();
				const auto step_ = m_base.step();
				const auto includeFunc = [&hasValue, x](const auto& value) { hasValue = (value == x); };
				const auto functions = m_functions;
				for (;;)
				{
					Apply(includeFunc, value, functions);
					if (hasValue)
						return true;
					if (--count_)
						value += step_;
					else
						break;
				}
				return false;
			}

			template <class Fty>
			bool include_if(Fty f) const
			{
				if (m_base.isEmpty())
					return false;
				bool hasValue = false;
				auto count_ = m_base.size();
				auto value = m_base.startValue();
				const auto step_ = m_base.step();
				const auto includeFunc = [&hasValue, f](const auto& value) { hasValue = f(value); };
				const auto functions = m_functions;
				for (;;)
				{
					Apply(includeFunc, value, functions);

					if (hasValue)
						return true;
					if (--count_)
						value += step_;
					else
						break;
				}
				return false;
			}

			String join(StringView sep = U", ", StringView begin = U"{", StringView end = U"}") const
			{
				String s;
				s.append(begin);
				bool isFirst = true;
				each([&s, &isFirst, sep = sep](const auto& value)
					{
						if (isFirst)
							isFirst = false;
						else
							s.append(sep);
						s.append(Format(value));
					});
				s.append(end);
				return s;
			}

			template <class Fty>
			constexpr auto map(Fty f) const
			{
				using Ret = std::decay_t<std::result_of_t<Fty(value_type)>>;
				const auto functions = std::tuple_cat(m_functions, std::make_tuple(MapFunction<Fty>{ f }));
				return F_Step<StepClass, Ret, decltype(functions)>(m_base, functions);
			}

			template <class Fty, class R = std::decay_t<std::result_of_t<Fty(ValueType, ValueType)>>>
			constexpr auto reduce(Fty f, R init) const
			{
				auto result = init;
				if (m_base.isEmpty())
					return result;
				auto count_ = m_base.size();
				auto value = m_base.startValue();
				const auto step_ = m_base.step();
				const auto functions = m_functions;
				for (;;)
				{
					Reduce(f, result, value, functions);
					if (--count_)
						value += step_;
					else
						break;
				}
				return result;
			}

			template <class Fty>
			auto reduce1(Fty f) const
			{
				if (m_base.isEmpty())
					throw std::out_of_range("F_Step::reduce1() reduce from empty range");
				auto count_ = m_base.size();
				auto value = m_base.startValue();
				const auto step_ = m_base.step();
				const auto functions = m_functions;
				std::result_of_t<Fty(value_type, value_type)> result;
				Apply([&result](const auto& v) { result = v; }, value, functions);
				if (--count_ == 0)
					return result;
				value += step_;
				for (;;)
				{
					Reduce(f, result, value, functions);
					if (--count_)
						value += step_;
					else
						break;
				}
				return result;
			}

			template <class Type = value_type> Type sum() const { return reduce(Plus<Type>(), Type{}); }

			Array<value_type> take(size_t n) const
			{
				Array<value_type> new_array;
				if (m_base.isEmpty() || n == 0)
					return new_array;
				auto count_ = m_base.size();
				auto value = m_base.startValue();
				const auto step_ = m_base.step();
				const auto pushFunc = [&new_array](const auto& value) { new_array.push_back(value); };
				const auto functions = m_functions;
				for (;;)
				{
					Apply(pushFunc, value, functions);
					if (--count_ && new_array.size() < n)
						value += step_;
					else
						break;
				}
				return new_array;
			}

			template <class Fty>
			Array<value_type> take_while(Fty f) const
			{
				Array<value_type> new_array;
				if (m_base.isEmpty())
					return new_array;
				bool finished = false;
				auto count_ = m_base.size();
				auto value = m_base.startValue();
				const auto step_ = m_base.step();
				const auto pushFunc = [&new_array, &finished, f = f](const auto& value)
				{
					if (f(value))
						new_array.push_back(value);
					else
						finished = true;
				};
				const auto functions = m_functions;
				for (;;)
				{
					Apply(pushFunc, value, functions);
					if (--count_ && !finished)
						value += step_;
					else
						break;
				}
				return new_array;
			}
		};
	}
	template <class T, class N, class S> template <class Fty>
	inline constexpr auto Step<T, N, S>::filter(Fty f) const
	{
		const auto tuple = std::make_tuple(detail::FilterFunction<Fty>{ f });
		return detail::F_Step<Step, value_type, decltype(tuple)>(*this, tuple);
	}
	template <class T, class N, class S> template <class Fty>
	inline constexpr auto Step<T, N, S>::map(Fty f) const
	{
		using Ret = std::decay_t<std::result_of_t<Fty(value_type)>>;
		const auto tuple = std::make_tuple(detail::MapFunction<Fty>{ f });
		return detail::F_Step<Step, Ret, decltype(tuple)>(*this, tuple);
	}

	template <class T, class N, class S> inline void Formatter(FormatData& f, const Step<T, N, S>& s) { Formatter(f, s.join()); }
	template <class StepClass, class ValueType, class Tuple> inline void Formatter(FormatData& f, const detail::F_Step<StepClass, ValueType, Tuple>& s) { Formatter(f, s.join()); }

	////////////////////////////////
	//
	//	9. 便利関数
	//
	////////////////////////////////

	namespace detail
	{
		struct Odd_impl { template <class Type> constexpr bool operator()(const Type& x) const { return (x % 2) != 0; } };
		struct Even_impl { template <class Type> constexpr bool operator()(const Type& x) const { return (x % 2) == 0; } };
	}

	// x が min 以上 max 以下である場合に `true` を返す
	template <class Type> inline constexpr bool InRange(const Type& x, const Type& min, const Type& max) { return (min <= x) && (x <= max); }

	// 奇数の場合に `true` を返す
	constexpr auto IsOdd = detail::Odd_impl();

	// 偶数の場合に `true` を返す
	constexpr auto IsEven = detail::Even_impl();

	// 10 の n 乗を整数型で返す
	template <class Integer> inline constexpr Integer PowerOf10(size_t n) noexcept
	{
		Integer x = 1;
		for (size_t i = 0; i < n; ++i)
			x *= 10;
		return x;
	}

	// 整数の (1 + N) 桁目の数を返す
	template <class Integer> int32 GetDigit(Integer n, size_t index) noexcept { return static_cast<int32>((n / PowerOf10<Integer>(index)) % 10); }
}

namespace std
{
	inline void swap(yat::StringView& a, yat::StringView& b) noexcept { a.swap(b); }
	inline void swap(yat::String& a, yat::String& b) noexcept { a.swap(b); }
	template <class Type, class Allocator> inline void swap(yat::Array<Type, Allocator>& a, yat::Array<Type, Allocator>& b) noexcept { a.swap(b); }
}

using namespace std;
using namespace yat;
//
////////////////////////////////////////////////////////////////
