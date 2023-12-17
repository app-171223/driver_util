#pragma once

#include "../utils/stdint.hxx"
#include "nt/unicode_string.hxx"
#include "nt/process.hxx"

struct import_table_t
{
	//auto(__fastcall* DbgPrintEx)(std::uint32_t component_id, std::uint32_t level, const char* format, ...)->std::uint32_t;

	auto(__fastcall* MmCopyMemory)(void* dst, void* src, std::size_t size, std::uint32_t flag, std::size_t* bytes) -> std::uint32_t;

	auto(__fastcall* MmMapIoSpaceEx)(std::uint64_t address, std::size_t size, std::uint32_t protect) -> void*;

	auto(__fastcall* MmUnmapIoSpace)(void* address, std::size_t size) -> std::uint32_t;

	auto(__fastcall* PsLookupProcessByProcessId)(std::uint64_t process_id, eprocess_t** size) -> std::uint32_t;

	auto(__fastcall* memmove)(std::uint64_t dst, std::uint64_t src, std::size_t size) -> std::uint32_t;

	std::uint64_t PsInitialSystemProcess;

	constexpr import_table_t& operator=(const import_table_t& table) = default;
};

extern import_table_t g_imp;