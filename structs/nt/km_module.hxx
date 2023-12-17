#pragma once

#include "../../utils/stdint.hxx"
#include "list_entry.hxx"
#include "unicode_string.hxx"

struct kldr_entry_t
{
   list_entry_t m_modules_load_order; //0x0
   void* m_extable; //0x10
   std::uint32_t m_extable_size; //0x18
   void* m_gp_value; //0x20
   void* m_debug_info; //0x28
   std::uint64_t m_mod_base; //0x30
   std::uint64_t m_entry_point; //0x38
   std::uint32_t m_image_size; //0x40
   unicode_string_t m_full_name; //0x48
   unicode_string_t m_base_name; //0x58
};