#include"AssemblerArchecture.h"
#include<stdio.h>
#include<string.h>
//build optab function using in main.c
void bulidOptab()
{
	FILE *file;//file use to read opcodes file
	file=fopen("opcodetable.txt","r");
	
	char s[1024];//s use read every line in file
	char tempcode[3];//record code two digits
	if(file==NULL)
	{
		printf("not found opcodes");
		return;
	}
	else
	{
		//when s !=NULL it have an opcode to send into optab 
		while(fgets(s,1024,file)!=NULL)
		{
			int i;
			for(i=0;i<strlen(s);i++)
			{
				if(s[i]==' ')
					break;
				//store name
				optab[opNumber].opcode[i]=s[i];
			}
			//store code in temp
			tempcode[0]=s[24];
			tempcode[1]=s[25];
			//store  tempcode in twocode
			sscanf(tempcode,"%X",&optab[opNumber].twocode);
			//store fotmat
			optab[opNumber].informat=s[27]-'0';
			//printf("%d %s %2X",opNumber,optab[opNumber].opcode,optab[opNumber].twocode);
			//printf("%d\n",optab[opNumber].informat);
			opNumber++;
		}
		fclose(file);			
	}
}
void buildRegister()//use in main
{
	FILE *file;
	char s[1024];
	file=fopen("registertable.txt","r");
	if(NULL==file)
	{
		printf("找不到暫存器\n");
		return; 
	}
	else
	{
		while(fgets(s,1024,file)!=NULL)
		{
			symtab[symNumber].symbolic[0]=s[0];
			char tempString[3];
			tempString[0]=s[2];
			sscanf(tempString,"%X",&symtab[symNumber].loc);
			//printf("%s %X\n",symtab[symNumber].symbolic,symtab[symNumber].loc);
			symNumber++;
		}
		fclose(file);
	}
}		
