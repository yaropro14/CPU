			if(strchr(com.arg,'['))
			{
				printf("1\n");
				commands[i].com = POP_ADR;
				commands[i].arg = atoi(com.arg);
			}
			else if(strchr(com.arg, 'R'))
			{
				printf("1\n");
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
			else
			{
				printf("1\n");
				//commands[i].com = POP_VAL;
			}
