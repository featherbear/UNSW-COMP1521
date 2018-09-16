.text
.globl main


help:
li $t0, 100

  jr $ra
  
main:
li $t0, 50
jal help
li $t0 0
nop
nop
nop
li $v0, 60

