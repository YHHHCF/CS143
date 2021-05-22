    .data
    .align  2
    .globl  class_nameTab
    .globl  Main_protObj
    .globl  Int_protObj
    .globl  String_protObj
    .globl  bool_const0
    .globl  bool_const1
    .globl  _int_tag
    .globl  _bool_tag
    .globl  _string_tag
    .globl  _max_tag
    .globl  class_objTab
    .globl  class_parentTab
    .globl  class_attrTabTab
    .globl  Object_protObj
    .globl  Object_init
    .globl  Object_attrTab
    .globl  C_protObj
    .globl  C_init
    .globl  C_attrTab
    .globl  D_protObj
    .globl  D_init
    .globl  D_attrTab
    .globl  A_protObj
    .globl  A_init
    .globl  A_attrTab
    .globl  B_protObj
    .globl  B_init
    .globl  B_attrTab
    .globl  Main_protObj
    .globl  Main_init
    .globl  Main_attrTab
    .globl  String_protObj
    .globl  String_init
    .globl  String_attrTab
    .globl  Bool_protObj
    .globl  Bool_init
    .globl  Bool_attrTab
    .globl  Int_protObj
    .globl  Int_init
    .globl  Int_attrTab
    .globl  IO_protObj
    .globl  IO_init
    .globl  IO_attrTab
_int_tag:
    .word   2
_bool_tag:
    .word   3
_string_tag:
    .word   4
    .globl  _MemMgr_INITIALIZER
_MemMgr_INITIALIZER:
    .word   _NoGC_Init
    .globl  _MemMgr_COLLECTOR
_MemMgr_COLLECTOR:
    .word   _NoGC_Collect
    .globl  _MemMgr_TEST
_MemMgr_TEST:
    .word   0
    .word   -1                // Garbage collection tag
str_const13:
    .word   4                 // String tag
    .word   5                 // Size in byte
    .word   String_dispTab    // String dispatch table
    .word   int_const0        // String length
    .byte   0                 // Handled by start code
    .align  2                 // Handled by start code
    .word   -1
str_const12:
    .word   4
    .word   5
    .word   String_dispTab
    .word   int_const2
    .ascii  "D"               // String value
    .byte   0   
    .align  2
    .word   -1
str_const11:
    .word   4
    .word   5
    .word   String_dispTab
    .word   int_const2
    .ascii  "C"
    .byte   0   
    .align  2
    .word   -1
str_const10:
    .word   4
    .word   5
    .word   String_dispTab
    .word   int_const2
    .ascii  "B"
    .byte   0   
    .align  2
    .word   -1
str_const9:
    .word   4
    .word   5
    .word   String_dispTab
    .word   int_const2
    .ascii  "A"
    .byte   0   
    .align  2
    .word   -1
str_const8:
    .word   4
    .word   6
    .word   String_dispTab
    .word   int_const3
    .ascii  "Main"
    .byte   0   
    .align  2
    .word   -1
str_const7:
    .word   4
    .word   6
    .word   String_dispTab
    .word   int_const4
    .ascii  "String"
    .byte   0   
    .align  2
    .word   -1
str_const6:
    .word   4
    .word   6
    .word   String_dispTab
    .word   int_const3
    .ascii  "Bool"
    .byte   0   
    .align  2
    .word   -1
str_const5:
    .word   4
    .word   5
    .word   String_dispTab
    .word   int_const1
    .ascii  "Int"
    .byte   0   
    .align  2
    .word   -1
str_const4:
    .word   4
    .word   5
    .word   String_dispTab
    .word   int_const5
    .ascii  "IO"
    .byte   0   
    .align  2
    .word   -1
str_const3:
    .word   4
    .word   6
    .word   String_dispTab
    .word   int_const4
    .ascii  "Object"
    .byte   0   
    .align  2
    .word   -1
str_const2:
    .word   4
    .word   8
    .word   String_dispTab
    .word   int_const6
    .ascii  "<basic class>"
    .byte   0   
    .align  2
    .word   -1
str_const1:
    .word   4
    .word   6
    .word   String_dispTab
    .word   int_const3
    .ascii  "test"
    .byte   0   
    .align  2
    .word   -1
str_const0:
    .word   4
    .word   7
    .word   String_dispTab
    .word   int_const7
    .ascii  "example.cl"
    .byte   0   
    .align  2
    .word   -1
int_const7:
    .word   2                 // Int tag
    .word   4                 // Size in byte
    .word   Int_dispTab       // Int dispatch table
    .word   10                // Int value
    .word   -1
int_const6:
    .word   2
    .word   4
    .word   Int_dispTab
    .word   13
    .word   -1
int_const5:
    .word   2
    .word   4
    .word   Int_dispTab
    .word   2
    .word   -1
int_const4:
    .word   2
    .word   4
    .word   Int_dispTab
    .word   6
    .word   -1
int_const3:
    .word   2
    .word   4
    .word   Int_dispTab
    .word   4
    .word   -1
int_const2:
    .word   2
    .word   4
    .word   Int_dispTab
    .word   1
    .word   -1
int_const1:
    .word   2
    .word   4
    .word   Int_dispTab
    .word   3
    .word   -1
int_const0:
    .word   2
    .word   4
    .word   Int_dispTab
    .word   0
    .word   -1
bool_const0:
    .word   3                 // Bool tag
    .word   4                 // Size in byte
    .word   Bool_dispTab      // Bool dispatch table
    .word   0                 // Bool value
    .word   -1
bool_const1:
    .word   3
    .word   4
    .word   Bool_dispTab
    .word   1
class_nameTab:
    .word   str_const3
    .word   str_const4
    .word   str_const5
    .word   str_const6
    .word   str_const7
    .word   str_const8
    .word   str_const9
    .word   str_const10
    .word   str_const11
    .word   str_const12
class_objTab:
    .word   Object_protObj
    .word   Object_init
    .word   IO_protObj
    .word   IO_init
    .word   Int_protObj
    .word   Int_init
    .word   Bool_protObj
    .word   Bool_init
    .word   String_protObj
    .word   String_init
    .word   Main_protObj
    .word   Main_init
    .word   A_protObj
    .word   A_init
    .word   B_protObj
    .word   B_init
    .word   C_protObj
    .word   C_init
    .word   D_protObj
    .word   D_init
_max_tag:
    .word   9
class_parentTab:
    .word   -2
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   6
    .word   0
    .word   8
class_attrTabTab:
    .word   Object_attrTab
    .word   IO_attrTab
    .word   Int_attrTab
    .word   Bool_attrTab
    .word   String_attrTab
    .word   Main_attrTab
    .word   A_attrTab
    .word   B_attrTab
    .word   C_attrTab
    .word   D_attrTab
Object_attrTab:
C_attrTab:
    .word   6
    .word   4
D_attrTab:
    .word   6
    .word   4
    .word   7
A_attrTab:
    .word   2
B_attrTab:
    .word   2
Main_attrTab:
String_attrTab:
    .word   2
    .word   -2
Bool_attrTab:
    .word   -2
Int_attrTab:
    .word   -2
IO_attrTab:
Object_dispTab:
    .word   Object.abort
    .word   Object.type_name
    .word   Object.copy
C_dispTab:
    .word   Object.abort
    .word   Object.type_name
    .word   Object.copy
    .word   C.mC
D_dispTab:
    .word   Object.abort
    .word   Object.type_name
    .word   Object.copy
    .word   D.mC
A_dispTab:
    .word   Object.abort
    .word   Object.type_name
    .word   Object.copy
B_dispTab:
    .word   Object.abort
    .word   Object.type_name
    .word   Object.copy
Main_dispTab:
    .word   Object.abort
    .word   Object.type_name
    .word   Object.copy
    .word   Main.main
String_dispTab:
    .word   Object.abort
    .word   Object.type_name
    .word   Object.copy
    .word   String.length
    .word   String.concat
    .word   String.substr
Bool_dispTab:
    .word   Object.abort
    .word   Object.type_name
    .word   Object.copy
Int_dispTab:
    .word   Object.abort
    .word   Object.type_name
    .word   Object.copy
IO_dispTab:
    .word   Object.abort
    .word   Object.type_name
    .word   Object.copy
    .word   IO.out_string
    .word   IO.out_int
    .word   IO.in_string
    .word   IO.in_int
    .word   -1
Object_protObj:
    .word   0
    .word   3
    .word   Object_dispTab
    .word   -1
C_protObj:
    .word   8
    .word   5
    .word   C_dispTab
    .word   0
    .word   str_const13
    .word   -1
D_protObj:
    .word   9
    .word   6
    .word   D_dispTab
    .word   0
    .word   str_const13
    .word   0
    .word   -1
A_protObj:
    .word   6
    .word   4
    .word   A_dispTab
    .word   int_const0
    .word   -1
B_protObj:
    .word   7
    .word   4
    .word   B_dispTab
    .word   int_const0
    .word   -1
Main_protObj:
    .word   5
    .word   3
    .word   Main_dispTab
    .word   -1
String_protObj:
    .word   4
    .word   5
    .word   String_dispTab
    .word   int_const0
    .word   0
    .word   -1
Bool_protObj:
    .word   3
    .word   4
    .word   Bool_dispTab
    .word   0
    .word   -1
Int_protObj:
    .word   2
    .word   4
    .word   Int_dispTab
    .word   0
    .word   -1
IO_protObj:
    .word   1
    .word   3
    .word   IO_dispTab
    .globl  heap_start
heap_start:
    .word   0
    .text
    .globl  Main_init
    .globl  Int_init
    .globl  String_init
    .globl  Bool_init
    .globl  Main.main
Object_init:
    addiu   $sp $sp -12
    sw  $fp 12($sp)
    sw  $s0 8($sp)
    sw  $ra 4($sp)
    addiu   $fp $sp 16
    move    $s0 $a0
    move    $a0 $s0
    lw  $fp 12($sp)
    lw  $s0 8($sp)
    lw  $ra 4($sp)
    addiu   $sp $sp 12
    jr  $ra 
C_init:
    addiu   $sp $sp -12
    sw  $fp 12($sp)
    sw  $s0 8($sp)
    sw  $ra 4($sp)
    addiu   $fp $sp 16
    move    $s0 $a0
    jal Object_init
    la  $a0 A_protObj
    jal Object.copy
    jal A_init
    sw  $a0 12($s0)
    la  $a0 str_const1
    sw  $a0 16($s0)
    move    $a0 $s0
    lw  $fp 12($sp)
    lw  $s0 8($sp)
    lw  $ra 4($sp)
    addiu   $sp $sp 12
    jr  $ra 
D_init:
    addiu   $sp $sp -12
    sw  $fp 12($sp)
    sw  $s0 8($sp)
    sw  $ra 4($sp)
    addiu   $fp $sp 16
    move    $s0 $a0
    jal C_init
    la  $a0 B_protObj
    jal Object.copy
    jal B_init
    sw  $a0 20($s0)
    move    $a0 $s0
    lw  $fp 12($sp)
    lw  $s0 8($sp)
    lw  $ra 4($sp)
    addiu   $sp $sp 12
    jr  $ra 
A_init:
    addiu   $sp $sp -12
    sw  $fp 12($sp)
    sw  $s0 8($sp)
    sw  $ra 4($sp)
    addiu   $fp $sp 16
    move    $s0 $a0
    jal Object_init
    la  $a0 int_const1
    sw  $a0 12($s0)
    move    $a0 $s0
    lw  $fp 12($sp)
    lw  $s0 8($sp)
    lw  $ra 4($sp)
    addiu   $sp $sp 12
    jr  $ra 
B_init:
    addiu   $sp $sp -12
    sw  $fp 12($sp)
    sw  $s0 8($sp)
    sw  $ra 4($sp)
    addiu   $fp $sp 16
    move    $s0 $a0
    jal A_init
    move    $a0 $s0
    lw  $fp 12($sp)
    lw  $s0 8($sp)
    lw  $ra 4($sp)
    addiu   $sp $sp 12
    jr  $ra 
Main_init:
    addiu   $sp $sp -12
    sw  $fp 12($sp)
    sw  $s0 8($sp)
    sw  $ra 4($sp)
    addiu   $fp $sp 16
    move    $s0 $a0
    jal Object_init
    move    $a0 $s0
    lw  $fp 12($sp)
    lw  $s0 8($sp)
    lw  $ra 4($sp)
    addiu   $sp $sp 12
    jr  $ra 
String_init:
    addiu   $sp $sp -12
    sw  $fp 12($sp)
    sw  $s0 8($sp)
    sw  $ra 4($sp)
    addiu   $fp $sp 16
    move    $s0 $a0
    jal Object_init
    move    $a0 $s0
    lw  $fp 12($sp)
    lw  $s0 8($sp)
    lw  $ra 4($sp)
    addiu   $sp $sp 12
    jr  $ra 
Bool_init:
    addiu   $sp $sp -12
    sw  $fp 12($sp)
    sw  $s0 8($sp)
    sw  $ra 4($sp)
    addiu   $fp $sp 16
    move    $s0 $a0
    jal Object_init
    move    $a0 $s0
    lw  $fp 12($sp)
    lw  $s0 8($sp)
    lw  $ra 4($sp)
    addiu   $sp $sp 12
    jr  $ra 
Int_init:
    addiu   $sp $sp -12
    sw  $fp 12($sp)
    sw  $s0 8($sp)
    sw  $ra 4($sp)
    addiu   $fp $sp 16
    move    $s0 $a0
    jal Object_init
    move    $a0 $s0
    lw  $fp 12($sp)
    lw  $s0 8($sp)
    lw  $ra 4($sp)
    addiu   $sp $sp 12
    jr  $ra 
IO_init:
    addiu   $sp $sp -12
    sw  $fp 12($sp)
    sw  $s0 8($sp)
    sw  $ra 4($sp)
    addiu   $fp $sp 16
    move    $s0 $a0
    jal Object_init
    move    $a0 $s0
    lw  $fp 12($sp)
    lw  $s0 8($sp)
    lw  $ra 4($sp)
    addiu   $sp $sp 12
    jr  $ra 
C.mC:
    addiu   $sp $sp -12
    sw  $fp 12($sp)
    sw  $s0 8($sp)
    sw  $ra 4($sp)
    addiu   $fp $sp 16
    move    $s0 $a0
    la  $a0 int_const0
    lw  $fp 12($sp)
    lw  $s0 8($sp)
    lw  $ra 4($sp)
    addiu   $sp $sp 12
    jr  $ra 
D.mC:
    addiu   $sp $sp -12
    sw  $fp 12($sp)
    sw  $s0 8($sp)
    sw  $ra 4($sp)
    addiu   $fp $sp 16
    move    $s0 $a0
    la  $a0 int_const2
    lw  $fp 12($sp)
    lw  $s0 8($sp)
    lw  $ra 4($sp)
    addiu   $sp $sp 12
    jr  $ra 
Main.main:
    addiu   $sp $sp -12
    sw  $fp 12($sp)
    sw  $s0 8($sp)
    sw  $ra 4($sp)
    addiu   $fp $sp 16
    move    $s0 $a0
    la  $a0 int_const0
    lw  $fp 12($sp)
    lw  $s0 8($sp)
    lw  $ra 4($sp)
    addiu   $sp $sp 12
    jr  $ra 
