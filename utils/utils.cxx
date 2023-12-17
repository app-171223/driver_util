#include "utils.hxx"

#include "str_encrypt.hxx"
#include "../mem/mem.hxx"
#include "../structs/nt/unicode_string.hxx"

auto utils::find_pattern(const kernel_module_t& module, const char* pattern, const char* mask) -> void*
{
   for (auto i = std::size_t{0}, sig_length = std::size_t{::str_len(mask)}; i < module.m_size - sig_length; i++)
   {
      auto found{true};
      for (auto j = std::size_t{0}; j < sig_length; j++)
         if (found &= mask[j] == '?' || pattern[j] == *((char*)(module.m_base) + i + j); !found)
            continue;

      if (found)
         return (char*)(module.m_base) + i;
   }
   
   return nullptr;
}

auto utils::null_mdl_pfn(mdl_t* mdl) -> void
{
   auto mdl_array = (std::uint32_t*)(mdl + 1);
   
   auto ratio = std::uint64_t{(mdl->m_mapped_va + mdl->m_byte_offset) & 0xfff};
   auto count = std::uint64_t{(ratio + mdl->m_byte_count + 0xfff) >> 0xc};
   
   for (auto i = std::uint32_t{0}; i < count; i++)
      mdl_array[i] = std::uint32_t{0};
}

auto utils::process_by_name(const std::wchar_t* name) -> eprocess_t*
{
   auto process = *(eprocess_t**)(g_imp.PsInitialSystemProcess); // first process
   
   auto unicode_name = unicode_string_t{name};
   auto list_end = process->m_active_links.m_prev; // end of the linked list

   while (
      process = (eprocess_t*)(*(std::uint64_t*)(process->m_active_links.m_next) - 0x448 /* offsetof */),
      process && process->m_active_links.m_prev != list_end)
   {
      if (process->m_name && *process->m_name == unicode_name)
         return process;
   }

   return nullptr;
}

auto utils::process_by_id(std::uint32_t id) -> eprocess_t*
{
   eprocess_t* process;
   g_imp.PsLookupProcessByProcessId((std::uint64_t)(id), &process);
   return process;
}

auto utils::module_by_name(eprocess_t* process, const std::wchar_t* name) -> std::uint64_t
{
   if (!name)
      return process->m_base_address;

   auto peb = mem::map_read<peb_t>(mem::to_phys(process, (std::uint64_t)(process->m_peb)));
   auto ldr = mem::map_read<ldr_t>(mem::to_phys(process, (std::uint64_t)(peb.m_ldr)));
   auto entry = mem::map_read<ldr_entry_t>(mem::to_phys(process, (std::uint64_t)(ldr.m_modules_load_order.m_next)));

   auto unicode_name = unicode_string_t{name};
   auto list_end = entry.m_modules_load_order.m_prev;
   
   while (
      entry = mem::map_read<ldr_entry_t>(mem::to_phys(process, (std::uint64_t)(entry.m_modules_load_order.m_next))),
      entry.m_modules_load_order.m_next && entry.m_modules_load_order.m_next != list_end)
   {
      if (!entry.m_base_name.m_buffer)
         continue;
      
      std::wchar_t name_storage[0xff];
      mem::map_read(mem::to_phys(process, (std::uint64_t)(entry.m_base_name.m_buffer)), (std::uint64_t)(name_storage), 0xff);

      if (auto entry_name = unicode_string_t{name_storage}; entry_name == unicode_name)
         return entry.m_dll_base;
   }
   
   return 0;
}

auto utils::remove_cid_entry(std::uint64_t entry_id) -> void
{
   typedef auto(__fastcall* ExpLookupHandleTableEntry)(std::uint64_t cid_table, std::uint64_t id) -> std::uint64_t;
   typedef auto(__fastcall* ExDestroyHandle)(std::uint64_t handle_table_ptr, std::uint64_t handle, std::uint64_t entry) -> std::uint8_t;

   auto cid_table_ptr = utils::find_pattern(g_mod.ntoskrnl, crypt("\x48\x8B\x05\x00\x00\x00\x00\x0F\xB6\xEA"), crypt("xxx????xxx"));
   auto cid_table = *(std::uint64_t*)(rva((std::uint8_t*)(cid_table_ptr), 3));

   auto lookup_handle_table_entry_fn = utils::find_pattern(g_mod.ntoskrnl, crypt("\xE8\x00\x00\x00\x00\x48\x8B\x0B\x48\x8B\xD7"), crypt("x????xxxxxx"));
   auto lookup_handle_table_entry = ExpLookupHandleTableEntry(rva((std::uint8_t*)(lookup_handle_table_entry_fn), 1));

   auto destroy_handle_fn = utils::find_pattern(g_mod.ntoskrnl, crypt("\xE8\x00\x00\x00\x00\x49\x8B\xCC\xE8\x00\x00\x00\x00\x48\x8B\xCF"), crypt("x????xxxx????xxx"));
   auto destroy_handle = ExDestroyHandle(rva((std::uint8_t*)(destroy_handle_fn), 1));

   auto cid_entry = lookup_handle_table_entry(cid_table, entry_id);
   destroy_handle(cid_table, entry_id, cid_entry);
}

auto utils::unlink_list(list_entry_t* list_entry) -> void
{
   list_entry->m_prev->m_next = list_entry->m_next;
   list_entry->m_next->m_prev = list_entry->m_prev;
}

/*auto utils::clear_ethread(ethread_t* thread) -> void
{
   //thread->m_tcb.m_stack_limit = uint64_t{0};
   thread->m_tcb.m_kernel_stack = std::uint64_t{0};
   //thread->m_tcb.m_win32_thread = std::uint64_t{0};
   thread->m_tcb.m_kernel_time = std::uint32_t{0};

   //thread->m_start_address = std::uint64_t{0};
   thread->m_cid_process = std::uint64_t{0};
   thread->m_cid_thread = std::uint64_t{0};
   thread->m_win32_start = std::uint64_t{0};
   thread->m_kernel_stack_ref = std::uint64_t{0};

   //*(std::uint64_t*)((std::uint64_t)(thread) + 0x290) = 0; // kapc->thread
}*/