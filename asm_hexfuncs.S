/* Assembly language implementation of hexdump functions */

	.section .rodata
	/* TODO: add read-only data such as strings here */
	.section .text

	.globl hex_write_string
hex_write_string:
	subq $8, %rsp  // stack pointer always has to be in intervals of 16 initialize as 8 so have to subtract
	movq %rdi, %r8 //save first parameter into r8
	call strLen 
	movq %rax, %rdx //return in third parameter for write
.Ldone: 
	movq %r8, %rsi //saved first paremter to second paremter in write  
	movq $1, %rdi //write to standard out
	call write
	addq $8, %rsp
	ret
	
	.globl hex_read
hex_read:
	subq $8, %rsp // stack pointer always has to be in intervals of 16 initialize as 8 so have to subtract
	movq %rdi, %rsi //second parameter into first 
	movq $0, %rdi
	movq $16, %rdx
	call read
	addq $8, %rsp
	ret
	


	.globl hex_format_offset
hex_format_offset:
	subq $8, %rsp // stack pointer always has to be in intervals of 16 initialize as 8 so have to subtract
	movl %edi, %r10d   // saves offset in callee register -- source - destination
	movl $28, %r11d //i = 28
	movl $0, %r9d // for loop paramter 
	jmp .Lloop
.Lloop:
	movl %r11d, %ecx /
	shrl %cl, %r10d //bitwise shift over counter  have to use cl to do bitwise shi
	andl $0xF, %r10d // bitwise and 15 
	movl %r10d, %r8d
	cmp $9, %r10d //compare with 9 to check if hex num or char
	movl %edi, %r10d
	jg .Lchar //if larger jump to first char
	jmp .Lnum // else go to first num 
.Lchar: 
	subl $10, %r8d //convert to hex 
	addl $97, %r8d //add a 
	movb %r8b, (%rsi, %r9)   // add to register at spot from counter  ------how to do this 
	jmp .Lfinal 
	
.Lnum:
    addl $48, %r8d //add 0  	 
	movb %r8b, (%rsi, %r9)  // add to register 
	jmp .Lfinal
	
.Lfinal:
	subl $4, %r11d //decrease counter 
	incl %r9d 
	cmpl $0, %r11d //check if still greater than or equal to zero 
	jge .Lloop 
	movb $0, (%rsi, %r9) // add null pointer to next spot 
	movq %rsi, %rax // move to return register 
	addq $8, %rsp
    ret 

	.globl hex_format_byte_as_hex
hex_format_byte_as_hex:
//rdi - first paremeter - byteval
//rsi - second paremeter  - sbuf
	subq $8, %rsp // stack pointer always has to be in intervals of 16 initialize as 8 so have to subtract
	movq %rdi, %r8   // saves byteval in callee register -- source - destination
	shrq $4, %r8 //bitwise shift over 4 
	andq $0xF, %r8 // bitwise and 15 
	cmp $9, %r8 //compare with 9 to check if hex num or char
	jg .Lchar1 //if larger jump to first char
	jmp .Lnum1 // else go to first num 

.Lsecond: 
	andq $0xF, %rdi // bitwise and
	cmp $9, %rdi //compare with 9 to check if hex num or char 
	jg .Lchar2 //if larger jump to first char
	jmp .Lnum2 // else go to first num 
.Lchar1: 
	subq $10, %r8 //convert to hex 
	addq $97, %r8 //add a 
	movq %r8,0(%rsi)  // add to register
	jmp .Lsecond  
.Lnum1:
    addq $48, %r8 //add 0  	 
	movq %r8,0(%rsi)  // add to register 
	jmp .Lsecond
.Lchar2: 
    subq $10, %rdi 
	addq $97, %rdi 
	movq %rdi, 1(%rsi) //add to next over in register 
	movq $0, 2(%rsi) // add null pointer to next spot 
	movq %rsi, %rax // move to return register 
	addq $8, %rsp 
    ret 
.Lnum2:
    addq $48, %rdi 	 
	movq %rdi, 1(%rsi) //add to next over in register
	movq $0, 2(%rsi) // add null pointer to next spot 
	movq %rsi, %rax // move to return register 
	addq $8, %rsp
    ret 

	.global hex_to_printable
hex_to_printable:
    subq $8, %rsp // stack pointer always has to be in intervals of 16 initialize as 8 so have to subtract
	cmp $31, %dil //if statement compare
	jl  .LoutRange //jumps if less than
	cmp $126,%dil
	jg .LoutRange //jumps if greater than
	jmp .LinRange 
.LoutRange: 
	movb $46, %al //move period to return register 
	addq $8, %rsp // readjust 
	ret
.LinRange:
	movb %dil, %al //moving byteval into return register 
	addq $8, %rsp //readjust 
	ret
/*
 * Determine the length of specified character string.
 *
 * Parameters:
 *   s - pointer to a NUL-terminated character string
 *
 * Returns:
 *    number of characters in the string
 */
	.globl strLen
strLen:
	subq $8, %rsp                 /* adjust stack pointer */
	movq $0, %r10                 /* initial count is 0 */

.LstrLenLoop:
	cmpb $0, (%rdi)               /* found NUL terminator? */
	jz .LstrLenDone               /* if so, done */
	inc %r10                      /* increment count */
	inc %rdi                      /* advance to next character */
	jmp .LstrLenLoop              /* continue loop */

.LstrLenDone:
	movq %r10, %rax               /* return count */
	addq $8, %rsp                 /* restore stack pointer */
	ret

/* vim:ft=gas:
 */
