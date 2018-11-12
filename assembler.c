/*!
 \file assembler.c
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

#define RAX 0
#define RBX 1
#define RCX 2
#define RDX 3

enum COMMANDS
{
	#include "com.list.c"
};


/*#define PUSH_VAL 11 	//14
#define PUSH_ADR 12		//14
#define PUSH_REG 13		//15
#define POP_VAL 21		//125
#define POP_ADR 22		//26
#define POP_REG 23		//27
#define ADD 3			//3
#define SUB 4			//4
#define MUL 5			//5
#define DIV 6			//6
#define OUT 7			//7
#define END 10			//12
*/

typedef struct initial_command
{
	char * com;
	char * arg;
}str_com;


struct LABLES
{
	char ** lable;
	int num[100];
	int number;
};

struct LABLES lables;


typedef struct command
{
	char com;
	int arg;
	char* label;
} command;


char * file_read(FILE * f, long int * file_length);
long int size_of_file(FILE * f_in);

char ** make_strings(char * copy_text, long int * s_num);
long int num_of_str(char * str);

command * make_commands(char ** com_text_str, long int s_num);
command * make_jmp_command(char ** com_text_str, command * commands , int s_num);
str_com make_str_coms(char * com_str, long int s_num);

void do_lable(char * lable, int n_com);
int search_lable(char * lable);

void file_print(char ** text_str, long int s_num, FILE * f_out);
void file_print_asm(command * com, long int s_num, FILE * f_out);
void file_print_bin(command * com, long int s_num, FILE * f_out);


int main(int argc, char * argv[])
{
	FILE * f_com = fopen(argv[1], "r");
	FILE * f_out_asm = fopen("assembler.txt", "w");
	FILE * f_out_bin = fopen("bin.txt", "w");
	
	if (f_com == NULL)
	{
		printf ("Error: file can't open f_com");
		return -1;
	}
	
	if (f_out_asm == NULL)
	{
		printf ("Error: file can't open f_out_asm");
		return -1;
	}
	
	if (f_out_bin == NULL)
	{
		printf ("Error: file can't open f_out_bin");
		return -1;
	}
	
	lables.lable =(char **) calloc(1, sizeof(char*));
	
	long int file_length = 0;
	char * com_text = file_read(f_com, &file_length);
	assert(com_text);
	
	long int s_num = 0;
	
	char ** com_text_str = make_strings(com_text, &s_num);
	assert(com_text_str);
	
	command * commands = make_commands(com_text_str, s_num);
	commands = make_jmp_command(com_text_str, commands , s_num);
	
	
	
	file_print_asm(commands, s_num, f_out_asm);
	file_print_bin(commands, s_num, f_out_bin);
	
	
	free(commands);
	free(com_text);
	free(com_text_str);
	
	fclose(f_com);
	fclose(f_out_asm);
	fclose(f_out_bin);
	
	return 0;
}


char * file_read(FILE * f_in, long int * file_length)
{
	assert(f_in);
	assert(file_length);
	
	*file_length = size_of_file(f_in);
	
	char * str = (char *) calloc(*file_length, sizeof(char));
	assert(str);
	
	fread(str, *file_length, sizeof (*str), f_in);
	assert(str);
	

	return str;
}


long int size_of_file(FILE * f_in)
{
	assert(f_in);
	
	fseek(f_in, 0, SEEK_END);
	int file_length = ftell(f_in);
	rewind(f_in);
	
	if(file_length == -1){
		printf("Error in function size_of_file");
		return -1;
	}
	
	return file_length;
}


char ** make_strings(char * copy_text, long int * s_num)
{
	assert(copy_text);
	* s_num = num_of_str(copy_text);
	
	char ** text_str = (char **) calloc(* s_num, sizeof(char*));
	
	char * p = copy_text;
	char * t = copy_text;
	
	p = strchr(t, '\n');
	
	int i = 0;
	while(p != NULL)
	{
		*p = '\0';
		text_str[i] = t;
		i++;
		t = p + 1;
		p = strchr(t, '\n');
	}
	
	text_str[i++] = t;
	return text_str;
}


long int num_of_str(char * str)
{
	assert(str);
	
	int num = 1;
	char * t = str;
	
	char * p = strchr(t, '\n');
	
	while(p != NULL)
	{
		num ++;
		t = p + 1;
		p = strchr(t, '\n');
	}
	
	return num;
}


str_com make_str_coms(char * com_str, long int s_num)
{
	//printf("1\n");
	assert(com_str);

	str_com  com;

	//------------------------------------------------------------------
	
	char * p = com_str;
	char * t = com_str;

	
	p = strchr(t, ' ');
	if(p)
	{
		//printf("2\n");
		*p = '\0';
		com.com = t;
		t = p + 1;
		
		p = strchr(t, '\n');
		
		if(p)
			*p = '\0';
		
		//------------------------------------------------------------------

		com.arg = t;
		//printf("super%s\n", com.arg);
		//printf ("I am HERE !!!!!!!!!!\n");
		//------------------------------------------------------------------
	}
	else
	{
		//printf("3\n");
		com.com = t;
		//printf("4\n");
	}
	//printf("5\n");
	//printf("super%s\n", com.arg);
	
	return com;
}


command * make_jmp_command(char ** com_text_str, command * commands , int s_num)
{
		for(int i = 0; i < s_num - 1; i ++)
		{
			if(commands[i].com >= JMP && commands[i].com < JW)
			{
				if(commands[i].com ==  JMP)
				{
					commands[i].com = JMP;
					commands[i].arg = search_lable(commands[i].label);
				}

				if(commands[i].com ==  JA)
				{
					commands[i].com = JA;
					commands[i].arg = search_lable(commands[i].label);
				}

				if(commands[i].com ==  JB)
				{
					commands[i].com = JB;
					commands[i].arg = search_lable(commands[i].label);

				}

				if(commands[i].com ==  JE)
				{
					commands[i].com = JE;
					commands[i].arg = search_lable(commands[i].label);
				}

				if(commands[i].com ==  JAE)
				{
					commands[i].com = JAE;
					commands[i].arg = search_lable(commands[i].label);
				}

				if(commands[i].com ==  JBE)
				{
					commands[i].com = JBE;
					commands[i].arg = search_lable(commands[i].label);
				}

				if(commands[i].com ==  JNE)
				{
					commands[i].com = JNE;
					commands[i].arg = search_lable(commands[i].label);
				}
			}
			else if(commands[i].com == CALL)
				{
					commands[i].com = CALL;
					commands[i].arg = search_lable(commands[i].label);
				}
				
		}
	return commands;
}


command * make_commands(char ** com_text_str, long int s_num)
{
	//printf("1\n");
	command * commands = (command *) calloc(s_num, sizeof(command));
	int n_com = 0;
	for(int i = 0; i < s_num; i ++)
	{
		//printf("i = %d\n", i);
		
		str_com com = make_str_coms(com_text_str[i], s_num);
		//printf("%s\n", com.arg);
		//printf("1%s\n", com.com);//111111111111111111111111111111
		if(strncmp(com.com, "PUSH", 3) == 0)
		{
			if(strchr(com.arg,'['))
			{
				com.arg ++;
				char * t = strchr(com.arg,']');
				* t = '\0';
				
				commands[i].com = PUSH_ADR;
				commands[i].arg = atoi(com.arg);
			}
			else if(strchr(com.arg, 'R'))
			{
				if(strchr(com.arg, 'A'))
					commands[i].arg = RAX;
					
				else if(strchr(com.arg, 'B'))
					commands[i].arg = RBX;
					
				else if(strchr(com.arg, 'C'))
					commands[i].arg = RCX;
				
				else if(strchr(com.arg, 'D'))
					commands[i].arg = RDX;
				
				commands[i].com = PUSH_REG;
			}
			else
			{
				char *restrict endptr;
				commands[i].com = PUSH_VAL;
				commands[i].arg = atoi(com.arg);
			}
				
			//commands[i].com = PUSH;
		}
			
		else if(strncmp(com.com, "POP", 3) == 0)
		{
			//printf("1%s\n", com.arg);
			
			if(com.arg)
			{
				if(strchr(com.arg,'['))
				{
					
					com.arg ++;
					char * t = strchr(com.arg,']');
					* t = '\0';
				
					commands[i].com = POP_ADR;
					commands[i].arg = atoi(com.arg);
				}
				else if(strchr(com.arg, 'R'))
				{
					//printf("1\n");
					if(strchr(com.arg, 'A'))
						commands[i].arg = RAX;
						
					else if(strchr(com.arg, 'B'))
						commands[i].arg = RBX;
						
					else if(strchr(com.arg, 'C'))
						commands[i].arg = RCX;
					
					else if(strchr(com.arg, 'D'))
						commands[i].arg = RDX;
					
					commands[i].com = POP_REG;
				}
			}
			else
			{
				commands[i].com = POP_VAL;
			}
			
			
				
			//commands[i].com = PUSH;
		}
		
		else if(strncmp(com.com, "ADD", 3) == 0) 
		{
			commands[i].com = ADD;
		}
			
		else if(strncmp(com.com, "SUB", 3) == 0) 
		{
			commands[i].com = SUB;
		}
			
		else if(strncmp(com.com, "MUL", 3) == 0)
		{
			commands[i].com = MUL;
		}
			
		else if(strncmp(com.com, "DIV", 3) == 0)
		{
			commands[i].com = DIV;
		}
			
		else if(strncmp(com.com, "OUT", 3) == 0)
		{
			commands[i].com = OUT;
		}
			
		else if(strncmp(com.com, "END", 3) == 0)
		{
			commands[i].com = END;
			//printf("%d %d", i, commands[i].com);
		}
		
		else if(strncmp(com.com, "JMP", 3) == 0)
		{
			commands[i].com = JMP;
			commands[i].arg = search_lable(com.arg);
			commands[i].label = com.arg;
		}
		
		else if(strncmp(com.com, "JA", 3) == 0)
		{
			commands[i].com = JA;
			commands[i].arg = search_lable(com.arg);
			commands[i].label = com.arg;
		}
		
		else if(strncmp(com.com, "JB", 3) == 0)
		{
			commands[i].com = JB;
			commands[i].arg = search_lable(com.arg);
			commands[i].label = com.arg;
		}
		
		else if(strncmp(com.com, "JE", 3) == 0)
		{
			commands[i].com = JE;
			commands[i].arg = search_lable(com.arg);
			commands[i].label = com.arg;
		}
		
		else if(strncmp(com.com, "JAE", 3) == 0)
		{
			commands[i].com = JAE;
			commands[i].arg = search_lable(com.arg);
			commands[i].label = com.arg;
		}
		
		else if(strncmp(com.com, "JBE", 3) == 0)
		{
			commands[i].com = JBE;
			commands[i].arg = search_lable(com.arg);
			commands[i].label = com.arg;
		}
		
		else if(strncmp(com.com, "JNE", 3) == 0)
		{
			commands[i].com = JNE;
			commands[i].arg = search_lable(com.arg);
			commands[i].label = com.arg;
		}
		
		else if(strncmp(com.com, "CALL", 4) == 0)
		{
			
			commands[i].com = CALL;
			commands[i].arg = search_lable(com.arg);
			commands[i].label = com.arg;
		}
		
		else if(strncmp(com.com, "RET", 3) == 0)
		{
			commands[i].com = RET;
		}
		
		else if(strncmp(com.com, ".=", 2) == 0)
		{	
			//printf("%s %D\n", com.arg, n_com);
			commands[i].com = LABEL;
			do_lable(com.arg, n_com);
			n_com --;
		}
		
		n_com ++;
	}
	return commands;
}

void do_lable(char * lable, int n_com)
{
		assert(lable);
		
		int i = 0;
		
		/*for( i = 0; i < lables.number; i++)
		{
			if(strcmp(lable, lables.lable[i]) == 0)
			lables.num[lables.number - 1] = n_com;
			printf("1 %s, %d\n", lables.lable[lables.number - 1], lables.num[lables.number - 1]);
		}*/
			lables.number ++;
			//printf("1\n");
			lables.lable = (char **) realloc(lables.lable, lables.number * sizeof(char*));
			//printf("1\n");
			lables.lable[lables.number - 1] = lable;
			//printf("1\n");
			lables.num[lables.number - 1] = n_com;
			//printf("1\n");
			//printf("2 %s, %d\n", lables.lable[lables.number - 1], lables.num[lables.number - 1]);
			//printf("1\n");
}


int search_lable(char * lable)
{
	assert(lable);
	//printf("%d\n", lables.number);
	//printf("1\n");
	int i = 0;
	//printf("2\n");
	for(i = 0; i < lables.number; i++)
	{
		//printf("%s", lables.lable[i]);
		//printf("3\n");
		if(strcmp(lable, lables.lable[i]) == 0)
			//printf("4\n");
			//printf("%d", lables.num[i]);
			return lables.num[i];
	}
	//printf("5\n");
	return 0;
}


void file_print(char ** text_str, long int s_num, FILE * f_out)
{
	assert(text_str);
	assert(f_out);
	
	for(int i = 0; i < s_num; i++)
	{
		printf("%s\n", text_str[i]);
	}
	
	fprintf(f_out, "\n");
}


void file_print_asm(command * com, long int s_num, FILE * f_out)
{
	assert(com);
	assert(f_out);
	
	for(int i = 0; i < s_num; i++)
	{
		if((com[i].com >= PUSH_VAL && com[i].com <= PUSH_REG )|| (com[i].com >= POP_VAL && com[i].com <= POP_REG) || (com[i].com >= JMP && com[i].com <= JW) || (com[i].com == CALL))
			fprintf(f_out, "%d %d\n", com[i].com, com[i].arg);
			
		else
			fprintf(f_out, "%d\n", com[i].com);
	}
}

void file_print_bin(command * com, long int s_num, FILE * f_out)
{
	assert(com);
	assert(f_out);
    char comn = 0;
    int arg = 0;
    char* buf = (char*)calloc(10, sizeof(char));
	//buf[0] = 90;
	//buf[1] = 120;
	//int retFrite = fwrite(&buf, sizeof(char), 1, f_out);
    for(int i = 0; i < s_num - 1; i++)
	{
		//printf("%x\n", com[i].com);
		if(com[i].com == 0x10)
		{
			//printf("No\n");
		} 
		
		else if((com[i].com >= PUSH_VAL && com[i].com <= PUSH_REG )|| (com[i].com > POP_VAL && com[i].com <= POP_REG) || (com[i].com >= JMP && com[i].com <= JW) || (com[i].com == CALL))
		{
			//printf("YeS\n");
			fprintf(f_out, "%02x%04x\n", com[i].com, com[i].arg);
		}

		else
		{
			fprintf(f_out, "%02x0000\n", com[i].com);
		}
	}
}
