                section         .text

                global          _start

 ;r11 -- char
    print_char:
        push r11
        mov [rsp], r11
        mov rax, 1
        mov rdi, 1
        mov rsi, rsp
        mov rdx, 1
        syscall
        pop r11
        ret

    print_number:
        cmp rax, 0
        jge parse
            mov r11, 45
            mov rbp, rax
            call print_char
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
              mov r11, rbx
              call print_char
          jmp loop2 
        end:
        mov r11, 10
        call print_char
        ret

_start:

                sub             rsp, 8 * 128 * 8
                lea             rdi, [rsp + 2 * 128 * 8]
                mov             rcx, 3
                call            read_long
                mov             rdi, rsp
                call            read_long
                lea             rsi, [rsp + 2 * 128 * 8]

                lea             r9, [rsp + 6 * 128 * 8]
                call            set_zero_result
                lea             r9, [rsp + 6 * 128 * 8]

                lea             r10, [rsp + 4 * 128 * 8]
                call            copy_array_save
                lea             r10, [rsp + 4 * 128 * 8]

                call            mul_long_long
                call            swap_result
                call            write_long

                mov             al, 0x0a
                call            write_char

                jmp             exit

set_zero_result:
                push            rbp
                push            rax
                mov             rbp, 256

.loop:
                xor             rax, rax
                mov             [r9], rax
                lea             r9, [r9 + 8]
                dec             rbp
                jnz             .loop

                pop             rax
                pop             rbp
                ret


copy_array_result:
                push            rbp
                push            rcx
                push            rax
                mov             rbp, rdi
.loop:
                mov             rax, [rbp]
                mov             [r9], rax
                lea             rbp, [rbp + 8]
                lea             r9, [r9 + 8]
                dec             rcx
                jnz             .loop

                pop             rax
                pop             rcx
                pop             rbp
                ret

copy_array_save:
                push            rbp
                push            rcx
                push            rax
                mov             rbp, rdi
.loop:
                mov             rax, [rbp]
                mov             [r10], rax
                lea             rbp, [rbp + 8]
                lea             r10, [r10 + 8]
                dec             rcx
                jnz             .loop

                pop             rax
                pop             rcx
                pop             rbp
                ret

;swaps rdi and r9
swap_result:
                push            rbp
                mov             rbp, rdi
                mov             rdi, r9
                mov             r9, rbp
                pop             rbp
                ret

;swaps rdi and r10
swap_save:
                push            rbp
                mov             rbp, rdi
                mov             rdi, r10
                mov             r10, rbp
                pop             rbp
                ret

; multiply two long number
;    rdi -- address of #1 (long number)
;    rsi -- address of #2 (long number)
;    rcx -- length of long numbers in qwords
; result:
;    is written to rdi
mul_long_long:
                push            rdi
                push            rsi
                push            rcx
                push            rbx

                clc
                mov             rbp, 0
.loop:
                mov             rbx, [rsi]
                lea             rsi, [rsi + 8]

               ; cmp rbp, 0
               ; jne .need
               ; call swap_result
               ; mov [rdi], rbx
               ; call write_long
               ; .need:

                call            mul_long_short ; rdi = cur_mul
                call write_long
                call            swap_save ; r10 = cur_mul rdi = start

                mov r11, rbp
.pow:
                cmp             r11, 0
                je              .finish_pow
                dec             r11
                mov             rbx, 10
                call            swap_save
                call            mul_long_short
                call            swap_save
                jmp             .pow
.finish_pow:          
                mov             rsi, r10 ; rsi = cur_mul

                call            swap_result ; rdi = result, r9 = start
                call            add_long_long ; rdi = sum
                call            swap_result ; rdi = start, r9 = result

                call            copy_array_save; r10 = rdi = start
                lea             rdi, [rdi + 8]
                inc             rbp
                dec             rcx
                jnz             .loop

                pop             rbx
                pop             rcx
                pop             rsi
                pop             rdi
                ret

; adds two long number
;    rdi -- address of summand #1 (long number)
;    rsi -- address of summand #2 (long number)
;    rcx -- length of long numbers in qwords
; result:
;    sum is written to rdi
add_long_long:
                push            rdi
                push            rsi
                push            rcx

                clc
.loop:
                mov             rax, [rsi]
                lea             rsi, [rsi + 8]
                adc             [rdi], rax
                lea             rdi, [rdi + 8]
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rsi
                pop             rdi
                ret

; adds 64-bit number to long number
;    rdi -- address of summand #1 (long number)
;    rax -- summand #2 (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    sum is written to rdi
add_long_short:
                push            rdi
                push            rcx
                push            rdx

                xor             rdx,rdx
.loop:
                add             [rdi], rax
                adc             rdx, 0
                mov             rax, rdx
                xor             rdx, rdx
                add             rdi, 8
                dec             rcx
                jnz             .loop

                pop             rdx
                pop             rcx
                pop             rdi
                ret

; multiplies long number by a short
;    rdi -- address of multiplier #1 (long number)
;    rbx -- multiplier #2 (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    product is written to rdi
mul_long_short:
                push            rax
                push            rdi
                push            rcx

                xor             rsi, rsi
.loop:
                mov             rax, [rdi]
                mul             rbx
                add             rax, rsi
                adc             rdx, 0
                mov             [rdi], rax
                add             rdi, 8
                mov             rsi, rdx
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rdi
                pop             rax
                ret

; divides long number by a short
;    rdi -- address of dividend (long number)
;    rbx -- divisor (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    quotient is written to rdi
;    rdx -- remainder
div_long_short:
                push            rdi
                push            rax
                push            rcx

                lea             rdi, [rdi + 8 * rcx - 8]
                xor             rdx, rdx

.loop:
                mov             rax, [rdi]
                div             rbx
                mov             [rdi], rax
                sub             rdi, 8
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rax
                pop             rdi
                ret

; assigns a zero to long number
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
set_zero:
                push            rax
                push            rdi
                push            rcx

                xor             rax, rax
                rep stosq

                pop             rcx
                pop             rdi
                pop             rax
                ret

; checks if a long number is a zero
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
; result:
;    ZF=1 if zero
is_zero:
                push            rax
                push            rdi
                push            rcx

                xor             rax, rax
                rep scasq

                pop             rcx
                pop             rdi
                pop             rax
                ret

; read long number from stdin
;    rdi -- location for output (long number)
;    rcx -- length of long number in qwords
read_long:
                push            rcx
                push            rdi

                call            set_zero
.loop:
                call            read_char
                or              rax, rax
                js              exit
                cmp             rax, 0x0a
                je              .done
                cmp             rax, '0'
                jb              .invalid_char
                cmp             rax, '9'
                ja              .invalid_char

                sub             rax, '0'
                mov             rbx, 10
                call            mul_long_short
                call            add_long_short
                jmp             .loop

.done:
                pop             rdi
                pop             rcx
                ret

.invalid_char:
                mov             rsi, invalid_char_msg
                mov             rdx, invalid_char_msg_size
                call            print_string
                call            write_char
                mov             al, 0x0a
                call            write_char

.skip_loop:
                call            read_char
                or              rax, rax
                js              exit
                cmp             rax, 0x0a
                je              exit
                jmp             .skip_loop

; write long number to stdout
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
write_long:
                push            rax
                push            rcx

                mov             rax, 20
                mul             rcx
                mov             rbp, rsp
                sub             rsp, rax

                mov             rsi, rbp

.loop:
                mov             rbx, 10
                call            div_long_short
                add             rdx, '0'
                dec             rsi
                mov             [rsi], dl
                call            is_zero
                jnz             .loop

                mov             rdx, rbp
                sub             rdx, rsi
                call            print_string

                mov             rsp, rbp
                pop             rcx
                pop             rax
                ret

; read one char from stdin
; result:
;    rax == -1 if error occurs
;    rax \in [0; 255] if OK
read_char:
                push            rcx
                push            rdi

                sub             rsp, 1
                xor             rax, rax
                xor             rdi, rdi
                mov             rsi, rsp
                mov             rdx, 1
                syscall

                cmp             rax, 1
                jne             .error
                xor             rax, rax
                mov             al, [rsp]
                add             rsp, 1

                pop             rdi
                pop             rcx
                ret
.error:
                mov             rax, -1
                add             rsp, 1
                pop             rdi
                pop             rcx
                ret

; write one char to stdout, errors are ignored
;    al -- char
write_char:
                sub             rsp, 1
                mov             [rsp], al

                mov             rax, 1
                mov             rdi, 1
                mov             rsi, rsp
                mov             rdx, 1
                syscall
                add             rsp, 1
                ret

exit:
                mov             rax, 60
                xor             rdi, rdi
                syscall

; print string to stdout
;    rsi -- string
;    rdx -- size
print_string:
                push            rax

                mov             rax, 1
                mov             rdi, 1
                syscall

                pop             rax
                ret


                section         .rodata
invalid_char_msg:
                db              "Invalid character: "
invalid_char_msg_size: equ             $ - invalid_char_msg
