#pragma once

#include "../structs/module_table.hxx"
#include "../structs/nt/mdl.hxx"
#include "../structs/nt/process.hxx"
#include "../structs/nt/thread.hxx"

#define rva(address, offset) address + (std::int32_t)((*(std::int32_t*)(address + offset) + offset) + sizeof(std::int32_t))

namespace utils
{
	auto find_pattern(const kernel_module_t& module, const char* pattern, const char* mask) -> void*;

	auto null_mdl_pfn(mdl_t* mdl) -> void;
	
	auto process_by_id(std::uint32_t id) -> eprocess_t*;

	auto process_by_name(const std::wchar_t* name) -> eprocess_t*;

	auto module_by_name(eprocess_t* process, const std::wchar_t* name) -> std::uint64_t;
		
	auto remove_cid_entry(std::uint64_t entry_id) -> void;

	auto unlink_list(list_entry_t* list_entry) -> void;

	//auto clear_ethread(ethread_t* thread) -> void;
}

namespace masm
{
	extern "C" auto ptr_exchange(void* src, void* dst) -> void*;

	extern "C" auto current_thread() -> ethread_t*;
}