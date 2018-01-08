#include "stdafx.h"
#include "memory.h"

#include <assert.h>
#include <string>
#include <Windows.h>

char CharsToByte(const char *chars)//(&chars)[2])
{
	char nibbleA = toupper(chars[0]);
	char nibbleB = toupper(chars[1]);

	nibbleA = (nibbleA >= '0' && nibbleA <= '9') ? nibbleA - '0' : (nibbleA >= 'A' && nibbleA <= 'F') ? (nibbleA - 'A') + 0xA : -1;
	nibbleB = (nibbleB >= '0' && nibbleB <= '9') ? nibbleB - '0' : (nibbleB >= 'A' && nibbleB <= 'F') ? (nibbleB - 'A') + 0xA : -1;
	assert(nibbleA != -1 && nibbleB != -1);

	// Return the characters representing nibbles stitched together to make a byte
	// ex. nibbleA = '9' -> 9, nibbleB = 'A' -> A (10)
	// ((nibbleA << 4 = 144) | (nibbleB = 10)) = 154 (9A)
	return (nibbleA << 4 | nibbleB);
}

// Pattern style: AA BB CC ? ? ? ? AA BB CC
void* ScanForPattern(void* start, size_t length, const char* pattern)
{
	// This function assumes the pattern isn't malformed
	int patternLength = strlen(pattern);

	int patternIndex = 0;
	void* matchStart = nullptr;

	for (size_t currentAddress = (size_t)start; currentAddress < (size_t)start + length; ++currentAddress)
	{
		// We finished the pattern and everything matched, return what we got
		if (patternIndex >= patternLength)
			return matchStart;

		// Wildcard
		if (pattern[patternIndex] == '?')
		{
			patternIndex += 2; // Skip the question mark and the space
			continue;
		}

		if (CharsToByte(&pattern[patternIndex]) == *(char*)currentAddress)
		{
			if (!matchStart)
				matchStart = (void*)currentAddress;

			patternIndex += 3; // Skip the two characters and the space
		}
		else
		{
			// Doesn't match pattern, reset everything and continue from where we started finding the match
			patternIndex = 0;

			if (matchStart)
				currentAddress = (size_t)matchStart;

			matchStart = nullptr;
		}
	}

	return {};
}

void* utils::ScanModule(const char* module_name, const char* pattern)
{
	HMODULE module_handle = GetModuleHandleA(module_name);
	PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)module_handle;
	PIMAGE_NT_HEADERS nt_headers = (PIMAGE_NT_HEADERS)(((DWORD)module_handle) + dos_header->e_lfanew);
	return ScanForPattern((void*)((size_t)module_handle + nt_headers->OptionalHeader.BaseOfCode),
		nt_headers->OptionalHeader.SizeOfCode,
		pattern);
}

void utils::HookVirtualMethod(void* instance, int index, void* new_func)
{
	void** method_table = *(void***)instance;
	void** pp_func = method_table + index;

	DWORD old_protection, dummy_protection;

	VirtualProtect(pp_func, sizeof(pp_func), PAGE_EXECUTE_READWRITE, &old_protection);

	*pp_func = new_func;

	VirtualProtect(pp_func, sizeof(pp_func), old_protection, &dummy_protection);
}

void* utils::GetVirtualMethod(void* instance, int index)
{
	void** method_table = *(void***)instance;
	return method_table[index];
}