

########################################################################
	.text
isUpper:

# inputs
# - $a0 | Character to check
# outputs
# - $v0 | 1 if the character is an uppercase letter, 0 if not

	# Not setting up a stack frame as we are not storing any values

    # Set the result to false (0)
	li $v0, 0       # result = 0

    # If `ch` < 'A' or `ch` > 'Z', then return from the function (as result was already set to false (1)
	blt $a0, 'A', end   # if (ch < 'A') end_function();
	nop

	bgt $a0, 'Z', end   # if (ch > 'Z') end_function();
	nop

    # 'A' <= `ch` <= 'Z', so set the result to true (1)
	li $v0, 1           # result = 1

    end:
        # return to parent subroutine
        jr	$ra
        nop
#################################################################### EOF

