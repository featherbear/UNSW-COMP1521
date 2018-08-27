

########################################################################
# .TEXT <setUpDisplay>
	.text
setUpDisplay:

# Frame:	$fp, $ra, ...
# Uses:		$a0, $a1, ...
# Clobbers:	...

# Locals:
#	- `row' in $...
#	- `out_col' in $...
#	- `in_col' in $...
#	- `first_col' in $...
#	- ...

# Structure:
#	setUpDisplay
#	-> [prologue]
#	-> ...
#	-> [epilogue]

# Code:
	# set up stack frame
	# sw	$fp, -4($sp)
	# la	$fp, -4($sp)
	# sw	$ra, -4($fp)
	# la	$sp, -8($fp)

	# ... TODO ...

    la $t7, display
    li $t0, 0

    # clear bigString

    # clear display



	# tear down stack frame
	# lw	$ra, -4($fp)
	# la	$sp, 4($fp)
	# lw	$fp, ($fp)
	jr	$ra
	nop	# in delay slot