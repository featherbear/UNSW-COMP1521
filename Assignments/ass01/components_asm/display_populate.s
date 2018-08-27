

.text
display_clear:
    li $t0, 0
    la $t1, display

    # li $... 720
    # mul NROWS*NDCOLS

    display_clear_loop:
        beq $t0, 720, display_clear_loopEnd
        # TODO hardcode?
        sw $t7, ($t1)
        # sw $0, ($t1)

        addi $t0, $t0, 1
        addi $t1, $t1, 4
        j display_clear_loop

    display_clear_loopEnd:
         jr $ra
         nop


