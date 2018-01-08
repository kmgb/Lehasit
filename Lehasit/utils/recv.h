#pragma once

class RecvProp;
class RecvTable;

namespace utils
{

	RecvProp* find_prop_in_table(RecvTable* table, const char* prop_name); // Find a prop by name in the specified table
	RecvTable* find_table_in_list(const char* table_name); // Find a table by searching through the main list
	RecvTable* find_table(const char* table_name); // Find a table by searching through the main list and their children

}