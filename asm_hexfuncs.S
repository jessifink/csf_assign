/* Assembly language implementation of hexdump functions */

	.section .rodata
	/* TODO: add read-only data such as strings here */


	.section .text

	.globl hex_read
hex_read: 
	read 
    cmpq $0, %rax                 /* see if read failed */
	jl .LreadError                /* handle read failure */


.LreadError:
	/* error handling goes here */

	.globl hex_format_offset
hex_format_offset:
	subq $8, %rsp // stack pointer always has to be in intervals of 16 initialize as 8 so have to subtract
	movq %rdi, %r8   // saves offset in callee register -- source - destination
	movq $28, %r9 //i = 28
	jmp .Lloop
.Lloop:
	//shrq %r9, %r8 //bitwise shift over counter  
	andq $0xF, %r8 // bitwise and 15 
	cmp $9, %r8 //compare with 9 to check if hex num or char
	jg .Lchar1 //if larger jump to first char
	jmp .Lnum1 // else go to first num 



.Lchar: 
	subq $10, %r8 //convert to hex 
	addq $97, %r8 //add a 
	movq %r8, (%rsi)  // add to register
	jmp .Lchecked  
.Lnum:
    addq $48, %r8 //add 0  	 
	movq %r8, (%rsi)  // add to register 
	jmp .Lchecked
.Lchecked
	subq $4, %r9 //decrease counter 
	cmpq $0, %r9 //check if still greater than or equal to zero 
	jge .Lloop 

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
	movq %r8, (%rsi)  // add to register
	jmp .Lsecond  
.Lnum1:
    addq $48, %r8 //add 0  	 
	movq %r8, (%rsi)  // add to register 
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
	cmp $31, %rdi //if statement compare
	jl  .LoutRange //jumps if less than
	cmp $126,%rdi
	jg .LoutRange //jumps if greater than
	jmp .LinRange 
.LoutRange: 
	movq $46, %rax //move period to return register 
	addq $8, %rsp // readjust 
	ret
.LinRange:
	movq %rdi, %rax //moving byteval into return register 
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