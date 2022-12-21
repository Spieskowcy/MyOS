#include <stddef.h>
#include <stdint.h>

#define USED_FLAG 1

typedef struct {
    size_t size;
    uint32_t flags;
} memory_descriptor_t;

void init_memory_manager(void* base, size_t length);

void* allocate_next(memory_descriptor_t* desc, size_t length);

void deallocate(memory_descriptor_t* desc);

void* malloc(size_t length);

memory_descriptor_t* get_initial_memory_descriptor();




