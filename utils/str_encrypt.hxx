#pragma once

#include "../utils/stdint.hxx"

namespace std
{
	// remove_reference
	template <class T>
	struct remove_reference { using type = T; };

	template <class T>
	struct remove_reference<T&> { using type = T; };

	template <class T>
	struct remove_reference<T&&> { using type = T; };

	template <class T>
	using remove_reference_t = typename remove_reference<T>::type;

	// remove_const
	template <class T>
	struct remove_const { using type = T; };

	template <class T>
	struct remove_const<const T> { using type = T; };

	template <class T>
	using remove_const_t = typename remove_const<T>::type;
}

namespace str_encrypt
{
	template<class T>
	using clean_type_t = typename std::remove_const_t<std::remove_reference_t<T>>;

	template <std::uint32_t m_size, std::uint8_t m_key_1, std::uint8_t m_key_2, typename T>
	class crypt_t
	{
		T m_storage[m_size];

		__forceinline constexpr auto crypt(T* data) -> void { for (auto i = std::uint32_t{0}; i < m_size; i++) m_storage[i] = data[i] ^ (m_key_1 + i % (1 + m_key_2)); }

	public:

		__forceinline constexpr crypt_t(T* data) { this->crypt(data); }

		__forceinline constexpr ~crypt_t() { this->clear(); }

		__forceinline constexpr auto get() -> T* { return m_storage; }

		__forceinline constexpr auto size() -> std::uint32_t { return m_size; }

		__forceinline constexpr auto is_encrypted() -> bool { return m_storage[m_size - 1]; }
		
		__forceinline constexpr auto encrypt() -> T* { if (!this->is_encrypted()) this->crypt(m_storage); return m_storage; }

		__forceinline constexpr auto decrypt() -> T* { if (this->is_encrypted()) this->crypt(m_storage); return m_storage; }

		__forceinline constexpr auto clear() -> void { for (auto i = std::uint32_t{0}; i < m_size; i++) m_storage[i] = 0; }

		__forceinline constexpr operator T* () { this->decrypt(); return m_storage; }
	};
}

#define crypt(str) []() {																																														\
	constexpr auto crypted = str_encrypt::crypt_t																																						\
		<sizeof(str) / sizeof(str[0]), __TIME__[6], __TIME__[7], str_encrypt::clean_type_t<decltype(str[0])>>((str_encrypt::clean_type_t<decltype(str[0])>*)str);	\
			return crypted; }()