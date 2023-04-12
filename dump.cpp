/*FILE *WriteFile;
	char WriteFilename[255];
	sprintf(WriteFilename, "%s_Disassembled.asm",ReadFilename);
	WriteFile = fopen(WriteFilename,"w");
	if(WriteFile == NULL)
	{
		printf("Error!");   
		exit(1);             
	}*/
	
	/*
uint8 AX = 0xFF & 0x00000000;
uint8 CX = 0xFF & 0x00000001;
uint8 DX = 0xFF & 0x00000010;
uint8 BX = 0xFF & 0x00000011;
uint8 SP = 0xFF & 0x00000100;
uint8 BP = 0xFF & 0x00000101;
uint8 SI = 0xFF & 0x00000110;
uint8 DI = 0xFF & 0x00000110;

uint8 AL = 0xFF & 0x00000000;
uint8 CL = 0xFF & 0x00000001;
uint8 DL = 0xFF & 0x00000010;
uint8 BL = 0xFF & 0x00000011;
uint8 AH = 0xFF & 0x00000100;
uint8 CH = 0xFF & 0x00000101;
uint8 DH = 0xFF & 0x00000110;
uint8 BH = 0xFF & 0x00000110;
*/

#define MOV 136
#define AX 0x00000000;
#define CX 0x00000001;
#define DX 0x00000010;
#define BX 0x00000011;
#define SP 0x00000100;
#define BP 0x00000101;
#define SI 0x00000110;
#define DI 0x00000110;

#define AL 0x00000000;
#define CL 0x00000001;
#define DL 0x00000010;
#define BL 0x00000011;
#define AH 0x00000100;
#define CH 0x00000101;
#define DH 0x00000110;
#define BH 0x00000110;

// MOV command
			// immediate to register
		if((FirstByte & 0b11110000)==0b10110000||
			// register/memory to/from register
			(FirstByte & 0b11111100)==0b10001000||
			// immediate to register/memory
			(FirstByte & 0b11111110)==0b10001000||
			// memory to accumulator
			(FirstByte & 0b11111110)==0b10100000||
			// accumulator to memory
			(FirstByte & 0b11111110)==0b10100010||
			// register/memory to segment register
			FirstByte==0b10001110||
			// segment register to register/memory
			FirstByte==0b10001100) {
				strcpy(StringCommand, "mov");
		
		// Could be ADD or SUB or CMP
				    // reg/memory with register to either
		} else if ((FirstByte & 0b11111100)==0b00000000||
					// immediate to register/memory
				   (FirstByte & 0b11111100)==0b10000000||
				    // immediate to accumulator 
				   (FirstByte & 0b11111110)==0b00000100||
			){
			strcpy(StringCommand, "add");
		}