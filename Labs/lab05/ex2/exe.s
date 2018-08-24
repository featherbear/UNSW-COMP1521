# COMP1521 Practice Prac Exam #1
# main program + show function

   .data
msg1:
   .asciiz "The matrix\n"
msg2:
   .asciiz "is an identity matrix\n"
msg3:
   .asciiz "is not an identity matrix\n"
   .align  2

   .text
   .globl main
main:
   addi $sp, $sp, -4
   sw   $fp, ($sp)
   la   $fp, ($sp)
   addi $sp, $sp, -4
   sw   $ra, ($sp)
   addi $sp, $sp, -4
   sw   $s1, ($sp)

   la   $a0, m
   lw   $a1, N
   jal  is_ident      # s1 = is_ident(m,N)
   move $s1, $v0      # RETURN V0

   la   $a0, msg1
   li   $v0, 4
   syscall           # printf("The matrix\n")
   la   $a0, m
   lw   $a1, N
   jal  showMatrix   # showMatrix(m, N)

main_if:             # if (s1)
   beqz $s1, main_else
   la   $a0, msg2
   li   $v0, 4
   syscall           # printf("is an identity matrix\n")
   j    end_main_if

main_else:           # else
   la   $a0, msg3
   li   $v0, 4
   syscall           # printf("is not an identity matrix\n")

end_main_if:
   lw   $s1, ($sp)
   addi $sp, $sp, 4
   lw   $ra, ($sp)
   addi $sp, $sp, 4
   lw   $fp, ($sp)
   addi $sp, $sp, 4
   j    $ra

# end main()

# void showMatrix(m, N)
# params: m=$a0, N=$a1
# locals: m=$s0, N=$s1, row=$s2, col=$s3
showMatrix:
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

   move $s0, $a0
   move $s1, $a1
   li   $s2, 0
show_matrix_loop1:
   bge  $s2, $s1, end_show_matrix_loop1

   li   $s3, 0
show_matrix_loop2:
   bge  $s3, $s1, end_show_matrix_loop2

   li   $a0, ' '          # putchar(' ')
   li   $v0, 11
   syscall

   move $t0, $s2
   mul  $t0, $t0, $s1
   add  $t0, $t0, $s3
   li   $t1, 4
   mul  $t0, $t0, $t1
   add  $t0, $t0, $s0
   lw   $a0, ($t0)
   li   $v0, 1            # printf("%d",m[row][col])
   syscall

   addi $s3, $s3, 1       # col++
   j    show_matrix_loop2

end_show_matrix_loop2:
   li   $a0, '\n'         # putchar('\n')
   li   $v0, 11
   syscall

   addi $s2, $s2, 1       # row++
   j    show_matrix_loop1

end_show_matrix_loop1:

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

   .text
   .globl is_ident

# params: m=$a0, n=$a1
is_ident:
# prologue
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
   # if you need to save more than four $s? registers
   # add extra code here to save them on the stack



   # 1000   # In our 5x5 matrix positions with 1 are (0-based)
   # 0100   # 0, 6, 12, 18, 24
   # 0010   # So therefore index % 6 == 0 for the identity values
   # 0001   # 6 = (5+1)

   # Flag!
   li $v0, 1

   # Find the range of values to iterate until
     mul $t0, $a1, $a1   ## n*n = square ## 1 - n^2
     sub $t0, $t0, 1     ## 0 - (n^2)-1

   # Create a counter
     li $t1, 0           ## counter

   # Define our divisor for our modulus
     addi $t2, $a1, 1    # (n+1)


   loop:
     # Loop while counter $t1 < max range $t0
       bgt $t1, $t0, end

     # Calculate our modulo
       divu $t1, $t2      #  LO = t1/t2, HI = t1%t2
       mfhi $t3
     ### This modulo value tells us if we are in a diagonal position,
     ### which therefore means that the value there needs to be one.

     # Calculate our address to load from
       mul $t6, $t1, 4    # A `word` is 4 bytes, so we need to increment our offset by four
       add $t4, $a0, $t6  # Address to search is base + offeset

     # Load our address
       lw $t4, ($t4)



     # Check if remainder is zero, then we have the diagonal cells, one-expected
       beq $t3, 0, checkOne

     # We didn't jump as our remainder is non zero, we have zero-expected cells
       bne $t4, 0, fail   # fail if the cell is not a 0
       j pass             # aye it worked, skip the checkOne code, and go to the next cell

     checkOne:
        bne $t4, 1, fail  # fail if the cell is not a 1
        j pass            # don't need this line since it goes to the next label anyway. eh

   pass:
     addi $t1, $t1, 1     # move our index to the next cell
     j loop               # start the loop again

   fail:
      li $v0, 0           # set out $v0 flag to 0

   end:
# epilogue
   # if you saved more than four $s? registers
   # add extra code here to restore them

   # Didn't use a single one :)

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

# COMP1521 18s1 Exam Q1
# Matrix data

   .data
N:
   .word   9
m:
   .word   1, 0, 0, 0, 0, 0, 0, 0, 0
   .word   0, 1, 0, 0, 0, 0, 0, 0, 0
   .word   0, 0, 1, 0, 0, 0, 0, 0, 0
   .word   0, 0, 0, 1, 0, 0, 0, 0, 0
   .word   0, 0, 0, 0, 1, 0, 0, 0, 0
   .word   0, 0, 0, 0, 0, 1, 0, 0, 0
   .word   0, 0, 0, 0, 0, 0, 1, 0, 0
   .word   0, 0, 0, 0, 0, 0, 0, 1, 0
   .word   0, 0, 0, 0, 0, 0, 0, 0, 1

   .align  2

