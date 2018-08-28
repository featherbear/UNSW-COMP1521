

.text
display_clear:
    li $t0, 0
    la $t1, display
    li $t2, '.'
    display_clear_loop:
        beq $t0, 720, display_clear_loopEnd
        sb $t2, ($t1)
        addi $t0, $t0, 1
        addi $t1, $t1, 1
        j display_clear_loop
    display_clear_loopEnd:
         jr $ra
         nop

