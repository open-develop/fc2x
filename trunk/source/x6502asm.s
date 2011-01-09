/**************************************************

X6502 cpu - ARM port

Ported by : Luke Arntson (Cthulhu32)
Original Credit: FCEUX authors and other various
                  great programmers.
Modified Date: 1/8/2011
Create Date: 1/8/2011

***************************************************/

.equ REG_A, r4
.equ REG_X, r5
.equ REG_Y, r6
.equ REG_PC, r7
.equ REG_S, r8
.equ REG_P_REST, r8
.equ REG_NZ, r9
.equ REG_ADDR, r10
.equ REG_CYCLE, r11
.equ REG_OP_TABLE r12

.text
.align 4

.macro ADDCYC X
.endm

.macro RdMem A
.endm

.macro WrMem A, V
.endm

.macro RdRam A
.endm

.macro WrRam A, V
.endm

.macro PUSH V
.endm

.macro POP
.endm

.macro X_ZN zort
.endm

.macro X_ZNT zort
.endm

.macro JN cond
.endm

.macro LDA
.endm

.macro LDX
.endm

.macro LDY
.endm

.macro AND
.endm

.macro BIT
.endm

.macro EOR
.endm

.macro ORA
.endm

.macro ADC
.endm

.macro SBC
.endm

.macro CMPL a1, a2
.endm

.macro AXS
.endm

.macro CMP
.endm

.macro CPX
.endm

.macro CPY
.endm

.macro DEC
.endm

.macro INC
.endm

op00:	/* BRK */
	
op40:	/* RTI */

op60:	/* RTS */

op48:	/* PHA */

op08:	/* PHP */

op68:	/* PLA */

op28:	/* PLP*/

op4C:	/* JMP ABSOLUTE */

op6C:	/* JMP SPECIFIC */

op20:	/* JSR */

opAA:	/* TAX */

op8A:	/* TXA */

opA8:	/* TAY */

op98:	/* TYA */

opBA:	/* TSX */

op9A:	/* TXS */

opCA:	/* DEX */

op88:	/* DEY */

opE8:	/* INX */

opC8:	/* INY */

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
opC7: RMW_ZP(DEC;CMP);
opD7: RMW_ZPX(DEC;CMP);
opCF: RMW_AB(DEC;CMP);
opDF: RMW_ABX(DEC;CMP);
opDB: RMW_ABY(DEC;CMP);
opC3: RMW_IX(DEC;CMP);
opD3: RMW_IY(DEC;CMP);

/* ISB */
opE7: RMW_ZP(INC;SBC);
opF7: RMW_ZPX(INC;SBC);
opEF: RMW_AB(INC;SBC);
opFF: RMW_ABX(INC;SBC);
opFB: RMW_ABY(INC;SBC);
opE3: RMW_IX(INC;SBC);
opF3: RMW_IY(INC;SBC);

/* DOP */

op04: _PC++;break;
op14: _PC++;break;
op34: _PC++;break;
op44: _PC++;break;
op54: _PC++;break;
op64: _PC++;break;
op74: _PC++;break;

op80: _PC++;break;
op82: _PC++;break;
op89: _PC++;break;
opC2: _PC++;break;
opD4: _PC++;break;
opE2: _PC++;break;
opF4: _PC++;break;

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

opBB:	/* LAR */

/* LAX */
opA7: LD_ZP(LDA;LDX);
opB7: LD_ZPY(LDA;LDX);
opAF: LD_AB(LDA;LDX);
opBF: LD_ABY(LDA;LDX);
opA3: LD_IX(LDA;LDX);
opB3: LD_IY(LDA;LDX);

/* NOP */
op1A:
op3A:
op5A:
op7A:
opDA:
opFA:

/* RLA */
op27: RMW_ZP(ROL;AND);
op37: RMW_ZPX(ROL;AND);
op2F: RMW_AB(ROL;AND);
op3F: RMW_ABX(ROL;AND);
op3B: RMW_ABY(ROL;AND);
op23: RMW_IX(ROL;AND);
op33: RMW_IY(ROL;AND);

/* RRA */
op67: RMW_ZP(ROR;ADC);
op77: RMW_ZPX(ROR;ADC);
op6F: RMW_AB(ROR;ADC);
op7F: RMW_ABX(ROR;ADC);
op7B: RMW_ABY(ROR;ADC);
op63: RMW_IX(ROR;ADC);
op73: RMW_IY(ROR;ADC);

/* SLO */
op07: RMW_ZP(ASL;ORA);
op17: RMW_ZPX(ASL;ORA);
op0F: RMW_AB(ASL;ORA);
op1F: RMW_ABX(ASL;ORA);
op1B: RMW_ABY(ASL;ORA);
op03: RMW_IX(ASL;ORA);
op13: RMW_IY(ASL;ORA);

/* SRE */
op47: RMW_ZP(LSR;EOR);
op57: RMW_ZPX(LSR;EOR);
op4F: RMW_AB(LSR;EOR);
op5F: RMW_ABX(LSR;EOR);
op5B: RMW_ABY(LSR;EOR);
op43: RMW_IX(LSR;EOR);
op53: RMW_IY(LSR;EOR);

/* AXA - SHA */
op93: ST_IY(_A&_X&(((A-_Y)>>8)+1));
op9F: ST_ABY(_A&_X&(((A-_Y)>>8)+1));

/* SYA */
op9C: ST_ABX(_Y&(((A-_X)>>8)+1));

/* SXA */
op9E: ST_ABY(_X&(((A-_Y)>>8)+1));

/* XAS */
op9B: _S=_A&_X;ST_ABY(_S& (((A-_Y)>>8)+1) );

/* TOP */
op0C: LD_AB(;);
op1C: 
op3C: 
op5C: 
op7C: 
opDC: 
opFC: LD_ABX(;);

/* XAA - BIG QUESTION MARK HERE */
op8B:

/* Interface with our C/C++ code
	Note: This code MUST be in an extern "C" loop in order to link
	with C++ properly.. */
 
.data
.align 4

.globl X6502_Init_a @ (void);
.globl X6502_Reset_a @ (void);
.globl X6502_Power_a @ (void);
.globl TriggerNMI_a @ (void);
.globl TriggerNMI2_a @ (void);
