  # timetemplate.asm
  # Written 2015 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

.macro	PUSH (%reg)
	addi	$sp,$sp,-4
	sw	%reg,0($sp)
.end_macro

.macro	POP (%reg)
	lw	%reg,0($sp)
	addi	$sp,$sp,4
.end_macro

	.data
	.align 2
mytime:	.word 0x5957
timstr:	.ascii "text more text lots of text\0"
	.text
main:
	# print timstr
	la	$a0,timstr
	li	$v0,4
	syscall
	nop
	# wait a little
	li	$a0,1000
	jal	delay
	nop
	# call tick
	la	$a0,mytime
	jal	tick
	nop
	# call your function time2string
	la	$a0,timstr
	la	$t0,mytime
	lw	$a1,0($t0)
	jal	time2string
	nop
	# print a newline
	li	$a0,10
	li	$v0,11
	syscall
	nop
	# go back and do it all again
	j	main
	nop
# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa000	# adjust last digit
tiend:	sw	$t0,0($a0)	# save updated result
	jr	$ra		# return
	nop

  # you can write your code for subroutine "hexasc" below this line
  #
hexasc:
	andi	$t0, $a0, 0xF 		# mask 4 relevant bits
	li	$t1, 0x9
	bgt	$t0, $t1, overnine	# if greater than 9, use letters
	nop
	add 	$v0, $t0, 0x30		#start at 0x30 = 0 + input number
	jr	$ra
	nop
overnine:
	add	$v0, $t0, 0x37		# start at 0x37 + 0xa = A + input number
	jr	$ra
	nop
	
delay:
	bleu	$a0, $0, finished
	nop
	add	$t0, $0, 0	# i = 0 loops currently
	add	$t1, $0, 38	# number of loops total
floop:				# finished loop = 1 second
	slt	$t2, $t0, $t1	# if t0 less than t1 set t2=1
	beq	$t2, $0, done	# t0 >=t1 done
	nop
	add	$t0, $t0, 1	# i++
	j 	floop
	nop
done:
	add	$a0, $a0, -1	
	bgt	$a0, $0, delay	# run for a0 seconds
	nop
finished:
	jr $ra
	nop
	
time2string:
	PUSH 	($ra)		# return to original call
	PUSH	($a0)		# save write address
	andi	$t1, $a1, 0xff	# mask for XX:00
	beq	$t1, $0, ding	# if 00 ding
	nop
	srl	$a0, $a1, 0xc	# shift 12 right to get high minute number
	PUSH	($a1)
	jal 	hexasc
	nop
	POP	($a1)
	POP	($a0)		# a0 case
	sb	$v0, 0($a0)	# store hex high minute
	PUSH	($a0)		# a0 case
	srl	$a0, $a1, 0x8	# shift 8 right to get low minute number
	PUSH	($a1)
	jal	hexasc
	nop	
	POP	($a1)
	POP	($a0)		# a0 case
	sb	$v0,1($a0)	# store hex low minute
	add	$t0, $0, 0x3a
	sb	$t0, 2($a0)	# store :
	PUSH	($a0)		#a0 case
	srl	$a0, $a1, 0x4	# shift 4 right to get high second number
	PUSH	($a1)
	jal	hexasc
	nop
	POP	($a1)
	POP	($a0)		# a0 case
	
	sb	$v0, 3($a0)	# store hex high second
	PUSH	($a0)		# a0 case
	move	$a0, $a1
	PUSH	($a1)
	jal	hexasc
	nop
	POP	($a1)
	POP	($a0)
	POP	($ra)
	sb	$v0, 4($a0)	# store hex low second
	sb	$0, 5($a0)	# store 0 = NULL
	jr	$ra
	nop

ding:
li 	$t0, 0x44	# D
li	$t1, 0x49	# I
li	$t2, 0x4e	# N
li	$t3, 0x47	# G
sb	$t0, 0($a0)
sb	$t1, 1($a0)
sb	$t2, 2($a0)
sb	$t3, 3($a0)
sb	$0, 4($a0)
jr 	$ra
nop