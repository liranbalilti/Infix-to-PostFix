// C program to evaluate value of a postfix 
// expression having multiple digit operands
#include <stdio.h> 
#include <string.h> 
#include <ctype.h> 
#include <stdlib.h> 
#include <math.h>

int precedence(char op1, char op2);
int OperatorP(char op);
void push(char ch);
int evaluatePostfix(char* exp);
void push(char ch);
void pushint(int num);
int popint();
char pop();
void infixTopostfix(char infix[], char postfix[]);

struct node {
	char ch;
	int num;
	struct node *next;
};

struct node *head = NULL; //my create


void push(char ch) //my push function
{     

	struct node *link = (struct node*) malloc(sizeof(struct node));  

	link->ch = ch;

	//point it to old first node
	link->next = head;

	//point first to new first node
	head = link;
}

char pop() //my pop function
{   

	//save reference to first link
	char ch;
	struct node *tempLink = head;

		ch = head->ch;
	//mark next to first link as first 
		head = head->next;

	//return the deleted link
	return ch;
}

void pushint(int num)   //my push function (for integers stack, for the evaluate function)
{

	struct node *link = (struct node*) malloc(sizeof(struct node));

	link->num = num;

	//point it to old first node
	link->next = head;

	//point first to new first node
	head = link;
}

int popint()  //my pop function (for integers stack, for the evaluate function)
{   

	int num;
	struct node *tempLink = head;

	//save reference to first link
	num = head->num;
	//mark next to first link as first 
	head = head->next;

	//return the deleted val
	return num;
}


void infixTopostfix(char infix[], char postfix[])
{
	char token;
	int i = 0, j = 0, k = 0, num = 1;
	
	while (infix[i] != '\0')
	{
		token = infix[i];
		if (token == '(')
			push(token);
		else if (token == ')')
		{
			while (head != NULL && head->ch !='(')
			{
				postfix[j] = pop(); //assume into the str elements that not '('
				j++;
				postfix[j] = ' ';  //assume into the str ' ' between each element
				j++;
			}
			if(head!=NULL)
				pop();
		}
		else if (token == '+' || token == '-' || token == '*' || token == '/' || token == '^')
		{
			while (head!=NULL && head->ch != '(' && precedence(head->ch, token))
			{
				postfix[j] = pop();  //poping from the stack until operator with lower precedence
				j++;
				postfix[j] = ' ';  //assume into the str ' ' between each element
				j++;
			}
			push(token);
		}
		else if ('0' <= token && token <= '9')
		{
			k = i;
			if ('0' <= infix[k + 1] && infix[k + 1] <= '9')  //check if the number is bigger/equal to 10
			{
				while ('0' <= infix[k + 1] && infix[k + 1] <= '9') //if biger/equal the input to my str will be without ' ' between each dig
				{
					postfix[j] = infix[k];
					j++;
					k++;
				}
			}
			else    //the num is smaller than 10, the input will be with ' ' between each dig
			{
				postfix[j] = infix[i];
				j++;
				postfix[j] = ' ';
				j++;
			}
		}
		i++;
	}
	while (head != NULL) //make sure my stack is empty when done the loop, and getting into my str(postfix)
	{
		postfix[j] = pop();
		j++;
		postfix[j] = ' ';
		j++;
	}

}

int precedence(char op1, char op2) //checking which 'bigger' operator
{
	int op1P = OperatorP(op1);
	int op2P = OperatorP(op2);

	if (op1P > op2P)
		return true;
	return false;
}

int OperatorP(char op) //checking the operators precedence by making their rank as a numbers.
{
	int weight = -1;

	if (op == '-' || op == '+')
		weight = 1;
	else if (op == '*' || op == '/')
		weight = 2;
	else if (op == '^')
		weight = 3;

	return weight;
}

int evaluatePostfix(char* exp) //evaluate the function
{
	char token;
	int i = 0, op1 = 0, op2 = 0, res = 0, num = 0;

	while(exp[i]!='\0')  
	{
		token = exp[i];

		if ('0' <= token && token <= '9')
		{
			num = 0;
			if (exp[i+1]!=' ')
			{
				while (exp[i + 1] != ' ') //if num>10 to input into my stack the whole num
				{
					num += (exp[i] - '0');
					num = num * 10;
					i++;
				}
				num += exp[i]-'0';
				pushint(num);
				i++;
			}
			else if(exp[i+1]==' ') //if the num is just a dig (smaller than 10)
			{
				pushint(token-'0');
			}
		}
		
		if (token == '+' || token == '-' || token == '*' || token == '/' || token == '^') //performing the operation on the elements
		{
			op1 = popint();
			op2 = popint();
			if (token == '+')
			{
				res = op1 + op2;
				pushint(res);
			}
			else if (token == '-')
			{
				res = op2 - op1;
				pushint(res);
			}
			else if (token == '*')
			{
				res = op1*op2;
				pushint(res);
			}
			else if (token == '/')
			{
				res = op2/op1;
				pushint(res);
			}
			else if (token == '^')
			{
				res = 1;
				while (op1 != 0)
				{
					res *= op2;
					op1--;
				}
				pushint(res);
			}
			op1 = 0;
			op2 = 0;
		}
		i++;
	}
	while(head != NULL)
		popint();

	return res;
}

void PrintExpDetails(char exp[])
{
	char sol[100] = { 0 };
	infixTopostfix(exp, sol);
	printf("Infix expresion: %s\n", exp);
	printf("Postfix expression: %s\n", sol);
	printf("It's Value: %d\n", evaluatePostfix(sol));
	printf("***********************************\n");
}


// Driver program to test the above functions 
int main()
{
	char ex[][50] = { "10*5-3", "(5-2)*13", "18-3*5", "4*5-2", "(10+3)*2", "20-3*5", "12-12/3", "(7+4)^2",
		"(4*5)^2", "(15-7)-4*(18-30)^2", "(23-2)*2^4" };

	for (int i = 0; i < 11; i++)
		PrintExpDetails(ex[i]);

	return 0;
}
