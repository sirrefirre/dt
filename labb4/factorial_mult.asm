
.text

	addi $a0, $0, 3		# factorial value
	addi $a1, $0, 0		# 
	addi $v0, $v0, 1	# base case 0! 1!
	
loop:	
	beq $a1, $a0, stop 	# if finished, branch to stop
	addi $a1, $a1, 1	#increment current factorial
	mul $v0, $v0, $a1	# old factorial * current number
	beq $0, $0, loop	#loop again

stop:
	beq $0, $0, stop