#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>


int get_average_lenght(int len_1,int len_2)
{
    return (len_1 + len_2) / 2;
}

int get_avarage_value_of_string(char* str)
{
    int avarage=0,i=0;

    while(str[i] != '\0')
    {
        avarage += (int) str[i];
        i++;
    }

    return avarage / i;
}

uint32_t generate_key(char* p1,char* p2)
{
    uint32_t key = 0; //key divided into three parts (2 two byte parts and 1 four byte parts)


    key += time(NULL) % 256; // first part is two bytes = epoch_time%255
    key = key << 8; //shifts the first part of the key 8 bits (2bytes) to the left

    key += (get_average_lenght(strlen(p1),strlen(p2)) * 125) % 255; // second part is two bytes = (average_length_of_inputs * 125) % 255
    key = key << 16;//shifts the second part of the key 16 bits (4 bytes) to the left

    key += get_avarage_value_of_string(p1) * get_avarage_value_of_string(p2);//third part is four bytes = avg. value of char in p1 * avg. value of char in p2

    return key;
}

char* encrypt()
{
    return 0;
}

char* decrypt()
{
    return 0;
}



//main is for testing the encryption/decryption functions
int main()
{
    char* p1 = (char*) "A1B2C3D4F5";
    char* p2 = (char*) "F5D4C3B2A1";


    //printf("p1/size:%s/%d\np2/size:%s/%d\naverage length:%d\n",p1,strlen(p1),p2,strlen(p2),get_average_lenght(strlen(p1),strlen(p2)));
    generate_key(p1,p2);
}
