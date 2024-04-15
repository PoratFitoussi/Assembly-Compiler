	.entry bGT
	.extern Add
HEAD:	add $0,$1,$2
	sub $3,$4,$5
	and $6,$7,$8
	or $9,$10,$11
	nor $12,$13,$14
HEad:	move $15,$16
	mvhi $17,$18
	mvlo $19,$20
	addi $20,-5555,$21
	subi $11,+6666,$23
	ori 		$24,7777    		,			$25
	; T e S		T
				
		; test
	; TEST
nori $26,223,$27
beq 	$28,$29		,	HEAD
		bne	$31,$31,HEad
bLt: blt	$14,$0,bGT

NEW: .asciz 	"ABCDEFG"
		.asciz "abcdfg"
 

New: lb $28,572,$1
   bGT: sb $28,572,$1
    lw $28,+527,$1
    sw $28,-527,$1
    lh $28,+572,$1
    sh $28,-572,$1
NEW1: jmp $5
jmp Add
.db -5,-50,-500
.db 5,50,500
.dh +5,+50,+500
la bGT
.dh 324
call New
.dw 500000
stop

