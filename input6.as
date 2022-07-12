;example 2-this is a valid code in assembly.

macro do1
mov r4, List
sub List, r5
endm

macro do2
prn String
sub r11, S
endm

do1
jmp Ex
do1
jmp E
do2
lea Main, String
do2
lea List,Main[r13]
cmp S, Ex

;visibilitys:
.extern Ex
.entry E
.entry S
.extern out


; now setting al the labels
List: .data -6, 4, 82, 11
String: .string "string"
Stop: stop
Main: add r3, r5
S: lea out, List
E: cmp #5, Main[r11] 	
