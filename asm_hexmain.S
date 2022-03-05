/* Hexdump main function, assembly language version */

/*                                                                                                                                                                                                        
 * Notes:                                                                                                                                                                                                 
 * Callee-saved registers: rbx, rbp, r12-r15                                                                                                                                                              
 * Subroutine arguments:  rdi, rsi, rdx, rcx, r8, r9                                                                                                                                                      
	*/
	.section .rodata
sColon: .string ": "
sSpace: .string " "
s2Space: .string "  "
s3Space: .string "   "
sNull: .string  "\0"
sNewLine: .string "\n"

    .section .bss
    .align 8
input: .space 17 
offset: .space  9
outputted: .space 17
charBuf: .space 2


	.section .text

    .globl main
main:
    /* TODO: add your code here */

    pushq %r12 /*to store count*/
    pushq %r13 /*to store length*/
   // pushq %r14 /*to store tracker done*/
    pushq %r15 
    pushq %rbx
    subq $8, %rsp /*align stack*/

    movq $0, %r12 /*initialize count to zero*/
    movq $0, %r13 /*initialize length to zero*/
    movq $0, %r14 /*initialize tracker to zero*/
	jmp .LEnter
.LEnter:
//    movq $0, %r15 /*set first index of array to null char*/
	//leaq (input,%r12,1), %r15
	movq $input, %r15
	movq %r15, %rdi
	//movq $input, %r15
	//mocq %r15, %rdi /*store base adress of array*/
	// movq %r15, %rdi /*move array into register as parameter*/
	call hex_read /*call hex_read */
	//movq %rdi, %r15 /*store input array into %r15 register*/
    movq %rax, %r13 /*store length returned by hex_read in register*/
    jmp .LMainLoop
.LMainLoop:
    cmp   $0, %r13
    jle .LEnd //see if reads in 0 --reads in the first 16 chars if nothing then skips to end other wise goes through and checks this again at end of full cycle
    movb $0, (%r15,%r13) //input length is null pointer 
    movq %r12, %rdi /*move count into parameter 1*/
    movq $outputted, %r8 //move the space into register
    movq %r8, %rsi //makes outputted the buffer that goes into hex_format_offset
    call hex_format_offset /*call hex_format_offset(count, output)*/
    movq %rax, %rdi //result of offest into write
    call hex_write_string /*call hex_write_string(output)*/
    movq $sColon, %rdi //print colon
    call hex_write_string /*hex_write_string(": ")*/
    movq $0, %r10 /*count var for loop */
    jmp .LWriteNums 

.LWriteNums:
	//	HELP R10 IS CHANGING WHEN CALLING HEXWRITESTRING AND HEX FORMAT SO NOT ABLE TO LOOP PROPERLY 
	//pushq %r10 
	cmp %r10, %r13 /*if length <= i MIGHT HAVE TO HAVE POPPER R13*/
	jle  .LNext /*leave loop*/
	pushq %r10
	movq $sSpace, %rdi
    call hex_write_string /*hex_write(" ")*/
	popq %r10
	movb (%r15,%r10),%dil /*input[i] as first param*/
	movq $outputted, %r9
	movq %r9,%rsi /*output as second param*/
	push %r10
	call hex_format_byte_as_hex
	pop %r10
	movq %rax, %r9  
	movq %r9, %rdi
	push %r10
	call hex_write_string
	pop %r10
	incq %r10 //increase 
    //movq %r13, %r14 
    jmp .LWriteNums

.LNext:
//ADD THE CORRECT AMOUNT OF SPACES	
	movq %r13, %r10	/*store length in counter in r10 register*/
	jmp .LPrintSpaces

.LPrintSpaces:
        cmp $16, %r10 /*while length < 16*/
        jge .LPrintInput /*CHANGE THIS*/
	movq $s2Space, %rdi
	pushq %r10 /*push onto stack before function call*/
	call hex_write_string
	popq %r10 /*pop off stack*/
	incq %r10
	jmp .LPrintSpaces

.LPrintInput:
	movq $0, %r10 /*store index for loop in %r10*/
	jmp .LPrintInputLoop
	
.LPrintInputLoop:
	cmp $0, %r13 /*if length >= i, end*/
	jge .LLast //exit
	movq $charBuf, %r8 /*to store char buffer*/
	movq (%r15, %r10), %r9 /*input[i] into %r8*/
	movq %r9, %rdi
	call hex_to_printable
	movq %rax, 0(%r8)
	movq $0, 1(%r8) /* '/0' into cbuf[1] */
	movq %r8, %rdi
	pushq %r10
	call hex_write_string
	popq %r10
	jmp .LPrintInputLoop

	
.LLast:
	//jmp .LEnd /*FOR TESTING GET RID OF THIS LATER*/
	movq $s2Space, %rdi
    call hex_write_string
    addq $16, %r12 /*increment count*/
    movq $sNewLine, %rdi
	call hex_write_string
	jmp .LEnter
	//position
//	jmp .LEnter
   // jmp .LPrintInput
/*.LPrintInput:	
    cmp %r9, %r14 //i< length stay in loop 14 - i 9 - length 
    jge .LEnter
    movq $charBuf, %rbx
    movb (%r15, %r14), %r11b // move string at counter into r13 
	movb %r11b, %dil
	//movq $0, %rax //idk
	call hex_to_printable //check that that charecter is printable
	movb %al,0(%rbx) // move that charecter into r11 
    movb $0, 1(%rbx) 
    movq %rbx, %rdi 
    call hex_write_string
    jmp .LPrintInput
  */  
.LEnd:
    movq $0, %rax
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    addq $8, %rsp /*readjust stack*/
    ret

/* vim:ft=gas:                                                                                                                                                                                            
 */
