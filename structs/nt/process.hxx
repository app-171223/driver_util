#pragma once

#include "../../utils/stdint.hxx"
#include "um_module.hxx"
#include "unicode_string.hxx"
#include "list_entry.hxx"

struct kprocess_t // size = 0x438
{
   std::uint8_t pad_0[0x28];
   std::uint64_t m_dir_table; // 0x28
   std::uint8_t pad_1[0x358];
   std::uint64_t m_user_dir_table; // 0x388
   std::uint8_t pad_2[0xa8];
};

struct eprocess_t
{
   kprocess_t m_pcb; // 0x0
   std::uint8_t pad_0[0x8];
   std::uint64_t m_id; // 0x440
   list_entry_t m_active_links; // 0x448
   std::uint8_t pad_1[0xc8];
   std::uint64_t m_base_address; // 0x520
   std::uint8_t pad_2[0x28];
   peb_t* m_peb; // 0x550
   std::uint8_t pad_3[0x50];
   std::uint8_t m_image_filename[0xf]; // 0x5a8
   std::uint8_t pad_4[0x9];
   unicode_string_t* m_name; // 0x5c0
   std::uint8_t pad_5[0x1f8];
   list_entry_t m_process_links; // 0x7c0
};