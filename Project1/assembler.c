#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);

/* project 1 */

// R-type			// binary code
#define ADD  		0x00000000
#define NOR  		0x00400000

// I-type
#define LW   		0x00800000
#define SW   		0x00C00000
#define BEQ  		0x01000000

// J-type
#define JALR 		0x01400000

// O-type
#define HALT 		0x01800000
#define NOOP 		0x01C00000

char label_list[1<<16][7];

int
dupLabel(char *label, int cnt_line) {
	int i;
	for (i = 0; i < cnt_line; i++) {
		if (!strcmp(label_list[i], label)) {
			return i;
		}
	}
	return -1;
}

int
main(int argc, char *argv[])
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], 
		 arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
	
	// input check
	if (argc != 3) {
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n", argv[0]);
		exit(1);
	}

	inFileString = argv[1];
    outFileString =argv[2];

	// input file check
	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL) {
		printf("error in opening %s\n", inFileString);
		exit(1);
	}

	// output file check
	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

	/* project 1 */

	int cnt_line = 0;
	while (1) {

		if (!readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) // end of file
			break;

		if (strlen(label) > 6) {
			printf("error: label has a maximum 6 char\n");
			exit(1);
		}

		if (!strcmp(label, "")) {
			cnt_line++;
			continue;
		}

    	if (dupLabel(label, cnt_line) != -1) {
           	printf("error: label is duplicated\n");
			exit(1);
        }

		strcpy(label_list[cnt_line++], label);
	}

	rewind(inFilePtr);

	int memory[1<<16];
	int regA, regB, offset, op, rem;

	int idx = 0;
	while (1) {
		if (!readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
			break;

		regA = regB = offset = op = rem =0;

		// R-type
		if (!strcmp(opcode, "add") || !strcmp(opcode, "nor")) {
			if (!(isNumber(arg0) && isNumber(arg1) && isNumber(arg2))) {
		        printf("error: invalid argument\n");
				printf("%s %s %s %s\n", opcode, arg0, arg1, arg2);
        		exit(1);
			}

			if (!strcmp(opcode, "add"))
				op = ADD;
			else
				op = NOR;

    		regA = atoi(arg0) << 19;
    		regB = atoi(arg1) << 16;
    		offset = atoi(arg2);

    		rem = regA | regB | offset;
		}

		// I-type
		else if (!strcmp(opcode, "lw") || !strcmp(opcode, "sw") || !strcmp(opcode, "beq")) {
			if (!(isNumber(arg0) && isNumber(arg1)) || !strcmp(arg2, "")) {
		        printf("error: invalid argument\n");
				printf("%s %s %s %s\n", opcode, arg0, arg1, arg2);
        		exit(1);
    		}

			if (!strcmp(opcode, "lw"))
				op = LW;
			else if (!strcmp(opcode, "sw"))
				op = SW;
			else if (!strcmp(opcode, "beq"))
				op = BEQ;
			
			regA = atoi(arg0) << 19;
		    regB = atoi(arg1) << 16;

    		if (isNumber(arg2))
        		offset = atoi(arg2);
    		else {
				offset = dupLabel(arg2, cnt_line);
        		
				if (offset == -1) {
            		printf("error: unknown label\n");
            		exit(1);
        		}

        		if (op == BEQ)
            		offset -= (idx + 1);
    		}

    		if (offset < -32768 || offset > 32767) {
        		printf("error: invalid offsetField range\n");
        		exit(1);
    		}
    		
			if (op == BEQ)
        		offset &= 0x0000FFFF;

    		rem =  regA | regB | offset;
		}

		// J-type
		else if (!strcmp(opcode, "jalr")) {
			if (!(isNumber(arg0) && isNumber(arg1))) {
		        printf("error: invalid argument\n");
				printf("%s %s %s\n", opcode, arg0, arg1);
        		exit(1);
    		}

			op = JALR;

    		regA = atoi(arg0) << 19;
    		regB = atoi(arg1) << 16;

    		rem = regA | regB;
		}

		// O-type
		else if (!strcmp(opcode, "halt"))
			op = HALT;
		else if (!strcmp(opcode, "noop"))
			op = NOOP;

		// .fill
		else if (!strcmp(opcode, ".fill")) {
			if (!strcmp(arg0, "")) {
				printf("error: invalid value\n");
				exit(1);
			}

			if (isNumber(arg0))
				rem = atoi(arg0);
			else {
				rem = dupLabel(arg0, cnt_line);
				
				if (rem == -1) {
					printf("error: unknown label.\n");
					exit(1);
				}
			}
		}

		// opcode name error
		else {
			printf("error: unrecognized opcode\n");
			printf("%s\n", opcode);
			exit(1);
		}

		memory[idx++] = (op | rem);
	}

	for (int i = 0; i < cnt_line; i++) {
        printf("(address %d): %d (hex 0x%x)\n", i, memory[i], memory[i]);
        if (outFilePtr != NULL)
            fprintf(outFilePtr, "%d\n", memory[i]);
    }

	exit(0);
}

int
readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0, char *arg1, char *arg2)
{
	char line[MAXLINELENGTH];
	char *ptr = line;

	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) // end of file
		return 0;

	if (strchr(line, '\n') == NULL) {
		printf("error: line too long\n");
		exit(1);
	}

	// label check
	ptr = line;
	if (sscanf(ptr, "%[^\t\n\r ]", label))
		ptr += strlen(label);

	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]", opcode, arg0, arg1, arg2);
	
	return 1;
}

int
isNumber(char *str)
{
	int i;
	return ((sscanf(str, "%d", &i)) == 1);
}
