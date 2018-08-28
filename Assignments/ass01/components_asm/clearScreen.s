

########################################################################
	.text
clearScreen:

    # store the current value of $v0, as we're forced to modify $v0 for our syscall
	sw	$v0, -4($sp)

    la $a0, CLEAR
    li $v0, 4
    syscall

    # restore the old value of $v0
	lw	$v0, -4($sp)

	# return function
	jr	$ra
	nop
	
	