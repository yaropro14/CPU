/*!
 \file CPU.c
 \mainpage
 \brief
 \note
*/


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>


#define ERROR -1
#define N_REGS 4


/*!
\file
* \brief Stack calss.
* There are all needed functions for work with stack.
*/


#include "stack.h"


/*!
 * It's size of memory of the CPU.
 */


#define MEM_SIZE 4096


/*!
* This define is used to execute the command
*/


#define CMD_DEF(name, num, code)	\
	case num: code break;


/*!
 * It's size of memory of the CPU.
 */


int MEM [MEM_SIZE];



/*!
 * It's registors of the CPU.
 */


int reg[4];


#define RAX reg[0];
#define RBX reg[1];
#define RCX reg[2];
#define RDX reg[3];



/*!
 * It's a program counter of the CPU.
 */


int PC;


int run(struct Stack * prog_stack, struct Stack * ret_lable_stack);

void loading_commands(FILE * f_bite_code);

void print_regs();
void memory_dump();


/*!
 * This defines are used for separating argument and command
 */


#define MASK_COM 0xff0000
#define MASK_ARG 0x00ffff
#define MAKE_COM(x) ((x & 0xff0000) >> 16)


/*!
 * \brief Opens file f_bite_code and la.
 * \warning Main uses file wich you write in launching the CPU.
 * \return 0 - if program finished correct, -1 - if there is any problems.
 * \throw "Error: file can't open" if there are problems opening a file.
*/


int main(int argc, char * argv[])
{
	struct Stack * prog_stack = stack_create(10);
	struct Stack * ret_lable_stack = stack_create(10);
	
	FILE * f_bite_code = fopen(argv[1], "r");
	
	if (f_bite_code == NULL)
	{
		printf ("Error: file can't open f_bite_code");
		return -1;
	}
	
	unit_test();
	
	//printf("1");
	loading_commands(f_bite_code);
	//printf("1");
	memory_dump();
	
	int error = run(prog_stack, ret_lable_stack);
	
	return 0;
}


/*!
 * \brief Execute the program.
 * \param [in] prog_stack - programe stack.
 * \param [in] ret_lable_stack - lables stack.
 * \param [out] error.
 * \return error, 0 if all correct, -1 if there are any problems.
 * \throw "commands.h" is used in this function.
 * \throw "Assert ret_lable_stack" if there are problems with the transfer of ret_lable_stack.
 * \throw "Assert prog_stack" if there are problems with the transfer of prog_stack.
*/	


int run(struct Stack * prog_stack, struct Stack * ret_lable_stack)
{
	assert(prog_stack);
	assert(ret_lable_stack);
	
	int error = 0;
	
	int com = 0;
	
	int arg;
	int j = 0;
	do
	{
		//printf("before PC =%d mem = %x\n", PC, MEM[PC]);
		com = MEM[PC];
		
		PC ++;
		
		arg = com & MASK_ARG;
		
		//printf("!!%x!! !!%x!!\n", MAKE_COM(com), arg);
		switch (MAKE_COM(com))
		{
			//scanf("%d", &j);
			//printf("%x\n", com);
			//printf("%d\n", com & MASK_COM);
			#include "commands.h"
			
			default : 
				printf("Unknown command.\nIf you want to continue press 'q', else no press 'n'\n");
				
				char choice = 0;
				
				scanf("%c", &choice);
				
				if (choice == 'q')
					error = ERROR;
				else 
					return ERROR;
		}
		//printf("PC =%d mem = %x\n", PC, MEM[PC]);
		//stack_print(prog_stack);
		//print_regs();
		
	} while(com & MASK_COM);
	
	stack_destroy(prog_stack);
	stack_destroy(ret_lable_stack);
	
	return error;
}


/*!
 * \brief Read comamnds from input file and write their to the memory.
 * \param [in]f_bite_code - File with commands(bite file).
 * \throw "Assert f_bite_code " if there are problems with the transfer of f_bite_code.
*/


void loading_commands(FILE * f_bite_code)
{
	assert(f_bite_code);
	
	int com = 0;
	
	int i = 0;
	
	fscanf(f_bite_code, "%x", &com);
	
	while(!feof(f_bite_code))
	{
		//printf("%x\n", com);
		MEM[PC] = com;
		PC ++;
		fscanf(f_bite_code, "%x", &com);
	}
	
	PC = 0;
}


/*!
 * \brief Print regisrers.
*/


void print_regs()
{
	for(int i = 0; i < N_REGS; i++)
		printf("\t%d\t", reg[i]);
		printf("\n");
}


/*!
 * \brief Print all memory.
*/	


void memory_dump()
{
	//printf("%d\n", MEM_SIZE);
	FILE * f_MEM = fopen("MEMORY.txt", "w");
	//printf("1");
	for(int i = 0; i < MEM_SIZE; i++)
	{
		//printf("%d", i);
		fprintf(f_MEM, "%04x : %06x\n", i, MEM[i]);
	}
	
	fclose(f_MEM);
}
