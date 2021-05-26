FLAG = as -g
FORM = ld --oformat binary

qemu: clean main.img
	qemu-system-i386 -drive 'file=main.img,format=raw' -serial mon:stdio -smp 2
main.img: main.o
	$(FORM) -o main.img -T link.ld main.o
main.o:
	$(FLAG) -o main.o main.S
clean:
	rm -f main.img main.o
#--oformat binary = el linker puede estar configuraro para que tenga object files
# 					diferentes del finario. Con esto nos aseguramos que solo sea binario.
#-mon = para poder comunicarse con una isntancia de qemu en este caso con el stdio

#qemu info https://documentation.suse.com/sles/15-GA/html/SLES-all/cha-qemu-monitor.html