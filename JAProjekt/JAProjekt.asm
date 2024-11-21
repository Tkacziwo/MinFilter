.code
MinimalFilter proc
;creating local variables
LOCAL min_R:BYTE, min_G:BYTE, min_B:BYTE,original_x_pos:WORD, decremented_bitmap_width:WORD ;min RGB value and original x position
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


;	initialising local variables
;---------------------------------------------------;
mov min_R, 255 ;initialise min_R with 255
mov min_G, 255 ;initialise min_G with 255
mov min_B, 255 ;initialise min_B with 255
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
;call FindMinRGB

call FindMinRGBVector

;restore local variables with min RGB values
mov min_R, al ;min_R from bl
mov min_G, ah ;min_G from cl
shr eax, 16
mov min_B, al ;min_B from dl

;mov min_R, bl
;mov min_G, cl
;mov min_B, al

;ChangeCentralPixel:
xor ebx, ebx
xor esi, esi
mov bx, y_pos ;load y_pos
mov si, x_pos ;load x_pos
xor r13,r13
xor r12,r12
mov r13b, 3
mov r12w, bitmap_width ;move bitmap width to r12
imul ebx, r13d ;multiply bx(y_pos) by 3 bytes per pixel
imul esi, r13d ;multiply si(x_pos) by 3 bytes per pixel
imul ebx, r12d ;multiply bx(y_pos) by bitmap width
add ebx, esi ; add si(x_pos), position of Red of pixel is now in bx
mov rdx, rbx ;store it in rdx register

mov al, min_R ;load minimum Red
mov [r9 + rdx], al ;change Red
mov al, min_G ;load minimum Green
mov [r9 + rdx + 1], al ;change Green
mov al, min_B ;load minimum Blue
mov [r9 + rdx + 2], al ;change Blue

;restore minR minG minB values
mov min_R, 255
mov min_G, 255
mov min_B, 255

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
;mov rsp, rbp    ; Restore old stack pointer
pop rbp         ; Restore old base pointer
;---------------------------------------------------;
ret
MinimalFilter endp

FindMinRGB proc
LOCAL min_R:BYTE, min_G:BYTE, min_B:BYTE ;local min RGB values
;	initialise local variables with min RGB values
;---------------------------------------------------;
mov min_R, 255		;min_R
mov min_G, 255		;min_G
mov min_B, 255		;min_B
	;bitmap_width is in r13w
	;y_pos is in r14w
	;x_pos is in r15w
;---------------------------------------------------;

;	get top left pixel coordinates
;---------------------------------------------------;
inc r14w		;increment y_pos
dec r15w		;decrement x_pos
;---------------------------------------------------;

xor rcx, rcx
RowLoop:			;outer loop for rows

;	calculate position in array
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
mov r10, r8				;move source bitmap pointer to r10
push cx
xor cx, cx
ColumnLoop: ;inner loop for columns
xor ax, ax
xor rbx, rbx
mov bl, [r10 + rdx]		;load Red
mov al, min_R
cmp ax, bx
cmovg ax, bx			;ax = r12w if ax > r12w
mov min_R, al			;load into memory min_R
mov bl, [r10 + rdx + 1]
mov al, min_G
cmp ax, bx
cmovg ax, bx			;ax = r12w if ax>r12w
mov min_G, al			;load into memory min_G
mov bl, [r10 + rdx + 2]
mov al, min_B
cmp ax, bx
cmovg ax, bx			;ax = r12w if ax>r12w
mov min_B, al			;load into memory min_B
add rdx, 3
inc cx
cmp cx, 3				;inner loop condition check
jl ColumnLoop
dec r14					;decrease y_pos by 1
pop cx					;pop outer loop iteration number
inc cx					;increase it by 1
cmp cx, 3				;outer loop condition check
jl RowLoop
;move min RGB values to registers
xor r14, r14
mov bl, min_R
mov cl, min_G
mov dl, min_B
ret
FindMinRGB endp


FindMinRGBVector proc
pxor xmm0,xmm0			;fill xmm0 with 0
pcmpeqb xmm0, xmm0		;fill xmm0 with 255

;	register explanation
;---------------------------------------------------;
	;bitmap_width is in r13w
	;y_pos is in r14w
	;x_pos is in r15w
;---------------------------------------------------;

;	get top left pixel coordinates
;---------------------------------------------------;
inc r14w		;increment y_pos
dec r15w		;decrement x_pos
;---------------------------------------------------;

xor rcx, rcx
RowLoop:			;outer loop for rows

;	calculate position in array
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
mov r10, r8				;move source bitmap pointer to r10
xor eax, eax
; top row			;Load 3 pixels immediately to xmm1 register (top row)
mov al, [r10 + rdx + 2] ;blue
shl eax, 8
mov al, [r10 + rdx + 1] ;green
shl eax, 8
mov al, [r10 + rdx]		;red
movd xmm1, eax			;move to xmm1
xor eax, eax
mov al, [r10 + rdx + 5]
shl eax, 8
mov al, [r10 + rdx + 4]
shl eax, 8
mov al, [r10 + rdx + 3]
movd xmm2, eax
xor eax, eax
mov al, [r10 + rdx + 8]
shl eax, 8
mov al, [r10 + rdx + 7]
shl eax, 8
mov al, [r10 + rdx + 6]
movd xmm3, eax

;		mid row			;
xor rdx, rdx		;reset rdx register
dec r14
add rdx, r14
add rdx, r14
add rdx, r14		;add 3 times == 3*r14w == multiplying y coordinate by 3 bytes per pixel
imul rdx, r13		;multiply y coordinate by bitmap_width
add rdx, r15		
add rdx, r15		
add rdx, r15		;add 3 times == multiplying x coordinate by 3 bytes per pixel
xor eax, eax
mov al, [r10 + rdx + 2] ;blue
shl eax, 8
mov al, [r10 + rdx + 1] ;green
shl eax, 8
mov al, [r10 + rdx]		;red
movd xmm4, eax			;move to xmm1
xor eax, eax
mov al, [r10 + rdx + 5]
shl eax, 8
mov al, [r10 + rdx + 4]
shl eax, 8
mov al, [r10 + rdx + 3]
movd xmm5, eax
xor eax, eax
mov al, [r10 + rdx + 8]
shl eax, 8
mov al, [r10 + rdx + 7]
shl eax, 8
mov al, [r10 + rdx + 6]
movd xmm6, eax

;		bottom row		;
xor rdx, rdx		;reset rdx register
dec r14
add rdx, r14
add rdx, r14
add rdx, r14		;add 3 times == 3*r14w == multiplying y coordinate by 3 bytes per pixel
imul rdx, r13		;multiply y coordinate by bitmap_width
add rdx, r15		
add rdx, r15		
add rdx, r15		;add 3 times == multiplying x coordinate by 3 bytes per pixel
xor eax, eax
mov al, [r10 + rdx + 2] ;blue
shl eax, 8
mov al, [r10 + rdx + 1] ;green
shl eax, 8
mov al, [r10 + rdx]		;red
movd xmm7, eax			;move to xmm1
xor eax, eax
mov al, [r10 + rdx + 5]
shl eax, 8
mov al, [r10 + rdx + 4]
shl eax, 8
mov al, [r10 + rdx + 3]
movd xmm8, eax
xor eax, eax
mov al, [r10 + rdx + 8]
shl eax, 8
mov al, [r10 + rdx + 7]
shl eax, 8
mov al, [r10 + rdx + 6]
movd xmm9, eax

pminub xmm0, xmm1
pminub xmm0, xmm2
pminub xmm0, xmm3
pminub xmm0, xmm4
pminub xmm0, xmm5
pminub xmm0, xmm6
pminub xmm0, xmm7
pminub xmm0, xmm8
pminub xmm0, xmm9

;;move min RGB values to registers
xor r14, r14
movd eax, xmm0
ret
FindMinRGBVector endp


MinimalFilterNoVector proc
;creating local variables
LOCAL min_R:BYTE, min_G:BYTE, min_B:BYTE,original_x_pos:WORD, decremented_bitmap_width:WORD ;min RGB value and original x position
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


;	initialising local variables
;---------------------------------------------------;
mov min_R, 255 ;initialise min_R with 255
mov min_G, 255 ;initialise min_G with 255
mov min_B, 255 ;initialise min_B with 255
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
call FindMinRGB

;call FindMinRGBVector

;restore local variables with min RGB values
;mov min_R, al ;min_R from bl
;mov min_G, ah ;min_G from cl
;shr eax, 16
;mov min_B, al ;min_B from dl

mov min_R, bl
mov min_G, cl
mov min_B, al

;ChangeCentralPixel:
xor ebx, ebx
xor esi, esi
mov bx, y_pos ;load y_pos
mov si, x_pos ;load x_pos
xor r13,r13
xor r12,r12
mov r13b, 3
mov r12w, bitmap_width ;move bitmap width to r12
imul ebx, r13d ;multiply bx(y_pos) by 3 bytes per pixel
imul esi, r13d ;multiply si(x_pos) by 3 bytes per pixel
imul ebx, r12d ;multiply bx(y_pos) by bitmap width
add ebx, esi ; add si(x_pos), position of Red of pixel is now in bx
mov rdx, rbx ;store it in rdx register

mov al, min_R ;load minimum Red
mov [r9 + rdx], al ;change Red
mov al, min_G ;load minimum Green
mov [r9 + rdx + 1], al ;change Green
mov al, min_B ;load minimum Blue
mov [r9 + rdx + 2], al ;change Blue

;restore minR minG minB values
mov min_R, 255
mov min_G, 255
mov min_B, 255

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
;mov rsp, rbp    ; Restore old stack pointer
pop rbp         ; Restore old base pointer
;---------------------------------------------------;
ret
MinimalFilterNoVector endp

end