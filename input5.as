; file ps.as

.entry LIST
.extern W1

MAIN:	add r3,LIST   


cmp r1 ,  r5     
cmp r1 , #5  
cmp TEst12[r10]  , #+5  
cmp TEst12[r10]  , #-5  
cmp r11  , TEst12  
 
add r1 ,  r5
sub  W1 , r2
mov  #12 , MAIN
sub  W1[r12] ,r2

lea TEst12[r10] , r7
lea MAIN,TEst12
lea TEst12[r10]   ,    r7
lea MAIN  ,  TEst12[r12]

.entry LOOP

clr r1  
not MAIN
inc MAIN[r12]
dec r5

jmp LOOP
bne LOOP[r12]
jsr MAIN
 

red r1  
red MAIN
red MAIN[r12]
LOOP:  red    r5

   prn #2341  
   prn MAIN
prn MAIN[r12]
  TEst12: prn r5


  rts

   END:   stop
STR1:          .string "abcd**************************"
LIST:       .data +6, -9
            .data -100
K1:          .data 31,+32,+56,-123
STR2:        .string "abcd***********43267fhdsjj9jwegyf6487wujdhgfue******"
