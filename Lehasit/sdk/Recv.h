#pragma once

// Credits: Valve

class RecvProp;

class RecvTable {
public:
	RecvProp*	props;
	int			num_props;
	void*		decoder;
	char*		name;
	bool		is_initialized;
	bool		is_in_main_list;
};

class RecvProp {
public:
	char*		name;

private:
	char pad0[(3 * sizeof(int)) + (1 * sizeof(bool)) + (3 * sizeof(void*))];

public:
	void*		proxy_fn;

private:
	char pad1[sizeof(void*)];

public:
	RecvTable*	data_table;
	int			offset;
	int			element_stride;
	int			num_elements;
	const char*	parent_array_prop_name;
};

class ClientClass {
public:
	void*			create_fn;
	void*			create_event_fn;
	char*			name;
	RecvTable*		table;
	ClientClass*	next;
	int				classID;
};

class DVariant {
public:
	union {
		float	floating;
		long	integer;
		char*	string;
		void*	pointer;
		float	vector[3];
	};
};

class CRecvProxyData {
public:
	RecvProp*	prop;
	DVariant	value;
	int			element;
	int			objectID;
};