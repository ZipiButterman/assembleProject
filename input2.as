; addressing_modes_working.as

MAIN:	clr r10
		clr K
		clr K[r10]

		prn #5
		prn K
		prn K[r10]
		prn r10
		
		red K
		red K[r10]
		red r0
		
		jsr SUB
		jsr SUB[r10]
		
		bne END
		bne END[r10]
		
		jmp END
		jmp END[r10]
		
		dec K
		dec K[r10]
		dec r0
		
		inc K
		inc K[r10]
		inc r0
		
		not K
		not K[r10]
		not r0
.entry END		
		mov #5, K
		mov #5, K[r10]
		mov #5, r0
		mov K, K
		mov K, K[r10]
		mov K, r0
		mov K[r10], K
		mov K[r10], K[r10]
		mov K[r10], r0
		mov r0, K
		mov r0, K[r10]
		mov r0, r0

		cmp #5, #4
		cmp #5, K
		cmp #5, K[r10]
		cmp #5, r0
		cmp K, #4
		cmp K, K
		cmp K, K[r10]
		cmp K, r0
		cmp K[r10], #4
		cmp K[r10], K
		cmp K[r10], K[r10]
		cmp K[r10], r0
		cmp r0, #4
		cmp r0, K
		cmp r0, K[r10]
		cmp r0, r0
		
		add #5, K
		add #5, K[r10]
		add #5, r0
		add K, K
		add K, K[r10]
		add K, r0
		add K[r10], K
		add K[r10], K[r10]
		add K[r10], r0
		add r0, K
		add r0, K[r10]
		add r0, r0

		sub #5, K
		sub #5, K[r10]
		sub #5, r0
		sub K, K
		sub K, K[r10]
		sub K, r0
		sub K[r10], K
		sub K[r10], K[r10]
		sub K[r10], r0
		sub r0, K
		sub r0, K[r10]
		sub r0, r0

		lea K, K
		lea K, K[r10]
		lea K, r0
		lea K[r10], K
		lea K[r10], K[r10]
		lea K[r10], r0

SUB:	rts
END: 	stop
K: 		.data 31
