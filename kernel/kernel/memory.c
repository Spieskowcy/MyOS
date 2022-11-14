#include <kernel/memory.h>

struct{
	void* base;
	size_t length;
} available_memory;

void init_memory_manager(void* base, size_t length){
	available_memory.base = base;
	available_memory.length = length;

	memory_descriptor_t* desc = available_memory.base;
	desc->size = available_memory.length;
	desc->flags = 0;
}

void* allocate_next(memory_descriptor_t* desc, size_t length){

}

void deallocate(memory_descriptor_t* desc){
	desc &= !USED_FLAG;
	
}

memory_descriptor_t* get_initial_memory_descriptor(){
	return available_memory.base;

}

size_t get_available_memory(){
	return available_memory.length - available_memory.base;
}
