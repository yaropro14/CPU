//commands.h

CMD_DEF(PUSH_VAL, 0x11, 
{
	stack_push(prog_stack, arg);
})


CMD_DEF(PUSH_ADR, 0x12, 
{
	stack_push(prog_stack, MEM[arg]);
})


CMD_DEF(PUSH_REG, 0x13, 
{
	stack_push(prog_stack, reg[arg]);
})


CMD_DEF(POP_VAL, 0x21, 
{
	stack_pop(prog_stack);
})


CMD_DEF(POP_ADR, 0x22, 
{
	MEM[arg] = stack_pop(prog_stack);
})


CMD_DEF(POP_REG, 0x23, 
{
	reg[arg] = stack_pop(prog_stack);
})


CMD_DEF(ADD, 0x3, 
{
	if(prog_stack->n_data < 2)
	{
		printf("Too few elements in stack\n");
	}
	else    stack_push(prog_stack, stack_pop(prog_stack) + stack_pop(prog_stack));
})


CMD_DEF(SUB, 0x4, 
{
	if(prog_stack->n_data < 2)
	{
		printf("Too few elements in stack\n");
	}
	else    stack_push(prog_stack, - stack_pop(prog_stack) + stack_pop(prog_stack));
})


CMD_DEF(MUL, 0x5, 
{
	if(prog_stack->n_data < 2)
	{
		printf("Too few elements in stack\n");
	}
	else    stack_push(prog_stack, stack_pop(prog_stack) * stack_pop(prog_stack));
})


CMD_DEF(DIV, 0x6, 
{
	if(prog_stack->n_data < 2)
	{
		printf("Too few elements in stack\n");
	}
	else
	{
		int x = stack_pop(prog_stack);
		int y = stack_pop(prog_stack);
		stack_push(prog_stack, y / x);
	}
})


CMD_DEF(SQRT, 0x15, 
{
	if(prog_stack->n_data < 1)
	{
		printf("Too few elements in stack for function\n");
		//pirntf("\n");
	}
	else
	{
		int x = stack_pop(prog_stack);
		x = sqrt(x);
		stack_push(prog_stack, x);
	}
})


CMD_DEF(OUT, 0x7, 
{
	printf("%d\n", stack_get(prog_stack));
})


CMD_DEF(CALL, 0x8, 
{
	stack_push(ret_lable_stack, PC);
	PC = arg;
})


CMD_DEF(RET, 0x9, 
{
	PC = stack_pop(ret_lable_stack);
})


CMD_DEF(JMP, 0x31, 
{
	PC = arg;
})


CMD_DEF(JA, 0x32, 
{
	if(stack_pop(prog_stack) < stack_pop(prog_stack))	PC = arg;
})


CMD_DEF(JB, 0x33, 
{
	if(stack_pop(prog_stack) > stack_pop(prog_stack))	PC = arg;
})


CMD_DEF(JE, 0x34, 
{
	if(stack_pop(prog_stack) == stack_pop(prog_stack))	PC = arg;
})


CMD_DEF(JAE, 0x35, 
{
	if(stack_pop(prog_stack) <= stack_pop(prog_stack))	PC = arg;
})


CMD_DEF(JBE, 0x36, 
{
	if(stack_pop(prog_stack) >= stack_pop(prog_stack))	PC = arg;
})


CMD_DEF(JNE, 0x37, 
{
	if(stack_pop(prog_stack) != stack_pop(prog_stack))	PC = arg;
})


CMD_DEF(END, 0x20, 
{
	//stack_print(prog_stack);
	stack_destroy(prog_stack);
	stack_destroy(ret_lable_stack);
	return 0;
})
