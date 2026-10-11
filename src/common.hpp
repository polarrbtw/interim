#pragma once
#include <globals.hpp>
#include <settings.hpp>

#include <util/memory.hpp>
#include <util/math.hpp>

#include <sdk/Entity.hpp>
#include <sdk/offsets.hpp>

inline void foo() {}

// allow using memory functions in other files
extern Memory Mem;

// debug prints
#ifdef _DEBUG
	#define okay(msg, ...) printf("[+] " msg "\n", ##__VA_ARGS__)
	#define info(msg, ...) printf("[*] " msg "\n", ##__VA_ARGS__)
	#define error(msg, ...) printf("[-] " msg "\n", ##__VA_ARGS__)
#else
	#define okay(msg, ...)
	#define info(msg, ...)
	#define error(msg, ...)
#endif


// imgui colors
constexpr ImU32 RED = IM_COL32(255, 0, 0, 255);
constexpr ImU32 GREEN = IM_COL32(0, 255, 0, 255);
constexpr ImU32 BLUE = IM_COL32(0, 0, 255, 255);
constexpr ImU32 ORANGE = IM_COL32(255, 165, 0, 255);
constexpr ImU32 PINK = IM_COL32(255, 105, 180, 255);
constexpr ImU32 YELLOW = IM_COL32(255, 255, 0, 255);
constexpr ImU32 PURPLE = IM_COL32(128, 0, 128, 255);
constexpr ImU32 CYAN = IM_COL32(0, 255, 255, 255);
constexpr ImU32 MAGENTA = IM_COL32(255, 0, 255, 255);
constexpr ImU32 LIME = IM_COL32(50, 205, 50, 255);
constexpr ImU32 TEAL = IM_COL32(0, 128, 128, 255);
constexpr ImU32 WHITE = IM_COL32(255, 255, 255, 255);
constexpr ImU32 BLACK = IM_COL32(0, 0, 0, 255);
constexpr ImU32 GRAY = IM_COL32(128, 128, 128, 255);