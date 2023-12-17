#pragma once

#include "../utils/stdint.hxx"

struct kernel_module_t
{
	std::uint64_t m_base;
	std::uint32_t m_size;
};

struct module_table_t
{
	kernel_module_t ntoskrnl;
	kernel_module_t win32kbase;

	constexpr module_table_t& operator=(const module_table_t& table) = default;
};

extern module_table_t g_mod;