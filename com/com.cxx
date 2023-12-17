#include "com.hxx"

#include "../utils/utils.hxx"
#include "../utils/str_encrypt.hxx"
#include "../structs/import_table.hxx"
#include "../mem/mem.hxx"

void com::init(std::uint32_t user_process_id)
{
	auto user_process = utils::process_by_id(user_process_id);
	
	utils::remove_cid_entry(user_process->m_id);
	//utils::unlink_list(&user_process->m_active_links); - pg
	//utils::unlink_list(&user_process->m_process_links); - pg
	
	utils::remove_cid_entry(masm::current_thread()->m_id);
	utils::unlink_list(&masm::current_thread()->m_tcb.m_list_entry);
	//utils::clear_ethread(masm::current_thread()); - pg
}

void com::hook_entry(std::uint32_t com_key, std::uint32_t low_part, std::uint64_t high_part)
{
	if (auto buff = (com::buff_t*)(high_part << 32 | (low_part & 0x00000000ffffffff)); buff && com_key == com::key)
	{
		switch (buff->m_operation)
		{
			case com::oper::read:
				mem::map_read(mem::to_phys(buff->m_process, buff->m_copy_src), buff->m_copy_dst, buff->m_copy_size);
				break;
			case com::oper::write:
				mem::map_write(mem::to_phys(buff->m_process, buff->m_copy_dst), buff->m_copy_src, buff->m_copy_size);
				break;
			case com::oper::get_mod_base:
				buff->m_mod_base = utils::module_by_name(buff->m_process, buff->m_name);
				break;
			case com::oper::get_process:
				buff->m_process = utils::process_by_id(buff->m_user_proc_id);
				break;
			case com::oper::init:
				com::init(buff->m_user_proc_id);
				break;
			default:
				break;
		}
	}
	
	com::original_add(com_key, low_part, high_part);
}

void com::place_hook()
{
	// NtUserOpenInputDesktop .data ptr
	auto ptr_1 = utils::find_pattern(g_mod.win32kbase, crypt("\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x74\x00\x44\x8B\xC7\x8B\xD6\x8B\xCD"), crypt("xxx????xxxx?xxxxxxx"));
	auto ptr_1_ref = rva((std::uint8_t*)(ptr_1), 3);

	// EngBitBlt .data ptr
	auto func = utils::find_pattern(g_mod.win32kbase, crypt("\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x4C\x8B\x15\x00\x00\x00\x00\x4D\x8B\xD9\x48\x8B\xDA\x48\x8B\xF9\x4D\x85\xD2\x74\x00\x8B"), crypt("xxxx?xxxx?xxx????xxxxxxxxxxxxx?x"));
	auto ptr_2_ref = rva((std::uint8_t*)(func) + 0xA, 3);
	// NtUserOpenInputDesktop ptr -> EngBitBlt fn
	*(void**)(&com::original_add) = masm::ptr_exchange((void*)(ptr_1_ref), (void*)(func));

	// EngBitBlt ptr -> hook entry
	masm::ptr_exchange((void*)(ptr_2_ref), &com::hook_entry);
}