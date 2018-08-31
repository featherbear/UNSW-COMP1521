
#
# <display_show>
# info: Prints out the contents of `display` (9x80 character matrix)
#       Function uses syscall 4 (print string) rather than syscall 11 (print char) as it is more efficient
#
# vars: a0 - temp
#       t0 - address position in `display`
#       t1 - temp
#       t7 - loop counter
#
    .text
display_show:
    la $a0, display       # a0 = &display     # address position in `display`
    li $t7, 0             # t7 = 0            # loop counter

    # Loop
    display_show_loop:
        # while (t7 < 9 ) { ... }
        beq  $t7, 9, display_show_loopEnd
        nop

        # Go to the start of the next row
        addi $t0, $a0, 80

        # Store the byte at the first column, and change it to a null terminator (\0)
        lb   $t1, ($t0)       # t1 = *(t0)
        sb   $0, ($t0)        # *(t0) = '\0'      # Strings end with \0. eg. { 'H', 'i', '\0' }

        # Print the row
        li   $v0, 4           # syscall 4 -> print string
        syscall
        nop

        # Restore the value of the byte that we changed into a null terminator
        sb   $t1, ($t0)       # *(t0) = t1

        # Print out a new-line character
        ## Note: Couldn't have replaced the first columns with a new line instead of a null terminator
        ##       because then the first column would have been lost (Need a 9x81 matrix instead of a 9x80)
        li   $a0, 10          # 0x10 is the new line character
        li   $v0, 11          # syscall 4 -> print character
        syscall
        nop

        move $a0, $t0         # Restore a0 as the address position in the display matrix

        # Go to the next row
        addi $t7, $t7, 1      # t7++      # Increment the row counter
        j    display_show_loop
        nop

    # End loop
    display_show_loopEnd:
        jr  $ra
        nop

### OLD VERSION - Uses syscall 11 (much slower!) ##
#  .text
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
#        jr  $ra
#        nop
