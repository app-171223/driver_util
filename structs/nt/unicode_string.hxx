#pragma once

#include "../../utils/stdint.hxx"

constexpr auto str_len(const char* str) -> std::uint16_t
{
	auto lenght = std::uint32_t{0};
	for (; str[lenght]; lenght++) {}
	return lenght;
}

constexpr auto wstr_comp(const std::wchar_t* str, const std::wchar_t* sub_str) -> const std::wchar_t*
{
	for (std::wchar_t* x = (std::wchar_t*)(str), *y{nullptr}, *c{nullptr}; *x; x++)
	{
		if (auto c = (std::wchar_t*)(sub_str); *x == *c)
		{
			for (y = x; *y && *c && *y == *c; c++, y++) {}
			if (!*c) return x;
		}
	}

	return nullptr;
}

struct unicode_string_t
{
	std::uint16_t m_lenght; // 0x0
	std::uint16_t m_maximum_length; // 0x2
	std::wchar_t* m_buffer; // 0x8

	unicode_string_t() = default;

	constexpr unicode_string_t(const std::wchar_t* source)
		: m_lenght{0},
		m_maximum_length{0},
		m_buffer{(std::wchar_t*)(source)}
	{
		auto lenght = std::uint16_t{0};
		for (; source[lenght]; lenght++) {}
		
		if (lenght <= 0x7ffe)
		{
			m_lenght = lenght * 2;
			m_maximum_length = m_lenght + 2;
		}
	}

	constexpr bool operator==(const unicode_string_t& sub_str) { return ::wstr_comp(m_buffer, sub_str.m_buffer); }
};