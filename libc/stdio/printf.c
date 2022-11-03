#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

bool check_errno(size_t maxrem, size_t len){
	if(maxrem < len){
		// TODO: Set errno to EOVERFLOW.
		return true;
	}
	return false;
}

char* prepare_number(int raddix, int value, bool to_lower, char* str){ 
	itoa(value, str, raddix);
        if(to_lower)
                strlwr(str);
        return str;
}

char* prepare_pointer(int value, char* str){

        str[0] = '0';
        str[1] = 'x';
        size_t idx = 2;
        unsigned int mask = 0xF0000000;
        while((mask&value) == 0){
        	str[idx] = '0';
                idx++;
                mask >>= 4;
        }
        itoa(value, str + idx, HEX);
	return str;
}

size_t print_string(char* str, size_t maxrem){
	size_t len = strlen(str);
	if (check_errno(maxrem, len))
		return -1;
	if(!print(str, len))
		return -1;
	return len;
}

size_t print_not_format(const char* restrict* format, size_t maxrem){
        if ((**format) != '%' || (*(*format+1)) == '%') {
                if ((**format) == '%')
                        (*format)++;
                size_t amount = 1;
                while ((*(*format+amount)) && (*(*format+amount)) != '%')
                        amount++;
                if (check_errno(maxrem, amount))
                        return -1;
                if (!print(*format, amount))
                        return -1;
                (*format) += amount;
                return amount;
        }
        return 0;
}

size_t print_format(const char* restrict* format, va_list* parameters,int maxrem){
	size_t len = -1;
	switch(**format)
	{
		case 'c':
			(*format)++;
			char c[2] = { (char) va_arg(*parameters, int /* char promotes to int */), '\0' };
                        len = print_string(&c, maxrem);
                        if((int) len == -1)
                                return -1;
			break;
		case 's':
			(*format)++;
			const char* str = va_arg(*parameters, const char*);
                        len = print_string(str, maxrem);
                        if ((int) len == -1)
                                return -1;
			break;
		case 'd':
		case 'i':
			(*format)++;
			char str_int[12] = "";
                        len = print_string(prepare_number(DECIMAL, va_arg(*parameters, int), false, str_int), maxrem);
                        if((int) len == -1)
                                return -1;
			break;
		case 'X':
		case 'x':
			bool to_lower = (**format) == 'x' ? true : false;
                        (*format)++;
			char str_hex[10] = "";
                        len = print_string(prepare_number(HEX, va_arg(*parameters, int), to_lower,  str_hex), maxrem);
                        if((int) len == -1)
                                return -1;
			break;
		case 'b':
			(*format)++;
			char str_bin[34] = "";
                        len = print_string(prepare_pointer(va_arg(*parameters, void*), str_bin), maxrem);
                        if ((int)len == -1)
                                return -1;
			break;
		case 'p':
			(*format)++;
			char str_ptr[12] = "";
                        len = print_string(prepare_pointer((int)va_arg(*parameters, void*), str_ptr), maxrem);
                        if ((int) len == -1)
                                return -1;
			break;
		default:
			(*format)--;
			len = print_string(*format, maxrem);
                        if((int) len == -1)
                                return -1;
                        (*format) += len;
			break;
	}
}

int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);
	
	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;
		
		size_t amount = print_not_format(&format,maxrem);
		if((int) amount == -1)
			return -1;
		written += amount;
		if (amount == 0)
			continue;
		
		
		format++;
		size_t len = print_format(&format, &parameters, maxrem);
		if((int) len == -1)
			return -1;
		written += len;
	}

	va_end(parameters);
	return written;
}
