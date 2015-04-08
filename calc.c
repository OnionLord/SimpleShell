#include<stdio.h>
#include<stdlib.h>


char pop_trans(char * stack, int *top)
{
	
	char result = stack[*top];
	(*top)--;
	return result;
}

void push_trans(char * stack, char m, int *top)
{
	(*top)++;
	stack[*top] = m;
}

int pop_eval(int * stack, int *top)
{
	int result = stack[*top];
	(*top)--;
	return result;
}

void push_eval(int * stack, int m, int *top)
{
	(*top)++;
	stack[*top] = m;
}

int judge_rank(char a)
{
	if( a == '(')
	{
		return 17;
	}
	else if (a == ')' )
	{
		return 17;
	}
	else if ( a == '*' || a =='/' || a=='%')
	{
		return 13;
	}
	else if( a =='+' || a == '-' )
	{
		return 12;
	}
	else if ( a=='<' || a== '>' || a =='C' || a == 'D' )
	{
		return 10;
	}
	else if (a=='F')
	{
		return 4;
	}
	else if( a== 'E')
	{
		return 5;
	}
	else if( a == 'A' || a == 'B' )
	{
		return 9;
	}
	else
	{
		return 0;
	}
}

char make_operate(char * ex_orig, int *locate, char m)
{
	char result = ' ';
	if ( m == '+' || m == '-' || m =='*' || m == '/' || m =='%' || m =='(' || m ==')')
	{
		result = m;
	}
	else if( m== '=')
	{
		if( ex_orig[(*locate)+1] == '=')
		{
			result = 'A';
		}
	}
	else if( m== '!')
	{
		if( ex_orig[(*locate)+1] == '=')
		{
			result = 'B';
		}
	}
	else if( m== '<')
	{
		if( ex_orig[(*locate)+1] == '=')
		{
			result = 'C';
		}
		else
		{
			result = m;
		}
	}
	else if( m== '>')
	{
		if( ex_orig[(*locate)+1] == '=')
		{
			result = 'D';
		}
		else
		{
			result = m;
		}
	}
	else if( m== '&')
	{
		if( ex_orig[(*locate)+1] == '&')
		{
			result = 'E';
		}
	}
	else if( m== '|')
	{
		if( ex_orig[(*locate)+1] == '|')
		{
			result = 'E';
		}
	}
	return result;
}

void translation(char * ex_orig, char * ex_result, int size_ex, int *result_loc)
{
	char *trans_stack;
	int i;
	int top = -1;
	int stack_size = 1;
	char optor;
	char temp;
	int judge;
	int equals = 0;
	int j;

	trans_stack = (char *)malloc(sizeof(char));

	for ( i = 0 ; i < size_ex - 1 ; i ++ )
	{
		if ( ex_orig[i] >= 48 && ex_orig[i] <= 57)//숫자
		{

			ex_result[*result_loc] = ex_orig[i];
			(*result_loc)++;
		}
		else //일반 문자
		{
			optor = make_operate(ex_orig, &i, ex_orig[i]);
			if(optor != ' ')
			{
				if(top == -1)
				{
					if(stack_size <= top + 1)
					{
						stack_size *= 2;
						trans_stack = (char *)realloc(trans_stack, sizeof(char) * stack_size);
					}
					push_trans(trans_stack, optor, &top);
				}
				else
				{
					if(optor == ')')
					{
						while(1)
						{
							temp = pop_trans(trans_stack, &top);
							if(temp == '(')
							{
								break;
							}
							else
							{
								ex_result[*result_loc] = temp;
								(*result_loc)++;
							}
						}
					}
					else if(optor == '(')
					{
						if(stack_size <= top + 1)
						{
							stack_size *= 2;
							trans_stack = (char *)realloc(trans_stack, sizeof(char) * stack_size);
						}
						push_trans(trans_stack, optor, &top);
					}
					else if(judge_rank(optor) <= judge_rank(trans_stack[top]) && trans_stack[top] != '(')
					{
						ex_result[*result_loc] = pop_trans(trans_stack, &top);
						(*result_loc)++;
						if(stack_size <= top + 1)
						{
							stack_size *= 2;
							trans_stack = (char *)realloc(trans_stack, sizeof(char) * stack_size);
						}
						push_trans(trans_stack, optor, &top);
					}
					else
					{
						if(stack_size <= top + 1)
						{
							stack_size *= 2;
							trans_stack = (char *)realloc(trans_stack, sizeof(char) * stack_size);
						}
						push_trans(trans_stack, optor, &top);
					}

				}
			}
		}
	}
	while(top != -1)
	{
		ex_result[*result_loc] = pop_trans(trans_stack, &top);
		(*result_loc)++;
	}

	free(trans_stack);
}

int eval(char * ev_orig, int ev_size)
{
	int *eval_stack;
	int top = -1;
	int result;
	int a, b;
	int stack_size = 1;
	int i, j;
	
	eval_stack = (int *)malloc(sizeof(int));
	for ( i = 0 ; i < ev_size ; i ++ )
	{
		if ( ev_orig[i] >= 48 && ev_orig[i] <= 57)
		{
			if(stack_size <= top + 1 )
			{
				stack_size *= 2;
				eval_stack = (int *)realloc(eval_stack,sizeof(int) * stack_size);
			}
			push_eval(eval_stack, (int)(ev_orig[i] - 48), &top);
		}
		else
		{
			if(ev_orig[i] == '+')
			{
				result = pop_eval(eval_stack, &top) + pop_eval(eval_stack, &top);
				push_eval(eval_stack, result, &top);
			}
			else if(ev_orig[i] == '-')
			{
				a = pop_eval(eval_stack, &top);
				b = pop_eval(eval_stack, &top);
				result = b - a;
				push_eval(eval_stack, result, &top);
			}
			else if(ev_orig[i] == '*')
			{
				a = pop_eval(eval_stack, &top);
				b = pop_eval(eval_stack, &top);
				result = b * a;
				push_eval(eval_stack, result, &top);
			}
			else if(ev_orig[i] == '/')
			{
				a = pop_eval(eval_stack, &top);
				b = pop_eval(eval_stack, &top);
				result = b / a;
				push_eval(eval_stack, result, &top);
			}
			else if(ev_orig[i] == '%')
			{
				a = pop_eval(eval_stack, &top);
				b = pop_eval(eval_stack, &top);
				result = b % a;
				push_eval(eval_stack, result, &top);
			}
			else if(ev_orig[i] == '<')
			{
				a = pop_eval(eval_stack, &top);
				b = pop_eval(eval_stack, &top);
				result = b > a;
				push_eval(eval_stack, result, &top);
			}
			else if(ev_orig[i] == '>')
			{
				a = pop_eval(eval_stack, &top);
				b = pop_eval(eval_stack, &top);
				result = b > a;
				push_eval(eval_stack, result, &top);
			}
			else if(ev_orig[i] == 'A')
			{
				a = pop_eval(eval_stack, &top);
				b = pop_eval(eval_stack, &top);
				result = (b == a);
				push_eval(eval_stack, result, &top);
			}
			else if(ev_orig[i] == 'B')
			{
				a = pop_eval(eval_stack, &top);
				b = pop_eval(eval_stack, &top);
				result = (b != a);
				push_eval(eval_stack, result, &top);
			}
			else if(ev_orig[i] == 'C')
			{
				a = pop_eval(eval_stack, &top);
				b = pop_eval(eval_stack, &top);
				result = (b <= a);
				push_eval(eval_stack, result, &top);
			}
			else if(ev_orig[i] == 'D')
			{
				a = pop_eval(eval_stack, &top);
				b = pop_eval(eval_stack, &top);
				result = (b >= a);
				push_eval(eval_stack, result, &top);
			}
			else if(ev_orig[i] == 'E')
			{
				a = pop_eval(eval_stack, &top);
				b = pop_eval(eval_stack, &top);
				result = (b && a);
				push_eval(eval_stack, result, &top);
			}
			else if(ev_orig[i] == 'F')
			{
				a = pop_eval(eval_stack, &top);
				b = pop_eval(eval_stack, &top);
				result = (b || a);
				push_eval(eval_stack, result, &top);
			}
		}
	}
	return eval_stack[top];
}

int main()
{
	char *trans_stack;
	int *eval_stack;
	char *ex_orig;
	char *ex_result;
	int result_loc = 0;
	char temp;
	//FILE * f = fopen("input.txt", "r");
	int i;
	int size_ex = 0;

	ex_orig = (char *)malloc(sizeof(char));
	ex_result = (char *)malloc(sizeof(char));
	

	while(!feof(stdin))
	{
		fscanf(stdin, "%c", &temp);
		if (temp != ' ' && temp != NULL)
		{
			size_ex++;
			ex_orig = (char *)realloc(ex_orig, sizeof(char) * (size_ex+1));
			ex_result = (char *)realloc(ex_result, sizeof(char) * (size_ex+1));
			ex_orig[size_ex-1] = temp;	
		}
		
	}
	
	translation(ex_orig, ex_result, size_ex, &result_loc);
	/*for ( i = 0 ; i < result_loc ; i ++ )
	{
		switch(ex_result[i])
		{
			case 'A':printf("== ");break;
			case 'B':printf("!= ");break;
			case 'C':printf("<= ");break;
			case 'D':printf(">= ");break;
			case 'E':printf("&& ");break;
			case 'F':printf("|| ");break;
			default:printf("%c ", ex_result[i]);
		}
	}
	printf("\t");*/
	printf(" = %d\n", eval(ex_result, result_loc));



	free(ex_orig);
	free(ex_result);
	//fclose(f);

	return 0;
}
