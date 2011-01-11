/**************************************************

X6502 cpu - ARM port

Ported by : Luke Arntson (Cthulhu32)
Original Credit: FCEUX authors and other various
                  great programmers.
Modified Date: 1/8/2011
Create Date: 1/8/2011

***************************************************/

REG_A .req r4
REG_X .req r5
REG_Y .req r6
REG_PC .req r7
REG_S .req r8
REG_P .req r9
REG_DB .req r10
REG_COUNT .req r11
REG_OP_TABLE .req r12

.equ N_FLAG,  0x80
.equ V_FLAG,  0x40
.equ U_FLAG,  0x20
.equ B_FLAG,  0x10
.equ D_FLAG,  0x08
.equ I_FLAG,  0x04
.equ Z_FLAG,  0x02
.equ C_FLAG,  0x01

.equ FCEU_IQRESET,  0x020
.equ FCEU_IQNMI2,  	0x040   /* Delayed NMI, gets converted to *_IQNMI */
.equ FCEU_IQNMI,  	0x080
.equ FCEU_IQDPCM,   0x100
.equ FCEU_IQFCOUNT, 0x200
.equ FCEU_IQTEMP,   0x800

.text
.align 4

.macro ADDCYC X
	ldr r3, =timestamp
	ldr r2, =tcount
	ldr r1, [r3]
	mov r0, #(\X)
	add r1, r1, r0
	str r1, [r3]
	ldr r1, [r2]
	add r1, r1, r0
	str r1, [r2]
	mov r1, #48
	mul r0, r0, r1
	sub REG_COUNT, REG_COUNT, r0
.endm

.macro RdMem A
.endm

.macro WrMem A, V
.endm

.macro RdRAM A
.endm

.macro WrRAM A, V
.endm

.macro PUSH V
	WrRAM(#0x100+REG_S,\V)
	sub REG_S, REG_S, #1
.endm

.macro POP
	add REG_S, REG_S, #1
	RdRAM(#0x100+REG_S)
.endm

.macro X_ZN zort
.endm

.macro X_ZNT zort
.endm

.macro JN cond
.endm

.macro LDA x
	mov REG_A, \x
	X_ZN(REG_A)
.endm

.macro LDX x
	mov REG_X, \x
	X_ZN(REG_X)
.endm

.macro LDY x
	mov REG_Y, \x
	X_ZN(REG_Y)
.endm

.macro LDA_LDX x
	LDA(\x)
	LDX(\x)
.endm

.macro AND x
	and REG_A, REG_A, \x
	X_ZN(REG_A)
.endm

.macro BIT
.endm

.macro EOR x
	eor REG_A, REG_A, \x
	X_ZN(REG_A)
.endm

.macro ORA x
	orr REG_A, REG_A, \x
	X_ZN(REG_A)
.endm

.macro ADC
.endm

.macro SBC
.endm

.macro CMPL a1, a2
.endm

.macro AXS
.endm

.macro CMP x
	CMPL(REG_A, \x)
.endm

.macro CPX x
	CMPL(REG_X, \x)
.endm

.macro CPY x
	CMPL(REG_Y, \x)
.endm

.macro DEC x
	sub \x, \x, #1
	X_ZN(\x)
.endm

.macro DEC_CMP
.endm

.macro INC x
	add \x, \x, #1
	X_ZN(\x)
.endm

.macro INC_SBC
.endm

.macro ASL
.endm

.macro ASL_ORA
.endm

.macro LSR
.endm

.macro LSR_EOR
.endm

.macro LSRA
.endm

.macro ROL
.endm

.macro ROL_AND
.endm

.macro ROR
.endm

.macro ROR_ADC
.endm

.macro GetAB target
.endm

.macro GetABIRD target, i
.endm

.macro GetABIWR target, i
.endm

.macro GetZP target
.endm

.macro GetZPI target, i
.endm

.macro GetIX target
.endm

.macro GetIYRD target
.endm

.macro GetIYWR target
.endm

.macro RMW_A op
.endm

.macro RMW_AB op
.endm

.macro RMW_ABI op
.endm

.macro RMW_ABX op
.endm

.macro RMW_ABY op
.endm

.macro RMW_IX op
.endm

.macro RMW_IY op
.endm

.macro RMW_ZP op
.endm

.macro RMW_ZPX op
.endm

.macro LD_IM op
.endm

.macro LD_ZP op
.endm

.macro LD_ZPX op
.endm

.macro LD_ZPY op
.endm

.macro LD_AB op
.endm

.macro LD_AB_NOP
.endm

.macro LD_ABI reg, op
.endm

.macro LD_ABX op
.endm

.macro LD_ABX_NOP
.endm

.macro LD_ABY op
.endm

.macro LD_IX op
.endm

.macro LD_IY op
.endm

.macro ST_ZP r
.endm

.macro ST_ZPX r
.endm

.macro ST_ZPY r
.endm

.macro ST_AB r
.endm

.macro ST_ABI reg, r
.endm

.macro ST_ABX r
.endm

.macro ST_ABY r
.endm

.macro ST_IX r
.endm

.macro ST_IY r
.endm

op00:	/* BRK */
		add REG_PC, REG_PC, #1
		PUSH(REG_PC>>8)
		PUSH(REG_PC)
	
op40:	/* RTI */

op60:	/* RTS */

op48:	/* PHA */
		PUSH(REG_A)

op08:	/* PHP */

op68:	/* PLA */

op28:	/* PLP*/
		POP

op4C:	/* JMP ABSOLUTE */

op6C:	/* JMP SPECIFIC */

op20:	/* JSR */

opAA:	/* TAX */
		mov REG_X, REG_A
		X_ZN(REG_A)

op8A:	/* TXA */
		mov REG_A, REG_X
		X_ZN(REG_A)

opA8:	/* TAY */
		mov REG_Y, REG_A
		X_ZN(REG_A)

op98:	/* TYA */
		mov REG_A, REG_Y
		X_ZN(REG_A)

opBA:	/* TSX */
		mov REG_X, REG_S
		X_ZN(REG_X)

op9A:	/* TXS */
		mov REG_S, REG_X

opCA:	/* DEX */
		sub REG_X, REG_X, #1
		X_ZN(REG_X)

op88:	/* DEY */
		sub REG_Y, REG_Y, #1
		X_ZN(REG_Y)

opE8:	/* INX */
		add REG_X, REG_X, #1
		X_ZN(REG_X)

opC8:	/* INY */
		add REG_Y, REG_Y, #1
		X_ZN(REG_Y)

op18:	/* CLC */

opD8:	/* CLD */

op58: 	/* CLI */

opB8:	/* CLV */

op38:	/* SEC */

opF8:	/* SED */

op78:	/* SEI */

opEA:	/* NOP */

/* ASL */
op0A: RMW_A(ASL);
op06: RMW_ZP(ASL);
op16: RMW_ZPX(ASL);
op0E: RMW_AB(ASL);
op1E: RMW_ABX(ASL);

/* DEC */
opC6: RMW_ZP(DEC);
opD6: RMW_ZPX(DEC);
opCE: RMW_AB(DEC);
opDE: RMW_ABX(DEC);

/* INC */
opE6: RMW_ZP(INC);
opF6: RMW_ZPX(INC);
opEE: RMW_AB(INC);
opFE: RMW_ABX(INC);

/* LSR */
op4A: RMW_A(LSR);
op46: RMW_ZP(LSR);
op56: RMW_ZPX(LSR);
op4E: RMW_AB(LSR);
op5E: RMW_ABX(LSR);

/* ROL */
op2A: RMW_A(ROL);
op26: RMW_ZP(ROL);
op36: RMW_ZPX(ROL);
op2E: RMW_AB(ROL);
op3E: RMW_ABX(ROL);

/* ROR */
op6A: RMW_A(ROR);
op66: RMW_ZP(ROR);
op76: RMW_ZPX(ROR);
op6E: RMW_AB(ROR);
op7E: RMW_ABX(ROR);

/* ADC */
op69: LD_IM(ADC);
op65: LD_ZP(ADC);
op75: LD_ZPX(ADC);
op6D: LD_AB(ADC);
op7D: LD_ABX(ADC);
op79: LD_ABY(ADC);
op61: LD_IX(ADC);
op71: LD_IY(ADC);

/* AND */
op29: LD_IM(AND);
op25: LD_ZP(AND);
op35: LD_ZPX(AND);
op2D: LD_AB(AND);
op3D: LD_ABX(AND);
op39: LD_ABY(AND);
op21: LD_IX(AND);
op31: LD_IY(AND);

/* BIT */
op24: LD_ZP(BIT);
op2C: LD_AB(BIT);

/* CMP */
opC9: LD_IM(CMP);
opC5: LD_ZP(CMP);
opD5: LD_ZPX(CMP);
opCD: LD_AB(CMP);
opDD: LD_ABX(CMP);
opD9: LD_ABY(CMP);
opC1: LD_IX(CMP);
opD1: LD_IY(CMP);

/* CPX */
opE0: LD_IM(CPX);
opE4: LD_ZP(CPX);
opEC: LD_AB(CPX);

/* CPY */
opC0: LD_IM(CPY);
opC4: LD_ZP(CPY);
opCC: LD_AB(CPY);

/* EOR */
op49: LD_IM(EOR);
op45: LD_ZP(EOR);
op55: LD_ZPX(EOR);
op4D: LD_AB(EOR);
op5D: LD_ABX(EOR);
op59: LD_ABY(EOR);
op41: LD_IX(EOR);
op51: LD_IY(EOR);

/* LDA */
opA9: LD_IM(LDA);
opA5: LD_ZP(LDA);
opB5: LD_ZPX(LDA);
opAD: LD_AB(LDA);
opBD: LD_ABX(LDA);
opB9: LD_ABY(LDA);
opA1: LD_IX(LDA);
opB1: LD_IY(LDA);

/* LDX */
opA2: LD_IM(LDX);
opA6: LD_ZP(LDX);
opB6: LD_ZPY(LDX);
opAE: LD_AB(LDX);
opBE: LD_ABY(LDX);

/* LDY */
opA0: LD_IM(LDY);
opA4: LD_ZP(LDY);
opB4: LD_ZPX(LDY);
opAC: LD_AB(LDY);
opBC: LD_ABX(LDY);

/* ORA */
op09: LD_IM(ORA);
op05: LD_ZP(ORA);
op15: LD_ZPX(ORA);
op0D: LD_AB(ORA);
op1D: LD_ABX(ORA);
op19: LD_ABY(ORA);
op01: LD_IX(ORA);
op11: LD_IY(ORA);

/* SBC */
opEB:  /* (undocumented) */
opE9: LD_IM(SBC);
opE5: LD_ZP(SBC);
opF5: LD_ZPX(SBC);
opED: LD_AB(SBC);
opFD: LD_ABX(SBC);
opF9: LD_ABY(SBC);
opE1: LD_IX(SBC);
opF1: LD_IY(SBC);

/* STA */
op85: ST_ZP(_A);
op95: ST_ZPX(_A);
op8D: ST_AB(_A);
op9D: ST_ABX(_A);
op99: ST_ABY(_A);
op81: ST_IX(_A);
op91: ST_IY(_A);

/* STX */
op86: ST_ZP(_X);
op96: ST_ZPY(_X);
op8E: ST_AB(_X);

/* STY */
op84: ST_ZP(_Y);
op94: ST_ZPX(_Y);
op8C: ST_AB(_Y);

op90:	/* BCC */

opB0:	/* BCS */

opF0:	/* BEQ */

opD0:	/* BNE */

op30:	/* BMI */

op10:	/* BPL */

op50:	/* BVC */

op70:	/* BVS */

/* Undocumented */
op2B:
op0B:	/* AAC */

/* AAX */
op87: ST_ZP(_A&_X);
op97: ST_ZPY(_A&_X);
op8F: ST_AB(_A&_X);
op83: ST_IX(_A&_X);

op6B:	/* ARR! */

op4B:	/* ASR */

opAB:	/* ATX */

opCB:	/* AXS */

/* DCP */
opC7: RMW_ZP(DEC_CMP);
opD7: RMW_ZPX(DEC_CMP);
opCF: RMW_AB(DEC_CMP);
opDF: RMW_ABX(DEC_CMP);
opDB: RMW_ABY(DEC_CMP);
opC3: RMW_IX(DEC_CMP);
opD3: RMW_IY(DEC_CMP);

/* ISB */
opE7: RMW_ZP(INC_SBC);
opF7: RMW_ZPX(INC_SBC);
opEF: RMW_AB(INC_SBC);
opFF: RMW_ABX(INC_SBC);
opFB: RMW_ABY(INC_SBC);
opE3: RMW_IX(INC_SBC);
opF3: RMW_IY(INC_SBC);

/* DOP */

op04: add REG_PC, REG_PC, #1
op14: add REG_PC, REG_PC, #1
op34: add REG_PC, REG_PC, #1
op44: add REG_PC, REG_PC, #1
op54: add REG_PC, REG_PC, #1
op64: add REG_PC, REG_PC, #1
op74: add REG_PC, REG_PC, #1

op80: add REG_PC, REG_PC, #1
op82: add REG_PC, REG_PC, #1
op89: add REG_PC, REG_PC, #1
opC2: add REG_PC, REG_PC, #1
opD4: add REG_PC, REG_PC, #1
opE2: add REG_PC, REG_PC, #1
opF4: add REG_PC, REG_PC, #1

/* KIL */
op02:
op12:
op22:
op32:
op42:
op52:
op62:
op72:
op92:
opB2:
opD2:
opF2:
	ADDCYC(0xFF)
	ldr r1, =jammed
	mov r0, #1
	str r0, [r1]
	sub REG_PC, REG_PC, #1

opBB:	/* LAR */

/* LAX */
opA7: LD_ZP(LDA_LDX);
opB7: LD_ZPY(LDA_LDX);
opAF: LD_AB(LDA_LDX);
opBF: LD_ABY(LDA_LDX);
opA3: LD_IX(LDA_LDX);
opB3: LD_IY(LDA_LDX);

/* NOP */
op1A:
op3A:
op5A:
op7A:
opDA:
opFA:

/* RLA */
op27: RMW_ZP(ROL_AND);
op37: RMW_ZPX(ROL_AND);
op2F: RMW_AB(ROL_AND);
op3F: RMW_ABX(ROL_AND);
op3B: RMW_ABY(ROL_AND);
op23: RMW_IX(ROL_AND);
op33: RMW_IY(ROL_AND);

/* RRA */
op67: RMW_ZP(ROR_ADC);
op77: RMW_ZPX(ROR_ADC);
op6F: RMW_AB(ROR_ADC);
op7F: RMW_ABX(ROR_ADC);
op7B: RMW_ABY(ROR_ADC);
op63: RMW_IX(ROR_ADC);
op73: RMW_IY(ROR_ADC);

/* SLO */
op07: RMW_ZP(ASL_ORA);
op17: RMW_ZPX(ASL_ORA);
op0F: RMW_AB(ASL_ORA);
op1F: RMW_ABX(ASL_ORA);
op1B: RMW_ABY(ASL_ORA);
op03: RMW_IX(ASL_ORA);
op13: RMW_IY(ASL_ORA);

/* SRE */
op47: RMW_ZP(LSR_EOR);
op57: RMW_ZPX(LSR_EOR);
op4F: RMW_AB(LSR_EOR);
op5F: RMW_ABX(LSR_EOR);
op5B: RMW_ABY(LSR_EOR);
op43: RMW_IX(LSR_EOR);
op53: RMW_IY(LSR_EOR);

/* AXA - SHA */
op93: /*ST_IY(REG_A&REG_X&(((REG_A-REG_Y)>>8)+1));*/
op9F: /*ST_ABY(REG_A&REG_X&(((REG_A-REG_Y)>>8)+1));*/

/* SYA */
op9C: /*ST_ABX(REG_Y&(((REG_A-_X)>>8)+1));*/

/* SXA */
op9E: /*ST_ABY(REG_X&(((REG_A-REG_Y)>>8)+1));*/

/* XAS */
op9B: 
	/*and		REG_S, REG_A, REG_X
	ST_ABY(REG_S& (((REG_A-REG_Y)>>8)+1))*/

/* TOP */
op0C: LD_AB_NOP

op1C: 
op3C: 
op5C: 
op7C: 
opDC: 
opFC: LD_ABX_NOP

/* XAA - BIG QUESTION MARK HERE */
op8B:

/* Pass in count argument (r0?) */
X6502_Run_a:
	stmfd   sp!, {r4-r5}

X6502_Init_a:
	
	
/* Interface with our C/C++ code
	Note: This code MUST be in an extern "C" loop in order to link
	with C++ properly.. */
 
.data
.align 4

/* Variables */
.globl nes_registers @ pass between C and ASM

.globl X6502_Run_a @ (int c);
.globl X6502_Init_a @ (void);
.globl X6502_Reset_a @ (void);
.globl X6502_Power_a @ (void);
.globl TriggerNMI_a @ (void);
.globl TriggerNMI2_a @ (void);

.globl X6502_DMR_a @ (uint32 A);
.globl X6502_DMW_a @ (uint32 A, uint8 V);

.globl X6502_IRQBegin_a @ (int w);
.globl X6502_IRQEnd_a @ (int w);

CycTable:
	.byte 7,6,2,8,3,3,5,5,3,2,2,2,4,4,6,6
	.byte 2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7
	.byte 6,6,2,8,3,3,5,5,4,2,2,2,4,4,6,6
	.byte 2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7
	.byte 6,6,2,8,3,3,5,5,3,2,2,2,3,4,6,6
	.byte 2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7
	.byte 6,6,2,8,3,3,5,5,4,2,2,2,5,4,6,6
	.byte 2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7
	.byte 2,6,2,6,3,3,3,3,2,2,2,2,4,4,4,4
	.byte 2,6,2,6,4,4,4,4,2,5,2,5,5,5,5,5
	.byte 2,6,2,6,3,3,3,3,2,2,2,2,4,4,4,4
	.byte 2,5,2,5,4,4,4,4,2,4,2,4,4,4,4,4
	.byte 2,6,2,8,3,3,5,5,2,2,2,2,4,4,6,6
	.byte 2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7
	.byte 2,6,3,8,3,3,5,5,2,2,2,2,4,4,6,6
	.byte 2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7
	
opsize:
	.byte 1,2,0,0,0,2,2,0,1,2,1,0,0,3,3,0
	.byte 2,2,0,0,0,2,2,0,1,3,0,0,0,3,3,0
	.byte 3,2,0,0,2,2,2,0,1,2,1,0,3,3,3,0
	.byte 2,2,0,0,0,2,2,0,1,3,0,0,0,3,3,0
	.byte 1,2,0,0,0,2,2,0,1,2,1,0,3,3,3,0
	.byte 2,2,0,0,0,2,2,0,1,3,0,0,0,3,3,0
	.byte 1,2,0,0,0,2,2,0,1,2,1,0,3,3,3,0
	.byte 2,2,0,0,0,2,2,0,1,3,0,0,0,3,3,0
	.byte 0,2,0,0,2,2,2,0,1,0,1,0,3,3,3,0
	.byte 2,2,0,0,2,2,2,0,1,3,1,0,0,3,0,0
	.byte 2,2,2,0,2,2,2,0,1,2,1,0,3,3,3,0
	.byte 2,2,0,0,2,2,2,0,1,3,1,0,3,3,3,0
	.byte 2,2,0,0,2,2,2,0,1,2,1,0,3,3,3,0
	.byte 2,2,0,0,0,2,2,0,1,3,0,0,0,3,3,0
	.byte 2,2,0,0,2,2,2,0,1,2,1,0,3,3,3,0
	.byte 2,2,0,0,0,2,2,0,1,3,0,0,0,3,3,0
	
optype:
	.byte 0,1,0,0,0,2,2,0,0,0,0,0,0,3,3,0
	.byte 0,4,0,0,0,5,5,0,0,6,0,0,0,7,7,0
	.byte 0,1,0,0,2,2,2,0,0,0,0,0,3,3,3,0
	.byte 0,4,0,0,0,5,5,0,0,6,0,0,0,7,7,0
	.byte 0,1,0,0,0,2,2,0,0,0,0,0,0,3,3,0
	.byte 0,4,0,0,0,5,5,0,0,6,0,0,0,7,7,0
	.byte 0,1,0,0,0,2,2,0,0,0,0,0,3,3,3,0
	.byte 0,4,0,0,0,5,5,0,0,6,0,0,0,7,7,0
	.byte 0,1,0,0,2,2,2,0,0,0,0,0,3,3,3,0
	.byte 0,4,0,0,5,5,8,0,0,6,0,0,0,7,0,0
	.byte 0,1,0,0,2,2,2,0,0,0,0,0,3,3,3,0
	.byte 0,4,0,0,5,5,8,0,0,6,0,0,7,7,6,0
	.byte 0,1,0,0,2,2,2,0,0,0,0,0,3,3,3,0
	.byte 0,4,0,0,0,5,5,0,0,6,0,0,0,7,7,0
	.byte 0,1,0,0,2,2,2,0,0,0,0,0,3,3,3,0
	.byte 0,4,0,0,0,5,5,0,0,6,0,0,0,7,7,0

ZNTable:
	.byte Z_FLAG,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG
	.byte N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG
	.byte N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG
	.byte N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG
	.byte N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG
	.byte N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG
	.byte N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG
	.byte N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG,N_FLAG
	
/* 0x10 ints to pass data between C++ and ASM */
nes_registers:
	.fill   0x40, 1, 0
	
/* 6502's temporary count */
tcount:
	.long	0
	
/* Trigger for IRQ Low (compatibility with FCEUX) */
IRQlow:
	.long	0
	
/* Temporary _P it seems... */
mooPI:
	.long	0
	
jammed:
	.long	0
