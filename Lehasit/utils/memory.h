#pragma once

namespace utils
{

	void*	ScanModule(const char* module_name, const char* pattern);

	void	HookVirtualMethod(void* instance, int index, void* new_func);
	void*	GetVirtualMethod(void* instance, int index);

}