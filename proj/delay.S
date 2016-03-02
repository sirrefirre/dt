  # delay.S

.global	delay
	
delay:
	bleu	$a0, $0, finished
	nop
	add	$t0, $0, 0	# i = 1 loops currently
	add	$t1, $0, 12000	# number of loops total
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
	