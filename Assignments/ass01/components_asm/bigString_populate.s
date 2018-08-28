

	.text
bigString_populate:
	sw	$fp, -4($sp)
	la	$fp, -4($sp)
	sw	$ra, -4($fp)
	la	$sp, -8($fp)

    la $t0, bigString
    # move $t0, $a0 # theString
    la $t1, all_chars # characters
    # $t2 = character -> offset

    li $t7, 0 # input string index

    # we can assume that all characters are valid, and that we have under 101 characters
    bigString_populate_loop:
        lb $t2, ($a0)

        # null
        beq $t2, 0, bigString_populate_loopEnd
        nop

        # TODO spaces
            # beq $t2, ' ', bigString_populate_loop_space
            # nop

        move $t3, $a0
        move $a0, $t2

        jal isLower
        nop

        move $a0, $t3

        # bigString_populate_loop_charWasLower
        sub $t2, $t2, 'A'

        beq $v0, 0, bigString_populate_loop_charWasUpper
        nop

        # ltr  all_char  ASCII val.
        # A-Z (0-25)    [65-90]
        # a-z   (26-51)    [97-122]
        # ---  -------   ---------
        #                   91-96 -> 6 chars extra offset
        sub $t2, $t2, 6

        bigString_populate_loop_charWasUpper:

        mul $t2, $t2, 81
        add $t2, $t1, $t2 # t2 = all_chars base + t3 offset

        li $t3, 0 # character (0 - 80)

        bigString_populate_loop_character_loop:
            beq $t3, 81, bigString_populate_loop_continue
            nop

            add $t4, $t2, $t3

            lb $t4, ($t4)

            # store at $t0 + index*10 + $t3/9*1000 + $t3%9
            move $t6, $t0

            mul $t5, $t7, 10
            add $t6, $t6, $t5

            li $t5, 9
            div $t3, $t5 # LO = $t3 / 9, HI = $t3 % 9
            mflo $t5

            mul $t5, $t5, 1000
            add $t6, $t6, $t5

            mfhi $t5
            add $t6, $t6, $t5

            sb $t4, ($t6)

            addi $t3, $t3, 1
            j bigString_populate_loop_character_loop
            nop

        bigString_populate_loop_continue:
            addi $t7, $t7, 1
            addi $a0, $a0, 1

            j bigString_populate_loop
            nop

        bigString_populate_loopEnd:
        lw	$ra, -4($fp)
        la	$sp, 4($fp)
        lw	$fp, ($fp)
        jr	$ra
        nop