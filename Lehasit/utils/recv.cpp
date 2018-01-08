#include "stdafx.h"

#include "recv.h"

#include "../sdk/Recv.h"
#include "../Interfaces.h"

RecvProp* utils::find_prop_in_table(RecvTable* table, const char* prop_name)
{
	for (int i = 0; i < table->num_props; ++i)
	{
		RecvProp* prop = &table->props[i];

		if (!prop)
			continue;

		if (strcmp(prop->name, prop_name) == 0)
			return prop; // The prop was found within the table
	}

	return nullptr; // The table doesn't contain the prop
}

RecvTable* utils::find_table_in_list(const char* table_name)
{
	for (ClientClass* client_class = g_interfaces.client->get_all_classes()
		; client_class != nullptr
		; client_class = client_class->next)
	{
		if (strcmp(client_class->table->name, table_name) == 0)
			return client_class->table;
	}

	return nullptr;
}

RecvTable* find_table_recursive(RecvTable* table, const char* table_name)
{
	if (strcmp(table->name, table_name) == 0)
		return table;

	for (int i = 0; i < table->num_props; ++i)
	{
		RecvProp* prop = &table->props[i];

		if (!prop)
			continue;

		RecvTable* child_table = prop->data_table;
		if (child_table)
		{
			// Check if it's the table we're looking for
			if (strcmp(child_table->name, table_name) == 0)
				return child_table;

			// Check if any of it's children have the table we're looking for
			RecvTable* recursive_table = find_table_recursive(child_table, table_name);

			if (recursive_table)
				return recursive_table;
		}
	}

	return nullptr; // The table (and all it's children) don't contain the prop
}

RecvTable* utils::find_table(const char* table_name)
{
	for (ClientClass* client_class = g_interfaces.client->get_all_classes()
		; client_class != nullptr
		; client_class = client_class->next)
	{
		RecvTable* table = find_table_recursive(client_class->table, table_name);
		if (table) // The table was found in this branch
			return table;
	}

	return nullptr;
}