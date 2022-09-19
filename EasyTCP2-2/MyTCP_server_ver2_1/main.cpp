#include <iostream>
#include <stdlib.h>
int main()
{
    /*
    char *data1 = new char[128];
    delete[] data1;
    char *data2 = new char;
    delete data2;
    char *data3 = new char[64];
    delete[] data3;
    */
    char* data[1100];
    for (int i = 0; i < 1100; i++) {
        data[i] = new char[1+i];
        
    }
    for (int i = 0; i < 1100; i++) {
        delete[] data[i];
    }
    return 0;
}