#ifndef STACK_TEMPLATE_STACK_H
#define STACK_TEMPLATE_STACK_H


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


/*!
 \file stack.c
 \mainpage
 \brief
 \note
*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


typedef int Data_type;


#define STACK_NULL -1
#define CANARY_ERROR -2
#define CANARY_DATA_ERROR -3
#define CHECK_SUM_ERROR -4
#define SIZE_ERROR -5
#define DATA_ERROR -6
#define EMPTY_ERROR -7

#define CANARY 2
#define EXTRA_SIZE 10


#define canary 0xBEDABEDA


#define UNIT_TEST(WHAT, OP, EXPECTED)											\
	do {																		\
		Data_type result = (WHAT);												\
		if(result OP EXPECTED);					\
		else 																	\
		{																		\
			printf(#WHAT"is %d, but %s be %d\n", result,  #OP, EXPECTED);		\
			return -1;															\
		}																		\
	}while(0)																	\


#define UNIT_TEST_POINTER(WHAT, OP, EXPECTED)									\
	do {																		\
		struct Stack * result = (WHAT);											\
		if(result OP EXPECTED);						\
		else 																	\
		{																		\
			 printf(#WHAT"is NULL, but %s be NULL\n", #OP);						\
			return -1;															\
		}																		\
	}while(0)																	\
	
	
#define CHECK_START					\
	int error = stack_OK(stack);	\
									\
		if(error != 0)				\
			return error;			\
			
#define CHECK						\
	error = stack_OK(stack);		\
									\
		if(error != 0)				\
			return error;			\
			
			
#define CHECK_END					\
	error = stack_OK(stack);		\
	return error;					\


struct Stack * stack_create(int size);
int stack_destroy(struct Stack * stack);

int stack_push(struct Stack * stack, Data_type new_element);
Data_type stack_pop(struct Stack * stack);

Data_type stack_get(struct Stack * stack);
int stack_size(struct Stack * stack);
int stack_clear(struct Stack * stack);

int stack_OK(struct Stack * stack);
void print_error(int error);
int unit_test();


Data_type sum_of_elements(struct Stack * stack);


/*!
 * \param canary_begin - Extreme structure element to protect against overlap. 
 * \param n_data - number of data. 
 * \param size - size of stack
 * \param data - Data_type massiv.
 * \param control_sum - element wich save sum of all elements (for check). 
 * \param canary_end - Extreme structure element to protect against overlap. 
*/


struct Stack
{
	int canary_begin;
	
    int n_data;
    int size;
    Data_type * data;
    Data_type control_sum;
     
    int canary_end;
};


#include "stack.c"

#endif //STACK_H
