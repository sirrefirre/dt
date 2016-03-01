  # hexmain.asm
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
main:
	li	$a0,11		# change this to test different values

	jal	hexasc		# call hexasc
	nop			# delay slot filler (just in case)	

	move	$a0,$v0		# copy return value to argument register

	li	$v0,11		# syscall with v0 = 11 will print out
	syscall			# one byte from a0 to the Run I/O window
	
stop:	j	stop		# stop after one run
	nop			# delay slot filler (just in case)

  # You can write your own code for hexasc here
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
	
