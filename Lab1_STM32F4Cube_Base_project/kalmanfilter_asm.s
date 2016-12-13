	AREA text, CODE, READONLY
	EXPORT kal
	IMPORT testbench
kal
	
				;KALMAN FILTERING SUBROUTINE
				
				;Passing parameters to subroutine
				;R0 - pointer to input (zk)
				;R1 - pointer to output (xk|k)
				;R2 - array's length
				;R3 - pointer to kalman states
				;R4 - loop counter
				;R5 - offset register for loop
				PUSH {R4-R12} ;load pointers
				MOV R4, #0 ;initialize loop couner register 
				
				;Holding values of kalman State
				VLDR.f32 S4, [R3] ;f
				VLDR.f32 S5, [R3, #4] ;h
				VLDR.f32 S6, [R3, #8] ;q
				VLDR.f32 S7, [R3, #12] ;r
				VLDR.f32 S8, [R3, #16] ;k
				VLDR.f32 S9, [R3, #20] ;x (initial k-1)
				VLDR.f32 S10, [R3, #24] ;p (initial k-1)
	
				;Loop iteration for process:
Calc		
				VLDR.f32 S11, [R0]
	
				;Calculate Pk|k-1 and put it in S10.
				VMUL.f32 S10, S4, S10 ; F*Pk-1|k-1
				VMUL.f32 S10, S10, S4 ; F*Pk-1|k-1*Ft
				VADD.f32 S10, S10, S6 ; F*Pk-1|k-1*Ft + Q
			
				;Calculate K and put in S8.
				VMUL.f32 S8, S10, S5 ; Pk|k-1*Ht
				VMUL.f32 S12, S8, S5 ; H*Pk|k-1*Ht
				VADD.f32 S12, S12, S7 ; H*Pk|k-1*Ht + R
				VCMP.f32 S12, #0.0 ;check if divisor is zero
				IT EQ 
				VLDREQ.f32 S12, =0.01 ;set to 0.01 to avoid division by 0
				VDIV.f32 S8, S8, S12 ; (Pk|k-1*Ht)/(H*Pk|k-1*Ht + R)
				VSTR.f32 S8, [R3, #16] ;push to kalman state vector
			
				;Calculate Pk|k and put in S10.
				VLDR.f32 S12, =1.0 ; I
				VMLS.f32 S12, S8, S5 ; I-KH
				VMUL.f32 S10, S10, S12 ; (I-KH)*Pk|k-1
				VSTR.f32 S10, [R3, #24] ;push to kalman state vector 
			
				;Caculate xk|k.
				VMUL.f32 S9, S4, S9 ; F*xk-1|k-1
				VMOV.f32 S12, S11 ; Load zk
				VMLS.f32 S12, S5, S9 ; zk - H*xk|k-1
				VMLA.f32 S9, S8, S12 ; xk|k-1 + K(zk - H*xk|k-1)
				VSTR.f32 S9, [R1] ;push to output array
				VSTR.f32 S9, [R3, #20] ;push to kalman state vector
			
				;Housekeeping.
				ADD R0, R0, #4  ; increment input counter
				ADD R1, R1, #4  ; increment output counter
				ADDS R4, R4, #1 ; incrememnt loop counter
				CMP R4, R2 			; compare counter to array length
				BNE Calc ; loop if not at end of array
				;LOOP END

				POP {R4-R12}
				BX LR
				END