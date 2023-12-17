#pragma once

#include "../../utils/stdint.hxx"
#include "list_entry.hxx"

struct kthread_t // size = 0x480
{
	std::uint8_t pad_0[0x30];
	std::uint64_t m_stack_limit; // 0x30
	std::uint8_t pad_1[0x20];
	std::uint64_t m_kernel_stack; // 0x58
	std::uint8_t pad_2[0x14];
	std::uint32_t m_misc_flags; // 0x74
	std::uint8_t pad_3[0x150];
	std::uint64_t m_win32_thread; // 0x1c8
	std::uint8_t pad_4[0xbc];
	std::uint32_t m_kernel_time; // 0x28c
	std::uint8_t pad_5[0x68];
	list_entry_t m_list_entry; // 0x2f8
	std::uint8_t pad_6[0x178];
};

struct ethread_t
{
	kthread_t m_tcb;
	std::uint64_t m_id; // 0x480
	std::uint8_t pad_0[0x18];
	std::uint64_t m_start_address; // 0x4a0
	std::uint8_t pad_1[0x20];
	std::uint64_t m_cid_process; // 0x4c8
	std::uint64_t m_cid_thread; // 0x4d0
	std::uint8_t pad_2[0x48];
	std::uint64_t m_win32_start; // 0x520
	std::uint8_t pad_3[0xd4];
	std::uint64_t m_kernel_stack_ref; // 0x5ac
};