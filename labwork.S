  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.data
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #
  .global hexasc
  .global time2string
  .global delay
  .global enable_interrupt
  .text
enable_interrupt:
    ei    # enable interrupts
    jr $ra    # return from function
hexasc:
  # add 0x31 if a0 < 10
  # add 0x37 to 0xa
  	andi	$a0,$a0,0xf	# remove unwanted bits

  	ble	$a0,9,numbers	# check if result is less than or equal to 9
  	nop			# delay slot filler (just in case)
  characters:
  	addi	$a0,$a0,0x37	# add 0x37 to represent characters
  	move	$v0,$a0		# move result to return register
  	jr	$ra		# return from function
  	nop
  numbers:
  	addi	$a0,$a0,0x30	# add 0x31 to represent numbers
  	move	$v0,$a0		# move result to return register
  	jr	$ra
  	nop
  # delay: wait a number of milliseconds
delay:
  	move	$t0,$a0		# int ms
  dwhl:				# dwhl: while loop
  	addi	$t0,$t0,-1	# ms--
  	addi	$t1,$0,0	# int i = 0
  dfor:				# dfor: for loop
  	addi	$t1,$t1,1	# i++
  	blt	$t1,105,dfor	# i < 101
  	nop
  	beqz	$t0,delayend
  	nop
  	j	dwhl
  	nop

  delayend:
  	jr	$ra
  	nop			# delay slot filler (just in case)

  # time2string: makes a string of character codes and stores it in the appropriate address
time2string:
  	# $a0 holds the adress of the display word
  	# $a1 holds the coded values of the numbers to be displayed

  	# use 'hexasc' to convert to characters

  	# PUSH and POP registers before and after calling 'hexasc' as if nothing was known beforehand about this subroutine

  	# store characters in ascii format somewhere in memory and then store to the adress in $a0 in order with appropriate offset
  	li	$t7,0xf000	# 0xf000 will sign extend, 0x0000f000 will not
  	and	$t0,$a1,$t7	# first number
  	srl	$t0,$t0,12
  	andi	$t1,$a1,0xf00	# second number
  	srl	$t1,$t1,8
  	li	$t6,0xff	# 0xff will mask the bits necessary for colon character
  	andi	$t2,$t6,0x3a	# colon
  	andi	$t3,$a1,0xf0	# third number
  	srl	$t3,$t3,4
  	andi	$t4,$a1,0xf	# fourth number
  	andi	$t5,$0,0	# NULL character

  	PUSH $a0
  	PUSH $ra
  	PUSH $t4
  	PUSH $t3
  	PUSH $t1

  	move	$a0,$t0		# convert first number to ascii
  	jal	hexasc
  	nop
  	move	$t0,$v0

  	POP $t1

  	move	$a0,$t1		# convert second number to ascii
  	jal	hexasc
  	nop
  	move	$t1,$v0

  	POP $t3

  	move	$a0,$t3		# convert third number to ascii
  	jal	hexasc
  	nop
  	move	$t3,$v0

  	POP $t4

  	move	$a0,$t4		# convert fourth number to ascii
  	jal	hexasc
  	nop
  	move	$t4,$v0

  	POP $ra
  	POP $a0

  	sb	$t0,0($a0)
  	sb	$t1,1($a0)
  	sb	$t2,2($a0)
  	sb	$t3,3($a0)
  	sb	$t4,4($a0)
  	sb	$t5,5($a0)

  	jr	$ra
  	nop
