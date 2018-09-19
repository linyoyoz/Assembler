#include"AssemblerArchecture.h"
#include<stdio.h>
#include<string.h>
#include<stdbool.h>

void symtabInitial()
{
	int i;
	for(i=0;i<MAX;i++)
        {
		//test
		//if(i<symNumber)
			//printf("%s %02X\n",symtab[i].symbolic,symtab[i].loc);
                
		strcpy(symtab[i].symbolic,"\0");
                symtab[i].loc=0;
        }
	symNumber=0;
	buildRegister();
}
//use for initial program every struct use in main.c
void programInitial()
{
	int i;
	
	//statement struct initial start
        for(i=0;i<MAX;i++)
        {
                strcpy(line[i].data,"\0");
                strcpy(line[i].mnemonic,"\0");
                strcpy(line[i].symbolic,"\0");
                line[i].length=0;
                line[i].format=0;
                line[i].addressing=3;//simple addressing
                line[i].loc=0;
                line[i].obcode=0;
                line[i].next=NULL;
		line[i].defineUse=0;
        }
	nowLoc=0;
        fileNumber=1;//start in line[lineNumber[1]];
        for(i=0;i<20;i++)
        {
                lineNumber[i]=0;
        }
	//statement struct initial end
	//
	//optab struct initial start
	for(i=0;i<MAX;i++)
        {
                strcpy(optab[i].opcode,"\0");
                optab[i].twocode=0;
                optab[i].informat=0;
        }
        opNumber=0;
	//optab struct initial end
	
	//symtab struct initial start
	for(i=0;i<MAX;i++)
	{
		strcpy(symtab[i].symbolic,"\0");
		symtab[i].loc=0;
	}	
	symNumber=0;
	//symtab struct end
	
	//base addressing start
	base.use=0;
        base.baseloc=0;
        strcpy(base.name,"\0");
	//base addressing end
	//
	//constantData initialized start
	for(i=0;i<MAX;i++)
	{
		strcpy(con[i].name,"\0");
	}
	conNumber=0;
	//constantData end
}
//the function end

//the function use bool to determine x is empty or not
bool empty(char *x)
{
	if(*x==9||*x==32||*x==10||*x==13)//x==empty
		return true;
	else
		return false;
}
//empty end

// the function use input s to line

//this function use in openFile function
void inputLine(char *s)
{
	int i,j;

	char temp[20];//use in first record s character
	char da[20];//use in second record temp data
	char mnem[20];//use in second recond temp mnemonic
	char sym[20];//use in second record temp symbolic
	memset(temp,'\0',sizeof(temp));
	memset(da,'\0',sizeof(da));
	memset(mnem,'\0',sizeof(mnem));
	memset(sym,'\0',sizeof(sym));
	
	/*
 * 	block record one line block
 * 	block=0 record empty
 * 	block=1 record symbol|mnemonic
 * 	block=2 record empty
 * 	block=3 record mnemonic
 * 	block=4 record empty
 * 	block=5 record data
 * 	block=6 record empty
	*/
	int block=0;
	
	//read s from left to right
	for(i=0;i<strlen(s);i++)
	{
		//block=0 
		if(!empty(&s[i])&&block==0)
		{
			if(s[i]=='.')//s=information
			{
				break;
			}
			j=0;
			if(s[i]=='+')//s=format4
			{
				line[lineNumber[0]].format=4;
				continue;//'+'not to record
			}
			block=1;
		}
		//block=1 and s[i] not empty
		if(!empty(&s[i])&&block==1)
		{
			//use first line command record to temp
			temp[j++]=s[i];
		}
		//block=1 and s[i] is empty
		if(empty(&s[i])&&block==1)
		{
			//because sometimes j!= temp length-1 so tmep must reduce length to j-1
			for(;j<=strlen(temp);j++)
			{
				temp[j]='\0';
			}
				
			//printf("t:%d %d %s \n",j,strlen(temp),temp);	
			
			block=2;
			
			if(findOptab(temp)!=-1)//in optab
			{
				block=4;//no symbolic
				strcpy(mnem,temp);//copy temp to mnem
			}
			else
			{
				strcpy(sym,temp);//copy temp to sym
			}
		}
		if(!empty(&s[i])&&block==2)
		{
			//restart j
			j=0;
			block=3;
			
			if(s[i]=='+')
			{
				line[lineNumber[0]].format=4;
				continue;
			}
		}
		if(!empty(&s[i])&&block==3)
		{
			mnem[j++]=s[i];
		}
		if(empty(&s[i])&&block==3)
		{
			//reduce mnem lenth to j-1
			for(;j<=strlen(mnem);j++)
			{
				mnem[j]='\0';
			}
			//printf("\n%d %d %s\n",j,strlen(mnem),mnem);
			block=4;
		}
		if(!empty(&s[i])&&block==4)
		{
			block=5;
			j=0;
			if(s[i]=='@')
			{
				line[lineNumber[0]].addressing=2;
				continue;
			}
			else if(s[i]=='#')
			{
				line[lineNumber[0]].addressing=1;
				continue;
			}
		}
		if(!empty(&s[i])&&block==5)
		{
			da[j++]=s[i];
		}
		if(empty(&s[i])&&block==5)
		{
			for(;j<=strlen(da);j++)
                        {
                                da[j]='\0';
                        }
			break;
		}	
	}
	if(block==0)//this line is empty
		return;
	/*scan line end
	
	find line format*/
	if(line[lineNumber[0]].format!=4)
	{
		line[lineNumber[0]].format=optab[findOptab(mnem)].informat;
	}
	line[lineNumber[0]].length=line[lineNumber[0]].format*2;
	/*format end
 * 	
 * 	copy three statement*/

	strcpy(line[lineNumber[0]].symbolic,sym);
	strcpy(line[lineNumber[0]].mnemonic,mnem);
	strcpy(line[lineNumber[0]].data,da);
	j=strlen(da);
	for(;j<strlen(line[lineNumber[0]].data);j++)
		line[lineNumber[0]].data[j]='\0';
	//copy end
	
	//link list
	//	
	if(lineNumber[0]!=0)
		line[lineNumber[0]-1].next=&line[lineNumber[0]];
	else
		root=&line[lineNumber[0]];
	//link list end
	
	//file add
	if(!strcmp(line[lineNumber[0]].mnemonic,"CSECT"))
	{
		fileNumber++;	
	}
	//file end
	//
	//printf("%2d  %2d  %2d  ",fileNumber,lineNumber[0],lineNumber[fileNumber]);
	//printf("%-8s %-6s %-20s",line[lineNumber[0]].symbolic,line[lineNumber[0]].mnemonic,line[lineNumber[0]].data);
	//printf("  %d %d %d\n",line[lineNumber[0]].format,line[lineNumber[0]].addressing,line[lineNumber[0]].defineUse);
	
	//number add
	lineNumber[0]++;
	lineNumber[fileNumber]++;
}
//the function end

//the function use in open test file

//this function use in main.c
void openFile(char *name)
{
	FILE *file;
	char s[1024];
	file=fopen(name,"r");
	if(NULL==file)
	{
		printf("找不到檔案\n");
		return; 
	}
	else
	{
		while(fgets(s,1024,file)!=NULL)
		{
			//printf("%s",s);
			inputLine(s);
		}
	}
}
//the function end
