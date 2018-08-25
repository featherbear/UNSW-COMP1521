.data
nrows: .word 6
ncols: .word 12
flag:  .byte '#', '#', '#', '#', '#', '.', '.', '#', '#', '#', '#', '#'
       .byte '#', '#', '#', '#', '#', '.', '.', '#', '#', '#', '#', '#'
       .byte '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'
       .byte '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'
       .byte '#', '#', '#', '#', '#', '.', '.', '#', '#', '#', '#', '#'
       .byte '#', '#', '#', '#', '#', '.', '.', '#', '#', '#', '#', '#'

      .globl main
      .text
main:
       lw $t5, nrows # nRows
       lw $t6, ncols # nCols
       li $t0, 0     # rowIndex
       
rowLoop:
        beq $t0, $t5, endRowLoop
        li $t1, 0 # $t1 = colIndex
        colLoop:
                beq $t1, $t6, endColLoop
                mul $t3, $t0, $t6
                add $t3, $t3, $t1
                
                
                
                          # $t4 = base address of flag
                la $t4, flag
                          # $t3 = base addr + offset (x 1 bytes)
                add $t3, $t3, $t4
                          # $t3 = value at $t3
                lb $a0, ($t3)
                
                
                # lb $a0, flag($t3) # --> $t3 + flag
                
                li $v0, 11 # syscall to print character
                syscall
                
                addi $t1, $t1, 1
                j colLoop
        endColLoop:
                li $a0, 10 # \10 is the newline character
                li $v0, 11 # syscall to print character
                syscall
                addi $t0, $t0, 1
                j rowLoop
endRowLoop:
        j $ra
