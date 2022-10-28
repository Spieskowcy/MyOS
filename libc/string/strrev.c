#include <string.h>

char* strrev(char* str){
	int len = strlen(str)-1;
	char tmp;
	for(size_t index = 0; index <= len/2; index++){
		tmp = str[index];
		str[index] = str[len-index];
		str[len-index] = tmp;
	}
	return str;
}
