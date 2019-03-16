                section         .text

                global          _start


_start:
                pop             rax
                cmp             rax, 2
                jne             bad_args

                pop             rax
                pop             rdi
                mov             rax, 2
                xor             rsi, rsi
                xor             rdx, rdx
                syscall
                cmp             rax, 0
                jl              open_fail
                mov             rdi, rax

                xor             rbx, rbx
                movdqa          xmm0, oword [new_line_mask]
                pxor            xmm3, xmm3

.read_again:
                xor             rax, rax
                mov             rsi, buf
                mov             rdx, buf_size * 16
                syscall

                cmp             rax, 0
                jz              exit
                jl              read_fail

                mov             rsi, buf
                mov             rdx, rax
                shr             rdx, 11
                or              rdx, rdx
                jz              .skip_by_2k_chunks

.next_2k_chunk:
                mov             rcx, 32
                xorpd           xmm2, xmm2

.next_64b_chunk:
                movdqa          xmm1, xmm0
                pcmpeqb         xmm1, oword [rsi]
                movdqa          xmm4, xmm0
                pcmpeqb         xmm4, oword [rsi + 16]
                movdqa          xmm5, xmm0
                pcmpeqb         xmm5, oword [rsi + 32]
                movdqa          xmm6, xmm0
                pcmpeqb         xmm6, oword [rsi + 48]
                paddb           xmm1, xmm4
                paddb           xmm5, xmm6
                psubb           xmm2, xmm1
                psubb           xmm2, xmm5
                add             rsi, 64
                sub             rcx, 1
                jnz             .next_64b_chunk

                psadbw          xmm2, xmm3
                movd            ecx, xmm2
                add             rbx, rcx
                movhlps         xmm2, xmm2
                movd            ecx, xmm2
                add             rbx, rcx
                dec             rdx

                jnz             .next_2k_chunk

.skip_by_2k_chunks:
                mov             rcx, rax
                and             rcx, 0x7ff
                shr             rcx, 4
                or              rcx, rcx
                jz              .skip_by_16b_chunk

                xorpd           xmm2, xmm2

.next_16b_chunk:
                movdqa          xmm1, oword [rsi]
                add             rsi, 16
                pcmpeqb         xmm1, xmm0
                psubb           xmm2, xmm1
                dec             rcx
                jnz             .next_16b_chunk

                psadbw          xmm2, xmm3
                movd            ecx, xmm2
                add             rbx, rcx
                movhlps         xmm2, xmm2
                movd            ecx, xmm2
                add             rbx, rcx

.skip_by_16b_chunk:
                and             rax, 15
                jz              .read_again

.next_byte:
                mov             dl, byte [rsi]
                cmp             dl, 10
                jne             .not_eof
                inc             rbx
.not_eof:
                inc             rsi
                dec             rax
                jnz             .next_byte

                jmp             .read_again

exit:
                mov             rax, 3
                syscall

                mov             rax, rbx
                call            write_number

                mov             rax, 60
                xor             rdi, rdi
                syscall

; rax -- number to print
write_number:
                mov             rbp, rsp
                mov             rdi, rsp
                sub             rsp, 24

                dec             rdi
                mov             byte [rdi], 10

                or              rax, rax
                jz              .write_zero

                mov             ebx, 10
.loop:
                xor             edx, edx
                div             rbx

                add             edx, '0'
                dec             rdi
                mov             byte [rdi], dl

                or              rax, rax
                jnz             .loop
                jmp             .print

.write_zero:
                dec             rdi
                mov             byte [rdi], '0'

.print:
                mov             eax, 1
                mov             rsi, rdi
                mov             rdx, rbp
                sub             rdx, rdi
                mov             edi, eax
                syscall

                mov             rsp, rbp
                ret

bad_args:
                mov             rsi, bad_args_msg
                mov             rdx, bad_args_msg_size
                jmp             print_error_and_quit

open_fail:
                mov             rsi, open_fail_msg
                mov             rdx, open_fail_msg_size
                jmp             print_error_and_quit

read_fail:
                mov             rsi, read_fail_msg
                mov             rdx, read_fail_msg_size
                jmp             print_error_and_quit

write_fail:
                mov             rsi, write_fail_msg
                mov             rdx, write_fail_msg_size
                jmp             print_error_and_quit

print_error_and_quit:
                mov             rax, 1
                mov             rdi, 1
                syscall

                mov             rax, 60
                mov             rdi, 1
                syscall

                section         .rodata
bad_args_msg:   db              "argument number mismatch", 10
bad_args_msg_size: equ $ - bad_args_msg
open_fail_msg:  db              "open failed", 10
open_fail_msg_size: equ $ - open_fail_msg
read_fail_msg:  db              "read failed", 10
read_fail_msg_size: equ $ - read_fail_msg
write_fail_msg: db              "write failed", 10
write_fail_msg_size: equ $ - write_fail_msg

                section         .bss

buf_size:       equ             1024 * 16 / 16
buf:            reso            buf_size

                section         .rodata

                align 16, db 0
new_line_mask:  times 16 db     10
