#include"AssemblerArchecture.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
//statemnet use
//1.use in passone.c special code LTORG
//2.use in passone.c every process end
void insertLine(struct statement *ptr,int count)
{
	struct statement *child=ptr->next;
	int i;
	for(i=0;i<conNumber;i++)
	{
		lineNumber[count]++;
		
		strcpy(line[lineNumber[0]].mnemonic,con[i].name);
		strcpy(line[lineNumber[0]].symbolic,"*");	
		strcpy(line[lineNumber[0]].data,"\0");
		line[lineNumber[0]].defineUse=1;
		line[lineNumber[0]].format=0;
		ptr->next=&line[lineNumber[0]];
		ptr=ptr->next;
		if(con[i].name[1]=='C')
		{
			line[lineNumber[0]].length=6;
			line[lineNumber[0]].format=3;
		}
		else if(con[i].name[1]=='X')
		{
			line[lineNumber[0]].length=2;
			line[lineNumber[0]].format=1;
		}
		lineNumber[0]++;
		strcpy(con[i].name,"\0");
	}
	conNumber=0;
	ptr->next=child;
}



//statment use end
//obtab use
//
/*find optab give name to find it information in optab
 * give s return it optab number
 * if not find return -1
 *
 * 1.the function use in init.c inputLine function
 * 2.the function use in passone.c passFirst function check mnemonic
 */
int findOptab(char *s)
{
	int i,j;
	for(i=0;i<opNumber;i++)
	{
		if(!strcmp(s,optab[i].opcode))
		{
			return i;
		}
	}
	return -1;
}
//findOptab end

/*use in symtab function
 *
 * to insert symbolic to symtab funcction
 * 1.use in passone.c passFirst check root
 * 2.use in passone.c firstCheckSymbol
 */
void insertSymbol(char *s,int pr)
{
	symtab[symNumber].loc=pr;
	strcpy(symtab[symNumber].symbolic,s);
	symNumber++;
	//printf("%s %2X\n",symtab[symNumber-1].symbolic,symtab[symNumber-1].loc);
}

//function end 

int findSymtab(char *s)
{
	int i,j;
	for(i=0;i<symNumber;i++)
	{
		if(!strcmp(s,symtab[i].symbolic))
		{
			return i;
		}
	}
	return -1;
}

//symtab use end
//
//constant use start
int findConstant(char *s)
{
        int i,j;
        for(i=0;i<conNumber;i++)
        {
                if(!strcmp(s,con[i].name))
                {
                        return i;
                }
        }
        return -1;
}

void insertConstant(char *s)
{
	if(findConstant(s)==-1)
	{
		strcpy(con[conNumber++].name,s);	
	}
}
//use in every file

//chang string to hexadecimal function
//1.passone.c passFirst function Start
//
int stringToHexadecimal(char *s)
{
	int a=0;
	int i=0;
	int h=1;
	for(i=strlen(s)-1;i>=0;i--)
	{
		if(s[i]>'9'||s[i]<'0')
		{
			 printf("%s can not change to integer\n");
                         exit(1);
		}
		a+=(s[i]-'0')*h;
		h*=16;
	}
	return a;
}
//function end
//
//function string to decimal start
//1.use int passone.c firstCheckMnemonic check "RESW"
//2.use int passone.c firstCheckMnemonic check "RESB"
int stringToDecimal(char *s)
{
	int a=0;
	int i=0;
	int d=1;
	for(i=strlen(s)-1;i>=0;i--)
	{
		if(s[i]<'0'||s[i]>'9')
		{
			printf("%s can not change to integer\n"); 
			exit(1);
		}
		a+=(s[i]-'0')*d;
		d*=10;
	}
	return a;
}
//function end

//every end
