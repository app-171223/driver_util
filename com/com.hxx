#pragma once

#include "../utils/stdint.hxx"
#include "../structs/com_buffer.hxx"
#include "../structs/nt/process.hxx"

namespace com
{
	enum class oper : std::uint8_t
	{
		read,
		write,
		get_mod_base,
		get_process,
		init
	};

	struct buff_t
	{
		std::uint64_t m_copy_src;
		std::uint64_t m_copy_dst;
		std::size_t m_copy_size;
		std::uint32_t m_user_proc_id;
		const std::wchar_t* m_name;
		std::uint64_t m_mod_base;
		eprocess_t* m_process;
		
		com::oper m_operation;
	};


	static std::int64_t(__fastcall* original_add)(std::uint32_t a1, std::uint32_t a2, std::uint32_t a3);
	constexpr auto key = std::uint32_t{0x6a6b6c};

	void init(std::uint32_t user_process_id);
	
	void hook_entry(std::uint32_t com_key, std::uint32_t low_part, std::uint64_t high_part);
	
	void place_hook();
}
