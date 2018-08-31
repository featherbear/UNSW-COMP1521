
#
# <isUpper>
# info: Check if a character is an uppercase letter
# args: a0 - character to check
#  ret: v0 - 1 if true, 0 if false
#
    .text
isUpper:
    li  $v0, 0                  # result = 0

    # If `ch` < 'A' or `ch` > 'Z', then return from the function (as result was already set to false (1)
    blt $a0, 'A', isUpper_end   # if (ch < 'A') --> false
    nop
    bgt $a0, 'Z', isUpper_end   # if (ch > 'Z') --> false
    nop

    # 'A' <= `ch` <= 'Z', so set the result to true (1)
    li  $v0, 1                  # result = true

    isUpper_end:
        jr  $ra
        nop
