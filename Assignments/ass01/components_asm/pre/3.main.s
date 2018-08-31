
                                        ########################################################################
                                        # .TEXT <main>
                                            .text
                                        main:

                                        # Frame:  $fp, $ra, ...
                                        # Uses:    $a0, $a1, $t0, $t1, $t2, $s0, $s1
                                        # Clobbers:  ...

                                        # Locals:
                                        #  - `theLength' in $s0
                                        #  - `bigLength' in $s1
                                        #  - `ch' in $s2
                                        #  - `str' in $t2
                                        #  - `i' in $...
                                        #  - `j' in $...
                                        #  - `row' in $...
                                        #  - `col' in $...
                                        #  - `iterations' in $...
                                        #  - `startingCol' in $...

                                        # Structure:
                                        #  main
                                        #  -> [prologue]
                                        #  -> main_argc_gt_two
                                        #  -> main_PTRs_init
                                        #    -> main_PTRs_cond
                                        #      -> main_ch_notspace
                                        #      -> main_ch_isLower
                                        #      -> main_ch_isSpace
                                        #    -> main_PTRs_step
                                        #  -> main_PTRs_f
                                        #  [theLength cond]
                                        #    | main_theLength_ge_MAXCHARS
                                        #    | main_theLength_lt_MAXCHARS
                                        #    | main_theLength_lt_1
                                        #    | main_theLength_ge_1
                                        #  ...
                                        #  -> [epilogue]

                                        # Code:
                                            # set up stack frame
                                            sw  $fp, -4($sp)
                                            la  $fp, -4($sp)
                                            sw  $ra, -4($fp)  # note: switch to $fp-relative
                                            sw  $s0, -8($fp)
                                            sw  $s1, -12($fp)
                                            sw  $s2, -16($fp)
                                            addi  $sp, $sp, -20

                                            # if (argc < 2)
                                            li  $t0, 2
                                            bge  $a0, $t0, main_argc_gt_two
                                            nop  # in delay slot
                                            # printf(...)
                                            la  $a0, main__0
                                            li  $v0, 4 # PRINT_STRING_SYSCALL
                                            syscall
                                            # return 1  =>  load $v0, jump to epilogue
                                            li  $v0, 1
                                            j  main__post
                                            nop  # in delay slot
                                        main_argc_gt_two:

                                            move  $s0, $zero
                                        main_PTRs_init:
                                            # s = argv[1]
                                            lw  $t2, 4($a1)

                                        main_PTRs_cond:
                                            # optimisation: `ch = *s' now
                                            # (ch = )*s
                                            lb  $s2, ($t2)
                                            # *s != '\0'  =>  ch != 0
                                            beqz  $s2, main_PTRs_f
                                            nop  # in delay slot

                                            # if (!isUpper(ch))
                                        main_ch_upper:
                                            move  $a0, $s2
                                            jal  isUpper
                                            nop  # in delay slot
                                            beqz  $v0, main_ch_lower
                                            nop  # in delay slot

                                            li $v0, 11
                                            syscall

                                            j  main_ch_ok
                                            nop  # in delay slot

                                            # if (!isLower(ch))
                                        main_ch_lower:
                                            move  $a0, $s2
                                            jal  isLower
                                            nop  # in delay slot
                                            beqz  $v0, main_ch_space
                                            nop  # in delay slot

                                            li $v0, 11
                                            syscall

                                            j  main_ch_ok
                                            nop  # in delay slot
                                            # if (ch != ' ')

                                        main_ch_space:
                                            li  $t0, ' '
                                            bne  $s2, $t0, main_ch_fail
                                            nop  # in delay slot

                                            li $v0, 11
                                            syscall

                                            j  main_ch_ok
                                            nop  # in delay slot

                                        main_ch_fail:
                                            # printf(...)
                                            la  $a0, main__1
                                            li  $v0, 4 # PRINT_STRING_SYSCALL
                                            syscall
                                            # exit(1)  =>  return 1  =>  load $v0, jump to epilogue
                                            li  $v0, 1
                                            j  main__post
                                            nop  # in delay slot

                                        main_ch_ok:
                                            # if (theLength >= MAXCHARS)
                                            la  $t0, MAXCHARS
                                            lw  $t0, ($t0)
                                            # break  =>  jump out of for(*s...)
                                            bge  $s0, $t0, main_PTRs_f

                                            # theString[theLength]
                                            la  $t0, theString
                                            addu  $t0, $t0, $s0  # ADDU because address
                                            # theString[theLength] = ch
                                            sb  $s2, ($t0)

                                            # theLength++
                                            addi  $s0, $s0, 1

                                        main_PTRs_step:
                                            # s++  =>  s = s + 1
                                            addiu  $t2, $t2, 1  # ADDIU because address
                                            j  main_PTRs_cond
                                            nop

                                        main_PTRs_f:

                                            # theString[theLength] = ...
                                            la  $t0, theString
                                            addu  $t0, $t0, $s0  # ADDU because address
                                            # theString[theLength] = '\0'
                                            sb  $zero, ($t0)

                                            # CHRSIZE + 1
                                            la  $t0, CHRSIZE
                                            lw  $t0, ($t0)
                                            addi  $t0, $t0, 1
                                            # bigLength = theLength * (CHRSIZE + 1)
                                            mul  $s1, $t0, $s0

                                            # if (theLength >= MAXCHARS)
                                            la  $t0, MAXCHARS
                                            lw  $t0, ($t0)
                                            blt  $s0, $t0, main_theLength_lt_MAXCHARS
                                            nop  # in delay slot

                                        main_theLength_ge_MAXCHARS:
                                            # printf(..., ..., ...)
                                            la  $a0, main__2
                                            li  $v0, 4 # PRINT_STRING_SYSCALL
                                            syscall

                                            move  $a0, $t0
                                            li  $v0, 1 # PRINT_INT_SYSCALL
                                            syscall

                                            la  $a0, main__3
                                            li  $v0, 4 # PRINT_STRING_SYSCALL
                                            syscall

                                            # return 1  =>  load $v0, jump to epilogue
                                            li  $v0, 1
                                            j  main__post
                                            nop  # in delay slot

                                        main_theLength_lt_MAXCHARS:
                                            # if (theLength < 1)
                                            li  $t0, 1
                                            bge  $s0, $t0, main_theLength_ge_1
                                            nop  # in delay slot

                                        main_theLength_lt_1:
                                            # printf(...)
                                            la  $a0, main__4
                                            li  $v0, 4 # PRINT_STRING_SYSCALL
                                            syscall
                                            # exit(1)  =>  return 1  =>  load $v0, jump to epilogue
                                            li  $v0, 1
                                            j  main__post
                                            nop  # in delay slot

main_theLength_ge_1:
    # Initialise the contents of `bigString`
    jal  bigString_clear
    nop

    # Populate the contents of `bigString` with `theString`
    la   $a0, theString       # a0 = &theString
    jal  bigString_populate   # v0 = bigString_populate(&theString)
    nop

    # Calculate the last starting column to display from
    move $s1, $v0             # s1 = v0            # Set s1 to the length of `theString`

    # Calculate the number of columns that each charcater will use
    # (9 columns + 1 space = 10 characters)
    lw   $t0, CHRSIZE         # t0 = 9 (CHRSIZE)
    addi $t0, $t0, 1          # t0 = CHRSIZE + 1 = 10

    mul  $s1, $s1, $t0        # s1 *= 10           # number of characters *  is the last start column

    lw   $s0, NDCOLS          # s0 = 80 (NDCOLS)
    sub  $s0, $0, $s0         # s0 = -80 (-NDCOLS) # Start off-screen

    # Display loop
    display_loop:
        # while (s0 < s1) { ... }
        beq  $s0, $s1, display_loopEnd

        # Populate the contents of `display` from `bigString` starting at the column offset
        move $a0, $s0       # a0 = s0       # a0 is the starting column offset
        jal  display_populate
        nop

        # Clear the screen
        jal  clearScreen
        nop

        # Show the display
        jal  display_show
        nop

        # Wait ... for some time ...
        li   $a0, 1 # <--- Change me to a higher value for a longer delay?
        jal  delay
        nop

        # Go to the next column in the loop
        addi $s0, $s0, 1    # s++           # Increment the starting column offset
        j    display_loop
        nop

    # End display loop
    display_loopEnd:
        move $v0, $zero     # return 0

        # Clear the screen
        jal  clearScreen
        nop

                                        main__post:
                                            # tear down stack frame
                                            lw  $s2, -16($fp)
                                            lw  $s1, -12($fp)
                                            lw  $s0, -8($fp)
                                            lw  $ra, -4($fp)
                                            la  $sp, 4($fp)
                                            lw  $fp, ($fp)
                                            jr  $ra
                                            nop  # in delay slot
