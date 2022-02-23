/* Assembly language implementation of hexdump functions */

	.section .rodata
	/* TODO: add read-only data such as strings here */


	.section .text
	.globl hex_format_offset
hex_format_offset:
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


/* vim:ft=gas:
 */