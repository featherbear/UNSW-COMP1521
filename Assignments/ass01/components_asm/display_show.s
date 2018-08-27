

	.text
display_show:
    # Method: Perform modulo operation to check for new lines
    li $t0, 0 # counter
    li $t1, 720
    lw $t2, NDCOLS
    # $t3 -> mfhi
    la $t4, display

    # TODO, too many syscalls
    #       syscall with v0 = 4 (string), terminate a null char
    display_show_loop:
        bgt $t0, 720, display_show_loopEnd

        li $v0, 11
        div $t0, $t2

        mfhi $t3
        bgtz $t3, display_show_loop_notEndLine

        li $a0, 10 # \n
        syscall

        display_show_loop_notEndLine:
            li $v0, 11
            lw $a0, ($t4)
            syscall

        addi $t0, $t0, 1
        addi $t4, $t4, 4
        j display_show_loop



    display_show_loopEnd:
        jr	$ra
        nop

