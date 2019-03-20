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

    _sys_exit:
    	mov rax, 1
    	mov rbx, 0
    	syscall
    	ret

_start:
	pop rax
	cmp rax, 2
	jne _sys_exit
	pop rax
	pop rdi
	mov rax, 2
	xor rsi, rsi
	xor rdx, rdx
	syscall
	cmp rax, 0
	jl _sys_exit
	mov r8, rax

	mov rbx, 0
	read:
		mov rdi, r8
		xor rax, rax
		mov rsi, file
		mov rdx, 1
		syscall
		cmp rax, 0
		je end_program	
		cmp [file], word 10
		jne read
		add rbx, 1
		jmp read
	end_program:
	mov rax, rbx
	call print_number

	mov rax, 60     ; sys_exit
	xor rdi, rdi
	syscall

section .bss
file: resb 1