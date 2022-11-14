#include <kernel/memory.h>

struct{
	void* base;
	size_t length;
} available_memory;

void init_memory_manager(void* base, size_t length){
	available_memory.base = base;
	available_memory.length = length;
}

