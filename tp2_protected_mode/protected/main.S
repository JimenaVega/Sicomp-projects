.equ CODE_SEG, 8
.equ DATA_SEG,  gdt_data - gdt_start

/*Al comenzar en modo real, se le avisa al compilador que son instrucciones de 16 bits*/
.code16
    cli                 /*Se deshabilitan interrupciones*/
    lgdt gdt_descriptor /*Se carga la GDT*/

    /*Se setea el bit PE (Protection enable) en CR0*/
    mov %cr0, %eax
    orl $0x1, %eax
    mov %eax, %cr0
    ljmp $CODE_SEG, $protected_mode

gdt_start:
gdt_null:
    .long 0x0         /*El pentium no usa al descriptor 0 de la GDT*/
    .long 0x0         /*i.e. Indice = 0, TI = 0*/

gdt_code:             /*CS deberia apuntar a este descriptor*/
    .word 0xffff      /*Limite de segmento 0-15b*/
    .word 0x0         /*1-base address 0-15*/
    .byte 0x0         /*2-base 16-23b*/
    .byte 0b10011010  /*type = 1010, S = 1, DPL = 00, P =1*/
    .byte 0b11001111  /*limit = 1111, A = 0, DB = 1, G = 1*/
    .byte 0x0         /*3-base 24-31b*/

gdt_data:             /*DS, SS, ES, FS y GS debrian apuntar a este descriptor*/
    .word 0xffff      /*Limite de segmento 0-15b*/
    .word 0x0         /*1-base address 0-15*/
    .byte 0x0         /*2-base 16-23b*/
    .byte 0b10010010  /*type = 0010, S = 1, DPL = 00, P =1*/
    .byte 0b11001111  /*limit = 1111, A = 0, DB = 1, G = 1*/
    .byte 0x0         /*3-base 24-31b*/
gdt_end:
gdt_descriptor:     
    .word gdt_end - gdt_start /*Tamanio del GDT descriptor*/
    .long gdt_start           /*Limite*/
vga_current_line:
    .long 0

.code32
    protected_mode:
    /*Se actualiza la Descriptor Cache */
    mov $DATA_SEG, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    /*Direcion de pila, el esp apunta a 0x700*/
    mov $0X7000, %ebp
    mov %ebp, %esp

vga_print:
    mov $message, %ecx
    mov vga_current_line, %eax
    mov $0, %edx
    /* Numero de lineas horizontales */
    mov $25, %ebx
    div %ebx
    mov %edx, %eax
    /*ancho de linea* bytes por caracter en la pantalla -> 160 = 80 * 2*/
    mov $160, %edx
    mul %edx
    /* 0xb8000 =direccion de video que se muestra en la pantalla */
    lea 0xb8000(%eax), %edx
    /* Color blanco sobre negro */
    mov $0x0f, %ah
loop:
    mov (%ecx), %al
    cmp $0, %al
    je end
    mov %ax, (%edx)
    add $1, %ecx
    add $2, %edx
    jmp loop
end:
    hlt
message:
    .asciz "IM INN PROTECTED MODE :D" 

/* it is impossible to write to the code segment
 * it is impossible execute the data segment*/