; vim:ft=nasm
; x86
; NASM version 2.15.05

BITS 16					; Instruct the system this is 16-bit code

						; This is the entry point, nothing should happen before this
						; other than setting the instruction size
main:
	mov ax, 07C0h		; Setup 4KB stack space after this bootloader
	add ax, 288			; (4096 + 515) / 16 bytes (aligned) per paragraph
	cli					; Disable interrupts (solves old DOS bug)
	mov ss, ax			; Assign current stack segment
	mov sp, 4096		; Setup our stack pointer
	sti					; Enable interrupts (solvs old DOS bug)
	mov ax, 07C0h		; 07C0h is where our program is located
	mov ds, ax			; Set data segment to the load point of our program
	call run			; Start the main loop

;------------------------------------------------------------------------------
; Constants
;------------------------------------------------------------------------------
COM1 equ 3F8h
COM2 equ 2F8h

%macro outb 2
    mov dx, %1
    mov al, %2
    out dx, al
%endmacro

;------------------------------------------------------------------------------
; The main loop of our program
;------------------------------------------------------------------------------
run:
	call init_com1 			; Init com1
	call init_com2 			; Init com2

	mov al, 'B'				; Set
	call putc_com1			; Put character to com1

	call wait_com2			; Wait data receive

	xor ax, ax				; Clear
	call getc_com2			; Read character from com2

	call putc				; Print

.loop:
	jmp .loop

;------------------------------------------------------------------------------
; Init COM
;------------------------------------------------------------------------------
init_com1:
	outb COM1 + 3, 10000000b ; Set DLAB
	outb COM1 + 0, 00h		 ; Divisor latch (9600 baud)
	outb COM1 + 1, 0ch		 ; 
	outb COM1 + 3, 00100011b ; Unset DLAB, 8 bits word, 1 stop bit, no parity
	outb COM1 + 1, 00000000b ; Disable interrupts
	outb COM1 + 4, 00000011b ; Set DTR, RTS
	ret

init_com2:
	outb COM2 + 3, 10000000b ; Set DLAB
	outb COM2 + 0, 00h		 ; Divisor latch (9600 baud)
	outb COM2 + 1, 0ch		 ; 
	outb COM2 + 3, 00100011b ; Unset DLAB, 8 bits word, 1 stop bit, no parity
	outb COM2 + 1, 00000000b ; Disable interrupts
	outb COM2 + 4, 00000011b ; Set DTR, RTS
	ret

;------------------------------------------------------------------------------
; Write COM1
;------------------------------------------------------------------------------
putc_com1:
	mov dx, COM1 + 0		; Send data
	out dx, al				;
	ret						;

;------------------------------------------------------------------------------
; Wait COM2
;------------------------------------------------------------------------------
wait_com2:
.test:
	mov dx, COM2 + 5		; Data ready
	in al, dx				; 
	test al, 00000001b		; Test data ready is set
	jz .test				;
	ret						;

;------------------------------------------------------------------------------
; Read COM2
;------------------------------------------------------------------------------
getc_com2:
	mov dx, COM2			; Read data
	in al, dx				;
	ret						;

;------------------------------------------------------------------------------
; Print character subroutine
;------------------------------------------------------------------------------
putc:
	push ax					
	mov ah, 0Eh
	mov bl, 0Fh
	int 10h
	pop ax
	ret

;------------------------------------------------------------------------------
; Print string subroutine
;------------------------------------------------------------------------------
puts:
	push ax
	mov ah, 0Eh
	mov bl, 0Fh
.repeat:
	lodsb
	cmp al, 00h
	je .done
	int 10h
	jmp .repeat
.done:
	pop ax
	ret

;------------------------------------------------------------------------------
; Boot loaders are 512 bytes in size so pad the remaining bytes with 0
;------------------------------------------------------------------------------

times 510-($-$$) db 0	; Pad (510 - current position) bytes of 0

dw 0xAA55				; Boot sector code trailer