#include"AssemblerArchecture.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void firstCheckSymbol(char *,int);
void firstCheckMnemonic(struct statement *,int);

void passFirst(int count)
{
	struct statement *ptr;
	int readNumber=0;
	//scan from start to next csect
	for(ptr=root;ptr!=NULL;ptr=ptr->next)
	{
		//count line 
		if(readNumber==lineNumber[count])
		{	
			break;
		}
		else
		{
			readNumber++;
		}
		/*
 * 		if ptr==NULL or readNumber==lineNumber break;
 * 		and new root ptr->next
 		*/
		//the first line
		if(ptr==root)
		{
			//if code = start
			if(!strcmp(ptr->mnemonic,"START"))
			{
				//use temp to recode data
				int temp;
				temp=stringToHexadecimal(ptr->data);
				ptr->loc=temp;
				nowLoc=temp;
			}
			else//include CSECT and no START
			{
				//initialize loc and nowLoc
				nowLoc=0;
				ptr->loc=0;
			}
			//if symbolic is not empty then it insert symtab
			if(strcmp(ptr->symbolic,"\0"))
			{
				insertSymbol(ptr->symbolic,ptr->loc);
			}
		}//the first line end
		else if(!strcmp(ptr->mnemonic,"END"))
		{
			insertLine(ptr,count);	
		}
		else
		{
			//give loc value to this line
			ptr->loc=nowLoc;
			
 			//determine this line have mnemonic or not
 			//check mnemonic
			if(strcmp(ptr->mnemonic,"\0"))
			{
				firstCheckMnemonic(ptr,count);
			}
			if(ptr->data[0]=='=')
			{
				insertConstant(ptr->data);
			}
			 /*
 *  *                      determine this line have symbolic or not
 *   *                      check symbolic and record in symtab
 *                       */
			if(strcmp(ptr->symbolic,"\0")&&ptr->defineUse==0)
                        {
                                firstCheckSymbol(ptr->symbolic,ptr->loc);
                        }
			else if(ptr->defineUse==1)
			{
				if(findSymtab(ptr->mnemonic)==-1)
					insertSymbol(ptr->mnemonic,ptr->loc);
			}
			/*
                      	 end symbolic check
  			*/

		}
		/*printf("%04X %2d %-8s",ptr->loc,readNumber,ptr->symbolic);    
                printf("%-8s %-20s",ptr->mnemonic,ptr->data);
                printf("%2d",ptr->addressing);
                printf("%2d %2d \n",ptr->format,ptr->length);
		*/
		//printf("%s\n",line[2].next->mnemonic);
	}
}
//EQU struct postfix
struct postfixExpression
{
	int op;
	int data;
	struct postfix *next;
}post[2000];

int postNum=0;
/*use pstack
1. to stack operand change format to postfix function 
that top=0 and pstack[0]=8

2. to stack constant change postfix to expression loc function
*/
int pstack[2000];
//(,),*,+,-,/
const int isp[]={0,19,13,12,12,12,12,13,0};
const int icp[]={20,19,13,12,12,12,12,13,0};
int top=-1;
int pop()
{
	if(top!=-1)
	return pstack[top--];
}

void push(int x)
{
	pstack[++top]=x;
}
//struct end
//
//function to change postfix to expression
int eval()
{
	int i=0;
	int op1=0;
	int op2=0;
	for(i=0;i<postNum;i++)
	{
		if(post[i].op==0)//constant
		{
			push(post[i].data);
		}
		else
		{
			if(top>=1)
			{
				op2=pop();
				op1=pop();
			}
			switch(post[i].data+40)
			{
				case '+':push(op1+op2);break;
				case '-':push(op1-op2);break;
				case '*':push(op1*op2);break;
				case '/':push(op1/op2);break;
				default: printf("EQU have error operator %c\n",post[i].data+40);
				exit(1);
			}
		}
	}
	return pop();
}
//function end

//passone to check EQU data return this line Loc
void postfix(char *s)
{
	//input one empty character
	pstack[0]=8;
	top=0;
        int i,j=0;
        char temp[20];
	postNum=0;
	//1. first scan
	for(i=0;i<=strlen(s);i++)
	{
		/*2.if s[i] is operand
 * 		   else s[i] is one character of constant then record
		'('=40 '*' = 42 '/'=47||i == final character
		*/
		if(empty(&s[i]))//
			continue;
		if((s[i]>=40&&s[i]<=47||i==strlen(s)))
		{
			//make temp length=j-1
			for(;j<strlen(temp);j++)
                       	{
				temp[j]='\0';
                     	}
			//if temp != empty then make it into postfix
			if(strlen(temp)!=0)
			{
				int sym=findSymtab(temp);
				if(sym==-1)
				{
					printf("sorry %s not to found\n",temp);
					exit(1);
				}
                		post[postNum].data=symtab[findSymtab(temp)].loc;
				post[postNum++].op=0;
			}
			//constant end
			j=0;
			//the scan end
			if(i==strlen(s))
			{
				//pop all stack in pstack and 
				//record into postfix
				int x;
				while((x=pop())!=8&&top!=-1)
				{
					post[postNum].data=x;
					post[postNum++].op=1;
				}
				break;
			}
			//operand start
			//postfix algorithm
			if(s[i]==')')
			{
				while(pstack[top]!='('-40&&top!=-1)
				{
					//record operand to postfix
					post[postNum].data=pop();
					post[postNum++].op=1;
				}
				pop();
			}
			else
			{
				//printf("%c ",s[i]);
				int us=s[i]-40;//record s[i] into index line
				while(isp[pstack[top]]>=icp[us]&&top!=-1)
				{
					post[postNum].data=pop();
					post[postNum++].op=1;
				}
				push(us);
			}
		}
		else
		{
			temp[j++]=s[i];//if s[i]!=operand temp record s[i]
		}
	}
	/*test
	for(i=0;i<postNum;i++)
	{
		if(post[i].op)
		printf("%c ",post[i].data+40);
		else
		printf("%X ",post[i].data);
	}
	printf("\n");*/
}
//function end
//
//passone one check mnemonic function
void firstCheckMnemonic(struct statement *ptr,int count)
{
	/*there are some special opcode should check
 * 	to find next loc
	*/
	if(findOptab(ptr->mnemonic)==-1&&strcmp(ptr->symbolic,"*"))
	{
		printf("%s is not availd operator\n",ptr->mnemonic);
		exit(1);
	}
	//1.BYTE
	if(!strcmp(ptr->mnemonic,"BYTE"))
	{
		//this is device
		if(ptr->data[0] == 'X')
		{
			ptr->length=2;
			nowLoc+=1;
		}
		//this is index
		else if(ptr->data[0]=='C')
		{
			ptr->length=6;
			nowLoc+=strlen(ptr->data)-3;
		}
	}
	//2.WORD
	else if(!strcmp(ptr->mnemonic,"WORD"))
	{
		nowLoc+=3;	
		ptr->length=6;
	}
	//3.RESW
	else if(!strcmp(ptr->mnemonic,"RESW"))
	{
		int temp=stringToDecimal(ptr->data);
		nowLoc+=3*temp;
                ptr->length=0;	
	}
	//4.RESB
	else if(!strcmp(ptr->mnemonic,"RESB"))
	{
		int temp=stringToDecimal(ptr->data);
                nowLoc+=temp;
                ptr->length=0;
	}
	else if(!strcmp(ptr->mnemonic,"EQU"))
	{
		if(!strcmp(ptr->data,"*"))
		{
			ptr->loc=nowLoc;
		}
		//use stack to define prefix 
		else
		{
			//check EQU data format
			postfix(ptr->data);
			ptr->loc=eval();
		}
	}
	else if(!strcmp(ptr->mnemonic,"LTORG"))
	{
		insertLine(ptr,count);	
	}
	else if(!strcmp(ptr->mnemonic,"EXTREF"))
	{
		//temp record every data
		char temp[20];
		int i=0,j=0;
		for(i=0;i<=strlen(ptr->data);i++)
		{
			if(ptr->data[i]==','||i==strlen(ptr->data))
			{
				for(j;j<strlen(temp);j++)
				{
					temp[j]='\0';
				}
				j=0;
				insertSymbol(temp,0);
			}
			else
			{
				temp[j++]=ptr->data[i];
			}
		}
	}
	else
	{
		nowLoc+=ptr->format;
	}
		
}

//passtwo twocheck symbolic function
void firstCheckSymbol(char *s,int loc)
{
	if(findSymtab(s)==-1)
        {
		 insertSymbol(s,loc);
        }
        else
        {
		printf("%s duplicate symbol\n",s);
		exit(1);
        }
	
}
