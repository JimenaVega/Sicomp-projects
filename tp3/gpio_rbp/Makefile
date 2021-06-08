CC=gcc
CFLAGS= -g -Wall -pedantic -Wextra -Wconversion -std=gnu11 #-Werror
obj-m += gpio_rbp.o

all:
	$(CC) $(CFLAGS) user_cli.c -o user_cli
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
#	make -C /home/paprika/Documents/Sistemas_de_computacion/practico/cross_comp/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
#	make -C /home/paprika/Documents/Sistemas_de_computacion/practico/cross_comp/ M=$(PWD) clean