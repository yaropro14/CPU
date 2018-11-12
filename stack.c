#ifndef STACK_C
#define STACK_C


#include "stack.h"


/*!
 * \brief Create new stack.
 * \warning Size of stack can't be 0.
 * \param [in] size - size of file.
 * \param [out] new_stack - pointer to satck.
 * \return new_stack -pointer to satck if all correct, NULL if there are any problems.
 * \throw If the function "stack_OK" finds problems in the stacks, it sends an error code to print problems.
*/


struct Stack * stack_create(int size)
{	
	if(size < 1)
		return NULL;
	
	struct Stack * new_stack = (struct Stack *) calloc(1, sizeof(struct Stack));
	
	new_stack->data = (Data_type *) calloc(size + 2, sizeof(Data_type));
	new_stack->n_data = 0; 
	new_stack->size = size + CANARY;
	
	new_stack->canary_begin = canary;
	new_stack->canary_end = canary;
	
	new_stack->data[0] = canary;
	new_stack->data[new_stack->size - 1] = canary;
	
	new_stack->control_sum = 0;
	
	int error = stack_OK(new_stack);
	
	if(error != 0)
		return NULL;
	
	return new_stack;
}


/*!
 * \brief Destroy stack.
 * \param [in] stack - stack for destroy.
 * \return  0 - if all correct.
 * \throw If the function "stack_OK" finds problems in the stacks, it sends an error code to print problems.
 * \throw "Assert stack->data " if there are problems with the transfer of stack->data.
 * \throw "Assert satck " if there are problems with the transfer of stack.
*/


int stack_destroy(struct Stack * stack)
{	
	int error = stack_OK(stack);
	
	if(error != 0)
		return error;
		
	assert(stack);
	assert(stack->data);
	
	free(stack->data);
	stack->data = NULL;
	
	free(stack);
	stack = NULL;
	
	return 0;
}


/*!
 * \brief Push an element to satck.
 * \param [in] stack.
 * \param [in] new_element.
 * \warning Type of element should be Data_type.
 * \return  0 - if all correct, -1 - if there is any problems.
 * \throw If the function "stack_OK" finds problems in the stacks, it sends an error code to print problems.
 * \throw "Assert stack->data " if there are problems with the transfer of stack->data.
 * \throw "Assert satck " if there are problems with the transfer of stack.
*/


int stack_push(struct Stack * stack, Data_type new_element)
{
	assert(stack);
	assert(stack->data);
	
	CHECK_START
		
	if (stack->n_data >= stack->size - CANARY)
	{
		stack->size += EXTRA_SIZE;
		stack->data = realloc(stack->data, sizeof(Data_type) * stack->size);
		
		stack->data[stack->size - EXTRA_SIZE - 1] = 0;
		stack->data[stack->size - 1] = canary;
	}
	
	stack->n_data ++;
	stack->data[stack->n_data] = new_element;
	stack->control_sum += new_element;
	
	CHECK_END
}


/*!
 * \brief Take an element from satck.
 * \param [in] stack.
 * \return  element - if all correct, -1 - if there is any problems.
 * \throw If the function "stack_OK" finds problems in the stacks, it sends an error code to print problems.
 * \throw "Assert stack->data " if there are problems with the transfer of stack->data.
 * \throw "Assert satck " if there are problems with the transfer of stack.
*/


Data_type stack_pop(struct Stack * stack)
{
	assert(stack);
	assert(stack->data);
	
	CHECK_START
		
	Data_type res;
	
	res = stack->data[stack->n_data];
	stack->n_data --;
	stack->control_sum -= res;
	
	
	
	if((stack->size - CANARY - stack->n_data) / EXTRA_SIZE)
	{
		stack->size -= EXTRA_SIZE;
		stack->data = realloc(stack->data, sizeof(Data_type) * stack->size);
		
		stack->data[stack->size - 1] = canary;
	}
	
	CHECK
		
	return res;	
}


/*!
 * \brief Get value an last element of satck.
 * \param [in] stack.
 * \warning Don't take an element from satck.
 * \return  value - if all correct, -1 - if there is any problems.
 * \throw If the function "stack_OK" finds problems in the stacks, it sends an error code to print problems.
 * \throw "Assert stack->data " if there are problems with the transfer of stack->data.
 * \throw "Assert satck " if there are problems with the transfer of stack.
*/


Data_type stack_get(struct Stack * stack)
{
	assert(stack);
	assert(stack->data);
	
	CHECK_START
	
	if(stack->n_data == 0)
	{
		printf("Stack is empty\n");
		error = EMPTY_ERROR;
		print_error(error);
		return -1;
	}
		
	Data_type res;
	res = stack->data[stack->n_data];
	
	error = stack_OK(stack);
	
	CHECK
	
	return res;
}


/*!
 * \brief Print elements of stack.
 * \param [in] stack.
 * \warning Don't take an element from satck.
 * \return  0 - if all correct, -1 - if there is any problems.
 * \throw If the function "stack_OK" finds problems in the stacks, it sends an error code to print problems.
 * \throw "Assert stack->data " if there are problems with the transfer of stack->data.
 * \throw "Assert satck " if there are problems with the transfer of stack.
*/


int stack_print(struct Stack * stack)
{
	assert(stack);
	assert(stack->data);
	
	CHECK_START
	
	//printf("|---------------------------------------------\n");
	printf("Print stack\n");
	//printf("|---------------------------------------------\n");
		
	Data_type i;
	
	if (stack->n_data == 0)
	{
		print_error(EMPTY_ERROR);
		return 0;
	}
		
	else
	{
		for (i = 1; i <= stack->n_data; i ++)
		{
			printf("data[%d] = %d ", i, stack->data[i]);
		}
		printf ("\n");
	}
	
	//printf("|---------------------------------------------\n");
	
	CHECK_END
}


/*!
 * \brief Get size of stack.
 * \param [in] stack.
 * \return  size - if all correct, -1 - if there is any problems.
 * \throw If the function "stack_OK" finds problems in the stacks, it sends an error code to print problems.
 * \throw "Assert stack->data " if there are problems with the transfer of stack->data.
 * \throw "Assert satck " if there are problems with the transfer of stack.
*/



int stack_size(struct Stack * stack)
{
	assert(stack);
	assert(stack->data);
	
	int error = stack_OK(stack);
	
	if(error != 0)
		return error;
		
	return stack->n_data;
}


/*!
 * \brief Resets all elements and make n_data = 0.
 * \param [in] stack.
 * \return  1 - if all correct, -1 - if there is any problems.
 * \throw If the function "stack_OK" finds problems in the stacks, it sends an error code to print problems.
 * \throw "Assert stack->data " if there are problems with the transfer of stack->data.
 * \throw "Assert satck " if there are problems with the transfer of stack.
*/


int stack_clear(struct Stack * stack)
{
	assert(stack);
	assert(stack->data);
	
	CHECK_START
	
	for(int i = 1; i <= stack->n_data; i++)
	{
		stack->data[i] = 0;
	}
	
	stack->n_data = 0;
	
	CHECK_END
}


/*!
 * \brief Checks the stack for possible errors, if any, then sends their codes to print.
 * \param [in] stack.
 * \return  0 - if all correct, -1 - if there is any problems.
 * \throw "Assert stack->data " if there are problems with the transfer of stack->data.
 * \throw "Assert satck " if there are problems with the transfer of stack.
*/



int stack_OK(struct Stack * stack)
{
	assert(stack);
	assert(stack->data);
	
	int err = 0;
	
	if(stack == NULL)
	{
		err = -1;
		print_error(STACK_NULL);
	}
		
	if(stack->canary_begin != canary || stack->canary_end != canary)
	{
		err = -1;
		print_error(CANARY_ERROR);
	}
		
	if(stack->data[0] != canary || stack->data[stack->size - 1] != canary)
	{
		err = -1;
		print_error(CANARY_DATA_ERROR);
	}
		
	if(sum_of_elements(stack) != stack->control_sum)
	{
		err = -1;
		print_error(CHECK_SUM_ERROR);
	}
		
	if(stack->size <= 0)
	{
		err = -1;
		print_error(SIZE_ERROR);
	}
		
	if(stack->n_data < 0)
	{
		err = -1;
		print_error(DATA_ERROR);
	}
		
	return err;
}


/*!
 * \brief Prints errors by their code.
 * \param [in] error - error code.
*/


void print_error(int error)
{
	switch (error){
	
		case STACK_NULL:
			printf("Stack = NULL\n");
			break;
	
		case CANARY_ERROR:
			printf("Struct canaries were died :(\n");
			break;
	
		case CANARY_DATA_ERROR:
			printf("\n");
			printf("Data canaries were died :(\n");
			break;
	
		case CHECK_SUM_ERROR:
			printf("Checksum broken\n");
			break;
	
		case SIZE_ERROR:
			printf("Stack size <= 0\n");
			break;
	
		case DATA_ERROR:
			printf("Stack data < 0\n");
			break;
	
		case EMPTY_ERROR:
			printf("Stack is empty\n");
			break;
	}
}


/*!
 * \brief Tests all functions.
 * \return  0 - if all correct, -1 - if there is any problems.
 * \throw Printf "passed" if function work propely, or print error.
*/


int unit_test()
{
	struct Stack * stack;
	
	//printf("|---------------------------------------------\n");
	
	UNIT_TEST_POINTER(stack = stack_create(1), !=, NULL);
	UNIT_TEST(stack_push(stack, 48), ==, 0);
	UNIT_TEST(stack_size(stack), ==, stack->n_data);
	UNIT_TEST(stack_get(stack), ==, 48);
	UNIT_TEST(stack_size(stack), ==, stack->n_data);
	UNIT_TEST(stack_pop(stack), ==, 48);
	UNIT_TEST(stack_clear(stack), ==, 0);
	UNIT_TEST(stack_size(stack), ==, stack->n_data);
	UNIT_TEST(stack_destroy(stack), ==, 0);
	
	//printf("|---------------------------------------------\n");
	//printf("\nAll tests passed, program work correctly :)\n\n");
	//printf("|---------------------------------------------\n");
	
	return 0;
}


/*!
 * \brief Counts the sum of stack elements.
 * \param [in] stack.
 * \return  control_summ - if all correct, -1 - if there is any problems.
 * \throw If the function "stack_OK" finds problems in the stacks, it sends an error code to print problems.
 * \throw "Assert stack->data " if there are problems with the transfer of stack->data.
 * \throw "Assert satck " if there are problems with the transfer of stack.
*/



Data_type sum_of_elements(struct Stack * stack)
{
	assert(stack);
	assert(stack->data);
	
	Data_type sum = 0;
	
	for(int i = 1; i <= stack->n_data; i++)
	{
		sum = sum + stack->data[i];
	}
	
	return sum;
}


#endif //STACK_C
