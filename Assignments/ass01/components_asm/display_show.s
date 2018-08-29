
#
# <display_show>
# info: Prints out the contents of `display` (9x80 character matrix)
#       Function uses syscall 4 (print string) rather than syscall 11 (print char) as it is more efficient
#
# vars: a0 - address position in `display`
#       t0 - temp
#       t7 - loop counter
#
	.text
display_show:
    la $a0, display     # a0 = &display     # address position in `display`
    li $t7, 0           # t7 = 0            # loop counter

    # Loop
    display_show_loop:
        beq $t7, 9, display_show_loopEnd
        nop

        addi $t0, $a0, 80

        lb $t1, ($t0)
        sb $0, ($t0)

        li $v0, 4
        syscall
        nop
        
        sb $t1, ($t0)

        li $a0, 10
        li $v0, 11
        syscall
        nops

        move $a0, $t0

        addi $t7, $t7, 1
        j display_show_loop
        nop

    # End loop
    display_show_loopEnd:
        jr	$ra
        nop

### OLD VERSION - Uses syscall 11 (much slower!) ##
#	.text
#display_show:
#    # Method: Perform modulo operation to check for new lines
#    li $t0, 0 # counter
#    li $t2, 80
#    la $t4, display
#
#    display_show_loop:
#        beq $t0, 720, display_show_loopEnd
#
#        li $v0, 11
#        div $t0, $t2
#
#        mfhi $t3
#        bgtz $t3, display_show_loop_notEndLine
#
#        li $a0, 10 # \n
#        syscall
#
#        display_show_loop_notEndLine:
#            li $v0, 11
#            lb $a0, ($t4)
#            syscall
#
#        addi $t0, $t0, 1
#        addi $t4, $t4, 1
#        j display_show_loop
#
#    display_show_loopEnd:
#        jr	$ra
#        nop
