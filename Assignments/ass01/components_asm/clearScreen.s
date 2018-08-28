
#
# <clearScreen>
# info: Clear the display of the output window
#       As a utility function, the previous values of a0 and v0 should probably be preserved... oh well!
#
	.text
clearScreen:
    la $a0, CLEAR # load the escape sequences that clear the screen
    li $v0, 4     # syscall 4 -> print string
    syscall
    nop

    # return from function
	jr	$ra
	nop
