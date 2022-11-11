#include <kernel/memory.h>

struct{
	void* base;
	size_t length;
} avaiable_memory;

void init_memory_manager(void* base, size_t length){
	avaiable_memory.base = base;
	avaiable_memory.length = length;
}

