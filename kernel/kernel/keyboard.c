#include <kernel/keyboard.h>
#include <kernel/tty.h>
#include <string.h>
#include <stdio.h>

char scan_codes[][3] =
{
	{0x02,'1','!'},
	{0x03,'2','@'},
	{0x04,'3','#'},
	{0x05,'4','$'},
	{0x06,'5','%'},
	{0x07,'6','^'},
	{0x08,'7','&'},
	{0x09,'8','*'},
	{0x0A,'9','('},
	{0x0B,'0',')'},
	{0x10,'q','Q'},
	{0x11,'w','W'},
	{0x12,'e','E'},
	{0x13,'r','R'},
	{0x14,'t','T'},
	{0x15,'y','Y'},
	{0x16,'u','U'},
	{0x17,'i','I'},
	{0x18,'o','O'},
	{0x19,'p','P'},
	{0x1E,'a','A'},
	{0x1F,'s','S'},
	{0x20,'d','D'},
	{0x21,'f','F'},
	{0x22,'g','G'},
	{0x23,'h','H'},
	{0x24,'j','J'},
	{0x25,'k','K'},
	{0x26,'l','L'},
	{0x2C,'z','Z'},
	{0x2D,'x','X'},
	{0x2E,'c','C'},
	{0x2F,'v','V'},
	{0x30,'b','B'},
	{0x31,'n','N'},
	{0x32,'m','M'},
	{0x39,' ',' '},
};

char keyboard_decode(unsigned char scan_code)
{
	static int shifted = 0;
	static int released = 0;
    char ret = '\0';

	switch (scan_code)
	{
		case 0x36:  // right shift
		case 0x2A:  // left shift
			shifted = 1;
			break;
		case 0xAA:  // left shift
		case 0xB6:  // right shift
			shifted = 0;
			break;
		default:
			int i;
			int size = sizeof(scan_codes) / sizeof(scan_codes[0]);
			for (i = 0; i<size; i++)
			{
				if (scan_code == scan_codes[i][0])
				{
					
					if (shifted)
						ret = scan_codes[i][2];
					else	
						ret = scan_codes[i][1];
					break;
				}
			}
	}

    return ret;
}


void keyboard_handler(BiosParamBlock* img)
{
	unsigned int port = 0x60;
	int state = 0;
	char fname[8];
	while(1)
	{
		if(inportb(0x64) & 1)
		{
			unsigned char scan_code = inportb(port);
			//printf(" %X", scan_code);
			if(scan_code==0x1C) //enter
			{
				char text[VGA_WIDTH];
				int length = terminal_getstring(text);
				terminal_putchar('\n');
				if(state == 0){
					if(memcmp(text, "ping",length) == 0)
						printf("pong\n");
					else if(memcmp(text, "save", length) == 0){
						state = 1;
						printf("File name: ");
					}else if(memcmp(text, "load", length) == 0){
						state = 3;
						printf("File name: ");

					} else {
						printf(text);
						terminal_putchar('\n');
					}
				} else if (state == 1){
					memcpy(fname, text, length);
					printf("File data: ");
					state = 2;
				} else if (state == 2){
				 DirEntry* entry = FatAddFile(img, fname, text, length);
					state = 0;
				}else if(state == 3){
					FatPrintFile(img, text);
					state = 0;
				}
					
				continue;
			}
			if(scan_code==0x0E) //backspace
			{
				terminal_putchar(0x08);
				continue;
			}
			unsigned char sign = keyboard_decode(scan_code);
			if(sign!='\0')
			{
				terminal_putchar(sign);
			}
			
		}
	}
}
 
