
###########################################################
# COMP1521 18s2 -- Assignment 1 -- Scrolling Text!
# Scroll letters from a message in argv[1]
# z5207930 Xu, Jennifer (Wednesday 6-8)


# Requires:
#  - `all_chars', defined in chars.s

# Provides:
    .globl  main                        # :: int, [char *], [char *] -> int
    .globl  setUpDisplay                # :: int, int -> void
    .globl  showDisplay                 # :: void -> void
    .globl  delay                       # :: int -> vovid
    .globl  isUpper                     # :: char -> int
    .globl  isLower                     # :: char -> int

    .globl  CHRSIZE
    .globl  NROWS
    .globl  NDCOLS
    .globl  MAXCHARS
    .globl  NSCOLS
    .globl  CLEAR


############################################################
    .data

    # # of rows and columns in each big char
CHRSIZE:    .word   9
    # number of rows in all matrices
NROWS:      .word   9
    # number of columns in display matrix
NDCOLS:     .word   80
    # max length of input string
MAXCHARS:   .word   100
    # number of columns in bigString matrix
    # max length of buffer to hold big version
    # the +1 allows for one blank column between letters
NSCOLS:     .word   9000    # (NROWS * MAXCHARS * (CHRSIZE + 1))
        # ANSI escape sequence for 'clear-screen'
CLEAR:  .asciiz "\033[H\033[2J"
# CLEAR:    .asciiz "__showpage__\n" # for debugging

main__0:    .asciiz "Usage: ./scroll String\n"
main__1:    .asciiz "Only letters and spaces are allowed in the string!\n"
main__2:    .asciiz "String must be < "
main__3:    .asciiz " chars\n"
main__4:    .asciiz "Please enter a string with at least one character!\n"

    .align  4
theString:  .space  101 # MAXCHARS + 1
    .align  4
display:    .space  720 # NROWS * NDCOLS
    .align  4
bigString:  .space  81000   # NROWS * NSCOLS


########################################################################
# .TEXT <main>
    .text
# Frame:    $fp, $ra, $s1, $s2, $s3, $s4, $s5, $s6
# Uses:     $a0, $a1, $a2, $v0
# Clobbers: $t0, $t1, $t2, $t3, $t4

# Locals:
#   - `theLength' in $s0
#   - `bigLength' in $s1
#   - `ch'        in $s2
#   - `row'       in $s3
#   - `col'       in $s4
#   - `i'         in $s5
#   - `j'         in $s6

#   - `startingCol'in $a0
#   - `iterations'in $a1

# Structure:
#   main
#   -> [prologue]
#   -> main_argc_gt_two
#   -> main_PTRs_init
#     -> main_PTRs_cond
#       -> main_ch_notspace
#       -> main_ch_isLower
#       -> main_ch_isSpace
#     -> main_PTRs_step
#   -> main_PTRs_f
#   [theLength cond]
#     | main_theLength_ge_MAXCHARS
#     | main_theLength_lt_MAXCHARS
#     | main_theLength_lt_1
#     | main_theLength_ge_1
#   /// - Added Code - ///
#   -> empty_display_loop_row
#     -> empty_display_loop_col
#   -> outer_char_loop
#     -> if_char_eq_space
#       -> if_char_loop_row
#         -> if_char_loop_col
#     -> else
#       -> else_cond_upper
#       -> else_cond_lower
#       -> else_char_loop_row
#         -> else_char_loop_col
#     -> add_spaces_loop
#     -> final_loop
#     -> main__post
#   -> [epilogue]

main:
    addi $sp, $sp, -4
    sw   $fp, ($sp)
    la   $fp, ($sp)
    addi $sp, $sp, -4
    sw   $ra, ($sp)
    addi $sp, $sp, -4
    sw   $s0, ($sp)
    addi $sp, $sp, -4
    sw   $s1, ($sp)
    addi $sp, $sp, -4
    sw   $s2, ($sp)
    addi $sp, $sp, -4
    sw   $s3, ($sp)
    addi $sp, $sp, -4
    sw   $s4, ($sp)
    addi $sp, $sp, -4
    sw   $s5, ($sp)
    addi $sp, $sp, -4
    sw   $s6, ($sp)


##
## Checks that user has included a string after ./scroll
##
    li  $t0, 2                          # int var = 2;
    bge $a0, $t0, main_argc_gt_two      # if (argc >= 2)
    nop

    la  $a0, main__0                    # Load string "Usage: ..."
    li  $v0, 4
    syscall

    li  $v0, 1
    j   main__post
    nop

main_argc_gt_two:
    move $s0, $zero                     # theLength = 0

main_PTRs_init:
    lw   $t2, 4($a1)                    # Load argv[1] into $t2

main_PTRs_cond:
    lb   $s2, ($t2)
    beqz $s2, main_PTRs_f               # if $s2 == '\0'
    nop

##
## Checks conditions: !isUpper(ch) && !isLower(ch) && ch != ' '
##
main_ch_upper:
    move $a0, $s2
    jal  isUpper
    nop
    beqz $v0, main_ch_lower
    nop
    j    main_ch_ok
    nop

main_ch_lower:
    move $a0, $s2
    jal  isLower
    nop
    beqz $v0, main_ch_space
    nop
    j    main_ch_ok
    nop

main_ch_space:
    li   $t0, ' '
    bne  $s2, $t0, main_ch_fail
    nop                                 # in delay slot
    j    main_ch_ok
    nop                                 # in delay slot

main_ch_fail:
    la  $a0, main__1
    li  $v0, 4                          # PRINT_STRING_SYSCALL
    syscall

    li  $v0, 1                          # load $v0, jump to epilogue
    j   main__post
    nop

main_ch_ok:
    la  $t0, MAXCHARS
    lw  $t0, ($t0)
    bge $s0, $t0, main_PTRs_f           # if (theLength >= MAXCHARS)

    # theString[theLength]
    la   $t0, theString
    addu $t0, $t0, $s0                  # ADDU because address
                                        # theString[theLength] = ch
    sb   $s2, ($t0)


    addi $s0, $s0, 1                    # theLength++

main_PTRs_step:
    addiu   $t2, $t2, 1                 # s++;
    j   main_PTRs_cond
    nop

main_PTRs_f:                            # Adds '\0' onto the end
    la   $t0, theString                 # Load theString --> $t0
    addu $t0, $t0, $s0
    sb   $zero, ($t0)                   # Put '\0' into theString[theLength]

    la   $t0, CHRSIZE                   # Load CHRSIZE --> $t0
    lw   $t0, ($t0)                     # Load value at new_address
    addi $t0, $t0, 1                    # var = CHRSIZE + 1
    mul  $s1, $t0, $s0                  # bigLength = var * theLength


    la  $t0, MAXCHARS
    lw  $t0, ($t0)
    blt $s0, $t0, main_theLength_lt_MAXCHARS
    nop

##
## printf("String must be < %d chars\n", MAXCHARS);
##
main_theLength_ge_MAXCHARS:
    la   $a0, main__2                   # Load String: "String must be < "
    li   $v0, 4                         # PRINT_STRING_SYSCALL
    syscall

    lw   $a0, MAXCHARS                  # Move MAXCHARS into $a0
    li   $v0, 1                         # PRINT_INT_SYSCALL
    syscall

    la   $a0, main__3                   # Load String: "chars\n"
    li   $v0, 4                         # PRINT_STRING_SYSCALL
    syscall

    li   $v0, 1                         # Load return value = 1
    j    main__post                     # Jump to epilogue
    nop

main_theLength_lt_MAXCHARS:

    # if (theLength < 1)
    li  $t0, 1                          # int var = 1;
    bge $s0, $t0, main_theLength_ge_1   # if(theLength >= 1), continue with main code
    nop

main_theLength_lt_1:
    la  $a0, main__4                    # Load String: "Please ... "
    li  $v0, 4                          # PRINT_STRING_SYSCALL
    syscall
    li  $v0, 1                          # Set return value = 1
    j   main__post                      # Jump to epilogue
    nop

################## - Beginning of added main code - #################
# Friendly Reminder of variables
# Locals:
#   - `theLength' in $s0
#   - `bigLength' in $s1
#   - `ch'        in $s2
#   - `row'       in $s3
#   - `col'       in $s4
#   - `i'         in $s5
#   - `j'         in $s6

#   - `startingCol'in $a0
#   - `iterations'in $a3


##
## Sets up a blank screen by setting every 'slot' into spaces
##
main_theLength_ge_1:
    li  $s5, 0                          # i = 0;

empty_display_loop_row:
    lw  $t0, NROWS                      # var = NROWS
    bge $s5, $t0, end_empty_display_loop_row
    li  $s6, 0                          # j = 0;

empty_display_loop_col:
    lw  $t1, NDCOLS                     # var = NDCOLS
    bge $s6, $t1, end_empty_display_loop_col


    # display[i][j] = ' ';              # offset = (i * NDCOLS) + j
    lw   $t1, NDCOLS                    # var = NDCOLS
    mul  $t2, $s5, $t1                  # offset = i * NDCOLS
    add  $t2, $t2, $s6                  # offset = offset + j
    la   $t3, display                   # Load address of display
    add  $t3, $t2, $t3                  # add offset onto address
    li   $t2, ' '                       # load ' ' into $t2
    sb   $t2, ($t3)                     # load ' ' into $t3

    addi $s6, $s6, 1                    # j++;
    j    empty_display_loop_col

end_empty_display_loop_col:
    addi $s5, $s5, 1                    # i++;
    j    empty_display_loop_row

 end_empty_display_loop_row:

 ##
 ## Create bigChars array
 ##
    li   $s5, 0                         # i = 0;

outer_char_loop:
    bge $s5, $s0, end_outer_char_loop

  # char ch = theString[i];             # offset = i
    la   $s2, theString                 # get the address of theString
    add  $s2, $s2, $s5,                 # add offset onto the address
    lb   $s2, ($s2)                     # load value of new_address

if_char_eq_space:
    li $t0, ' '                         # load ' ' into $t0
    bne $s2, $t0, else                  # if ch != ' ', goto else

    li $s3, 0                           # row = 0;
if_char_loop_row:
    lw  $t0, CHRSIZE
    bge $s3, $t0, end_if_char_loop_row  # if (row >= CHRSIZE)
    li  $s4, 0                          # col = 0;

if_char_loop_col:
    lw  $t0, CHRSIZE
    bge $s4, $t0, end_if_char_loop_col  # if(col >= CHRSIZE)

    # bigString[row][col + i * (CHRSIZE + 1)] = ' ';
    # offset = row * NSCOLS + (col + i * (CHRSIZE + 1))
    lw   $t0, NSCOLS                    # var0 = NSCOLS
    lw   $t4, CHRSIZE                   # var4 = CHRSIZE
    mul  $t1, $s3, $t0                  # var1 = row * NSCOLS
    addi $t2, $t4, 1                    # var2 = CHRSIZE + 1
    mul  $t2, $t2, $s5                  # var2 = var2 * i
    add  $t2, $t2, $s4                  # var2 = var2 + col
    add  $t3, $t1, $t2                  # offset = var1 + var2

    la   $t0, bigString                 # load address of bigString
    add  $t0, $t0, $t3                  # add offset onto address
    li   $t1, ' '                       # load ' ' into $t1
    sb   $t1, ($t0)                     # store ' ' into $t0

    addi $s4, $s4, 1                    # col++;
    j    if_char_loop_col

end_if_char_loop_col:
    addi $s3, $s3, 1                    # row++;
    j    if_char_loop_row

end_if_char_loop_row:
    j   end_outer_conditions            # jumps out of massive loop
                                        # (ln126)

else:
else_cond_upper:
    move $a0, $s2                       # give argument 'ch'
    jal  isUpper                        # isUpper Uses: $a0, $v0
    move $t0, $v0                       # store its return value
    beq  $t0, $zero, call_is_lower      # if (isUpper == 0)
    li   $t0, 'A'                       # which 'A'
    li   $t1, -1                        # var = -1
    mul  $t0, $t0, $t1                  # which = - 'A'
    add  $t0, $t0, $s2                  # which = ch - 'A'
    move $v1, $t0                       # which is in $v1

call_is_lower:
    move $a0, $s2                       # give argument 'ch'
    jal  isLower                        # isUpper Uses: $a0, $v0
    move $t0, $v0                       # store its return value
    beq  $t0, $zero, found_which        # if (isLower == 0)
    li   $t0, 'a'                       # which 'a'
    li   $t1, -1                        # var1 = -1
    li   $t2, 26                        # var2 = 26
    mul  $t0, $t0, $t1                  # which = - 'a'
    add  $t0, $t0, $s2                  # which = ch - 'a'
    add  $t0, $t0, $t2                  # which  = ch - 'a' + 26
    move $v1, $t0                       # which is in $v1

found_which:

##
## Nested for loop Copies char to the buffer
##
    li  $s3, 0                          # row = 0;
else_char_loop_row:
    lw  $t0, CHRSIZE                    # var0 = CHRSIZE
    bge $s3, $t0, end_else_char_loop_row # if(row >= CHRSIZE)

else_char_loop_col:
    lw  $t0, CHRSIZE                    # var0 = CHRSIZE
    bge $s4, $t0, end_else_char_loop_col # if(col >= CHRSIZE)

    # bigString[row][col + i * (CHRSIZE+1)] = all_chars[which][row][col];
    # all_chars[which][row][col] --> $t1
    # offset = (which * NROWS * NROWS) + (row * CHRSIZE) + col

    lw  $t0, CHRSIZE                    # var0 = CHRSIZE
    mul $t0, $s3, $t0                   # var0 = row * CHRSIZE
    lw  $t1, NROWS                      # var1 = NROWS
    mul $t1, $t1, $t1                   # var1 = NROWS * NROWS
    mul $t1, $t1, $a0                   # var1 = var1 * which
    add $t2, $t0, $t1                   # offset = var0 + var1
    add $t2, $t2, $s4                   # offset = offset + col
    la  $t0, all_chars                  # Load address of all_chars
    add $t0, $t0, $t2                   # Add offset onto address
    lb  $a1, ($t0)                      # Load value of new_address

    # bigString[row][col + i * (CHRSIZE + 1)]
    # offset = row * NSCOLS + (col + i * (CHRSIZE + 1))

    lw   $t0, NSCOLS                    # var0 = NSCOLS
    mul  $t0, $t0, $s3                  # var0 = NSCOLS * row
    lw   $t1, CHRSIZE                   # var1 = CHRSIZE
    addi $t1, $t1, 1                    # var1 = CHRSIZE + 1
    mul  $t1, $t1, $s5                  # var1 = var1 * i
    add  $t1, $t1, $s4                  # var1 = var1 * col
    add  $t2, $t0, $t1                  # offset = var1 + var2

    la   $t0, bigString                 # Load address of bigString
    add  $t0, $t0, $t1                  # Add offset onto address
    sb   $a1, ($t0)                     # Store value into new_address

    addi $s4, $s4, 1                    # col++;
    j   else_char_loop_col

end_else_char_loop_col:
    addi $s3, $s3, 1                    # row++;
    j    else_char_loop_row

end_else_char_loop_row:
end_outer_conditions:

##
## For loop creates spaces for when each letter is finished
##
                                        # col = (i * (CHRSIZE+1)) + CHRSIZE;
    lw   $t0, CHRSIZE                   # var = CHRSIZE
    addi $s4, $t0, 1                    # col = CHRSIZE + 1
    mul  $s4, $s4, $s5                  # col = col * i
    add  $s4, $s4, $t0                  # col = col + CHRSIZE

    li   $s3, 0                         # row = 0
add_spaces_loop:
    lw  $t0, CHRSIZE                    # var0 = CHRSIZE
    bge $s3, $t0, end_add_spaces_loop   # if(row >= CHRSIZE)

    # bigString[row][col] = ' ';        # offset = row * NSCOLS + col
    lw   $t0, NSCOLS                    # var = NSCOLS
    mul  $t1, $s3, $t0                  # offset = row * NSCOLS
    add  $t1, $t1, $s4                  # offset = offset + col
    la   $t2, bigString                 # load address of bigString
    add  $t2, $t2, $t1                  # add offset into address
    li   $t3, ' '                       # load ' ' into $t3
    sb   $t3, ($t2)                     # store ' 'into $t2

    addi $s3, $s3, 1                    # row++;
    j    add_spaces_loop

end_add_spaces_loop:
    addi $s5, $s5, 1                    # i++;
    j   outer_char_loop

end_outer_char_loop:

    lw   $t0, NDCOLS                    # var0 = NDCOLS
    addi $a0, $t0, -1                   # starting_col = NDCOLS - 1
    add  $a3, $t0, $s1                  # interations = NDCOLS + biglength
                                        # Stored in $a_ because used as arguments later

    li   $s5, 0                         # i = 0;

final_loop:
    bge  $s5, $a3, end_final_loop       # if(i >= iterations)
    jal  setUpDisplay                   # setUpDisplay uses: $a0, $a1

    jal  showDisplay                    # showDisplay uses: $a2

    addi $a0, $a0, -1                   # starting_col--;
    addi $s5, $s5, 1                    # i++;

    li   $a3, 1                         # give argument of 1
    jal  delay                                                  # delay uses: $a3
    j    final_loop

end_final_loop:
    move $v0, $zero                     # Set return value = 0

main__post:
    # tear down stack frame
    lw   $s6, ($sp)
    addi $sp, $sp, 4
    lw   $s5, ($sp)
    addi $sp, $sp, 4
    lw   $s4, ($sp)
    addi $sp, $sp, 4
    lw   $s3, ($sp)
    addi $sp, $sp, 4
    lw   $s2, ($sp)
    addi $sp, $sp, 4
    lw   $s1, ($sp)
    addi $sp, $sp, 4
    lw   $s0, ($sp)
    addi $sp, $sp, 4
    lw   $ra, ($sp)
    addi $sp, $sp, 4
    lw   $fp, ($sp)
    addi $sp, $sp, 4
    j    $ra
    nop # in delay slot

########################################################################
# .TEXT <setUpDisplay>
    .text
setUpDisplay:

# Frame:    $fp, $ra, $s0, $s1, $s2, $s3
# Uses:     $a0, $a1
# Clobbers: $t0, $t1, $t2, $t3

# Locals:
#   - `row'       in $s0
#   - `out_col'   in $s1
#   - `in_col'    in $s2
#   - `first_col' in $s3

# Structure:
#   setUpDisplay
#   -> [prologue]
#   -> if_starting
#   -> else_starting
#     -> else_loop_col
#       -> else_loop_row
#   -> copy_loop_in_col
#     -> break_cond
#     -> copy_loop_row
#   -> [epilogue]

# Code:
    # set up stack frame
    addi $sp, $sp, -4
    sw   $fp, ($sp)
    la   $fp, ($sp)
    addi $sp, $sp, -4
    sw   $ra, ($sp)
    addi $sp, $sp, -4
    sw   $s0, ($sp)
    addi $sp, $sp, -4
    sw   $s1, ($sp)
    addi $sp, $sp, -4
    sw   $s2, ($sp)
    addi $sp, $sp, -4
    sw   $s3, ($sp)

    li   $s1, 0                         # out_col = 0
if_starting:
    bge  $a0, $zero, else_starting      # if (starting >= 0)
    li   $t0, -1                        # var0 = -1
    mul  $t0, $t0, $a0                  # var = -starting
    move $s3, $t0                       # first_col = var

else_starting:
    li   $s1, 0                         # out_col = 0

else_loop_col:
    bge  $s1, $a0, end_else_loop_col    # if (out_col >= starting)
    li   $s0, 0                         # row = 0;

else_loop_row:
    lw   $t0, NROWS                     # var = NROWS
    bge  $s0, $t0, end_else_loop_row

    # display[row][out_col] = ' ';      # offset = row * NDCOLS + out_col
    lw   $t0, NDCOLS                    # var = NDCOLS
    mul  $t1, $s0, $t0                  # offset = row * NDCOLS
    add  $t1, $t1, $s1                  # offset = offset + out_col
    la   $t2, display                   # Load address of display
    add  $t2, $t2, $t1                  # add offset onto the address
    li   $t3, ' '                       # load ' ' into $t3
    sb   $t3, ($t2)                     # store ' ' into new_address

    addi $s0, $s0, 1                    # row++;
    j    else_loop_row

end_else_loop_row:
    addi $s1, $s1, 1                    # out_col++;
    j    else_loop_col

end_else_loop_col:
    li   $s3, 0                         # first_col = 0;
    move $s2, $s3                       # in_col = first_col

##################################################################
copy_loop_in_col:
    bge $s2, $a1, end_copy_loop_in_col  # if (in_col >= length)

    lw  $t0, NDCOLS
    blt $s1, $t0, end_copy_loop_in_col  # if (out_col < NDCOLS) break;
    li  $s0, 0                          # row = 0;

copy_loop_row:
    lw  $t0, NROWS
    bge $s0, $t0, end_copy_loop_row    # if(row >= NROWS)

    # display[row][out_col] = bigString[row][in_col];
    # char display[NROWS][NDCOLS];
    # char bigString[NROWS][NSCOLS];

    # bigString[row][in_col]            # offset = row * NSCOLS + in_col
    lw   $t0, NSCOLS                    # var = NSCOLS
    mul  $t1, $s0, $t0                  # offset = row * NSCOLS
    add  $t1, $t1, $s2                  # offset = offset + in_col

    la   $t2, bigString                 # load address of bigString
    add  $t2, $t2, $t1                  # add offset onto address
    lb   $t3, ($t2)                     # load value of new_address

    # display[row][out_col]             # offset = row * NDCOLS + out_col
    lw   $t0, NDCOLS                    # var = NDCOLS
    mul  $t1, $s0, $t0                  # offset = row * NDCOLS
    add  $t1, $t1, $s1                  # offset = offset + out_col

    la   $t2, display                   # load address of display
    add  $t2, $t2, $t1                  # add offset into address
    sb   $t3, ($t2)                     # store value into new_address

    addi $s0, $s0, 1                    # row++;
    j    copy_loop_row

end_copy_loop_row:
    addi $s1, $s1, 1                    # out_col++;
    j    copy_loop_in_col

end_copy_loop_in_col:

  # tear down stack frame
    lw   $s3, ($sp)
    addi $sp, $sp, 4
    lw   $s2, ($sp)
    addi $sp, $sp, 4
    lw   $s1, ($sp)
    addi $sp, $sp, 4
    lw   $s0, ($sp)
    addi $sp, $sp, 4
    lw   $ra, ($sp)
    addi $sp, $sp, 4
    lw   $fp, ($sp)
    addi $sp, $sp, 4
    j    $ra
    nop # in delay slot

########################################################################
# .TEXT <showDisplay>
    .text
showDisplay:

# Frame:    $fp, $ra, $s0, $s1
# Uses:     $a2
# Clobbers: $t0, $t1, $t2, $t3

# Locals:
#   - `i' in $s0
#   - `j' in $s1

# Structure:
#   showDisplay
#   -> [prologue]
#   -> print_clear
#   -> outer_loop
#     -> inner_loop
#   -> [epilogue]

# Code:
    addi $sp, $sp, -4
    sw   $fp, ($sp)
    la   $fp, ($sp)
    addi $sp, $sp, -4
    sw   $ra, ($sp)
    addi $sp, $sp, -4
    sw   $s0, ($sp)
    addi $sp, $sp, -4
    sw   $s1, ($sp)

print_clear:
    la   $a2, CLEAR                     # Load String: "CLEAR"
    li   $v0, 4                         # PRINT_STRING_SYSCALL
    syscall

    li   $s0, 0                         # int i = 0;

outer_loop:
    lw   $t0, NROWS                     # var = NROWS
    bge  $s0, $t0, end_outer_loop       # if(i >= NROWS)
    li   $s1, 0                         # int j = 0;

inner_loop:
    lw   $t0, NDCOLS                    # var = NDCOLS
    bge  $s1, $t0, end_inner_loop

    # putchar(display[i][j])            # offset = (i * NDCOLS) + j

    lw   $t0, NDCOLS                    # var = NDCOLS
    mul  $t1, $s0, $t0                  # offset = i * NDCOLS
    add  $t1, $t1, $s1                  # offset = offset + j

    la   $t2, display                   # Load address of display
    add  $t2, $t1, $t2                  # Add the offset to address
    lb   $a2, ($t2)                     # Load value of new_address
    li   $v0, 11                        # print_char display[i][j]
    syscall

    addi $s1, $s1, 1                    # j++;
    j    inner_loop

end_inner_loop:
                                        # putchar("\n")
    li   $a2, '\n'                      # Load String: "\n"
    li   $v0, 4                         # PRINT_STRING_SYSCALL
    syscall

    addi $s0, $s0, 1                    # i++;
    j    outer_loop

end_outer_loop:

    lw   $s1, ($sp)
    addi $sp, $sp, 4
    lw   $s0, ($sp)
    addi $sp, $sp, 4
    lw   $ra, ($sp)
    addi $sp, $sp, 4
    lw   $fp, ($sp)
    addi $sp, $sp, 4
    nop

########################################################################

##################### - Pre-written code - #############################
# .TEXT <delay>
    .text
delay:

# Frame:    $fp, $ra
# Uses:     $a3
# Clobbers: $t0, $t1, $t2, $t3, $t4, $t5

# Locals:
#   - `n' in $a3 --> Given argument
#   - `x' in $t0
#   - `i' in $t1
#   - `j' in $t2
#   - `k' in $t3

# Structure:
#   delay
#   -> [prologue]
#   -> delay_i_init
#   -> delay_i_cond
#      -> delay_j_init
#      -> delay_j_cond
#         -> delay_k_init
#         -> delay_k_cond
#            -> delay_k_step
#         -> delay_k_f
#         -> delay_j_step
#      -> delay_j_f
#      -> delay_i_step
#   -> delay_i_f
#   -> [epilogue]

# Code:
    sw  $fp, -4($sp)
    la  $fp, -4($sp)
    sw  $ra, -4($fp)
    la  $sp, -8($fp)

    # x <- 0
    move    $t0, $zero
    # These values control the busy-wait.
    li  $t4, 20000
    li  $t5, 1000

delay_i_init:
    # i = 0;
    move $t1, $zero
delay_i_cond:
    # i < n;
    bge  $t1, $a3, delay_i_f
    nop # in delay slot

delay_j_init:
    # j = 0;
    move $t2, $zero
delay_j_cond:
    # j < DELAY_J;
    bge $t2, $t4, delay_j_f
    nop # in delay slot

delay_k_init:
    # k = 0;
    move $t3, $zero
delay_k_cond:
    # k < DELAY_K;
    bge  $t3, $t5, delay_k_f
    nop # in delay slot

    addi $t0, $t0, 1            # x++;

delay_k_step:
    addi $t3, $t3, 1            # k++;
    j    delay_k_cond
    nop                         # in delay slot

delay_k_f:

delay_j_step:
    addi $t2, $t2, 1            # j++;
    j    delay_j_cond
    nop                         # in delay slot

delay_j_f:

delay_i_step:
    addi $t1, $t1, 1            # i++;
    j    delay_i_cond
    nop                         # in delay slot

delay_i_f:

delay__post:
    # tear down stack frame
    lw  $ra, -4($fp)
    la  $sp,  4($fp)
    lw  $fp,  ($fp)
    jr  $ra
    nop # in delay slot

######################## - isUpper Function - ##############################
# .TEXT <isUpper>
    .text
isUpper:

# Frame:    N/A
# Uses:     $a0, $v0
# Clobbers: $v0

# Locals:
#   - `ch'         in $a0
#   - 'comparison' in $v0

# Structure:
#   isUpper
#   -> [prologue]
#       -> [prologue]
#       -> condition_A
#       -> condition_Z
#       -> fail_conditions_upper
#       -> pass_conditions_upper
#   -> [epilogue]

# Code:
condition_A:
    li  $v0, 'A'                        # char c = 'A';
    blt $a0, $v0, fail_conditions_upper     # if (ch < A)

condition_Z:
    li  $v0, 'Z'                        # char c = 'Z'
    bgt $a0, $v0, fail_conditions_upper # if (ch > Z)

    addi    $v0, $zero, 1               # Set return value as 1
    j   pass_conditions_upper
    nop # in delay slot

fail_conditions_upper:
    move    $v0, $zero                  # Set return value as 0

pass_conditions_upper:
    jr  $ra
    nop # in delay slot

########################################################################
# .TEXT <isLower>
    .text
isLower:

# Frame:    N/A
# Uses:     $a0, $v0
# Clobbers: $v0

# Locals:
#   - `ch'         in $a0
#   - 'comparison' in $v0

# Structure:
#   isLower
#       -> [prologue]
#       -> condition_a
#       -> condition_z
#       -> fail_conditions_lower
#       -> pass_conditions_lower
#   -> [epilogue]

# Code:
condition_a:
    li  $v0, 'a'                        # char c = 'a';
    blt $a0, $v0, fail_conditions_lower # if (ch < a)

condition_z:
    li  $v0, 'z'                        # char c = 'z'
    bgt $a0, $v0, fail_conditions_lower # if (ch > z)

    addi    $v0, $zero, 1               # Set return value as 1
    j   pass_conditions_lower
    nop # in delay slot


fail_conditions_lower:
    move    $v0, $zero                  # Set return value as 0

pass_conditions_lower:

isLower__post:
    jr  $ra
    nop # in delay slot

#################################################################### EOF

