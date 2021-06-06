.code16
    mov $msg, %si 
    mov $0x0e, %ah
loop:
    lodsb /*utiliza SI para saber donde empeiza la instruccion y se cargue msg en el reg AL*/
    or %al, %al
    jz halt
    int $0x10
    jmp loop
halt:
    hlt
msg:
    .asciz "hello world"
