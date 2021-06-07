#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LEN 64

int main(){
    char buffer[LEN];

    while(1){
        printf("Seleccione la señal a leer (1 - 2): ");
        
        if(fgets(buffer, LEN, stdin) == NULL){
            printf("fgets() error.\n");

            exit(EXIT_FAILURE);
        }

        strcpy(buffer, strtok(buffer, "\n"));
        printf("Input: %s\n", buffer);
        
        if(strcmp(buffer, "exit") == 0){
            printf("Exiting...\n");
            break;
        }
        else if(strcmp(buffer, "1") == 0){
            system("sudo echo \"test\" > /dev/gpio_rbp");
        }
    }

    return 0;
}