.code
FindMinRGB proc
;	register explanation
;---------------------------------------------------;
	;bitmap_width is in r13w
	;y_pos is in r14w
	;x_pos is in r15w
;---------------------------------------------------;

;			get top left pixel coordinates			;
;---------------------------------------------------;
inc r14w		;increment y_pos
dec r15w		;decrement x_pos
;---------------------------------------------------;

;			calculate position in array				;
;---------------------------------------------------;
xor rdx, rdx		;reset rdx register
add rdx, r14
add rdx, r14
add rdx, r14		;add 3 times == 3*r14w == multiplying y coordinate by 3 bytes per pixel
imul rdx, r13		;multiply y coordinate by bitmap_width
add rdx, r15		
add rdx, r15		
add rdx, r15		;add 3 times == multiplying x coordinate by 3 bytes per pixel
;---------------------------------------------------;

;				Calculate subtraction				;
;---------------------------------------------------;
push cx
xor rcx, rcx
dec r14
add rcx, r14
add rcx, r14
add rcx, r14
imul rcx, r13
add rcx, r15
add rcx, r15
add rcx, r15
mov r10, rdx
sub r10, rcx
;---------------------------------------------------;

;			load and find min of pixel				;
;---------------------------------------------------;
movdqu xmm0, [r8+rdx]		;loads 16 bytes into xmm0 (top row)
sub rdx, r10
movdqu xmm1, [r8+rdx]		;loads 16 bytes into xmm1 (mid row)
sub rdx, r10
movdqu xmm2, [r8+rdx]		;loads 16 bytes into xmm2 (bot row)
pminub xmm0, xmm1			;minimum of top and min row
pminub xmm0, xmm2			;minimum of top and bot row
movdqu xmm1, xmm0			;mov xmm0 to xmm1
movdqu xmm2, xmm0			;mov xmm0 to xmm2
psrldq xmm1, 3				;shift xmm1 to align with pixel 0 in xmm0
psrldq xmm2, 6				;shift xmm2 to align with pixel 0 in xmm0
pminub xmm0, xmm1			;minimum of top pixel and mid pixel
pminub xmm0, xmm2			;minimum of mid pixel and bot pixel
;---------------------------------------------------;

;			move result to esi register				;
;---------------------------------------------------;
xor r14, r14
pop cx
movd esi, xmm0
ret
FindMinRGB endp

MinimalFilter proc
;creating local variables
LOCAL original_x_pos:WORD, operations_count:WORD, remainder:WORD ;original x position
LOCAL y_pos:WORD, x_pos:WORD ;central y and x position
LOCAL bitmap_width:WORD ;bmp width

;			pushing on stack registers				;
;---------------------------------------------------;
push rbp        ;Preserve old base pointer
push rsi        ;Preserve register ESI
push rdi        ;Preserve register EDI
push r10		;Preserve r10
push r11		;Preserve r11
push r12		;Preserve r12
push r13		;Preserve r13
push r14		;Preserve r14
push r15		;Preserve r15
push rbx        ;Preserve register EBX
;---------------------------------------------------;


;			Initialising local variables			;
;---------------------------------------------------;
mov original_x_pos, 1 ;initialise original x_pos
mov y_pos, cx
mov x_pos, 1
mov bitmap_width, dx
sub dx,2
mov ax, dx
mov cx, 3
xor dx, dx
div cx
mov operations_count, ax
mov remainder, dx
;---------------------------------------------------;

xor rcx, rcx
MainLoop:
push cx ;store loop iterator
		;move y_pos, x_pos, bitmap_width to FindMinRGB
xor r13, r13				;zero r13
xor r14, r14				;zero r14
xor r15, r15				;zero r15
mov r13w, bitmap_width		;r13w is bitmap_width
mov r14w, y_pos				;r14w is y_pos
mov r15w, x_pos				;r15w is x_pos

call FindMinRGB
;			calculate position in array				;
;---------------------------------------------------;
mov r14w, y_pos				;r14w is y_pos
mov r15w, x_pos				;r15w is x_pos		
mov r13w, bitmap_width		;r13w is bitmap_width
xor rdx, rdx				;reset rdx register
add rdx, r14
add rdx, r14
add rdx, r14				;add 3 times == 3*r14w == multiplying y coordinate by 3 bytes per pixel
imul rdx, r13				;multiply y coordinate by bitmap_width
add rdx, r15		
add rdx, r15		
add rdx, r15				;add 3 times == multiplying x coordinate by 3 bytes per pixel
;---------------------------------------------------;

mov [r9 + rdx], sil			;change Red
shr esi, 8
mov [r9 + rdx + 1], sil		;change Green
shr esi, 8
mov [r9 + rdx + 2], sil		;change Blue
psrldq xmm0, 3
xor esi, esi
movd esi, xmm0
mov [r9 + rdx + 3], sil		;change Red
shr esi, 8
mov [r9 + rdx + 4], sil		;change Green
shr esi, 8
mov [r9 + rdx + 5], sil		;change Blue
psrldq xmm0, 3
xor esi, esi
movd esi, xmm0
mov [r9 + rdx + 6], sil		;change Red
shr esi, 8
mov [r9 + rdx + 7], sil		;change Green
shr esi, 8
mov [r9 + rdx + 8], sil		;change Blue

mov ax, x_pos
add ax, 3
mov x_pos, ax
pop cx
inc cx
cmp cx, operations_count
jl MainLoop

mov cx, remainder		;mov remainder to cx
mov ax, x_pos
dec ax
mov x_pos, ax
RemainderPositiveLoop:
mov ax, x_pos
inc ax
mov x_pos, ax
xor r13, r13				;zero r13
xor r14, r14				;zero r14
xor r15, r15				;zero r15
mov r13w, bitmap_width		;r13w is bitmap_width
mov r14w, y_pos				;r14w is y_pos
mov r15w, x_pos				;r15w is x_pos
call FindMinRGB
mov r14w, y_pos				;r14w is y_pos
mov r15w, x_pos				;r15w is x_pos		
mov r13w, bitmap_width		;r13w is bitmap_width
xor rdx, rdx				;reset rdx register
add rdx, r14
add rdx, r14
add rdx, r14				;add 3 times == 3*r14w == multiplying y coordinate by 3 bytes per pixel
imul rdx, r13				;multiply y coordinate by bitmap_width
add rdx, r15		
add rdx, r15		
add rdx, r15				;add 3 times == multiplying x coordinate by 3 bytes per pixel;

mov [r9 + rdx], sil			;change Red
shr esi, 8
mov [r9 + rdx + 1], sil		;change Green
shr esi, 8
mov [r9 + rdx + 2], sil		;change Blue
dec cx
cmp cx, 0
jg RemainderPositiveLoop

;restoring registers
;---------------------------------------------------;
pop rbx         ; Restore EBX
pop r15			;restore r15
pop r14			;restore r14
pop r13			;restore r13
pop r12			;restore r12
pop r11			;restore r11
pop r10			;restore r10
pop rdi         ; Restore EDI
pop rsi         ; Restore ESI
pop rbp         ; Restore old base pointer
;---------------------------------------------------;
ret
MinimalFilter endp
end