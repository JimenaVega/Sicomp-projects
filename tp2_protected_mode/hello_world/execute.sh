echo "generating main.o"
as -g -o main.o main.S
echo "generatin main.img"
ld --oformat binary -o main.img -T link.ld main.o
echo "initiate qemu"
qemu-system-x86_64 -hda main.img


#for gdb dashboard
#qemu-system-i386 -fda ./main.img -boot a -s -S -monitor stdio
#telnet remote localhost:1234
#br *0x7c00