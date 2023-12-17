#include "mem.hxx"

std::uint64_t mem::to_phys(eprocess_t* process, std::uint64_t virt_add)
{
   auto dir_table = process->m_pcb.m_dir_table ? process->m_pcb.m_dir_table : process->m_pcb.m_user_dir_table & 0xfffffffffffffff0;
   
   std::uint64_t page[] = {
      (virt_add << 0x10) >> 0x37,   // page directory pointer
      (virt_add << 0x19) >> 0x37,   // page directory
      (virt_add << 0x22) >> 0x37,   // page table
      (virt_add << 0x2b) >> 0x37,   // page table entry
      (virt_add << 0x34) >> 0x34    // page offset
   };
   
   auto page_dir_ptr = mem::mm_read<std::uint64_t>(dir_table + 0x8 * page[0]);
   auto page_dir = mem::mm_read<std::uint64_t>((page_dir_ptr & 0xffffff000) + 0x8 * page[1]);
   auto page_table = mem::mm_read<std::uint64_t>((page_dir & 0xffffff000) + 0x8 * page[2]);
   auto page_table_entry = mem::mm_read<std::uint64_t>((page_table & 0xffffff000) + 0x8 * page[3]);
   
   if (page_dir & 0x80) return (page_dir & 0xfffffc0000000) + (virt_add & 0x3fffffffll);
   if (page_table & 0x80) return (page_table & 0x0000ffffff000) + (virt_add & 0x001fffffll);
   return (page_table_entry & 0x0000ffffff000) + page[4];
}

void mem::map_read(std::uint64_t phys_address, std::uint64_t buffer, size_t size)
{
   if (auto memory = g_imp.MmMapIoSpaceEx(phys_address, size, 0x4); memory && size)
   {
      g_imp.memmove(buffer, (std::uint64_t)(memory), size);
      g_imp.MmUnmapIoSpace(memory, size);
   }
}

void mem::map_write(std::uint64_t phys_address, std::uint64_t buffer, size_t size)
{
   if (auto memory = g_imp.MmMapIoSpaceEx(phys_address, size, 0x4); memory && size)
   {
      g_imp.memmove((std::uint64_t)(memory), buffer, size);
      g_imp.MmUnmapIoSpace(memory, size);
   }
}