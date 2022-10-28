#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void transform_decimal(int value, size_t * num_of_nums, int* numbers, int base){
	while(value > 0){
		numbers[*num_of_nums] = value%base;
		(*num_of_nums)++;
		value = value/base;
	}
	num_of_nums--;
}

char* itoa(int value, char* str, int base){
	int numbers[12];
	size_t num_of_char = 0, num_of_nums = 0;
	if(value < 0){
		str[num_of_char] = '-';
		num_of_char++;
	}
	int unsign_val = abs(value);
	if(base == DECIMAL){
		transform_decimal(unsign_val, &num_of_nums, numbers, base);
	}
	while(num_of_nums){
		str[num_of_char] = numbers[num_of_nums]+49;
		num_of_nums--;
		num_of_char++;
	}
	str[num_of_char] = '\0';
	return str;
}
