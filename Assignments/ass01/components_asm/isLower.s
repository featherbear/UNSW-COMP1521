

########################################################################
# .TEXT <isLower>
	.text
isLower:

# Frame:	$fp, $ra
# Uses:		$a0
# Clobbers:	$v0

# Locals:
#	- `ch' in $a0
#	- ... $v0 used as temporary register

# Structure:
#	isLower
#	-> [prologue]
#	[ch cond]
#	   | isLower_ch_ge_a
#	   | isLower_ch_le_z
#	   | isLower_ch_lt_a
#	   | isLower_ch_gt_z
#	-> isLower_ch_phi
#	-> [epilogue]

# Code:
	# set up stack frame
	sw	$fp, -4($sp)
	la	$fp, -4($sp)
	sw	$ra, -4($fp)
	la	$sp, -8($fp)

	# if (ch >= 'a')
	li	$v0, 'a'
	blt	$a0, $v0, isLower_ch_lt_a
	nop	# in delay slot
isLower_ch_ge_a:
	# if (ch <= 'z')
	li	$v0, 'z'
	bgt	$a0, $v0, isLower_ch_gt_z
	nop	# in delay slot
isLower_ch_le_z:
	addi	$v0, $zero, 1
	j	isLower_ch_phi
	nop	# in delay slot

	# ... else
isLower_ch_lt_a:
isLower_ch_gt_z:
	move	$v0, $zero
	# fallthrough
isLower_ch_phi:

isLower__post:
	# tear down stack frame
	lw	$ra, -4($fp)
	la	$sp, 4($fp)
	lw	$fp, ($fp)
	jr	$ra
	nop	# in delay slot

