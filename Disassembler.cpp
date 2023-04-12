#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double real64;


enum RegisterEnum {NO_REG=0, AL, CL, DL, BL, AH, CH, DH, BH, AX, BX, CX, DX, SP, BP, SI, DI};

enum EffectiveAddresses {NO_ADDR = 0, ADDR_BX_PLUS_SI, ADDR_BX_PLUS_DI, ADDR_BP_PLUS_SI, 
						 ADDR_BP_PLUS_DI, ADDR_SI, ADDR_DI, DIRECT_ADDR, ADDR_BX, ADDR_BP};


enum Commands {NO_COMMMAND=0, MOV, ADD, ADC, SBB, CMP, SUB, JNZ=0b01110101, JE=0b01110100, JL=0b01111100, 
				JLE=0b01111110, JB=0b01110010, JBE=0b01110110, JP=0b01111010, 
				JO=0b01110000, JS=0b01111000, /*JNE=JNZ*/ JNL=0b01111101, JG=0b01111111, 
				JNB=0b01110011, JA=0b01110111, JNP=0b01111011,JNO=0b01110001, 
				JNS=0b01111001, LOOP=0b11100010, LOOPZ=0b11100001, LOOPNZ=0b11100000, 
				JCXZ=0b11100011};

struct short_string {
	char S[255];
};
				
short_string ConvertCommandToString(uint32 Command) {
	short_string Result = {};
	switch(Command){
		case NO_COMMMAND: {strcpy(Result.S, "NO_COMMMAND" );} break; 
		case MOV: {strcpy(Result.S, "mov" );} break; 
		case ADD: {strcpy(Result.S, "add" );} break; 
		case ADC: {strcpy(Result.S, "adc" );} break; 
		case SBB: {strcpy(Result.S, "sbb" );} break; 
		case CMP: {strcpy(Result.S, "cmp" );} break; 
		case SUB: {strcpy(Result.S, "sub" );} break; 
		case JNZ: {strcpy(Result.S, "jnz" );} break; 
		case JE: {strcpy(Result.S, "je" );} break; 
		case JL: {strcpy(Result.S, "jl" );} break; 
		case JO: {strcpy(Result.S, "jo" );} break; 
		case JLE: {strcpy(Result.S, "jle" );} break; 
		case JB: {strcpy(Result.S, "jb" );} break; 
		case JBE: {strcpy(Result.S, "jbe" );} break; 
		case JP: {strcpy(Result.S, "jp" );} break;
		case JS: {strcpy(Result.S, "js" );} break;
		case JNL: {strcpy(Result.S, "jnl" );} break;
		case JG: {strcpy(Result.S, "jg" );} break;
		case JNB: {strcpy(Result.S, "jnb" );} break;
		case JA: {strcpy(Result.S, "ja" );} break;
		case JNP: {strcpy(Result.S, "jnp" );} break; 
		case JNO: {strcpy(Result.S, "jno" );} break; 
		case JNS: {strcpy(Result.S, "jns" );} break; 
		case LOOP: {strcpy(Result.S, "loop" );} break; 
		case LOOPZ: {strcpy(Result.S, "loopz" );} break; 
		case LOOPNZ: {strcpy(Result.S, "loopnz" );} break; 
		case JCXZ: {strcpy(Result.S, "jcxz" );} break; 
		default : {strcpy(Result.S, "NO_COMMMAND" );}
	}
	return Result;
}

enum ByteLength {Byte, Word};

struct asm_register {
	enum RegisterEnum RegisterId;
};

struct asm_line {
	uint32 Command;
	int DirectNumber;
	uint8 Mod;
	uint8 Reg;
	uint8 R_M;
	bool DestInRegField;
	bool SignExtension;
	bool WordData;
	uint8 LowDisp;
	uint8 HighDisp;
	uint8 LowData;
	uint8 HighData;
};

struct asm_line_expanded {
	uint32 Command = NO_COMMMAND;
	uint32 SourceRegister = NO_REG;
	uint32 DestinationRegister = NO_REG;
	uint32 EffectiveAddress = NO_ADDR;
	uint16 Displacement = 0;
	uint16 Data = 0;
	bool HasData = false;
	bool ToRegister = 0;
	bool EffectiveAddressInDestination = 0;
};

struct a_bunch_of_asm_expanded_lines {
	asm_line_expanded L[255];
};

short_string ConvertEffectiveAddressToString(uint32 EffectiveAddress) {
	short_string Result = {};
	switch(EffectiveAddress){
		case ADDR_BX_PLUS_SI: {strcpy(Result.S, "bx + si");} break; 
		case ADDR_BX_PLUS_DI: {strcpy(Result.S, "bx + di");} break;
		case ADDR_BP_PLUS_SI: {strcpy(Result.S, "bp + si");} break;
		case ADDR_BP_PLUS_DI: {strcpy(Result.S, "bp + di");} break;
		case ADDR_SI: {strcpy(Result.S, "si");} break;
		case ADDR_DI: {strcpy(Result.S, "di");} break;
		case ADDR_BP: {strcpy(Result.S, "bp");} break;
		case ADDR_BX: {strcpy(Result.S, "bx");} break;
		default : {}
	}
	return Result;	
}

short_string ConvertRegisterToString(uint32 Register) {
	short_string Result = {};
	switch(Register){
		case AL: {strcpy(Result.S, "al");} break; 
		case CL: {strcpy(Result.S, "cl");} break;
		case DL: {strcpy(Result.S, "dl");} break;
		case BL: {strcpy(Result.S, "bl");} break;
		case AH: {strcpy(Result.S, "ah");} break;
		case CH: {strcpy(Result.S, "ch");} break;
		case DH: {strcpy(Result.S, "dh");} break;
		case BH: {strcpy(Result.S, "bh");} break;
		case AX: {strcpy(Result.S, "ax");} break;
		case BX: {strcpy(Result.S, "bx");} break;
		case CX: {strcpy(Result.S, "cx");} break;
		case DX: {strcpy(Result.S, "dx");} break;
		case SP: {strcpy(Result.S, "sp");} break;
		case BP: {strcpy(Result.S, "bp");} break;
		case SI: {strcpy(Result.S, "si");} break;
		case DI: {strcpy(Result.S, "di");} break;
		default : {}
	}
	return Result;
}

void PrintASMLine(asm_line_expanded ASMLine) {
	
	// Print the command
	short_string CommandString = ConvertCommandToString(ASMLine.Command);	
	
	// Determine what the string will look like
	short_string DestinationString = {};
	short_string SourceString = {};
	
	strcpy(DestinationString.S, "DESTINATION");
	strcpy(SourceString.S, "SOURCE");
	
	// registers
	if(ASMLine.SourceRegister!=NO_REG) {
		short_string SourceRegisterString = ConvertRegisterToString(ASMLine.SourceRegister);
		SourceString = {};
		strcpy(SourceString.S, SourceRegisterString.S);
	}
	if(ASMLine.DestinationRegister!=NO_REG) {
		short_string DestinationRegisterString = ConvertRegisterToString(ASMLine.DestinationRegister);
		DestinationString = {};
		strcpy(DestinationString.S, DestinationRegisterString.S);
	}
	
	// effective address
	short_string EffectiveAddressString = {};
	short_string EffectiveAddressFinalString = {};
	if(ASMLine.EffectiveAddress!=NO_ADDR){
		// Direct Address exception
		if(ASMLine.EffectiveAddress==DIRECT_ADDR) {
			if(ASMLine.EffectiveAddressInDestination) {
				sprintf(DestinationString.S, "[%i]", ASMLine.Displacement);				
			} else {
				sprintf(SourceString.S, "[%i]", ASMLine.Displacement);
			}
		// All other cases
		} else {
			EffectiveAddressString = ConvertEffectiveAddressToString(ASMLine.EffectiveAddress);
			if(ASMLine.Displacement>0) {
				sprintf(EffectiveAddressFinalString.S, "[%s + %i]", EffectiveAddressString.S, ASMLine.Displacement);
			} else {
				sprintf(EffectiveAddressFinalString.S, "[%s]", EffectiveAddressString.S);
			}
			
			if(ASMLine.EffectiveAddressInDestination) {
				strcpy(DestinationString.S, EffectiveAddressFinalString.S);
			} else {
				strcpy(SourceString.S, EffectiveAddressFinalString.S);
			}
		}
	}
	if(ASMLine.HasData==true) {
		sprintf(SourceString.S, "%i", (int16)ASMLine.Data);
	}
	
	// if we're doing jmp stuff
	if(ASMLine.Command>SUB) {
		printf("%s %i", CommandString.S, (int8) ASMLine.Displacement);
	} else {
		printf("%s %s, %s", CommandString.S, DestinationString.S, SourceString.S);			
	}
	
	// End of line
	printf("\n");
	
}

void PrintASMLines(asm_line_expanded *ASMLines, uint32 NumberOfLines) {
	for(int i=0; i<NumberOfLines; i++) {
		PrintASMLine(ASMLines[i]);
	}
}

char *GetRegisterString(uint8 RegisterNumber, bool WordData) {
	// TODO: Memory leak fix
	char *Result = (char *) malloc(3);
	switch(RegisterNumber){
		case 0b000: {
			if(WordData)
				strcpy(Result, "ax");
			else
				strcpy(Result, "al");
		} break; 
		case 0b001: {
			if(WordData)
				strcpy(Result, "cx");
			else
				strcpy(Result, "cl");
		} break; 
		case 0b010: {
			if(WordData)
				strcpy(Result, "dx");
			else
				strcpy(Result, "dl");
		} break; 
		case 0b011: {
			if(WordData)
				strcpy(Result, "bx");
			else
				strcpy(Result, "bl");
		} break; 
		case 0b100: {
			if(WordData)
				strcpy(Result, "sp");
			else
				strcpy(Result, "ah");
		} break; 
		case 0b101: {
			if(WordData)
				strcpy(Result, "bp");
			else
				strcpy(Result, "ch");
		} break; 
		case 0b110: {
			if(WordData)
				strcpy(Result, "si");
			else
				strcpy(Result, "dh");
		} break; 
		case 0b111: {
			if(WordData)
				strcpy(Result, "di");
			else
				strcpy(Result, "bh");
		} break;
		default: {
			//printf("We don't know the code.\n");
		}
	}

	return Result;
}

uint32 GetRegisterCode(uint8 RegisterNumber, bool WordData) {
	uint32 Result = NO_REG;
	switch(RegisterNumber){
		case 0b000: {
			if(WordData)
				Result = AX;
			else
				Result = AL;
		} break; 
		case 0b001: {
			if(WordData)
				Result = CX;
			else
				Result = CL;
		} break; 
		case 0b010: {
			if(WordData)
				Result = DX;
			else
				Result = DL;
		} break; 
		case 0b011: {
			if(WordData)
				Result = BX;
			else
				Result = BL;
		} break; 
		case 0b100: {
			if(WordData)
				Result = SP;
			else
				Result = AH;
		} break; 
		case 0b101: {
			if(WordData)
				Result = BP;
			else
				Result = CH;
		} break; 
		case 0b110: {
			if(WordData)
				Result = SI;
			else
				Result = DH;
		} break; 
		case 0b111: {
			if(WordData)
				Result = DI;
			else
				Result = BH;
		} break;
		default: {
			Result = NO_REG;
			//printf("We don't know the code.\n");
		}
	}

	return Result;
}

char *GetEffectiveAddressString(uint8 R_M, uint8 MemoryModeNotZero) {
	// TODO: Memory Leak Fix 
	char *Result = (char *) malloc(10);
	switch(R_M){
		case 0b000: {strcpy(Result, "bx + si");} break; 
		case 0b001: {strcpy(Result, "bx + di");} break;
		case 0b010: {strcpy(Result, "bp + si");} break;
		case 0b011: {strcpy(Result, "bp + di");} break;
		case 0b100: {strcpy(Result, "si");} break;
		case 0b101: {strcpy(Result, "di");} break;
		case 0b110: {
			if(MemoryModeNotZero) {
				strcpy(Result, "bp");
			} else {
				printf("EFFECTIVE ADDRESS STRING FUNCTION HAS GOTTEN TO DIRECT ADDRESS BRANCH. THIS SHOULD NOT BE POSSIBLE\n");
			}
		} break;
		case 0b111: {strcpy(Result, "bx");} break;
		default : {}
	}
	return Result;	
}

uint32 GetEffectiveAddressCode(uint8 R_M, uint8 MemoryModeNotZero) {
	uint32 Result = NO_ADDR;
	switch(R_M){
		case 0b000: {Result = ADDR_BX_PLUS_SI;} break; 
		case 0b001: {Result = ADDR_BX_PLUS_DI;} break;
		case 0b010: {Result = ADDR_BP_PLUS_SI;} break;
		case 0b011: {Result = ADDR_BP_PLUS_DI;} break;
		case 0b100: {Result = ADDR_SI;} break;
		case 0b101: {Result = ADDR_DI;} break;
		case 0b110: {
			if(MemoryModeNotZero) {
				Result = ADDR_BP;
			} else {
				Result = DIRECT_ADDR;
				printf("EFFECTIVE ADDRESS CODE FUNCTION HAS GOTTEN TO DIRECT ADDRESS BRANCH. THIS SHOULD NOT BE POSSIBLE\n");
			}
		} break;
		case 0b111: {Result = ADDR_BX;} break;
		default : {Result = NO_ADDR;}
	}
	return Result;	
}

char *GetCommandFromDeterminingBits(uint8 DeterminingBits) {
	// TODO: Memory Leak Fix 
	char *Result = (char *) malloc(10);
	switch(DeterminingBits){
		case 0b000: {strcpy(Result, "add");} break; 
		case 0b010: {strcpy(Result, "adc");} break;
		case 0b101: {strcpy(Result, "sub");} break;
		case 0b011: {strcpy(Result, "sbb");} break;
		case 0b111: {strcpy(Result, "cmp");} break;
		default: {}
	}
	return Result;
}

uint32 GetCommandCodeFromDeterminingBits(uint8 DeterminingBits) {
	// TODO: Memory Leak Fix 
	uint32 Result = 0;
	switch(DeterminingBits){
		case 0b000: {Result = ADD;} break; 
		case 0b010: {Result = ADC;} break;
		case 0b101: {Result = SUB;} break;
		case 0b011: {Result = SBB;} break;
		case 0b111: {Result = CMP;} break;
		default: {}
	}
	return Result;
}

uint8 MEMORY_MODE_MOSTLY_NO_DIS = 0b00;
uint8 MEMORY_MODE_8BIT_DIS = 0b01;
uint8 MEMORY_MODE_16BIT_DIS = 0b10;
uint8 REGISTER_MODE = 0b011;

struct registers {
	uint16 *Memory;
	uint8 LowRegisters[32];
	uint16 HighRegisters[32];
	bool ZeroFlag = false;
	bool ParityFlag = false;
	bool SignFlag = false;
	bool OverflowFlag = false;
};

void Set8BitValue(uint8 *Register, int ID, uint8 Value) {
	Register[ID] = Value;
}

void Set16BitValue(uint16 *Register, int ID, uint16 Value) {
	Register[ID] = Value;
}


uint16 GetMemoryAddressFromEffectiveAdress(uint32 EffectiveAddress, uint16 Displacement, registers R){
	uint16 Result = 0;
	switch(EffectiveAddress){
		case NO_ADDR: {Result = 0;} break; 
		case ADDR_BX_PLUS_SI: {Result = R.HighRegisters[BX] + R.HighRegisters[DI] + Displacement;} break;
		case ADDR_BX_PLUS_DI: {Result = R.HighRegisters[BX] + R.HighRegisters[DI] + Displacement;} break;
		case ADDR_BP_PLUS_DI: {Result = R.HighRegisters[BP] + R.HighRegisters[DI] + Displacement;} break;
		case ADDR_BP_PLUS_SI: {Result = R.HighRegisters[BP] + R.HighRegisters[SI] + Displacement;} break;
		case ADDR_SI: {Result = R.HighRegisters[SI] + Displacement;} break;
		case ADDR_DI: {Result = R.HighRegisters[DI] + Displacement;} break;
		case ADDR_BP:  {Result = R.HighRegisters[BP] + Displacement;} break;
		case DIRECT_ADDR: {
			//printf("DIRECT ADDR HAS BEEN REACHED IN THE GET MEMORY ADDRESS FROM EFFECTIVE ADDRESS FUNCTION. THIS SHOULD NOT BE POSSIBLE!");
		} break;
		case ADDR_BX: {Result = R.HighRegisters[BX] + Displacement;} break;
		default : {Result = 0;} break;
	}
	return Result;
}

void PrintOutRegisters (registers Registers) {
	//printf("AH[%X] AL[%X]\n", Registers.LowRegisters[AH], Registers.LowRegisters[AL]);
    //printf("BH[%X] BL[%X]\n", Registers.LowRegisters[BH], Registers.LowRegisters[BL]);
	//printf("CH[%X] CL[%X]\n", Registers.LowRegisters[CH], Registers.LowRegisters[CL]);
	//printf("DH[%X] DL[%X]\n", Registers.LowRegisters[DH], Registers.LowRegisters[DL]);
	printf("   AX[%i]\n", (int16) Registers.HighRegisters[AX]);
	printf("   BX[%i]\n", (int16) Registers.HighRegisters[BX]);
	printf("   CX[%i]\n", (int16) Registers.HighRegisters[CX]);
	printf("   DX[%i]\n", (int16) Registers.HighRegisters[DX]);
	printf("   SP[%i]\n", (int16) Registers.HighRegisters[SP]);
	printf("   BP[%i]\n", (int16) Registers.HighRegisters[BP]);
	printf("   SI[%i]\n", (int16) Registers.HighRegisters[SI]);
	printf("   DI[%i]\n", (int16) Registers.HighRegisters[DI]);
	printf("ZeroFlag=%i\n", Registers.ZeroFlag);
	printf("SignFlag=%i\n", Registers.SignFlag);
	printf("Memory 1000[%i]\n", (int16) Registers.Memory[1000]);
	printf("Memory 1002[%i]\n", (int16) Registers.Memory[1002]);
	printf("Memory 1004[%i]\n", (int16) Registers.Memory[1004]);
	printf("Memory 1006[%i]\n", (int16) Registers.Memory[1006]);
	printf("\n");
}

registers RunASMLine(asm_line_expanded ASMLine, registers R) {
	if(ASMLine.Command==MOV) {
		// DIRECT DATA WRITE
		if(ASMLine.HasData==true) {
			if(ASMLine.EffectiveAddressInDestination==true) {
				// Direct data to Memory Address
				if(ASMLine.EffectiveAddress==DIRECT_ADDR) {
					R.Memory[ASMLine.Displacement] = ASMLine.Data;
				// Direct data to Effective Memory Address
				} else {
					uint16 ActualMemoryAddress = GetMemoryAddressFromEffectiveAdress(ASMLine.EffectiveAddress, ASMLine.Displacement, R);
					R.Memory[ActualMemoryAddress] = ASMLine.Data;
				}
			// Direct data to register
			} else {
				Set16BitValue(R.HighRegisters, ASMLine.DestinationRegister, ASMLine.Data);				
			}
		// MEMORY TO REG or REG TO MEMORY
		} else if (ASMLine.EffectiveAddress!=NO_ADDR) {
			uint16 ActualMemoryAddress = 0;
			if(ASMLine.EffectiveAddress==DIRECT_ADDR) {
				// have not implemented direct address yet, lol
				ActualMemoryAddress = ASMLine.Displacement;
				
			} else {
				ActualMemoryAddress = GetMemoryAddressFromEffectiveAdress(ASMLine.EffectiveAddress, ASMLine.Displacement, R);
				//printf("Displacement: %i\n", ASMLine.Displacement);
				//printf("ActualMemoryAddress: %i\n", ActualMemoryAddress);
			// Reg to memory
			}
			//printf("Reg to Memory\n");
			//printf("Displacement: %i\n", ASMLine.Displacement);
			//printf("ActualMemoryAddress: %i\n", ActualMemoryAddress);
			// put addreesadsfgsd fb
			if(ASMLine.EffectiveAddressInDestination) {
				R.Memory[ActualMemoryAddress] = R.HighRegisters[ASMLine.SourceRegister];
			// Memory to reg
			} else {
				R.HighRegisters[ASMLine.DestinationRegister] = R.Memory[ActualMemoryAddress];
			}
		// REG TO REG 
		} else if(ASMLine.SourceRegister!=NO_REG&&ASMLine.DestinationRegister!=NO_REG) {
			Set16BitValue(R.HighRegisters, ASMLine.DestinationRegister, R.HighRegisters[ASMLine.SourceRegister]);	
		}
	} else if (ASMLine.Command==ADD||ASMLine.Command==SUB||ASMLine.Command==CMP) {
		uint16 SourceNumber;
		uint16 DestinationNumber;
		uint16 FinalDestinationData;
		
		// Get source and destination numbers depending on command intricacies
		if(ASMLine.HasData==true) {
			SourceNumber = ASMLine.Data;
			DestinationNumber = R.HighRegisters[ASMLine.DestinationRegister];
		} else if(ASMLine.SourceRegister!=NO_REG&&ASMLine.DestinationRegister!=NO_REG) {
			SourceNumber = R.HighRegisters[ASMLine.SourceRegister];
			DestinationNumber = R.HighRegisters[ASMLine.DestinationRegister];
		}
		
		// Set the register and the flags to what they need to be set
		if(ASMLine.Command==ADD) {
			FinalDestinationData = DestinationNumber + SourceNumber;	
			Set16BitValue(R.HighRegisters, ASMLine.DestinationRegister, FinalDestinationData);
		} else if (ASMLine.Command==SUB) {
			FinalDestinationData = DestinationNumber - SourceNumber;
			Set16BitValue(R.HighRegisters, ASMLine.DestinationRegister, FinalDestinationData);
		} else if (ASMLine.Command==CMP) {
			FinalDestinationData = DestinationNumber - SourceNumber;
		}
		if(FinalDestinationData == 0) {
			R.ZeroFlag = true;
		} else {
			R.ZeroFlag = false;
		}
		//printf("FinDestData: %i\n FinDestData & 0x8000: %X\n", FinalDestinationData, FinalDestinationData & 0x8000);
		if((FinalDestinationData & 0x8000)==0x8000) {
			R.SignFlag = true;
		} else {
			R.SignFlag = false;
		}
	}
		return R;
}


registers RunASMLines(asm_line_expanded *ASMLines, uint32 NumberOfLines, registers Registers) {
	for(int i=0; i<NumberOfLines; i++) {
		Registers = RunASMLine(ASMLines[i], Registers);
		PrintASMLine(ASMLines[i]);
		PrintOutRegisters(Registers);
	}

	return Registers;
}

int main(int argc, char *argv[])

{

	char ReadFilename[255];	
	//strcpy(ReadFilename, "listing_0052_memory_add_loop");
	//strcpy(ReadFilename, "listing_0051_memory_mov");
	
	if( argc == 2 ) {
		strcpy(ReadFilename, argv[1]);
	} else if( argc > 2 ) {
		printf("This program only expects one argument.\nUsage: disassembler.exe pathToCompiledAssembly");
		return 0;
	} else {
		printf("Usage: disassembler.exe pathToCompiledAssembly");
		return 0;
	}
		
	void *PointerToFileData;
	
	//TODO: Byte/word thing
	
	// Dump the file contents into memory and point pointer to memory
	FILE *ReadFile = fopen(ReadFilename, "rb");
	fseek(ReadFile, 0, SEEK_END);
	int FileSize = ftell(ReadFile);
	fseek(ReadFile, 0, SEEK_SET);
	PointerToFileData = malloc(FileSize+1);
	fread(PointerToFileData, FileSize, 1, ReadFile);
	fclose(ReadFile);
	
	printf("FileSize: %i\n", FileSize); 

	printf("bits 16\n");
	
	
	
	// Set up the emulated 8086 state
	registers Registers = {};
	Registers.Memory = (uint16 *) malloc(1024*1024); 
	
	// Do the logic
	uint8 *CommandChunkCursor = (uint8*)PointerToFileData;
	
	uint32 NumberOfLines = 0;
	asm_line_expanded LinesOfAsm[100];
	
	// In this functionality, i Will be the value for the Instruction Pointer
	for(int i=0;i<FileSize;i++) {
		uint8 CurrentCommandChunk = *CommandChunkCursor;
		char StringCommand[3];
		char StringDestination[32];
		char StringSource[32];

		uint8 FirstByte = CommandChunkCursor[i];
		
		// JNZ
		if(FirstByte==JNZ) {
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			LinesOfAsm[NumberOfLines].Displacement = SecondByte;
			LinesOfAsm[NumberOfLines].Command = JNZ;
			if(Registers.ZeroFlag==false) {
				i += (int8) SecondByte;				
			}
		// JE
		} else if(FirstByte==JE) {
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			LinesOfAsm[NumberOfLines].Displacement = SecondByte;
			LinesOfAsm[NumberOfLines].Command = JE;
			//i += (int8) SecondByte;
		// JL	
		} else if(FirstByte==JL) { 
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			LinesOfAsm[NumberOfLines].Displacement = SecondByte;
			LinesOfAsm[NumberOfLines].Command = JL;
			//i += (int8) SecondByte;
		// JLE	
		} else if(FirstByte==JLE) {
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			LinesOfAsm[NumberOfLines].Displacement = SecondByte;
			LinesOfAsm[NumberOfLines].Command = JLE;
			//i += (int8) SecondByte;
		// JB	
		} else if(FirstByte==JB) {
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			LinesOfAsm[NumberOfLines].Displacement = SecondByte;
			LinesOfAsm[NumberOfLines].Command = JB;
			//i += (int8) SecondByte;
		// JBE
		} else if(FirstByte==JBE) {
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			LinesOfAsm[NumberOfLines].Displacement = SecondByte;
			LinesOfAsm[NumberOfLines].Command = JBE;
			//i += (int8) SecondByte;
		// JP	
		} else if(FirstByte==JP) {
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			LinesOfAsm[NumberOfLines].Displacement = SecondByte;
			LinesOfAsm[NumberOfLines].Command = JP;
			//i += (int8) SecondByte;
		// JO	
		} else if(FirstByte==JO) {
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			LinesOfAsm[NumberOfLines].Displacement = SecondByte;
			LinesOfAsm[NumberOfLines].Command = JO;
			//i += (int8) SecondByte;
		// JS	
		} else if(FirstByte==JS) {
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			LinesOfAsm[NumberOfLines].Displacement = SecondByte;
			LinesOfAsm[NumberOfLines].Command = JS;
			//i += (int8) SecondByte;
		// JNE is the same as JNZ
		/*} else if(FirstByte==JNZ) {
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			LinesOfAsm[NumberOfLines].Displacement = SecondByte;
			LinesOfAsm[NumberOfLines].Command = JNZ;
		// JNL*/	
		} else if(FirstByte==JNL) {
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			LinesOfAsm[NumberOfLines].Displacement = SecondByte;
			LinesOfAsm[NumberOfLines].Command = JNL;
			//i += (int8) SecondByte;
		// JG	
		}  else if(FirstByte==JG) {
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			LinesOfAsm[NumberOfLines].Displacement = SecondByte;
			LinesOfAsm[NumberOfLines].Command = JG;
			//i += (int8) SecondByte;
		// JNB	
		} else if(FirstByte==JNB) {
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			LinesOfAsm[NumberOfLines].Displacement = SecondByte;
			LinesOfAsm[NumberOfLines].Command = JNB;
			//i += (int8) SecondByte;
		// JA
		} else if(FirstByte==JA) {
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			LinesOfAsm[NumberOfLines].Displacement = SecondByte;
			LinesOfAsm[NumberOfLines].Command = JA;
			//i += (int8) SecondByte;
		// JNP	
		} else if(FirstByte==JNP) {
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			LinesOfAsm[NumberOfLines].Displacement = SecondByte;
			LinesOfAsm[NumberOfLines].Command = JNP;
			//i += (int8) SecondByte;
		// JNO	
		} else if(FirstByte==JNO) {
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			LinesOfAsm[NumberOfLines].Displacement = SecondByte;
			LinesOfAsm[NumberOfLines].Command = JNO;
			//i += (int8) SecondByte;
		// JNS	
		} else if(FirstByte==JNS) {
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			LinesOfAsm[NumberOfLines].Displacement = SecondByte;
			LinesOfAsm[NumberOfLines].Command = JNS;
			//i += (int8) SecondByte;
		// LOOP	
		} else if(FirstByte==LOOP) {
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			LinesOfAsm[NumberOfLines].Displacement = SecondByte;
			LinesOfAsm[NumberOfLines].Command = LOOP;
			//i += (int8) SecondByte;
		// LOOPZ	
		} else if(FirstByte==LOOPZ) {
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			LinesOfAsm[NumberOfLines].Displacement = SecondByte;
			LinesOfAsm[NumberOfLines].Command = LOOPZ;
			//i += (int8) SecondByte;
		// LOOPNZ	
		} else if(FirstByte==LOOPNZ) {
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			LinesOfAsm[NumberOfLines].Displacement = SecondByte;
			LinesOfAsm[NumberOfLines].Command = LOOPNZ;
			//i += (int8) SecondByte;
		// JCXZ
		} else if(FirstByte==JCXZ) {
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			LinesOfAsm[NumberOfLines].Displacement = SecondByte;
			LinesOfAsm[NumberOfLines].Command = JCXZ;
			//i += (int8) SecondByte;
		// IMMEDIATE TO REGISTER mov (1-0-1-1-w-reg-reg-reg)
		} else if((FirstByte & 0b11110000)==0b10110000) {
			LinesOfAsm[NumberOfLines].Command = MOV;
			
			bool WordData = FirstByte & 0b00001000;
			uint8 REG = FirstByte & 0b00000111;
			
			//CommandChunkCursor++; 
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			
			// Since we're directly putting data, we dont' have to worry about whether the REG is source or destination; it's always destination
			//strcpy(StringDestination, GetRegisterString(REG, WordData));
			LinesOfAsm[NumberOfLines].DestinationRegister = GetRegisterCode(REG, WordData);
		
			// 8 bit data
			if(WordData==0) {
				LinesOfAsm[NumberOfLines].HasData = true;
				LinesOfAsm[NumberOfLines].Data = SecondByte;
			// 16 bit data
			} else {
				// We need the third byte, so grab it and push forward 1
				//CommandChunkCursor++; 
				i++;
				uint8 ThirdByte = CommandChunkCursor[i];
				uint16 D16Data = ((uint16) ThirdByte << 8) | ((uint16) SecondByte);
				LinesOfAsm[NumberOfLines].Data = D16Data;
				LinesOfAsm[NumberOfLines].HasData = true;
			}
		// IMMEDIATE TO ACCUMULATOR
		} else if ((FirstByte & 0b11000110)==0b00000100) {
			uint8 DeterminingBits = (FirstByte & 0b00111000) >> 3;
			LinesOfAsm[NumberOfLines].Command = GetCommandCodeFromDeterminingBits(DeterminingBits);
			strcpy(StringCommand, GetCommandFromDeterminingBits(DeterminingBits));
			
			bool WordData = FirstByte & 0b00000001;
			
			
			//CommandChunkCursor++; 
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			uint8 ThirdByte = 0;

			
			if(WordData) {
				//CommandChunkCursor++; 
				i++;
				uint8 ThirdByte = CommandChunkCursor[i];
				strcpy(StringDestination, "ax");
				LinesOfAsm[NumberOfLines].DestinationRegister = AX;
				
			} else {
				strcpy(StringDestination, "al");
				LinesOfAsm[NumberOfLines].DestinationRegister = AL;
			}
			
			uint16 D16Data = ((uint16) ThirdByte << 8) | ((uint16) SecondByte);
			
			LinesOfAsm[NumberOfLines].Data = D16Data;
			LinesOfAsm[NumberOfLines].HasData = true;
			
		// REGISTER/MEMORY TO/FROM REGISTER  
				   // mov (1-0-0-0-1-0-d-w)
		} else if((FirstByte & 0b11111100)==0b10001000||
				   // add
				  (FirstByte & 0b11111100)==0b00000000||
				   // sub
				  (FirstByte & 0b11111100)==0b00101000||
				   // cmp
				  (FirstByte & 0b11111100)==0b00111000) {
				
			if((FirstByte & 0b11111100)==0b10001000) {
				LinesOfAsm[NumberOfLines].Command = MOV;
				strcpy(StringCommand, "mov");
			} else {
				uint8 DeterminingBits = (FirstByte & 0b00111000) >> 3;
				LinesOfAsm[NumberOfLines].Command = GetCommandCodeFromDeterminingBits(DeterminingBits);
				strcpy(StringCommand, GetCommandFromDeterminingBits(DeterminingBits));
			}
			//CommandChunkCursor++; 
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			
			// Get required info from first byte
			bool DestInRegField = FirstByte & 0b00000010;
			bool WordData = FirstByte & 0b00000001;
			
			// Get required info from second byte
			uint8 Mode = SecondByte >> 6;
			uint8 REG = (SecondByte & 0b00111000) >> 3;
			uint8 R_M = SecondByte & 0b00000111;
			
			// REGISTER MODE
			if(Mode==REGISTER_MODE) {
				if(DestInRegField==true) {
					LinesOfAsm[NumberOfLines].DestinationRegister = GetRegisterCode(REG, WordData);
					LinesOfAsm[NumberOfLines].SourceRegister = GetRegisterCode(R_M, WordData);
					//strcpy(StringDestination, GetRegisterString(REG, WordData));
					//strcpy(StringSource, GetRegisterString(R_M, WordData));
				} else {
					LinesOfAsm[NumberOfLines].DestinationRegister = GetRegisterCode(R_M, WordData);
					LinesOfAsm[NumberOfLines].SourceRegister = GetRegisterCode(REG, WordData);
					//strcpy(StringDestination, GetRegisterString(R_M, WordData));
					//strcpy(StringSource, GetRegisterString(REG, WordData));
				}
			// MEMORY MODES
			} else {
				
				char StringMemoryAddress[32];
				
				strcpy(StringMemoryAddress, "[");
				
				LinesOfAsm[NumberOfLines].EffectiveAddress = GetEffectiveAddressCode(R_M, Mode);
				
				
				if (Mode==MEMORY_MODE_MOSTLY_NO_DIS) {
					// Direct Address
					if(R_M==0b110){
						//CommandChunkCursor++; 
						i++;
						uint8 ThirdByte = CommandChunkCursor[i];
						//CommandChunkCursor++; 
						i++;
						uint8 FourthByte = CommandChunkCursor[i];
						uint16 D16Displacement = ((uint16) FourthByte << 8) | ((uint16) ThirdByte);
						LinesOfAsm[NumberOfLines].Displacement = D16Displacement;
						LinesOfAsm[NumberOfLines].EffectiveAddress = DIRECT_ADDR;
						
					} else {
					}
				} else if(Mode==MEMORY_MODE_8BIT_DIS) {
					// get the third byte
					//CommandChunkCursor++; 
					i++;
					uint8 ThirdByte = CommandChunkCursor[i];
					if(ThirdByte>0) {
						LinesOfAsm[NumberOfLines].Displacement = ThirdByte;					
					}
				
				} else if(Mode==MEMORY_MODE_16BIT_DIS) {
					// get the third and fourth bytes
					//CommandChunkCursor++; 
					i++;
					uint8 ThirdByte = CommandChunkCursor[i];
					//CommandChunkCursor++; 
					i++;
					uint8 FourthByte = CommandChunkCursor[i];
					
					uint16 D16Displacement = ((uint16) FourthByte << 8) | ((uint16) ThirdByte);
					if(D16Displacement>0) {
						LinesOfAsm[NumberOfLines].Displacement = D16Displacement;					
					}
				}
				
				if(DestInRegField==true) {
					LinesOfAsm[NumberOfLines].DestinationRegister = GetRegisterCode(REG, WordData);
					LinesOfAsm[NumberOfLines].SourceRegister = GetRegisterCode(R_M, WordData);
					//strcpy(StringDestination, GetRegisterString(REG, WordData));
					//strcpy(StringSource, GetRegisterString(R_M, WordData));
				} else {
					LinesOfAsm[NumberOfLines].EffectiveAddressInDestination = true;
					LinesOfAsm[NumberOfLines].DestinationRegister = GetRegisterCode(R_M, WordData);
					LinesOfAsm[NumberOfLines].SourceRegister = GetRegisterCode(REG, WordData);
					//strcpy(StringDestination, GetRegisterString(R_M, WordData));
					//strcpy(StringSource, GetRegisterString(REG, WordData));
				}
				
			}
		// IMMEDIATE TO REGISTER/MEMORY MOV and ADD/SUB/CMP
		} else if (((FirstByte & 0b11111100)==0b10000000)
			||((FirstByte & 0b11111110)==0b11000110)) {
			uint16 FinalData;
			bool SignExtension = FirstByte & 0b00000010;
			bool WordData = FirstByte & 0b00000001;
		
			//CommandChunkCursor++; 
			i++;
			uint8 SecondByte = CommandChunkCursor[i];
			
			// Get required info from second byte
			uint8 Mode = SecondByte >> 6;
			uint8 R_M = SecondByte & 0b00000111;
			
			// We can get the command name here
			// mov
			if((FirstByte & 0b11111110)==0b11000110) {
				strcpy(StringCommand, "mov");
				LinesOfAsm[NumberOfLines].Command = MOV;	
			// add/sub/cmp
			} else {
				uint8 DeterminingBits = (SecondByte & 0b00111000) >> 3;
				strcpy(StringCommand, GetCommandFromDeterminingBits(DeterminingBits));
				LinesOfAsm[NumberOfLines].Command = GetCommandCodeFromDeterminingBits(DeterminingBits);				
			}

			// this is true unlesss REGISTER_MODE
			LinesOfAsm[NumberOfLines].EffectiveAddressInDestination = true;
			if(Mode==REGISTER_MODE) {
					//strcpy(StringDestination, GetRegisterString(R_M, WordData));	
					LinesOfAsm[NumberOfLines].DestinationRegister = GetRegisterCode(R_M, WordData);		
					LinesOfAsm[NumberOfLines].EffectiveAddressInDestination = false;
			} else {
				char StringMemoryAddress[32];
				strcpy(StringMemoryAddress, "[");		
				
				if (Mode==MEMORY_MODE_MOSTLY_NO_DIS) {
					// Direct Address
					if(R_M==0b110){
						//CommandChunkCursor++; 
						i++;
						uint8 ThirdByte = CommandChunkCursor[i];
						//CommandChunkCursor++; 
						i++;
						uint8 FourthByte = CommandChunkCursor[i];
						uint16 D16Displacement = ((uint16) FourthByte << 8) | ((uint16) ThirdByte);
						LinesOfAsm[NumberOfLines].Displacement = D16Displacement;
						LinesOfAsm[NumberOfLines].EffectiveAddress = DIRECT_ADDR;			
					
					} else {
						LinesOfAsm[NumberOfLines].EffectiveAddress = GetEffectiveAddressCode(R_M, Mode);
						
					}
				} else if(Mode==MEMORY_MODE_8BIT_DIS) {
					//CommandChunkCursor++; 
					i++;
					uint8 ThirdByte = CommandChunkCursor[i];
					
					LinesOfAsm[NumberOfLines].EffectiveAddress = GetEffectiveAddressCode(R_M, Mode);
					if(ThirdByte>0) {
						LinesOfAsm[NumberOfLines].Displacement = ThirdByte;					
					}
				
				} else if(Mode==MEMORY_MODE_16BIT_DIS) {
					//CommandChunkCursor++; 
					i++;
					uint8 ThirdByte = CommandChunkCursor[i];
					
					//CommandChunkCursor++; 
					i++;
					uint8 FourthByte = CommandChunkCursor[i];
					
					LinesOfAsm[NumberOfLines].EffectiveAddress = GetEffectiveAddressCode(R_M, Mode);
					
					uint16 D16Displacement = ((uint16) FourthByte << 8) | ((uint16) ThirdByte);
					if(D16Displacement>0) {
						LinesOfAsm[NumberOfLines].Displacement = D16Displacement;					
					}
				}
				
				strcpy(StringDestination, StringMemoryAddress);
			}
			i++;
			uint8 FirstDataByte = CommandChunkCursor[i];
			uint8 SecondDataByte = 0;
			if(WordData&&(LinesOfAsm[NumberOfLines].Command==MOV||(SignExtension==false))) {
				//CommandChunkCursor++; 
				i++;
				uint8 SecondDataByte = CommandChunkCursor[i];
			}
			
			FinalData = ((uint16) SecondDataByte << 8) | ((uint16) FirstDataByte);
			
			LinesOfAsm[NumberOfLines].Data = FinalData;
			LinesOfAsm[NumberOfLines].HasData = true;
		}
		Registers = RunASMLine(LinesOfAsm[NumberOfLines], Registers);
		PrintASMLine(LinesOfAsm[NumberOfLines]);
		PrintOutRegisters(Registers);
		NumberOfLines++;
		//CommandChunkCursor++;
		//i++;
	}
	
	printf("NumberOfLines: %i\n", NumberOfLines);
	PrintASMLines(LinesOfAsm, NumberOfLines);
	//Registers = RunASMLines(LinesOfAsm, NumberOfLines, Registers);
	
	return 0;
}