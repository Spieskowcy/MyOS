#include <kernel/fat16.h>
char keyboard_decode(unsigned char scan_code);
void keyboard_handler(BiosParamBlock* img);
/* Input a byte from a port */
inline unsigned char inportb(unsigned int port)
{
   unsigned char ret;
   asm volatile ("inb %%dx,%%al":"=a" (ret):"d" (port));
   return ret;
}

/* Output a byte to a port */
inline void outportb(unsigned int port,unsigned char value)
{
   asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
}
