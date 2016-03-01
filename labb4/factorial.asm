
.text

	addi $a0, $0, 8		# factorial value
	addi $a1, $0, 0		# 
	addi $v0, $v0, 1	# base case 0! 1!
	
loop:	
	beq $a1, $a0, stop 	# if finished, branch to stop
	addi $a1, $a1, 1	#increment loop variable/ current factorial
	addi $a2, $0, 1		# base value current factorial
	add $a3, $0, $v0	# put old factorial to variable

mul:
	beq $a1, $a2, loop	# if loops for current, branch to next factorial
	addi $a2, $a2, 1	# loop increase
	add $v0, $v0, $a3	# add last factorial to value
	beq $0, $0, mul		# loop again
	
stop:
	beq $0, $0, stop