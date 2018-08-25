

########################################################################
# .TEXT <isUpper>
	.text
isUpper:

# Frame:	$fp, $ra, ...
# Uses:		$a0, ...
# Clobbers:	$v0, ...

# Locals:
#	- ...

# Structure:
#	isUpper
#	-> [prologue]
#	-> [epilogue]

# Code:
	# set up stack frame
	# ... TODO ...
	# tear down stack frame
	jr	$ra
	nop	# in delay slot

#################################################################### EOF

