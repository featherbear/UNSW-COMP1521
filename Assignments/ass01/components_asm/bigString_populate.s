

	.text
bigString_populate:
	sw	$fp, -4($sp)
	la	$fp, -4($sp)
	sw	$ra, -4($fp)  # note: switch to $fp-relative
	sw	$s0, -8($fp)
	sw	$s1, -12($fp)
	sw	$s2, -16($fp)
	addi	$sp, $sp, -20

    # $a0 = theString
    move $s2, $a0

    la $s0, bigString
    la $s1, all_chars # characters
    # $t0 = character -> offset
    
    li $t7, 0 # input string index

    # we can assume that all characters are valid, and that we have under 101 characters
    bigString_populate_loop:
        lb $t0, ($s2)

        # null
        beq $t0, 0, bigString_populate_loopEnd
        nop

                                                # space
                                                bne $t0, ' ', bigString_populate_loop_notSpace
                                                nop

                                                # set char to space in array
                                                li $t0, 52
                                                j bigString_populate_loop_character_loopPrepare
                                                nop

                                                bigString_populate_loop_notSpace:

                                                move $a0, $t0
                                                jal isLower
                                                nop
                                                sub $t0, $t0, 'A'

                                                beq $v0, 0, bigString_populate_loop_character_loopPrepare
                                                nop

                                                # ltr  all_char  ASCII val.
                                                # A-Z (0-25)    [65-90]
                                                # a-z   (26-51)    [97-122]
                                                # spc       52
                                                # ---  -------   ---------
                                                #                   91-96 -> 6 chars extra offset
                                                sub $t0, $t0, 6

        bigString_populate_loop_character_loopPrepare:

        mul $t0, $t0, 81
        add $t0, $s1, $t0 # t0 = all_chars base + t1 offset

        li $t1, 0 # character (0 - 80)

        bigString_populate_loop_character_loop:
            beq $t1, 81, bigString_populate_loop_continue
            nop

            add $t2, $t0, $t1
            lb $t2, ($t2)

            # store at $t6 = $s0 + index*10 + $t1/9*1000 + $t1%9
            move $t6, $s0

            mul $t3, $t7, 10
            add $t6, $t6, $t3

            li $t3, 9
            div $t1, $t3
            mflo $t3 # LO = $t1 / 9
            mfhi $t4 # HI = $t1 % 9

            mul $t3, $t3, 1000
            add $t6, $t6, $t3
            add $t6, $t6, $t4

            # li $t2, 'Z'
            sb $t2, ($t6)

            addi $t1, $t1, 1
            j bigString_populate_loop_character_loop
            nop

        bigString_populate_loop_continue:
            addi $s2, $s2, 1
            addi $t7, $t7, 1

            j bigString_populate_loop
            nop

        bigString_populate_loopEnd:
    move $v0, $t7 # $t7 contains the length
 	lw	$s2, -16($fp)
 	lw	$s1, -12($fp)
 	lw	$s0, -8($fp)
 	lw	$ra, -4($fp)
 	la	$sp, 4($fp)
 	lw	$fp, ($fp)
 	jr	$ra
 	nop