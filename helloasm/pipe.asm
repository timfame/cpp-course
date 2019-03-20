section .text
global _start
_start:

again:
xor rax, rax
xor rdi, rdi
mov rsi, buf
mov rdx, 1024
syscall
cmp rax, 0
jl read_fail
je exit
mov rdx, rax
mov rax, 1
mov rdi, 1
mov rsi, buf
syscall
cmp rax, rdx
jne write_fail
jmp again

exit:
mov rax, 60
xor rdi, rdi
syscall

read_fail: ud2
write_fail: ud2

section .bss
buf: resb 1024
