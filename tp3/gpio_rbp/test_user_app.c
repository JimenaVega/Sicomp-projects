#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#define TAM 100

int main(int argc, char *argv[]){

    assert(argc > 1);
    char buf[TAM];
    char i = 0;

    memset(buf, 0, 100);

    printf("User enter this : %s\n", argv[1]);
    int fp = open("/dev/test_read", O_RDWR);

    write(fp, argv[1], strlen(argv[1]));

    read(fp, &buf[i], 1);

    printf("Señal que llego = %s\n", buf);

    close(fp);
}