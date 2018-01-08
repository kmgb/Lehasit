#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <assert.h>
#include <stdio.h>
#include <string>
#include <vector>

// Remove macro naming clashes with stdlib and source sdk
#undef min
#undef max
#undef DrawText