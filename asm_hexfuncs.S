/* Assembly language implementation of hexdump functions */

	.section .rodata
	sHexidecimal: .string "0123456789abcdef"
	/* TODO: add read-only data such as strings here */


	.section .text
	.globl hex_format_offset
hex_format_offset:
ret
	.globl hex_format_byte_as_hex
hex_format_byte_as_hex:
//rdi - first paremeter - byteval
//rsi - second paremeter  - sbuf
	subq $8, %rsp
	movq %rdi, %r8   // saves byteval in callee register -- source - destination
	shrq $4, %r8 //bitwise shift over 4 
	andq $0xF, %r8 // bitwise and 15 
	leaq sHexidecimal, %r9 //save the string to r9
	addq %r8, %r9 // move over r8 places in string and save in r9
	movq %r9, %rsi // trying to add to first place in register 
	andq $0xF, %rdi // and bitwise 
	leaq sHexidecimal, %r10
	addq %rdi, %r10 //shift over that amount of places and save in rdi 
	incq %rsi // next spot in rsi 
	movq (%r10), %rsi // trying to add to second place in register 
	incq %rsi // next spot in rsi 
	movq $0, %rsi // null pointer 
	movq %rsi, %rax
	addq $8, %rsp
    ret 

	.global hex_to_printable
hex_to_printable:
    subq $8, %rsp // stack pointer always has to be in intervals of 16 initialize as 8 so have to subtract // 
	cmp $31, %rdi //if statement 
	jl  .LoutRange //jumps if less than
	cmp $126,%rdi
	jg .LoutRange //jumps if greater than
	jmp .LinRange
.LoutRange: 
	movq $46, %rax //move period to return register 
	addq $8, %rsp
	ret
.LinRange:
	movq %rdi, %rax //moving byteval into return register 
	addq $8, %rsp
	ret




	







	/* TODO: add functions here */

/* vim:ft=gas:
 */