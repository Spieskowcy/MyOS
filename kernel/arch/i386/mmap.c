#include <kernel/multiboot.h>
#include <kernel/memory.h>
#include <stdint.h>

void _init_mmap(multiboot_info_t* mbd, uint32_t magic) {
    if(magic != MULTIBOOT_BOOTLOADER_MAGIC || !(mbd->flags >> 6 & 0x1)) {
        return; // at this point no logging system is initialised
    }
 
    for(int i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) {
        multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (mbd->mmap_addr + i);
 
        if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE && !mmmt->addr_low) {
            init_memory_manager((void*)mmmt->addr_low, (size_t) mmmt->len_low);
        }
    }
}
