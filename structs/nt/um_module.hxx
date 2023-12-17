#pragma once

#include "../../utils/stdint.hxx"
#include "list_entry.hxx"
#include "unicode_string.hxx"

typedef struct ldr_entry_t
{
   list_entry_t m_modules_load_order; // 0x0
   list_entry_t m_modules_memory_order; // 0x10
   list_entry_t m_modules_init_order; // 0x20
   std::uint64_t m_dll_base; // 0x30
   std::uint64_t m_entry_point; // 0x38
   std::uint32_t m_image_size; // 0x40
   unicode_string_t m_full_name; // 0x48
   unicode_string_t m_base_name; // 0x58
};

struct ldr_t
{
   std::uint32_t m_lenght; // 0x0
   std::uint8_t m_initialized; // 0x4
   std::uint64_t m_sshandle; // 0x8
   list_entry_t m_modules_load_order; // 0x10 -> ldr_entry_t links
   list_entry_t m_modules_memory_order; // 0x20
   list_entry_t m_modules_init_order; // 0x30
   std::uint64_t m_entry_in_progress; // 0x40
   std::uint8_t m_shutdown_in_progress; // 0x48
   std::uint64_t m_shutdown_thread_id; // 0x50
};

struct peb_t
{
	std::uint8_t pad_0[0x18];
	ldr_t* m_ldr; // 0x18
};