; vim:ft=nasm
; x86
; NASM version 2.15.05

BITS 16                 ; Instruct the system this is 16-bit code

                        ; This is the entry point, nothing should happen before this
                        ; other than setting the instruction size
main:
    mov ax, 07C0h       ; Setup 4KB stack space after this bootloader
	add ax, 288         ; (4096 + 515) / 16 bytes (aligned) per paragraph
	cli                 ; Disable interrupts (solves old DOS bug)
	mov ss, ax          ; Assign current stack segment
	mov sp, 4096        ; Setup our stack pointer
	sti	                ; Enable interrupts (solvs old DOS bug)
	mov ax, 07C0h		; 07C0h is where our program is located
	mov ds, ax			; Set data segment to the load point of our program
	call run			; Start the main loop

;------------------------------------------------------------------------------
; Constants
;------------------------------------------------------------------------------
COM1 equ 0
COM2 equ 1

msg_err_put db "Put error", 0Dh, 0Ah, 00h
msg_err_get db "Get error", 0Dh, 0Ah, 00h

;------------------------------------------------------------------------------
; The main loop of our program
;------------------------------------------------------------------------------
run:
	mov dx, COM1        ; Init com1
	call init_com       ;

	mov dx, COM2        ; Init com2
	call init_com       ;

	mov al, 'A'         ; Set
	call putc_com1      ; Put character to com1
	
	xor ax, ax          ; Clear
	call getc_com2      ; Read character from com2

	call putc           ; Print

.loop:
	jmp .loop

;------------------------------------------------------------------------------
; Init COM
;------------------------------------------------------------------------------
init_com:
	xor ax, ax          ;	111xxxxx - 9600 baud
	mov al, 11100011b   ;	xxx00xxx - no parity
	int 14h				;	xxxxx0xx - 1 bit stop
	ret					;	xxxxxx11 - 8 bit word

;------------------------------------------------------------------------------
; Write COM1
;------------------------------------------------------------------------------
putc_com1:
	mov ah, 1           ; Write mode
	mov dx, COM1		;
	int 14h				;
	test ah, 10000000	; Check writing success
	jnz .err_put		; 
	ret					;
.err_put:				;
	mov si, msg_err_put	; Print error message
	call puts			;
	ret					;

;------------------------------------------------------------------------------
; Read COM2
;------------------------------------------------------------------------------
getc_com2:			
	mov ah, 2			; Read mode
	mov dx, COM2        ;
	int 14h             ;
	test ah, 10000000	; Check reading success
	jnz .err_get		;
	ret					;
.err_get:				;
	mov si, msg_err_get	; Print error message
	call puts			;
	ret					;

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
