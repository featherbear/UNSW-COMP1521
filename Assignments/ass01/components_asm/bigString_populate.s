
#
# <bigString_populate>
# info: Fills `bigString` with letter matrices of the input string
#       Function assumes that the input is valid
# args: a0 - Starting address of string to populate into `bigString`
#  ret: v0 - Input string length
#
# vars: s0 - base address of `bigString`
#       s1 - base address of `all_chars`
#       s2 - address position in the input string
#       t0 - character at s2, then (&all_chars + letter offset)
#       t1 - counter
#       t2 - character at (&all_chars + letter offset + t1)
#       t3 - address to store in `bigString`
#       t4 - letter index of the input string
#       t5 - temp
#       t6 - temp
#       t7 - 81 (CHRSIZE * NROWS)
#       t8 - 10 (NROWS + 1)
#       t9 - 1000 ( (NROWS+1) * MAXCHARS )
#
    .text
bigString_populate:
    # Set up stack frame
    sw   $fp, -4($sp)
    la   $fp, -4($sp)
    sw   $ra, -4($fp)
    sw   $s0, -8($fp)
    sw   $s1, -12($fp)
    sw   $s2, -16($fp)
    addi $sp, $sp, -20

    # Setup
    la   $s0, bigString # s0 = &bigString
    la   $s1, all_chars # s1 = &all_chars
    move $s2, $a0       # s2 = a0

    li   $t4, 0         # t4 = 0          # index of input string

    # Loop
    bigString_populate_loop:
        lb   $t0, ($s2) # t0 = *(s2)      # t0 holds the character at address in s2

        # if (t0 == NULL) --> break
        beq  $t0, 0, bigString_populate_loopEnd
        nop

        # if (t0 == ' ') --> continue
        beq  $t0, ' ', bigString_populate_loop_continue
        nop

        # Check if t0 is a lowercase letter
        move $a0, $t0
        jal  isLower
        nop

        # if (!isLower()) { ... }
        beq  $v0, 0, bigString_populate_loop_character_loopPrepare
        nop

        # Subtract 6 from t0 for lowercase letters to handle the ASCII offset difference
        ##### ltr | all_chars | ASCII val
        ##### ----|-----------|------------
        ##### A-Z |   0 - 25  |  65 -  90
        ##### a-z |  26 - 51  |  97 - 122
        ##### spc |       52  |
         #### ----|-----------|------------
          ###                    There are 6 characters between 'Z' and 'a'
           ##                                                    90      97
            #                       91-96 -> 6 chars extra offset
        sub  $t0, $t0, 6 # t0 -= 6
        # We could have subtracted 'a' instead of 'A', but this method saves one line :)

        bigString_populate_loop_character_loopPrepare:

        # Subtract the value of 'A' (65) from the letter to get the 0-based alphabet offset
        # 'A'   - 'A' == 0
        # 'B'   - 'A' == 1
        # 'Z'   - 'A' == 25
        # 'a'-6 - 'A' == 26
        # 'z'-6 - 'A' == 51
        sub  $t0, $t0, 'A'   # t0 -= 'A'

        lw   $t7, CHRSIZE    # t7  = CHRSIZE         = 9
        lw   $t8, NROWS      # t8  = NROWS           = 9
        mul  $t7, $t7, $t8   # t7  = CHRSIZE * NROWS = 81
        mul  $t0, $t0, $t7   # t0 *= 81      # Multiply the offset by 81 to get the 9x9 letter matrix offset
        add  $t0, $s1, $t0   # t0 += s1      # Set t0 to the address of all_chars offset by the previous result

        addi $t8, $t8, 1     # t8 = NROWS + 1 = 10 (9 columns + 1 space)
        lw   $t9, MAXCHARS   # t9 = MAXCHARS = 100
        mul  $t9, $t8, $t9   # t9 = (NROWS + 1) * MAXCHARS  = 1000

        li   $t1, 0  # loop counter

        # Matrix loop
        bigString_populate_loop_character_loop:
            # while (t1 < 81) { ... }
            beq  $t1, $t7, bigString_populate_loop_continue
            nop

            # Get the character in the 9x9 letter matrix
            add  $t2, $t0, $t1 # t2 = t0 #   # t2 is (&all_chars + letter offset) + position offset
            lb   $t2, ($t2)     # t2 = *(t2)  # Load the character at the memory of t2

            # Calculate the address in `bigString` to store our character
            ##### Address = base address + index*10 + offset/9*1000 + offset%9
            #####           |              |          |               | column position in the 9x9 matrix
             ####           |              |          | row position in the matrix * columns per row in `bigString`
              ###           |              | current letter count * (number of columns in a 9x9 matrix + 1 space column)
               ##           | base address of `bigString`
                # Address = $s0 + $t4*10 + $t1/9*1000 + $t1%9

            move $t3, $s0                 # address = base

            mul  $t5, $t4, $t8            # temp1 = t4 * 10    # offset to get correct position (with space separators)
            add  $t3, $t3, $t5            # address = base + temp1

            lw   $t5, CHRSIZE             # set our divisor to 9 (CHRSIZE)
            div  $t1, $t5                 # divide by 9        # LO = $t1 / 9, HI = $t1 % 9
            mflo $t5                      # temp2 = LO
            mfhi $t6                      # temp3 = HI

            mul  $t5, $t5, $t9            # temp2 *= 1000      # Multiply temp2 by 1000
            add  $t3, $t3, $t5            # address = base + temp1 + temp2

            add  $t3, $t3, $t6            # address = base + temp1 + temp2 + temp3

            sb   $t2, ($t3)               # *(address) = t2    # store the character into the address in `bigString`

            # Go to next character in the matrix
            addi $t1, $t1, 1              # t1++
            j    bigString_populate_loop_character_loop
            nop

        bigString_populate_loop_continue:
            # Go to next letter in the input
            addi $s2, $s2, 1              # s2++               # Increment the address position in input string
            addi $t4, $t4, 1              # t4++               # Increment the string length count
            j    bigString_populate_loop
            nop

        # End of loop
        bigString_populate_loopEnd:
            # Set the return value to the number of characters read
            move $v0, $t4                 # v0 = t4

            # Clean up
            lw   $s2, -16($fp)
            lw   $s1, -12($fp)
            lw   $s0, -8($fp)
            lw   $ra, -4($fp)
            la   $sp, 4($fp)
            lw   $fp, ($fp)
            jr   $ra
            nop
