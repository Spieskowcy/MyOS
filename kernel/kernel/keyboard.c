#include <kernel/keyboard.h>

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

void enable_cursor(unsigned char cursor_start, unsigned char cursor_end)
{
	outportb(0x3D4, 0x0A);
	outportb(0x3D5, (inportb(0x3D5) & 0xC0) | cursor_start);
 
	outportb(0x3D4, 0x0B);
	outportb(0x3D5, (inportb(0x3D5) & 0xE0) | cursor_end);
}
void update_cursor(int x, int y)
{
	unsigned int pos = y * VGA_WIDTH + x;
 
	outportb(0x3D4, 0x0F);
	outportb(0x3D5, (unsigned char) (pos & 0xFF));
	outportb(0x3D4, 0x0E);
	outportb(0x3D5, (unsigned char) ((pos >> 8) & 0xFF));
}
void keyboard_handler()
{
	int x=0, y=0;
	unsigned char* display_address = 0xB8000;
	enable_cursor(0,15);
	update_cursor(0,0);
	unsigned int port = 0x60;
	char text[2];
	text[1] = '\0';
	while(1)
	{
		update_cursor(x,y);
		if(x==VGA_WIDTH)
		{
			y++;
			x=0;
		}
		if(inportb(0x64) & 1)
		{
			unsigned char scan_code = inportb(port);
			//printf(" %X", scan_code);
			if(scan_code==0x1C) //enter
			{
				y++;
				x=0;
				continue;
			}
			if(scan_code==0x0E) //backspace
			{
				if(x==0 && y==0)
					continue;
				if(x==0)
				{
					x=VGA_WIDTH-1;
					y--;
				}
				else
				{
					x--;
				}
				*(display_address+2*x+2*y*VGA_WIDTH) = ' ';
				continue;
			}
			unsigned char sign = keyboard_decode(scan_code);
			if(sign!='\0')
			{
				*(display_address+2*x+2*y*VGA_WIDTH) = sign;
				*(display_address+2*x+2*y*VGA_WIDTH+1) = 0x0F;
				x++;
			}
			
		}
	}
}
 