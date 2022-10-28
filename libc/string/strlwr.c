#include <string.h>

char* strlwr(char* str){
	int len = strlen(str);
	for(size_t index = 0; index < len; index++){
		if(64 < str[index] && str[index] < 91){
			str[index] += 32;
		}
	}
	return str;
}
