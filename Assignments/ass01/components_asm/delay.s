
                                        ########################################################################
                                        # .TEXT <delay>
                                            .text
                                        delay:

                                        # Frame:	$fp, $ra
                                        # Uses:		$a0, $t0, $t1, $t2, $t3, $t4, $t5
                                        # Clobbers:	$t0, $t1, $t2, $t3, $t4, $t5

                                        # Locals:
                                        #	- `n' in $a0
                                        #	- `x' in $t0
                                        #	- `i' in $t1
                                        #	- `j' in $t2
                                        #	- `k' in $t3

                                        # Structure:
                                        #	delay
                                        #	-> [prologue]
                                        #	-> delay_i_init
                                        #	-> delay_i_cond
                                        #	   -> delay_j_init
                                        #	   -> delay_j_cond
                                        #	      -> delay_k_init
                                        #	      -> delay_k_cond
                                        #	         -> delay_k_step
                                        #	      -> delay_k_f
                                        #	      -> delay_j_step
                                        #	   -> delay_j_f
                                        #	   -> delay_i_step
                                        #	-> delay_i_f
                                        #	-> [epilogue]

                                        # Code:
                                            sw	$fp, -4($sp)
                                            la	$fp, -4($sp)
                                            sw	$ra, -4($fp)
                                            la	$sp, -8($fp)

                                            # x <- 0
                                            move	$t0, $zero
                                            # These values control the busy-wait.
                                            li	$t4, 150
                                            li	$t5, 100

                                        delay_i_init:
                                            # i = 0;
                                            move	$t1, $zero
                                        delay_i_cond:
                                            # i < n;
                                            bge	$t1, $a0, delay_i_f
                                            nop	# in delay slot

                                        delay_j_init:
                                            # j = 0;
                                            move	$t2, $zero
                                        delay_j_cond:
                                            # j < DELAY_J;
                                            bge	$t2, $t4, delay_j_f
                                            nop	# in delay slot

                                        delay_k_init:
                                            # k = 0;
                                            move	$t3, $zero
                                        delay_k_cond:
                                            # k < DELAY_K;
                                            bge	$t3, $t5, delay_k_f
                                            nop	# in delay slot

                                            # x = x + 1
                                            addi	$t0, $t0, 1

                                        delay_k_step:
                                            # k = k + 1
                                            addi	$t3, $t3, 1
                                            j	delay_k_cond
                                            nop	# in delay slot
                                        delay_k_f:

                                        delay_j_step:
                                            # j = j + 1
                                            addi	$t2, $t2, 1
                                            j	delay_j_cond
                                            nop	# in delay slot
                                        delay_j_f:

                                        delay_i_step:
                                            # i = i + 1
                                            addi	$t1, $t1, 1
                                            j	delay_i_cond
                                            nop	# in delay slot
                                        delay_i_f:

                                        delay__post:
                                            # tear down stack frame
                                            lw	$ra, -4($fp)
                                            la	$sp, 4($fp)
                                            lw	$fp, ($fp)
                                            jr	$ra
                                            nop	# in delay slot
