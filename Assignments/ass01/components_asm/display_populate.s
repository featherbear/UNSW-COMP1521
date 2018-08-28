
#
# <display_populate>
# info: Fills `display` with a 9x80 matrix subset of `bigString`.
#       Starting column in `bigString` provided by $a0
# args: a0 - start column offset (can be negative)
#
# vars: t0 - address position in `display`
#       t1 - base address of `bigString`
#       t2 - row counter
#       t3 - column counter
#       t4 - space character
#       t5 - starting offset + column counter offset
#
    .text
display_populate:
    # Setup
    la $t0, display         # t0 = &display
    la $t1, bigString       # t1 = &bigString
    li $t2, 0               # t2 = 0             # row counter

    # Row loop
    display_populate_loopRow:
        # while (t2 < 9) { ... }
        beq $t2, 9, display_populate_loopRowEnd
        nop

        li $t3, 0           # t3 = 0             # column counter

        # Column loop
        display_populate_loopCol:
            # while (t3 < 80) { ... }
            beq $t3, 80, display_populate_loopColEnd
            nop

            li $t4, ' '     # t4 = ' '           # t4 stores the space character

            add $t5, $t3, $a0   # t5 = offset + current position

            # if t5 is less than zero, or greater than or equal to 1000,
            # then the current position in our viewing window is not inside `bigString`,
            # so just display blank in that position (a space character)
            blt $t5, 0, display_populate_loopCol_write
            nop
            bge $t5, 1000, display_populate_loopCol_write
            nop

            # 0 <= t5 < 1000 has passed, meaning that the current position is inside `bigString`
            mul $t4, $t2, 1000  # t4 = t2*1000   # Calculate the `bigString` offset, given a row position
            nop

            add $t4, $t4, $t5   # t4 += t5       # Add on the start offset and column counter offset
            add $t4, $t4, $t1   # t4 += t1       # Add on the base address of `bigString`

            lb $t4, ($t4)       # t4 = (*t4)     # Get *(&bigString + start offset + column offset + row offset)

            display_populate_loopCol_write:
            sb $t4, ($t0)       # *(t0) = t4     # Store the character to the current position in `display`

            # Go the next column in the row
            addi $t0, $t0, 1    # t0++           # Increment the address position in `display`
            addi $t3, $t3, 1    # t3++           # Increment the column counter
            j display_populate_loopCol
            nop

        # End of column loop
        display_populate_loopColEnd:
        addi $t2, $t2, 1        # t2++           # Increment the row counter

        j display_populate_loopRow
        nop

    # End of row loop
    display_populate_loopRowEnd:
        jr $ra
        nop
