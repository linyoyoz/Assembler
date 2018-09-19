/*this file name AssemblerArchecture 
 *6~48 define statement struct
 *
 */
#include<stdbool.h>

#define MAX 2000
#ifdef CREAT
	#define AUTOX
#else
	#define AUTOX extern
#endif

//statement start

//定義了program 裡面的一行statement的資訊
struct statement
{
/*
 * 	addressing:0~3
 * 	0-> sic 
 * 	1-> indirect addressing 
 * 	2->immediate addressing 
 * 	3->sic/xe simple address
*/
	int addressing;
/*
 * 	format:0~4
 * 	record this statement format	
 */
	int format;
	
	/*defineUse
	true(1):this line is define
	symbol not send to symtab
	opcode not to check
	false(0):this line is simple
	symbol and opcode is normal	
	*/
	bool defineUse;
	

	char symbolic[20];
	char mnemonic[20];
	char data[20];
	
	int loc;
	int obcode;
	int length;//record this statement length
	struct statement *next;//record next number
}line[MAX],*root;
//record root about statement

AUTOX int fileNumber;//record files number
AUTOX int lineNumber[20];//record one file lines number

AUTOX int nowLoc;//record now loc

//statement struct end

//optab start

//define operator table struct
struct opcodes
{
	int informat;//0~4 record operator format
	int twocode;//two digits for 16 record obcode for operator
	char  opcode[20];//operator name
}optab[MAX];

AUTOX int opNumber;//record operator number

//optab end
//
//define symbolic table
struct symtablics
{
	int loc;//define symtab memory location
	char symbolic[20];//define symbolic name
}symtab[MAX];
AUTOX int symNumber;

//symtab end
//
//define base addressing 
//use in record base
struct BaseAddressing
{
	int baseloc;
	char name[20];
	int use;
}base;

//base addressing end
//
//temp constant struct 
//for use record data have '=' is use to represent constant
struct constantData
{
	char name[20];
}con[MAX];
AUTOX int conNumber;

//constandData end
