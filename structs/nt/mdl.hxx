#pragma once

#include "../../utils/stdint.hxx"
#include "process.hxx"

struct mdl_t
{
	mdl_t* m_next; // 0x0
	std::int16_t m_size; // 0x8
	std::int16_t m_flags; // 0xa
	eprocess_t* m_proc; // 0x10
	std::uint64_t m_mapped_va; // 0x18
	std::uint64_t m_start_va; // 0x20
	std::uint32_t m_byte_count; // 0x28
	std::uint32_t m_byte_offset; // 0x2c
};
