section .text

global _start

_start:
	call fwait
	mov rax, 60
	mov rdi, 0
	syscall
	ret

fwait:
	mov rax, 16    ; __NR_ioctl
	mov rdi, 0     ; fd: stdin
	mov rsi, 21505 ; cmd: TCGETS
	mov rdx, orig  ; arg: the buffer, orig
	syscall
	; again, but this time for the 'new' buffer
	mov rax, 16
	mov rdi, 0
	mov rsi, 21505
	mov rdx, new
	syscall

	; change settings
	and dword [new+0], -1516    ; ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON)
	and dword [new+4], -2       ; ~OPOST
	and dword [new+12], -32844  ; ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN)
	and dword [new+8], -305     ; ~(CSIZE | PARENB)
	or  dword [new+8], 48        ; CS8

	; set settings (with ioctl again)
	mov rax, 16    ; __NR_ioctl
	mov rdi, 0     ; fd: stdin
	mov rsi, 21506 ; cmd: TCSETS
	mov rdx, new   ; arg: the buffer, new
	syscall

	; read a character
	mov rax, 0     ; __NR_read
	mov rdi, 0     ; fd: stdin
	mov rsi, char  ; buf: the temporary buffer, char
	mov rdx, 1     ; count: the length of the buffer, 1
	syscall

	; reset settings (with ioctl again)
	mov rax, 16    ; __NR_ioctl
	mov rdi, 0     ; fd: stdin
	mov rsi, 21506 ; cmd: TCSETS
	mov rdx, orig  ; arg: the buffer, orig
	syscall

	ret

section .bss

orig:
	resb 36
new:
	resb 36
char:
	resb 1
