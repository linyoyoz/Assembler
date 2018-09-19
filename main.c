#include<stdio.h>
#define CREAT
#include"AssemblerArchecture.h"
#undef CREAT
void print(int);
int main(int argc,char *args[])
{
	int i;
	programInitial();
	bulidOptab();
	buildRegister();
	openFile(args[1]);
	for(i=1;i<=fileNumber;i++)
	{
		passFirst(i);
		passSecond(i);
		print(i);
		symtabInitial();
	}
}
void print(int count)
{
	struct statement *ptr;
	int read=0;
	for(ptr=root;ptr!=NULL;ptr=ptr->next)
	{
		if(read==lineNumber[count])
		{
			root=ptr;
			break;
		}
		else
		{
			read++;
		}
		if(strcmp(ptr->mnemonic,"END")&&strcmp(ptr->mnemonic,"EXTREF")&&strcmp(ptr->data,"EXTDEF")&&strcmp(ptr->mnemonic,"LTORG"))
			printf("%04X  ",ptr->loc);
		else
			printf("      ");
		printf("  %-8s ",ptr->symbolic);
		if(ptr->format==4)
			printf("+%-8s ",ptr->mnemonic);
		else
			printf(" %-8s ",ptr->mnemonic);
		if(ptr->addressing==1)
			printf("#%-20s ",ptr->data);
		else if(ptr->addressing==2)
			printf("@%-20s ",ptr->data);
		else
			printf(" %-20s ",ptr->data);
		switch(ptr->length)
		{
			case 0:printf("\n");break;
			case 2:printf("%02X\n",ptr->obcode);break;
			case 4:printf("%04X\n",ptr->obcode);break;
			case 6:printf("%06X\n",ptr->obcode);break;
			case 8:printf("%08X\n",ptr->obcode);break;
		}
	}	
}
