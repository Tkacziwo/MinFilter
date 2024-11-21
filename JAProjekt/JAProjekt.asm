.code
MinimalFilter proc
;creating local variables
LOCAL original_x_pos:WORD, decremented_bitmap_width:WORD ;original x position
LOCAL y_pos:WORD, x_pos:WORD ;central y and x position
LOCAL bitmap_width:WORD ;bmp width

;	pushing on stack registers
;---------------------------------------------------;
push rbp        ; Preserve old base pointer
push rsi        ; Preserve register ESI
push rdi        ; Preserve register EDI
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
mov decremented_bitmap_width, dx
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

call FindMinRGBVector

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

mov ax, x_pos
inc ax
mov x_pos, ax
pop cx
inc cx
cmp cx, decremented_bitmap_width
jl MainLoop
mov ax, original_x_pos
mov x_pos, ax

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

FindMinRGBVector proc
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

;					top row							;
;---------------------------------------------------;
movdqu xmm0, [r8 + rdx]
movdqu xmm1, [r8 + rdx + 3]
movdqu xmm2, [r8 + rdx + 6]
;---------------------------------------------------;

;					mid row							;
;---------------------------------------------------;
sub rdx, r10
movdqu xmm3, [r8 + rdx]
movdqu xmm4, [r8 + rdx + 3]
movdqu xmm5, [r8 + rdx + 6]
;---------------------------------------------------;

;					bottom row						;
;---------------------------------------------------;
sub rdx, r10
movdqu xmm6, [r8 + rdx]
movdqu xmm7, [r8 + rdx + 3]
movdqu xmm8, [r8 + rdx + 6]
;---------------------------------------------------;

;			take minimum of all pixels				;
;---------------------------------------------------;
pminub xmm0, xmm1
pminub xmm0, xmm2
pminub xmm0, xmm3
pminub xmm0, xmm4
pminub xmm0, xmm5
pminub xmm0, xmm6
pminub xmm0, xmm7
pminub xmm0, xmm8
;---------------------------------------------------;

;;move min RGB values to registers
xor r14, r14
movd esi, xmm0
ret
FindMinRGBVector endp
end