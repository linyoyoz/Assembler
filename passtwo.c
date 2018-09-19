#include"AssemblerArchecture.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int formatZeroCheck(struct statement *);
int formatTwoCheck(struct statement *);
int formatThreeCheck(struct statement *);
int formatFourCheck(struct statement *);
void passSecond(int count)
{
	int readLine=0;
	struct statement *ptr;
	for(ptr=root;ptr!=NULL;ptr=ptr->next)
	{
		int opcode=0;
		if(readLine==lineNumber[count])
		{
			break;
		}
		else
		{
			readLine++;
		}
		//end
		//
		//read every format from 1~4
		//first define it is constant or not
		if(!strcmp(ptr->symbolic,"*"))
		{
			if(ptr->mnemonic[1]=='C')
			{
				sscanf("454F46","%X",&opcode);
			}
			else if(ptr->mnemonic[1]=='X')
			{
				int i,j=0;
				char temp[20];
				for(i=3;i<strlen(ptr->mnemonic)-1;i++)
				{
					temp[j++]=ptr->mnemonic[i];
				}
				for(;j<strlen(ptr->mnemonic);j++)
				{
					temp[j]='\0';
				}
				//printf("%s\n",temp);
				sscanf(temp,"%X",&opcode);
			}
		}
		else
		{
			//if format = 0
			if(ptr->format==0)
			{
				opcode=formatZeroCheck(ptr);
			}
			//format =2 have 1 or 2 register
			else if(ptr->format==2)
			{
				opcode=formatTwoCheck(ptr);
			}
			else if(ptr->format==3)
			{
				opcode=formatThreeCheck(ptr);
			}
			else if(ptr->format==4)
			{
				opcode=formatFourCheck(ptr);
			}
		}
		ptr->obcode=opcode;
		//printf("%04X %2d %-8s",ptr->loc,readLine,ptr->symbolic);
                //printf("%-8s %-20s",ptr->mnemonic,ptr->data);
                //printf("%06X %2d",ptr->obcode,ptr->addressing);
                //printf("%2d %2d\n",ptr->format,ptr->length);
	}
}
//function end
//
//function check index and constant start
int checkIndexAndConstant(struct statement *ptr)
{
	int opcode=0;
	char temp[20];
	int i=0;
	for(i=0;i<strlen(ptr->data);i++)
	{
		if(ptr->data[i]==',')
		{
			 break;
		}
		else
		{
			temp[i]=ptr->data[i];
		}
	}		
	for(;i<strlen(temp);i++)
	{
		temp[i]='\0';
	}
	if(findSymtab(temp)!=-1)
	{
		opcode+=symtab[findSymtab(temp)].loc+(1<<20)+(1<<23);
	}
	else
	{
		 if(ptr->data[0]<='9'&&ptr->data[0]>='0')
		 {
			opcode+=stringToHexadecimal(ptr->data);
		 }
		 else
		 {
			printf("%s not declare in data\n",temp);
			exit(1);
		 }
	}
	return opcode;
}
//emd
//start


//start
int formatFourCheck(struct statement *ptr)
{
	int opcode=(optab[findOptab(ptr->mnemonic)].twocode+ptr->addressing)<<24;
	if(findSymtab(ptr->data)!=-1)//data is in symtab
	{
		opcode+=symtab[findSymtab(ptr->data)].loc+(1<<20);
	}
	else//data is constant or index
	{
		opcode+=checkIndexAndConstant(ptr);
	}
	return opcode;
}
//start
int formatThreeCheck(struct statement *ptr)
{
	int opcode=0;
	/*
	opcode = optab 2 bits + addressing code
	if immediate address line addressing = 1
	if indirect address  line addressing = 2
	lse line addressing = 3
	*/
	opcode=(optab[findOptab(ptr->mnemonic)].twocode+ptr->addressing)<<16;
	//if data != NULL
	if(strcmp(ptr->data,"\0"))
	{
		int sym=findSymtab(ptr->data);
		if(sym!=-1)
		{
			if(ptr->addressing!=0)
			{
				int target=(symtab[sym].loc-ptr->loc-ptr->format);
				if(target<=2047&&target>=-2048)
				//p=1,b=0
				{
					/*
  					target have 12 bits
					p=1<<13
					*/
					target=target&4095;
					opcode+=((1<<13)+(target));
				}
				else
				//p=0,b=1
				{
					target=base.baseloc-symtab[sym].loc;
					target=target&4095;
					//b=1<<14
					opcode+=(1<<14)+(target);
				}	
			}
			
		}
		else
		{
			opcode+=checkIndexAndConstant(ptr);
		}
	}
	return opcode;
}
//format2 check start
int formatTwoCheck(struct statement *ptr)
{
	int opcode=(optab[findOptab(ptr->mnemonic)].twocode)<<8;
	//use one register
	if(strlen(ptr->data)==1)
	{
		opcode+=symtab[findSymtab(ptr->data)].loc<<4;
	}
	//use two register
	else
	{
		char tempString[3];
		memset(tempString,'\0',sizeof(tempString));
		tempString[0]=ptr->data[0];
		opcode+=(symtab[findSymtab(tempString)].loc<<4);
		tempString[0]=ptr->data[2];
		opcode+=(symtab[findSymtab(tempString)].loc);
	}
	return opcode;
}
int formatZeroCheck(struct statement *ptr)
{
	int opcode=0;
	return 0;
}
//format check end
