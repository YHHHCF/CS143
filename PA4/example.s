# start of generated code
	.data
	.align	2
	.globl	class_nameTab
	.globl	Main_protObj
	.globl	Int_protObj
	.globl	String_protObj
	.globl	bool_const0
	.globl	bool_const1
	.globl	_int_tag
	.globl	_bool_tag
	.globl	_string_tag
	.globl	_max_tag
	.globl	class_objTab
	.globl	class_parentTab
	.globl	class_attrTabTab
	.globl	E_protObj
	.globl	E_init
	.globl	E_attrTab
	.globl	D_protObj
	.globl	D_init
	.globl	D_attrTab
	.globl	C_protObj
	.globl	C_init
	.globl	C_attrTab
	.globl	B_protObj
	.globl	B_init
	.globl	B_attrTab
	.globl	A_protObj
	.globl	A_init
	.globl	A_attrTab
	.globl	Main_protObj
	.globl	Main_init
	.globl	Main_attrTab
	.globl	String_protObj
	.globl	String_init
	.globl	String_attrTab
	.globl	Bool_protObj
	.globl	Bool_init
	.globl	Bool_attrTab
	.globl	Int_protObj
	.globl	Int_init
	.globl	Int_attrTab
	.globl	IO_protObj
	.globl	IO_init
	.globl	IO_attrTab
	.globl	Object_protObj
	.globl	Object_init
	.globl	Object_attrTab
_int_tag:
	.word	2
_bool_tag:
	.word	3
_string_tag:
	.word	4
	.globl	_MemMgr_INITIALIZER
_MemMgr_INITIALIZER:
	.word	_NoGC_Init
	.globl	_MemMgr_COLLECTOR
_MemMgr_COLLECTOR:
	.word	_NoGC_Collect
	.globl	_MemMgr_TEST
_MemMgr_TEST:
	.word	0
	.word	-1
str_const17:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const0
	.byte	0	
	.align	2
	.word	-1
str_const16:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const1
	.ascii	"E"
	.byte	0	
	.align	2
	.word	-1
str_const15:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const1
	.ascii	"D"
	.byte	0	
	.align	2
	.word	-1
str_const14:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const1
	.ascii	"C"
	.byte	0	
	.align	2
	.word	-1
str_const13:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const1
	.ascii	"B"
	.byte	0	
	.align	2
	.word	-1
str_const12:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const1
	.ascii	"A"
	.byte	0	
	.align	2
	.word	-1
str_const11:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const4
	.ascii	"Main"
	.byte	0	
	.align	2
	.word	-1
str_const10:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const5
	.ascii	"String"
	.byte	0	
	.align	2
	.word	-1
str_const9:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const4
	.ascii	"Bool"
	.byte	0	
	.align	2
	.word	-1
str_const8:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const3
	.ascii	"Int"
	.byte	0	
	.align	2
	.word	-1
str_const7:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const2
	.ascii	"IO"
	.byte	0	
	.align	2
	.word	-1
str_const6:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const5
	.ascii	"Object"
	.byte	0	
	.align	2
	.word	-1
str_const2:
	.word	4
	.word	8
	.word	String_dispTab
	.word	int_const6
	.ascii	"<basic class>"
	.byte	0	
	.align	2
	.word	-1
str_const1:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const4
	.ascii	"test"
	.byte	0	
	.align	2
	.word	-1
str_const0:
	.word	4
	.word	7
	.word	String_dispTab
	.word	int_const7
	.ascii	"example.cl"
	.byte	0	
	.align	2
	.word	-1
int_const7:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	10
	.word	-1
int_const6:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	13
	.word	-1
int_const5:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	6
	.word	-1
int_const4:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	4
	.word	-1
int_const3:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	3
	.word	-1
int_const2:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	2
	.word	-1
int_const1:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	1
	.word	-1
int_const0:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	0
	.word	-1
bool_const0:
	.word	3
	.word	4
	.word	Bool_dispTab
	.word	0
	.word	-1
bool_const1:
	.word	3
	.word	4
	.word	Bool_dispTab
	.word	1
class_nameTab:
	.word	str_const6
	.word	str_const7
	.word	str_const8
	.word	str_const9
	.word	str_const10
	.word	str_const11
	.word	str_const12
	.word	str_const13
	.word	str_const14
	.word	str_const15
	.word	str_const16
class_objTab:
	.word	Object_protObj
	.word	Object_init
	.word	IO_protObj
	.word	IO_init
	.word	Int_protObj
	.word	Int_init
	.word	Bool_protObj
	.word	Bool_init
	.word	String_protObj
	.word	String_init
	.word	Main_protObj
	.word	Main_init
	.word	A_protObj
	.word	A_init
	.word	B_protObj
	.word	B_init
	.word	C_protObj
	.word	C_init
	.word	D_protObj
	.word	D_init
	.word	E_protObj
	.word	E_init
_max_tag:
	.word	10
class_parentTab:
	.word	-3
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	6
	.word	7
	.word	0
	.word	9
class_attrTabTab:
	.word	Object_attrTab
	.word	IO_attrTab
	.word	Int_attrTab
	.word	Bool_attrTab
	.word	String_attrTab
	.word	Main_attrTab
	.word	A_attrTab
	.word	B_attrTab
	.word	C_attrTab
	.word	D_attrTab
	.word	E_attrTab
Object_attrTab:
IO_attrTab:
Int_attrTab:
	.word	-1
Bool_attrTab:
	.word	-1
String_attrTab:
	.word	2
	.word	-1
Main_attrTab:
A_attrTab:
	.word	2
	.word	2
B_attrTab:
	.word	2
	.word	2
	.word	2
C_attrTab:
	.word	2
	.word	2
	.word	2
	.word	2
D_attrTab:
	.word	6
	.word	4
E_attrTab:
	.word	6
	.word	4
	.word	7
Object_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
IO_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	IO.out_string
	.word	IO.out_int
	.word	IO.in_string
	.word	IO.in_int
Int_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
Bool_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
String_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	String.length
	.word	String.concat
	.word	String.substr
Main_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	Main.main
A_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	A.a
B_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	A.a
C_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	A.a
D_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	D.mC
E_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	E.mC
	.word	-1
Object_protObj:
	.word	0
	.word	3
	.word	Object_dispTab
	.word	-1
IO_protObj:
	.word	1
	.word	3
	.word	IO_dispTab
	.word	-1
Int_protObj:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	0
	.word	-1
Bool_protObj:
	.word	3
	.word	4
	.word	Bool_dispTab
	.word	0
	.word	-1
String_protObj:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const0
	.word	0
	.word	-1
Main_protObj:
	.word	5
	.word	3
	.word	Main_dispTab
	.word	-1
A_protObj:
	.word	6
	.word	5
	.word	A_dispTab
	.word	int_const0
	.word	int_const0
	.word	-1
B_protObj:
	.word	7
	.word	6
	.word	B_dispTab
	.word	int_const0
	.word	int_const0
	.word	int_const0
	.word	-1
C_protObj:
	.word	8
	.word	7
	.word	C_dispTab
	.word	int_const0
	.word	int_const0
	.word	int_const0
	.word	int_const0
	.word	-1
D_protObj:
	.word	9
	.word	5
	.word	D_dispTab
	.word	0
	.word	str_const17
	.word	-1
E_protObj:
	.word	10
	.word	6
	.word	E_dispTab
	.word	0
	.word	str_const17
	.word	0
	.globl	heap_start
heap_start:
	.word	0
	.text
	.globl	Main_init
	.globl	Int_init
	.globl	String_init
	.globl	Bool_init
	.globl	Main.main
Object_init:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	sw	$a0 0($sp)
	addiu	$sp $sp -4
	move	$fp $sp
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	addiu	$sp $sp 4
	lw	$ra 0($sp)
	addiu	$sp $sp 4
	lw	$a0 0($sp)
	addiu	$sp $sp 4
	lw	$fp 0($sp)
	jr	$ra	
IO_init:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	sw	$a0 0($sp)
	addiu	$sp $sp -4
	move	$fp $sp
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	addiu	$sp $sp 4
	lw	$ra 0($sp)
	addiu	$sp $sp 4
	lw	$a0 0($sp)
	addiu	$sp $sp 4
	lw	$fp 0($sp)
	jr	$ra	
Int_init:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	sw	$a0 0($sp)
	addiu	$sp $sp -4
	move	$fp $sp
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	sw	$a0 12($s0)
	addiu	$sp $sp 4
	lw	$ra 0($sp)
	addiu	$sp $sp 4
	lw	$a0 0($sp)
	addiu	$sp $sp 4
	lw	$fp 0($sp)
	jr	$ra	
Bool_init:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	sw	$a0 0($sp)
	addiu	$sp $sp -4
	move	$fp $sp
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	sw	$a0 12($s0)
	addiu	$sp $sp 4
	lw	$ra 0($sp)
	addiu	$sp $sp 4
	lw	$a0 0($sp)
	addiu	$sp $sp 4
	lw	$fp 0($sp)
	jr	$ra	
String_init:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	sw	$a0 0($sp)
	addiu	$sp $sp -4
	move	$fp $sp
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	sw	$a0 12($s0)
	sw	$a0 16($s0)
	addiu	$sp $sp 4
	lw	$ra 0($sp)
	addiu	$sp $sp 4
	lw	$a0 0($sp)
	addiu	$sp $sp 4
	lw	$fp 0($sp)
	jr	$ra	
Main_init:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	sw	$a0 0($sp)
	addiu	$sp $sp -4
	move	$fp $sp
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	addiu	$sp $sp 4
	lw	$ra 0($sp)
	addiu	$sp $sp 4
	lw	$a0 0($sp)
	addiu	$sp $sp 4
	lw	$fp 0($sp)
	jr	$ra	
A_init:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	sw	$a0 0($sp)
	addiu	$sp $sp -4
	move	$fp $sp
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	la	$a0 int_const1
	sw	$a0 12($s0)
	la	$a0 int_const2
	sw	$a0 16($s0)
	addiu	$sp $sp 4
	lw	$ra 0($sp)
	addiu	$sp $sp 4
	lw	$a0 0($sp)
	addiu	$sp $sp 4
	lw	$fp 0($sp)
	jr	$ra	
B_init:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	sw	$a0 0($sp)
	addiu	$sp $sp -4
	move	$fp $sp
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	la	$a0 int_const1
	sw	$a0 12($s0)
	la	$a0 int_const2
	sw	$a0 16($s0)
	la	$a0 int_const3
	sw	$a0 20($s0)
	addiu	$sp $sp 4
	lw	$ra 0($sp)
	addiu	$sp $sp 4
	lw	$a0 0($sp)
	addiu	$sp $sp 4
	lw	$fp 0($sp)
	jr	$ra	
C_init:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	sw	$a0 0($sp)
	addiu	$sp $sp -4
	move	$fp $sp
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	la	$a0 int_const1
	sw	$a0 12($s0)
	la	$a0 int_const2
	sw	$a0 16($s0)
	la	$a0 int_const3
	sw	$a0 20($s0)
	la	$a0 int_const4
	sw	$a0 24($s0)
	addiu	$sp $sp 4
	lw	$ra 0($sp)
	addiu	$sp $sp 4
	lw	$a0 0($sp)
	addiu	$sp $sp 4
	lw	$fp 0($sp)
	jr	$ra	
D_init:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	sw	$a0 0($sp)
	addiu	$sp $sp -4
	move	$fp $sp
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	sw	$a0 12($s0)
	la	$a0 str_const1
	sw	$a0 16($s0)
	addiu	$sp $sp 4
	lw	$ra 0($sp)
	addiu	$sp $sp 4
	lw	$a0 0($sp)
	addiu	$sp $sp 4
	lw	$fp 0($sp)
	jr	$ra	
E_init:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	sw	$a0 0($sp)
	addiu	$sp $sp -4
	move	$fp $sp
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	sw	$a0 12($s0)
	la	$a0 str_const1
	sw	$a0 16($s0)
	sw	$a0 20($s0)
	addiu	$sp $sp 4
	lw	$ra 0($sp)
	addiu	$sp $sp 4
	lw	$a0 0($sp)
	addiu	$sp $sp 4
	lw	$fp 0($sp)
	jr	$ra	

# end of generated code
