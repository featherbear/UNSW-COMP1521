
.text
display_populate:
# takes in $a0 (start col)
    la $t0, display
    la $t1, bigString
    li $t2, 0 # row 0-8

    display_populate_loopRow:
        beq $t2, 9, display_populate_loopRowEnd
        nop

        li $t3, 0 # col 0-79

        display_populate_loopCol:
            beq $t3, 80, display_populate_loopColEnd
            nop

            li $t4, 'S'

            add $t5, $t3, $a0
            blt $t5, 0, display_populate_loopCol_write
            nop
            bge $t5, 1000, display_populate_loopCol_write
            nop

            mul $t4, $t2, 1000
            add $t4, $t4, $t3
            add $t4, $t1, $t4
            add $t4, $t4, $a0
            lb $t4, ($t4)

            #move $t6, $t4
            #move $a0, $t4
            #li $v0, 1
            #syscall
            #move $a0, $t6

            display_populate_loopCol_write:
            sb $t4, ($t0)
            addi $t0, $t0, 1
            addi $t3, $t3, 1
            j display_populate_loopCol
            nop


        display_populate_loopColEnd:
        addi $t2, $t2, 1

        j display_populate_loopRow
        nop
    display_populate_loopRowEnd:

        jr $ra
        nop
        
        