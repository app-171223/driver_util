#include "com/com.hxx"
#include "utils/utils.hxx"

#include "structs/nt/mdl.hxx"
#include "structs/nt/km_module.hxx"
#include "structs/module_table.hxx"
#include "structs/import_table.hxx"
#include "utils/str_encrypt.hxx"
#include "mem/mem.hxx"

module_table_t g_mod; // global @structs/module_table.hxx
import_table_t g_imp; // global @structs/import_table.hxx

auto main_entry(module_table_t* module_table, import_table_t* import_table, mdl_t* mdl) -> std::int32_t
{
	g_mod = *module_table;
	g_imp = *import_table;
	
	utils::null_mdl_pfn(mdl);
	com::place_hook();
	
	return 0; // STATUS_SUCCESS
}