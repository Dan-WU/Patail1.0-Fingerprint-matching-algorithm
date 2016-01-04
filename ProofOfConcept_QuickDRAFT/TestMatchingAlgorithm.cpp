// TestMatchingAlgorithm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
//#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include "dirent.h"
#include <windows.h>
//#include "LSMatcher.h"

#define FMD_MINUTIA_TYPE_MASK	0xC000
#define FMD_MINUTIA_TYPE_SHIFT	14
#define FMD_RESERVED_MASK		0xC000
#define FMD_RESERVED_SHIFT		14
#define FMD_X_COORD_MASK		0x3FFF
#define FMD_Y_COORD_MASK		0x3FFF

#define round(x) ((x)>=0?(double)((x)+0.5):(double)((x)-0.5))



float **fcreate_matrix(int rows, int cols);

int **icreate_matrix(int rows, int cols);

void ffree_matrix(int rows, float **mat);

void ifree_matrix(int rows, int **mat1);

void fprint_matrix(int rows, int cols, float **mat);

void iprint_matrix(int rows, int cols, int **mat1);

void fill_matrix(int rows, int cols, float **mat);

void vp_calc(int rows, int cols, int **mat);

float sd (float x1,float y1,float x2,float y2);

int getXYTFromFMR(const unsigned char* fmrData, const unsigned int fmrLen, struct iso2xyt_struct* xyt_data);

void vp_calc_1(int rows, int cols, int **mat);

float compare(int rowsf,int rowsf1);

float glocmp(int **mat, int **mat1,int rows,int rows1);

void free_list(struct basic **head);

void free_list1(struct neighbours **head);

void free_list2(struct tuples **head);

void free_list3(struct basic1 **head);

void free_list4(struct neighbours1 **head);

void free_list5(struct tuples1 **head);

void free_list6(struct scores **head);

struct basic
{
	int x;
	int y;
	float dist;
	int andf;
	struct basic *next;
}*head,*curr;

struct neighbours
{
	int na;
	float nd;
	struct neighbours *next1;
}*head1,*curr1;

struct tuples
{
	int root;
	int numofneigh;
	float dismat[300];
	int angmat[300];
	int ismatched;
	struct tuples *next2;
}*head2,*curr2;

struct basic1
{
	int x;
	int y;
	float dist;
	int andf;
	struct basic1 *next3;
}*head3,*curr3;

struct neighbours1
{
	int na;
	float nd;
	struct neighbours1 *next4;
}*head4,*curr4;

struct tuples1
{
	int root;
	int numofneigh;
	float dismat[300];
	int angmat[300];
	int ismatched;
	struct tuples1 *next5;
}*head5,*curr5;

struct scores
{
	int idi;
	int idt;
	float sco;
	struct scores *next6;
}*head6,*curr6;

struct iso2xyt_struct
{
	int nrows;
	int xcol[100];
	int ycol[100];
	int thetacol[100];
	int quality[100];
	int type[100];
};




struct basic* create_basiclist(int xf,int yf,float distf,int andff)
{

	struct basic *ptr = (struct basic*)malloc(sizeof(struct basic));
	if(NULL == ptr)
	{
		printf("\n Node creation failed \n");
		return NULL;
	}
	ptr->x = xf;
	ptr->y = yf;

	ptr->dist = distf;
	ptr->andf = andff;

	ptr->next = NULL;

	head = curr = ptr;
	return ptr;
}

struct neighbours* create_neighbourslist(float ndf, int naf)
{

	struct neighbours *ptr1 = (struct neighbours*)malloc(sizeof(struct neighbours));
	if(NULL == ptr1)
	{
		printf("\n Node creation failed \n");
		return NULL;
	}

	ptr1->nd = ndf;
	ptr1->na = naf;
	ptr1->next1 = NULL;

	head1 = curr1 = ptr1;
	return ptr1;
}

struct tuples* create_tupleslist(int rtf,int nonf,float darr[],int aarr[],int match)
{

	struct tuples *ptr2 = (struct tuples*)malloc(sizeof(struct tuples));
	if(NULL == ptr2)
	{
		printf("\n Node creation failed \n");
		return NULL;
	}
	int i;
	int nonf1;
	nonf1=(nonf)*(nonf-1);

	ptr2->root = rtf;
	ptr2->numofneigh = nonf;

	for(i=0;i<nonf1;i++)
	{
		ptr2->dismat[i]=darr[i];
	}

	for(i=0;i<nonf;i++)
	{
		ptr2->angmat[i]=aarr[i];
	}

	ptr2->ismatched=match;

	ptr2->next2 = NULL;

	head2 = curr2 = ptr2;
	return ptr2;
}

struct basic1* create_basiclist1(int xf,int yf,float distf,int andff)
{

	struct basic1 *ptr3 = (struct basic1*)malloc(sizeof(struct basic1));
	if(NULL == ptr3)
	{
		printf("\n Node creation failed \n");
		return NULL;
	}
	ptr3->x = xf;
	ptr3->y = yf;

	ptr3->dist = distf;
	ptr3->andf = andff;
	ptr3->next3 = NULL;

	head3 = curr3 = ptr3;
	return ptr3;
}

struct neighbours1* create_neighbourslist1(float ndf, int naf)
{

	struct neighbours1 *ptr4 = (struct neighbours1*)malloc(sizeof(struct neighbours1));
	if(NULL == ptr4)
	{
		printf("\n Node creation failed \n");
		return NULL;
	}

	ptr4->nd = ndf;
	ptr4->na = naf;
	ptr4->next4 = NULL;

	head4 = curr4 = ptr4;
	return ptr4;
}

struct tuples1* create_tupleslist1(int rtf,int nonf,float darr[],int aarr[],int match)
{

	struct tuples1 *ptr5 = (struct tuples1*)malloc(sizeof(struct tuples1));
	if(NULL == ptr5)
	{
		printf("\n Node creation failed \n");
		return NULL;
	}
	int i;
	int nonf1;
	nonf1=(nonf)*(nonf-1);

	ptr5->root = rtf;
	ptr5->numofneigh = nonf;

	for(i=0;i<nonf1;i++)
	{
		ptr5->dismat[i]=darr[i];
	}

	for(i=0;i<nonf;i++)
	{
		ptr5->angmat[i]=aarr[i];
	}

	ptr5->ismatched=match;

	ptr5->next5 = NULL;

	head5 = curr5 = ptr5;
	return ptr5;
}

struct scores* create_scorelist(int idif,float idtf,float scof)
{
	struct scores *ptr6 = (struct scores*)malloc(sizeof(struct scores));
	if(NULL == ptr6)
	{
		printf("\n Node creation failed \n");
		return NULL;
	}

	ptr6->idi = idif;
	ptr6->idt = idtf;
	ptr6->sco = scof;
	ptr6->next6 = NULL;

	head6 = curr6 = ptr6;
	return ptr6;
}




struct basic* add_to_basiclist(int xf,int yf,float distf, int andff)
{
	if(NULL == head)
	{
		return (create_basiclist(xf,yf,distf,andff));
	}

	struct basic *ptr = (struct basic*)malloc(sizeof(struct basic));
	if(NULL == ptr)
	{
		printf("\n Node creation failed \n");
		return NULL;
	}
	ptr->x = xf;
	ptr->y = yf;
	ptr->dist = distf;
	ptr->andf = andff;
	ptr->next = NULL;

	curr->next = ptr;
	curr = ptr;

	return ptr;
}

struct neighbours* add_to_neighbourslist(float ndf, int naf)
{
	if(NULL == head1)
	{
		return (create_neighbourslist(ndf,naf));
	}


	struct neighbours *ptr1 = (struct neighbours*)malloc(sizeof(struct neighbours));
	if(NULL == ptr1)
	{
		printf("\n Node creation failed \n");
		return NULL;
	}

	ptr1->nd = ndf;
	ptr1->na = naf;
	ptr1->next1 = NULL;


	curr1->next1 = ptr1;
	curr1 = ptr1;

	return ptr1;
}

struct tuples* add_to_tupleslist(int rtf,int nonf,float darr[],int aarr[],int match)
{
	if(NULL == head2)
	{
		return (create_tupleslist(rtf,nonf,darr,aarr,match));
	}

	struct tuples *ptr2 = (struct tuples*)malloc(sizeof(struct tuples));
	if(NULL == ptr2)
	{
		printf("\n Node creation failed \n");
		return NULL;
	}

	int i;
	int nonf1;
	nonf1=(nonf)*(nonf-1);

	ptr2->root = rtf;
	ptr2->numofneigh = nonf;

	for(i=0;i<nonf;i++)
	{
		ptr2->dismat[i]=darr[i];
	}

	for(i=0;i<nonf;i++)
	{
		ptr2->angmat[i]=aarr[i];
	}

	ptr2->ismatched=match;

	ptr2->next2 = NULL;

	curr2->next2 = ptr2;
	curr2 = ptr2;


	return ptr2;
}

struct basic1* add_to_basiclist1(int xf,int yf,float distf, int andff)
{
	if(NULL == head3)
	{
		return (create_basiclist1(xf,yf,distf,andff));
	}

	struct basic1 *ptr3 = (struct basic1*)malloc(sizeof(struct basic1));
	if(NULL == ptr3)
	{
		printf("\n Node creation failed \n");
		return NULL;
	}
	ptr3->x = xf;
	ptr3->y = yf;
	ptr3->dist = distf;
	ptr3->andf = andff;
	ptr3->next3 = NULL;

	curr3->next3 = ptr3;
	curr3 = ptr3;

	return ptr3;
}

struct neighbours1* add_to_neighbourslist1(float ndf, int naf)
{
	if(NULL == head4)
	{
		return (create_neighbourslist1(ndf,naf));
	}


	struct neighbours1 *ptr4 = (struct neighbours1*)malloc(sizeof(struct neighbours1));
	if(NULL == ptr4)
	{
		printf("\n Node creation failed \n");
		return NULL;
	}

	ptr4->nd = ndf;
	ptr4->na = naf;
	ptr4->next4 = NULL;


	curr4->next4 = ptr4;
	curr4 = ptr4;

	return ptr4;
}

struct tuples1* add_to_tupleslist1(int rtf,int nonf,float darr[],int aarr[],int match)
{
	if(NULL == head5)
	{
		return (create_tupleslist1(rtf,nonf,darr,aarr,match));
	}

	struct tuples1 *ptr5 = (struct tuples1*)malloc(sizeof(struct tuples1));
	if(NULL == ptr5)
	{
		printf("\n Node creation failed \n");
		return NULL;
	}

	int i;
	int nonf1;
	nonf1=(nonf)*(nonf-1);

	ptr5->root = rtf;
	ptr5->numofneigh = nonf;

	for(i=0;i<nonf;i++)
	{
		ptr5->dismat[i]=darr[i];
	}

	for(i=0;i<nonf;i++)
	{
		ptr5->angmat[i]=aarr[i];
	}

	ptr5->ismatched=match;

	ptr5->next5 = NULL;

	curr5->next5 = ptr5;
	curr5 = ptr5;


	return ptr5;
}

struct scores* add_to_scoreslist(int idif, float idtf,float scof)
{
	if(NULL == head6)
	{
		return (create_scorelist(idif,idtf,scof));
	}

	struct scores *ptr6 = (struct scores*)malloc(sizeof(struct scores));
	if(NULL == ptr6)
	{
		printf("\n Node creation failed \n");
		return NULL;
	}

	ptr6->idi = idif;
	ptr6->idt = idtf;
	ptr6->sco = scof;
	ptr6->next6 = NULL;

	curr6->next6 = ptr6;
	curr6 = ptr6;


	return ptr6;
}








int _tmain(int argc, _TCHAR* argv[])
{
	int **matrix;
	int **matrix1;
	//int **matrix2;
	int i,j,k,iC,jC;
	float finalscore;
	int rows, cols, rows1, cols1;
	int fileCount=0;
	int fileCount1=0;
	FILE *fileptr= NULL;
	//FILE *file1  = NULL;
	FILE *fp = NULL;
	FILE *fp1 = NULL;

	char fileName[500][100];
	char file[500];
	char fileName1[1000][100];
	char file1[500];

	unsigned char *fmrDataset;
	unsigned int fmrLength;
	unsigned char *fmrDataset1;
	unsigned int fmrLength1;
	fmrDataset = (unsigned char*)malloc(sizeof(unsigned char)*(2000));
	fmrDataset1 = (unsigned char*)malloc(sizeof(unsigned char)*(2000));
	struct iso2xyt_struct *iso2xytptr = (struct iso2xyt_struct*)malloc(sizeof(struct iso2xyt_struct));
	struct iso2xyt_struct *iso2xytptr1 = (struct iso2xyt_struct*)malloc(sizeof(struct iso2xyt_struct));



	DIR *dir;
	DIR *dir1;

	dir=opendir("FMR");
	if (dir != NULL)
	{
		struct dirent *ent;
		while ((ent = readdir (dir)) != NULL)
		{   //printf("\nblah");
			strcpy(fileName[fileCount],ent->d_name);
			fileCount++;
		}

		closedir(dir);
	}

	else
		perror ("Couldn't open the directory");


	dir1=opendir("FMR1");
	if (dir1 != NULL)
	{
		struct dirent *ent1;
		while ((ent1 = readdir (dir1)) != NULL)
		{   //printf("\nblah");
			strcpy(fileName1[fileCount1],ent1->d_name);
			fileCount1++;
		}

		closedir(dir1);
	}

	else
		perror ("Couldn't open the directory");

	//printf("arun");
	for(iC=2;iC<142;iC++)
	{
		printf("\nblah");
		GetCurrentDirectoryA(400,file);
		strcat (file,"\\FMR\\");
		//file1[400]=NULL;
		//strcpy(file1,"D:\\Adaptive_Gen\\Adaptive_Gen\\IOB\\Neuro\\Uma\\");
		strcat(file,fileName[iC]);
		fp=fopen(file,"rb");
		//fp = fopen("E:\\Arumugam\\Enrollment\\1\\FMR\\P16_01_01.fmr","rb");



		//fp = fopen(file,"rb");
		if(fp==NULL)
			printf("\nFile not found\n");
		else printf("\nfile open panniyachu thambi");
		fmrLength=0;
		fseek(fp,0,SEEK_END);
		fmrLength = ftell(fp);
		fseek(fp,0,SEEK_SET);
		//fmrDataset = (unsigned char*)malloc(sizeof(unsigned char)*(fmrLength));
		fread(fmrDataset,1,fmrLength,fp);
		fclose(fp);
		//struct iso2xyt_struct *iso2xytptr = (struct iso2xyt_struct*)malloc(sizeof(struct iso2xyt_struct));
		getXYTFromFMR(fmrDataset,fmrLength,iso2xytptr);

		rows=(iso2xytptr->nrows); cols=4;

		matrix = icreate_matrix(rows,cols);

		for(i=0;i<rows;i++)
		{
			matrix[i][0]=iso2xytptr->xcol[i];
			matrix[i][1]=iso2xytptr->ycol[i];
			matrix[i][2]=iso2xytptr->thetacol[i];
			matrix[i][3]=iso2xytptr->quality[i];
		}


		vp_calc(rows,cols,matrix);

		printf("\nvp_calc done\n");




		for(jC=2;jC<700;jC++)
		{
			GetCurrentDirectoryA(400,file1);

			strcat (file1,"\\FMR1\\");

			strcat(file1,fileName1[jC]);
			printf ("\n%s : %s taken\n",fileName[iC],fileName1[jC]);
			fp1=fopen(file1,"rb");


			//fp1 = fopen("E:\\Arumugam\\Enrollment\\1\\FMR\\P16_01_01.fmr","rb");*/

			//fp1 = fopen(file1,"rb");
			if(fp1==NULL)
				printf("\nFile not found\n");
			fmrLength1=0;
			fseek(fp1,0,SEEK_END);
			fmrLength1 = ftell(fp1);
			fseek(fp1,0,SEEK_SET);
			//fmrDataset1 = (unsigned char*)malloc(sizeof(unsigned char)*(fmrLength1));
			fread(fmrDataset1,1,fmrLength1,fp1);
			fclose(fp1);
			//struct iso2xyt_struct *iso2xytptr1 = (struct iso2xyt_struct*)malloc(sizeof(struct iso2xyt_struct));
			getXYTFromFMR(fmrDataset1,fmrLength1,iso2xytptr1);

			rows1=(iso2xytptr1->nrows); cols1=4;

			matrix1 = icreate_matrix(rows1,cols1);

			for(i=0;i<rows1;i++)
			{
				matrix1[i][0]=iso2xytptr1->xcol[i];
				matrix1[i][1]=iso2xytptr1->ycol[i];
				matrix1[i][2]=iso2xytptr1->thetacol[i];
				matrix1[i][3]=iso2xytptr1->quality[i];
			}

			//printf("\nWriting basic list for 1st fp\n");

			//print_basiclist();

			//printf("\nWriting basic list for 2nd fp\n");

			//vp_calc_1(rows1,cols1,matrix1); //Commented by siva.

			vp_calc(rows1,cols1,matrix1);


			//printf("\nvp_calc1 done");

			//print_tupleslist();

			finalscore=compare(rows,rows1);

			//printf("\ncomparison done");

			finalscore=glocmp(matrix,matrix1,rows,rows1);

			//printf("\nglocomp done");

			printf("\nAnd the score is %f\n",finalscore);

			//printf ("\n%s : %s taken\n",fileName[iC],fileName1[jC]);


			/*fileptr = fopen("C:\\Documents and Settings\\arun.r\\Desktop\\zunathen.txt", "a");
			if(fileptr==NULL)
				printf("\nFile illa thambi");
			fputs(fileName[iC],fileptr);
			fprintf(fileptr, "\t");
			fputs(fileName1[jC],fileptr);
			fprintf(fileptr, "\t");
			fprintf(fileptr, "%f",finalscore);
			fprintf(fileptr, "\n");
			fclose(fileptr);*/



			ifree_matrix(rows1, matrix1);
			//printf("\nmatrix freed");

			free_list3(&head3);
			//printf("\nlist3 freed");
			free_list5(&head5);
			//printf("\nlist5 freed");
			free_list6(&head6);
			//printf("\nlist6 freed");

			/* if(fmrDataset1!=NULL)
			{
			free(fmrDataset1);
			fmrDataset1=NULL;
			}
			if(iso2xytptr1!=NULL)
			{
			free(iso2xytptr1);
			iso2xytptr1=NULL;
			}*/

		}   //printf("\n----- OVER -----\n");
		// printf("\nwe're out");
		//if(head!=NULL)

		//printf("\nblah");
		//print_tupleslist();
		free_list2(&head2);
		//printf("\nlist2 freed");
		// printf("\nblah1");
		//print_tupleslist();
		free_list(&head);
		//printf("\nlist freed");
		ifree_matrix(rows, matrix);
		// printf("\ndone all freeing");

		/*   if(fmrDataset!=NULL)
		{
		free(fmrDataset);
		fmrDataset=NULL;
		}
		if(iso2xytptr!=NULL)
		{
		free(iso2xytptr);
		iso2xytptr=NULL;
		}*/


	}

	getchar();

	//printf("\n--- OVER ---\n");

	return 0;
}




void print_basiclist(void)
{
	struct basic *ptr = head;
	printf("\n\n");

	while(ptr != NULL)
	{
		printf("[%d] [%d] [%f] [%d]---",ptr->x,ptr->y,ptr->dist,ptr->andf);
		ptr = ptr->next;
	}

	return;
}

void print_basiclist1(void)
{
	struct basic1 *ptr3 = head3;
	printf("\n\n");

	while(ptr3 != NULL)
	{
		printf("[%d] [%d] [%f] [%d]---",ptr3->x,ptr3->y,ptr3->dist,ptr3->andf);
		ptr3 = ptr3->next3;
	}

	return;
}




void print_neighbourslist(void)
{
	struct neighbours *ptr1 = head1;

	while(ptr1 != NULL)
	{
		printf("\n[%d] [%f]\n",ptr1->na,ptr1->nd);
		ptr1 = ptr1->next1;
	}

	return;
}

void print_tupleslist(void)
{
	struct tuples *ptr2 = head2; int i,lim;

	while(ptr2 != NULL)
	{
		printf("\n\n[%d] [%d] [%d]\n",ptr2->root,ptr2->numofneigh,ptr2->ismatched);
		lim=(ptr2->numofneigh);
		printf("\nDistance ratios\n\n");
		for(i=0;i<lim;i++)
			printf(" [%f] ",ptr2->dismat[i]);

		printf("\n\nAngle differences\n\n");
		for(i=0;i<ptr2->numofneigh;i++)
			printf(" [%d] ",ptr2->angmat[i]);


		ptr2 = ptr2->next2;
	}

	return;
}

void print_neighbourslist1(void)
{
	struct neighbours1 *ptr4 = head4;

	while(ptr4 != NULL)
	{
		printf("\n[%d] [%f]\n",ptr4->na,ptr4->nd);
		ptr4 = ptr4->next4;
	}

	return;
}

void print_tupleslist1(void)
{
	struct tuples1 *ptr5 = head5; int i,lim;

	while(ptr5 != NULL)
	{
		printf("\n\n[%d] [%d] [%d]\n",ptr5->root,ptr5->numofneigh,ptr5->ismatched);
		lim=(ptr5->numofneigh);
		printf("\nDistance ratios\n\n");
		for(i=0;i<lim;i++)
			printf(" [%f] ",ptr5->dismat[i]);

		printf("\n\nAngle differences\n\n");
		for(i=0;i<ptr5->numofneigh;i++)
			printf(" [%d] ",ptr5->angmat[i]);


		ptr5 = ptr5->next5;
	}

	return;
}

void print_scoreslist(void)
{
	struct scores *ptr6 = head6;

	while(ptr6 != NULL)
	{
		printf("\n[%d] [%d] [%f]\n",ptr6->idi,ptr6->idt,ptr6->sco);
		ptr6 = ptr6->next6;
	}

	return;
}




int **icreate_matrix(int rows, int cols){

	int **mat1 = (int **) malloc(sizeof(int *)*rows);
	int i=0;
	for(i=0; i<rows; i++)
		/* Allocate array, store pointer  */
		mat1[i] = (int *) malloc(sizeof(int)*cols);

	return mat1;
}


void ifree_matrix(int rows, int **mat1){
	int i=0;
	for(i=0;i<rows;i++)
	{free(mat1[i]);
	mat1[i]=NULL;
	}
	free(mat1);
}


void iprint_matrix(int rows, int cols, int **mat1){
	int i,j;
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			printf("%d \n",mat1[i][j]);
		}
		printf("read attempt: %d",i);
	}
}





void free_list(struct basic **head)
{
	struct basic *nnode=NULL;
	struct basic *cnode=NULL;
	cnode = *head;

	while (cnode!=NULL)
	{
		nnode = cnode->next;

		cnode->x=NULL;
		cnode->y=NULL;
		cnode->dist=NULL;
		cnode->andf=NULL;

		free(cnode);
		cnode=NULL;
		cnode = nnode;
	}

	*head = NULL;
}

void free_list1(struct neighbours **head)
{
	struct neighbours *nnode=NULL;
	struct neighbours *cnode=NULL;
	cnode = *head;

	while (cnode!=NULL)
	{
		nnode = cnode->next1;
		free(cnode);
		cnode=NULL;
		cnode = nnode;
	}

	*head = NULL;
}

void free_list2(struct tuples **head)
{
	//struct tuples *nnode, *cnode;
	struct tuples *nnode=NULL;
	struct tuples *cnode=NULL;
	cnode = *head;

	while (cnode!=NULL)
	{
		nnode = cnode->next2;

		cnode->root=NULL;
		cnode->numofneigh=NULL;
		cnode->ismatched=NULL;
		for(int i=0;i<cnode->numofneigh;i++)
		{
			cnode->dismat[i]=NULL;
			cnode->angmat[i]=NULL;
		}

		free(cnode);
		cnode=NULL;
		cnode = nnode;
	}

	*head = NULL;
}

void free_list3(struct basic1 **head)
{
	//struct basic1 *nnode, *cnode;
	struct basic1 *nnode=NULL;
	struct basic1 *cnode=NULL;
	cnode = *head;

	while (cnode!=NULL)
	{
		nnode = cnode->next3;

		cnode->x=NULL;
		cnode->y=NULL;
		cnode->dist=NULL;
		cnode->andf=NULL;

		free(cnode);
		cnode=NULL;
		cnode = nnode;
	}

	*head = NULL;
}

void free_list4(struct neighbours1 **head)
{
	//struct neighbours1 *nnode, *cnode;
	struct neighbours1 *nnode=NULL;
	struct neighbours1 *cnode=NULL;
	cnode = *head;

	while (cnode!=NULL)
	{
		nnode = cnode->next4;
		free(cnode);
		cnode=NULL;
		cnode = nnode;
	}

	*head = NULL;
}

void free_list5(struct tuples1 **head)
{
	//struct tuples1 *nnode, *cnode;
	struct tuples1 *nnode=NULL;
	struct tuples1 *cnode=NULL;
	cnode = *head;
	int i=0;
	//printf("inside");
	while (cnode!=NULL)
	{  // printf(" %d ",i);
		nnode = cnode->next5;

		cnode->root=NULL;
		//printf(" %d.1 ",i);
		cnode->numofneigh=NULL;
		//printf(" %d.2 ",i);
		cnode->ismatched=NULL;
		// printf(" %d.3 ",i);
		for(int i=0;i<cnode->numofneigh;i++)
		{
			cnode->dismat[i]=NULL;
			cnode->angmat[i]=NULL;
		}
		//printf(" %d.4 ",i);
		//if(cnode!=NULL)
		free(cnode);
		// printf(" %d.5 ",i);
		cnode=NULL;
		cnode = nnode;
		i++;
	}

	*head = NULL;
}

void free_list6(struct scores **head)
{
	//struct scores *nnode, *cnode;
	struct scores *nnode=NULL;
	struct scores *cnode=NULL;
	cnode = *head;

	while (cnode!=NULL)
	{
		nnode = cnode->next6;

		cnode->idi=NULL;
		cnode->idt=NULL;
		cnode->sco=NULL;

		free(cnode);
		cnode=NULL;
		cnode = nnode;
	}

	*head = NULL;
}





int getXYTFromFMR(const unsigned char* fmrData, const unsigned int fmrLen, struct iso2xyt_struct* xyt_data)
{
	int ret=-1;
	int jLoop=0,kLoop=0;
	int x=0,y=0,type=0,reserved=0,angle=0;
	int NoOfMinutiae=0;
	int result=0;
	double ang;


	/* Assuming FVMR view is only 1. */

	if((int)fmrData[27] <= 10)
	{
		ret = -1;
	}
	else
	{

		xyt_data->nrows=(int)fmrData[27];
		NoOfMinutiae=(int)fmrData[27];

		for(kLoop=0,jLoop=28;kLoop<NoOfMinutiae;jLoop+=6,kLoop++)
		{
			x |=(int)fmrData[jLoop] & 0xFF;
			x <<= 8;
			x |=(int)fmrData[jLoop + 1] & 0xFF;

			y |=(int)fmrData[jLoop + 2] & 0xFF;
			y <<= 8;
			y |=(int)fmrData[jLoop + 3] & 0xFF;






			type =	(int)((x & FMD_MINUTIA_TYPE_MASK) >> FMD_MINUTIA_TYPE_SHIFT);
			x= (x & FMD_X_COORD_MASK);

			reserved = (int)((y & FMD_RESERVED_MASK) >> FMD_RESERVED_SHIFT);
			y = (y & FMD_Y_COORD_MASK);

			/* x coord of the minutiae */
			xyt_data-> xcol[kLoop] = x;
			/* y coord of the minutiae */
			xyt_data->ycol[kLoop] = y;
			/* type of the minutiae */
			xyt_data->type[kLoop] = type;
			/* angle of the minutiae	 */

			angle = (int)fmrData[jLoop + 4];
			/* angle = angle / 2;
			angle = (int)round((angle * 360) / 256);	*/
			ang = ((double)(angle * 360) / 256);
			angle = (int)round(ang);
			ang = 0;



			xyt_data->thetacol[kLoop] = angle;
			/* quality of the minutiae */
			xyt_data->quality[kLoop] = fmrData[jLoop + 5];

			x=0;
			y=0;
			type=0;
			angle=0;
		}
		ret=1;
	}
	return ret;
}





void vp_calc(int rows, int cols, int **mat)
{
	float dis,ad;
	float dmatrix[300]={ 0 };
	int amatrix[300]={ 0 };
	int cc,idis;
	struct basic *ptr;
	struct neighbours *ptr1;


	for(int i=0; i<rows-1; i++)
	{
		for(int j=i+1; j<rows; j++)
		{
			dis= sqrt(pow((double)(mat[j][1]-mat[i][1]),(double)2) + pow((double)(mat[j][0]-mat[i][0]),(double)2));
			//idis=(int)(dis*100);
			//dis=(float)idis/100;
			if(dis<45)
			{
				ad=sqrt(pow((double)(mat[i][2]-mat[j][2]),(double)2));
				add_to_basiclist(i,j,dis,ad);
			}

		}
	}

	//printf("\nOver to stage 1\n");

	for(int cq=0;cq<rows;cq++)
	{
		ptr=head;
		cc=0;

		while(ptr != NULL)
		{
			if((ptr->x)==cq)
			{
				dmatrix[cc]=ptr->dist;
				amatrix[cc]=ptr->andf;
				cc++;
			}
			else if((ptr->y)==cq)
			{
				dmatrix[cc]=ptr->dist;
				amatrix[cc]=ptr->andf;
				cc++;
			}

			ptr = ptr->next;
		}

		add_to_tupleslist(cq,cc,dmatrix,amatrix,0);

		for(int k=0;k<cc;k++)
		{
			dmatrix[cc]=0;
			amatrix[cc]=0;
		}

	}

	//print_tupleslist();
	//printf("\nvp_calc done\n");

}


void vp_calc_1(int rows, int cols, int **mat)
{
	float dis,ad;
	float dmatrix[300]={ 0 };
	int amatrix[300]={ 0 };
	int cc,idis;
	struct basic1 *ptr3;


	for(int i=0; i<rows-1; i++)
	{
		for(int j=i+1; j<rows; j++)
		{
			dis= sqrt(pow((double)(mat[j][1]-mat[i][1]),(double)2) + pow((double)(mat[j][0]-mat[i][0]),(double)2));
			idis=(int)(dis*100);
			dis=(float)idis/100;
			if(dis<45)
			{
				ad=sqrt(pow((double)(mat[i][2]-mat[j][2]),(double)2));
				add_to_basiclist1(i,j,dis,ad);
			}

		}
	}

	//printf("\nOver to stage 1\n");

	for(int cq=0;cq<rows;cq++)
	{
		ptr3=head3;
		cc=0;

		while(ptr3 != NULL)
		{
			if((ptr3->x)==cq)
			{
				dmatrix[cc]=ptr3->dist;
				amatrix[cc]=ptr3->andf;
				cc++;
			}
			else if((ptr3->y)==cq)
			{
				dmatrix[cc]=ptr3->dist;
				amatrix[cc]=ptr3->andf;
				cc++;
			}

			ptr3 = ptr3->next3;
		}

		add_to_tupleslist1(cq,cc,dmatrix,amatrix,0);

		for(int k=0;k<cc;k++)
		{
			dmatrix[cc]=0;
			amatrix[cc]=0;
		}

	}

	//print_tupleslist();
	//printf("\nvp_calc_1 done\n");

}





float compare(int rowsf,int rowsf1)
{
	//int **matrix2;
	float rden,ratiodiff,rmatchscore,bestrmatchscore,countcm,thescore;
	float aden,angdiff,amatchscore,bestamatchscore,countcm1;
	int bestmatch,bestmatch1,lim,lim1,i,j,nonmatratios,nonmatratios1,intsco;

	rden=ratiodiff=rmatchscore=bestrmatchscore=countcm=thescore=0;
	aden=angdiff=amatchscore=bestamatchscore=countcm1=0;
	bestmatch=bestmatch1=lim=lim1=i=j=nonmatratios=nonmatratios1=intsco=0;

	struct tuples *ptr2 = head2;

	//printf(" inside compare fn ");

	//print_tupleslist();
	//print_tupleslist1();

	while(ptr2!=NULL)
	{

		//lim=(ptr2->numofneigh)*((ptr2->numofneigh)-1);
		bestrmatchscore=bestmatch=bestmatch1=0;
		bestamatchscore=0;

		struct tuples1 *ptr5 = head5;


		//printf("\n\n%d\n\n",ptr2->root);

		while(ptr5!=NULL)
		{
			{
				//printf(" [%d] ",ptr5->root);

				//lim1=rden=ratiodiff=rmatchscore=countcm=nonmatratios=nonmatratios1=0;
				aden=angdiff=amatchscore=0;
				//printf("\nstart\n");
				lim1=(ptr5->numofneigh)*((ptr5->numofneigh)-1);

				for(i=0;i<ptr2->numofneigh;i++)
				{
					for(j=0;j<ptr5->numofneigh;j++)
					{
						ratiodiff=(pow((double)(ptr2->dismat[i]-ptr5->dismat[j]),(double)2));
						angdiff=(pow((double)(ptr2->angmat[i]-ptr5->angmat[j]),(double)2));


						//printf("\n[%f] [%f] [%f]\n",ptr2->dismat[i],ptr5->dismat[j],ratiodiff);
						if(ratiodiff<=2 || angdiff<=12*12)
						{
							//printf("\n [%f] [%f] [%f] \n",ptr2->dismat[i],ptr5->dismat[j],ratiodiff);
							countcm++;
							//ptr2->dismat[i]=0;
							//ptr5->dismat[j]=0;
							//ptr2->angmat[i]=0;
							//ptr5->angmat[j]=0;
							//printf("\nstop\n");
							break;
						}
						else
							continue;
					}
				}
				if(ptr2->numofneigh<ptr5->numofneigh)
					rden=(float)ptr2->numofneigh;
				else
					rden=(float)ptr5->numofneigh;
				//nonmatratios=ptr2->numofneigh-countcm;
				//nonmatratios1=ptr5->numofneigh-countcm;
				rmatchscore=countcm/rden;

				if(ptr5==head5)
				{
					bestrmatchscore=rmatchscore;
					bestmatch=ptr5->root;
				}
				else
				{
					if(rmatchscore>bestrmatchscore)
					{
						bestrmatchscore=rmatchscore;
						bestmatch=ptr5->root;
					}
				}
			}

			ptr5=ptr5->next5;
		}

		//printf("\n[%f] [%d]\n",bestrmatchscore,bestmatch);
		if(bestrmatchscore>=0.85)
		{
			//printf("---[%f]---",bestrmatchscore);
			add_to_scoreslist(ptr2->root,bestmatch,bestrmatchscore);
			//countcm1++;
			//printf("[%d] [%d] [%f]---",ptr2->root,bestmatch,bestrmatchscore);
		}

		/*ptr5=head5;
		while(ptr5!=NULL)
		{
		if(ptr5->root==bestmatch)
		ptr5->ismatched=1;

		ptr5=ptr5->next5;
		}*/

		//thescore=thescore+bestrmatchscore;
		ptr2=ptr2->next2;
		//printf("\n________________________________________________");


	}
	//print_scoreslist();
	//print_tupleslist1();

	//printf("\ncomparison done");

	if(rowsf<=rowsf1)
		thescore=countcm1/(float)rowsf;
	else
		thescore=countcm1/(float)rowsf1;


	return thescore;

}


float glocmp(int **mat, int **mat1,int rows,int rows1)
{
	//printf("\n\n[%d] [%d]",rows,rows1);
	float hs,dis,fsc; int tx,ty;
	struct scores *ptr6 = head6;
	int xcorr,ycorr,acorr,i,j,ad,admin,mc,nonmat,nonmat1;
	float thescore;
	int cc=0;

	//if(head6==NULL)
	//printf("\n\nbow\n\n");

	while(ptr6)
	{
		int x,y,z,xd,yd,ad;
		x=y=z=xd=yd=ad=admin=0;
		xcorr=ycorr=acorr=0;mc=nonmat=nonmat1=0;

		// if(ptr6->sco>=0.5)
		// {
		//printf("\nIN %d",cc);
		//printf(" [%d] [%d]\n",ptr6->idi,ptr6->idt);
		//printf("\n");
		//printf("\nGoing to calculate correctional parameters");
		//printf("\n%d",mat[ptr6->idi][0]);
		xcorr=((mat[ptr6->idi][0])-(mat1[ptr6->idt][0]));
		//printf("\n%d %d [%d] ",mat[ptr6->idi][0],mat1[ptr6->idi][0],xcorr);
		ycorr=mat[ptr6->idi][1]-mat1[ptr6->idt][1];
		//printf("%d %d [%d] ",mat[ptr6->idi][1],mat1[ptr6->idi][1],ycorr);
		acorr=mat[ptr6->idi][2]-mat1[ptr6->idt][2];
		//printf("%d %d [%d]\n",mat[ptr6->idi][2],mat1[ptr6->idi][2],acorr);



		for(i=0;i<rows;i++)
		{
			x=mat[i][0]-xcorr;
			y=mat[i][1]-ycorr;
			z=mat[i][2]-acorr;


			// printf("\n%d %d %d",x,y,z);

			for(j=0;j<rows1;j++)
			{
				xd= sqrt(pow((double)(mat1[j][0]-x),(double)2));
				yd= sqrt(pow((double)(mat1[j][1]-y),(double)2));
				ad= sqrt(pow((double)(mat1[j][2]-z),(double)2));
				admin=360-ad;
				if (admin>ad)
					admin=ad;

				//printf("\n%d %d %d\n",xd,yd,ad);

				if((xd<15)&&(yd<15)&&(admin<150))
				{
					mc++;
					break;
				}


			}
		}
		//nonmat=rows-mc;
		//nonmat1=rows1-mc;
		fsc=(float)(2*mc)/(float)(rows+rows1);

		// printf("\n [%d] [%d] [%f] ",mc,rows,fsc);


		//printf("\n%f\n",fsc);
		if (ptr6==head6)
			thescore=fsc;
		else if(fsc>thescore)
			thescore=fsc;

		ptr6=ptr6->next6;
		// printf("\n%d",cc);
		cc++;
	}


	// printf("\n%d\n",cc);

	//printf("\nglocomp done");
	return thescore;
}


