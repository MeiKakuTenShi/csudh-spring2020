.data
min:            .word 0
max:            .word 0
avg:            .float 0.0

count_query:    .asciiz "Number of elements: "
val_query:      .asciiz "Next item: "

output:         .asciiz "Array Details"
output_max:     .asciiz "Maximum: "
output_min:     .asciiz "Minimum: "
output_avg:     .asciiz "Average: "
output_sort:    .asciiz "Sorted (Descending): "

item_sep:       .asciiz ", "
tab_line:       .asciiz "\t"
new_line:       .asciiz "\n"

.text

.globl main

main:
# ASK HOW MANY ELEMENTS IN ARRAY
li      $v0, 4
la      $a0, count_query
syscall

# GET INT
li      $v0, 5
syscall
move    $t0, $v0

# ALLOCATE SPACE
li      $v0, 9
sll     $a0, $t0, 2
syscall
move    $t7, $v0               # address of array memory

# GET VALUES FROM USER
move    $t1, $t7               # array index address
li      $t2, 0                 # counter

query_loop:
# CONDITION TO TERMINATE LOOP
slt     $t6, $t2, $t0
beq     $t6, $0,  end_query

li      $v0, 4
la      $a0, val_query
syscall
li      $v0, 5
syscall
sw      $v0, ($t1)

addiu	$t1, $t1, 4             # advance $t1 by 4
addiu   $t2, $t2, 1             # i++

j       query_loop
end_query:

li      $v0, 4
la      $a0, new_line
syscall
la      $a0, output
syscall
la      $a0, new_line
syscall
la      $a0, tab_line
syscall

# PRINT ARRAY
# FIND MIN, MAX, AVG
move    $t1, $t7               # array index address
li      $t2, 0                 # counter
li      $t5, 0                 # sum
print_loop:
# CONDITION TO TERMINATE LOOP
slt     $t6, $t2, $t0
beq     $t6, $0,  end_print

lw      $t3, ($t1)
li      $v0, 1
addu    $a0, $0, $t3
syscall

lw      $t4, min
slt     $t6, $t3, $t4
movn    $t4, $t3, $t6
sw      $t4, min

lw      $t4, max
slt     $t6, $t3, $t4
movz    $t4, $t3, $t6
sw      $t4, max

add     $t5, $t5, $t3

addiu   $t4, $t2, 1
slt     $t6, $t4, $t0
beq     $t6, $0,  last_item

li      $v0, 4
la      $a0, item_sep
syscall

last_item:

addiu	$t1, $t1, 4             # advance $t1 by 4
addiu   $t2, $t2, 1             # i++

j       print_loop
end_print:

# PRINT MINIMUM
li      $v0, 4
la      $a0, new_line
syscall
la      $a0, tab_line
syscall
la      $a0, output_min
syscall

li      $v0, 1
lw      $a0, min
syscall

# PRINT MAXIMUM
li      $v0, 4
la      $a0, new_line
syscall
la      $a0, tab_line
syscall
la      $a0, output_max
syscall

li      $v0, 1
lw      $a0, max
syscall

# PRINT AVERAGE
mtc1    $t5, $f2
mtc1    $t0, $f3
div.s   $f2, $f2, $f3

li      $v0, 4
la      $a0, new_line
syscall
la      $a0, tab_line
syscall
la      $a0, output_avg
syscall

li      $v0, 2
mov.s   $f12, $f2
syscall


# SORT ARRAY
li      $v0, 4
la      $a0, new_line
syscall
la      $a0, tab_line
syscall
la      $a0, output_sort
syscall

li      $t2, 0				# i : outer loop

print_sort:
slt     $t6, $t2, $t0
beq     $t6, $0,  end_sort

move    $t1, $t7
li      $t5, 0				# j : inner loop

lw      $t4, ($t1)

find_max:
slt     $t6, $t5, $t0
beq     $t6, $0,  max_done

lw      $t3, ($t1)

slt     $t6, $t4, $t3
movn    $t4, $t1, $t6

addiu	$t1, $t1, 4
addiu   $t5, $t5, 1

j       find_max
max_done:

li      $v0, 1
lw      $a0, ($t4)
syscall

addiu   $a1, $t2, 1
slt     $t6, $a1, $t0
beq     $t6, $0,  no_sep

li      $v0, 4
la      $a0, item_sep
syscall

no_sep:
        
li      $t6, -999
sw      $t6, ($t4)

addiu   $t2, $t2, 1
j       print_sort
end_sort:


# CALL EXIT
li	$v0, 10
syscall