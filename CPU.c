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

#include "stack.h"


#define MEM_SIZE 4096

#define CMD_DEF(name, num, code)	\
	case num: code break;


int MEM [MEM_SIZE];


int reg[4];

/*
typedef reg[0] rax;
typedef reg[1] rbx;
typedef reg[2] rcx;
typedef reg[3] rdx;
*/

int PC;


int run(struct Stack * prog_stack, struct Stack * ret_lable_stack);

void loading_commands(FILE * f_bite_code);

void memory_dump();


#define MASK_COM 0xff0000
#define MASK_ARG 0x00ffff
#define MAKE_COM(x) ((x & 0xff0000) >> 16)



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


int run(struct Stack * prog_stack, struct Stack * ret_lable_stack)
{
	int error = 0;
	
	int com = 0;
	
	int arg;
	do
	{
		//printf("before PC =%d mem = %x\n", PC, MEM[PC]);
		com = MEM[PC];
		
		PC ++;
		
		arg = com & MASK_ARG;
		
		//printf("!!%x!! !!%x!!\n", MAKE_COM(com), arg);
		switch (MAKE_COM(com))
		{
			//printf("%x\n", com);
			//printf("%d\n", com & MASK_COM);
			#include "commands.h"
			
			default : 
				printf("Unknown command.\nIf you want to continue press 'q', else no press 'n'\n");
				
				char choice = 0;
				
				scanf("%c", &choice);
				
				if (choice == 'q')
					error = -1;
				else 
					return -1;
		}
		//printf("PC =%d mem = %x\n", PC, MEM[PC]);
		//stack_print(prog_stack);
		
	} while(com & MASK_COM);
	
	
	
	return error;
}


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


int do_push_val(int arg)
{
	return 0;
}
