
#
# <display_show>
# info: Prints out the contents of `display` (9x80 character matrix)
#       Function uses syscall 4 (print string) rather than syscall 11 (print char) as it is more efficient
#
# vars: a0 - address position in `display`
#       t0 - temp
#       t7 - loop counter
#
	.text
display_show:
    la $a0, display     # a0 = &display     # address position in `display`
    li $v0, 13          # v0 = new line character

    lb $t0, 80($a0)
    lb $t1, 160($a0)
    lb $t2, 240($a0)
    lb $t3, 320($a0)
    lb $t4, 400($a0)
    lb $t5, 480($a0)
    lb $t6, 560($a0)
    lb $t7, 640($a0)
    lb $t8, 720($a0)
    lb $t9, 721($a0)

    sb $t0, 80($a0)
    sb $t0, 160($a0)
    sb $t0, 240($a0)
    sb $t0, 320($a0)
    sb $t0, 400($a0)
    sb $t0, 480($a0)
    sb $t0, 560($a0)
    sb $t0, 640($a0)
    sb $0, 720($a0)
    #sb $0,  721($a0)

    li $v0, 4
    syscall

    sb $t0, 80($a0)
    sb $t1, 160($a0)
    sb $t2, 240($a0)
    sb $t3, 320($a0)
    sb $t4, 400($a0)
    sb $t5, 480($a0)
    sb $t6, 560($a0)
    sb $t7, 640($a0)
    sb $t8, 720($a0)
#    sb $t9, 721($a0)

    jr	$ra
    nop