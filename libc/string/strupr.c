#include <string.h>

char* strupr(char* str){
        int len = strlen(str);
        for(size_t index = 0; index < len; index++){
                if(96 < str[index] && str[index] < 123){
                        str[index] -= 32;
                }
        }
        return str;
}
