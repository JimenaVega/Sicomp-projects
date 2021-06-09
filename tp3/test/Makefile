obj-m += test_read.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
#	make -C /home/paprika/Documents/Sistemas_de_computacion/practico/cross_comp/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
#	make -C /home/paprika/Documents/Sistemas_de_computacion/practico/cross_comp/ M=$(PWD) clean
