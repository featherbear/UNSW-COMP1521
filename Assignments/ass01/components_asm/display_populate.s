
.text
display_populate:
# takes in $a0 (start col)
    # modulus would be inefficient here
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

            add $t5, $t3, $a0
            bge $t5, 1000, display_populate_loopCol_next
            nop

            mul $t4, $t2, 1000
            add $t4, $t4, $t3
            add $t4, $t4, $a0
                # add offsets to base
            add $t4, $t1, $t4
            lb $t4, ($t4)

            mul $t5, $t2, 80
            add $t5, $t5, $t3
            add $t5, $t0, $t5
            sb $t4, ($t5)

            display_populate_loopCol_next:
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