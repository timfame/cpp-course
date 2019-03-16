section .text
global _start

section .data
digit: dw ""

    print_number:
        cmp rax, 0
        jge parse
            mov [digit], word 45
            mov rbp, rax
            mov rax, 1
            mov rdi, 1
            mov rsi, digit
            mov rdx, 1
            syscall
            mov rax, rbp
            neg rax
        parse:
        push -1
        loop1:      
          xor rdx, rdx
          mov rcx, 10
          div rcx
          mov rbx, rdx
          push rbx
          cmp rax, 0
          jne loop1
        loop2:
          pop rbx
          cmp rbx, -1
          je end
              add rbx, 48
              mov [digit], rbx
              mov rax, 1
              mov rdi, 1
              mov rsi, digit
              mov rdx, 1
              syscall
          jmp loop2 
        end:
        mov [digit], word 10
        mov rax, 1
        mov rdi, 1
        mov rsi, digit
        mov rdx, 1
        syscall
        ret

_start:
    mov rax, -1387249812374981
    call print_number

mov rax, 60     ; sys_exit
xor rdi, rdi
syscall


