
                                        ############################################################ -*- asm -*-
                                        # COMP1521 18s2 -- Assignment 1 -- Scrolling Text!
                                        # Scroll letters from a message in argv[1]
                                        #
                                        # Base code by Jashank Jeremy
                                        # Tweaked by John Shepherd
                                        # $Revision: 1.5$

                                        # Requires:
                                        #  - `all_chars', defined in chars.s

                                        # Provides:
                                        .globl  main               # :: int, [char *], [char *] -> int
                                        .globl  bigString_populate # :: str -> int
                                        .globl  bigString_clear    # :: void -> void
                                        .globl  clearScreen        # :: void -> void
                                        .globl  delay              # :: int -> vovid
                                        .globl  display_populate   # :: int -> void
                                        .globl  display_show       # :: void -> void
                                        .globl  isUpper            # :: char -> int
                                        .globl  isLower            # :: char -> int

                                        .globl  CHRSIZE
                                        .globl  NROWS
                                        .globl  NDCOLS
                                        .globl  MAXCHARS
                                        .globl  NSCOLS
                                        .globl  CLEAR
