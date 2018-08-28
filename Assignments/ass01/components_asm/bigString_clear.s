

	.text
bigString_clear:
    la $t0, bigString
    li $t1, 0
    li $t2, '.'

    bigString_clear_loop:
        beq $t1, 9000, bigString_clear_loopEnd
        nop

        sb $t2, ($t0)

        addi $t0, $t0, 1
        addi $t1, $t1, 1
        j bigString_clear_loop
        nop

        bigString_clear_loopEnd:

    la $t0, bigString
    li $t7, 'B'
    sb $t7, ($t0)
    li $t7, 'L'
    sb $t7, 1000($t0)
    sb $t7, 8000($t0)


 	jr	$ra
 	nop