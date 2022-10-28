#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


char get_ascii(int number){
	if(number < 10){
		return number+48;
	}else{
		return number+55;
	}
}

void split_to_numbers(int value, size_t * num_of_char, char* str, int base){
        while(value > 0){
                str[(*num_of_char)] = get_ascii(value%base);
                (*num_of_char)++;
                value = value/base;
        }
}

char* itoa(int value, char* str, int base){
	size_t num_of_char = 0;
	int unsign_val = abs(value);
	split_to_numbers(unsign_val, &num_of_char, str, base);
	if(value < 0){
                str[num_of_char] = '-';
                num_of_char++;
        }

	str[num_of_char] = '\0';
//	strrev(str);
	return str;
}
