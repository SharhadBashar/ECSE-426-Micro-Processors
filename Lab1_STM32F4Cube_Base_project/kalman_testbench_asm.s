	AREA text, CODE, READONLY
	EXPORT testbench
	IMPORT inpoint
	IMPORT outpoint
	IMPORT alength
	IMPORT kstate
	IMPORT kal
	
testbench
	
	;KALMAN FILTER TESTBENCH
	
	;load memory areas into a ppopriate registers
	LDR R0, =inpoint
	LDR R1, =outpoint
	LDR R2, =alength
	LDR	R3, =kstate
	
	VLDR.f32 S4, =0.1
	VLDR.f32 S5, =2.2
	VLDR.f32 S6, =-0.1
	VLDR.f32 S7, =3.5
	VLDR.f32 S8, =4.0
	VLDR.f32 S9, =4.1
	VLDR.f32 S10, =9.9
	VLDR.f32 S11, =0.0
	VLDR.f32 S12, =0.0
	VLDR.f32 S13, =2.3
	VSTM R0, {S4-S13}
	
	MOV R4, #10
	STR R4, [R2]
	LDR R2, [R2]

	VLDR.f32 S4, =0.80
	VLDR.f32 S5, =1.20
	VLDR.f32 S6, =0.10
	VLDR.f32 S7, =0.10
	VLDR.f32 S8, =0.00
	VLDR.f32 S9, =0.0
	VLDR.f32 S10, =0.10
	VSTM R3, {S4-S10}
	
	;load program and execute 
	LDR R4, =kal
	BX R4
	END