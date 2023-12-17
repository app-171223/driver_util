#pragma once

#include "../structs/nt/process.hxx"
#include "../structs/import_table.hxx"

namespace mem
{
   enum class copy_flag : std::uint8_t
   {
      phys = 0x1,
      virt = 0x2
   };
   
   template <class type_t, copy_flag flag = copy_flag::phys>
   type_t mm_read(std::uint64_t address)
   {
      auto buffer = type_t{};
      auto bytes_copied = size_t{0};

      g_imp.MmCopyMemory(
         &buffer,
         (void*)(address),
         sizeof(type_t),
         (std::uint32_t)(flag),
         &bytes_copied
      );

      return buffer;
   }
   
   std::uint64_t to_phys(eprocess_t* process, std::uint64_t virt_add);

   void map_read(std::uint64_t phys_address, std::uint64_t buffer, size_t size);
   
   void map_write(std::uint64_t phys_address, std::uint64_t buffer, size_t size);
   
   template<class type_t>
   type_t map_read(std::uint64_t address)
   {
      auto buffer = type_t{};
      mem::map_read(address, (std::uint64_t)(&buffer), sizeof(type_t));
      return buffer;
   }
}