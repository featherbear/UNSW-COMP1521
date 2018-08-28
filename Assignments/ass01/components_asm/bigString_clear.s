
#
# <bigString_clear>
# info: Sets all bytes of `bigString` to the space character
#
# vars: t0 - base address of `bigString`
#       t1 - counter
#       t2 - space character
#
	.text
bigString_clear:
    # Setup
    la $t0, bigString    # t0 = &bigString # t0 is the address of `bigString`
    li $t1, 0            # t1 = 0          # t1 is a counter
    li $t2, ' '          # t2 = ' '        # t2 is the space character (dec 32, hex 20)

    # Loop
    bigString_clear_loop:
        # while (t1 < 9000) { ... }
        beq $t1, 9000, bigString_clear_loopEnd
        nop

        # set the value at address t0 to t2 (space)
        sb $t2, ($t0)    # *(t0) = t2

        # increment counters
        addi $t0, $t0, 1 # t0++
        addi $t1, $t1, 1 # t1++

        # repeat loop
        j bigString_clear_loop
        nop

    # End of loop
    bigString_clear_loopEnd:
        jr	$ra
        nop
