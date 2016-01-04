// Testlatest.cpp : Defines the entry point for the console application.
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
#define PI 3.14159265358979323846

#define DM	125

#define IANGLE180(deg)		( ( (deg) > 180 ) ? ( (deg) - 360 ) : ( (deg) <= -180 ? ( (deg) + 360 ) : (deg) ) )

#define SENSE(a,b)		( (a) < (b) ? (-1) : ( ( (a) == (b) ) ? 0 : 1 ) )
#define SENSE_NEG_POS(a,b)	( (a) < (b) ? (-1) : 1 )

#define SQUARED(n)		( (n) * (n) )


struct iso2xyt_struct
{
	int nrows;
	int xcol[100];
	int ycol[100];
	int thetacol[100];
	int quality[100];
	int type[100];
};

struct min_pair
{
	int root;
	int numofneigh;
	float* dismat;
	int* angmat;
	struct min_pair *next;
};

int getXYTFromFMR(const unsigned char* fmrData, const unsigned int fmrLen, struct iso2xyt_struct* xyt_data);
void vp_calc(struct iso2xyt_struct* xyt_data,struct min_pair** pair_list);
float compare(struct min_pair* pair_list1,int nofMinutiae1,struct min_pair* pair_list2,int nofMinutiae2);

int _tmain(int argc, _TCHAR* argv[])
{
	int iC,jC;
	int fileCount=0;
	int fileCount1=0;
	FILE *fp = NULL;
	char fileName[500][100];
	char file[500];
	char fileName1[1000][100];
	char file1[500];

	unsigned char *fmrDataset;
	unsigned int fmrLength;

	struct iso2xyt_struct *iso2xytptr = (struct iso2xyt_struct*)malloc(sizeof(struct iso2xyt_struct));
	struct iso2xyt_struct *iso2xytptr1 = (struct iso2xyt_struct*)malloc(sizeof(struct iso2xyt_struct));

	DIR *dir;

	dir=opendir("FMR");
	if (dir != NULL)
	{
		struct dirent *ent;
		while ((ent = readdir (dir)) != NULL)
		{    
			strcpy(fileName[fileCount],ent->d_name);
			fileCount++;
		}

		closedir(dir);
	}
	else
		perror ("Couldn't open the directory");


	dir=opendir("FMR1");
	if (dir != NULL)
	{
		struct dirent *ent;
		while ((ent = readdir (dir)) != NULL)
		{   //printf("\nblah");
			strcpy(fileName1[fileCount1],ent->d_name);
			fileCount1++;
		}

		closedir(dir);
	}
	else
		perror ("Couldn't open the directory");

	for(iC=2;iC<fileCount;iC++)
	{
		printf("\nblah");
		GetCurrentDirectoryA(400,file);
		strcat (file,"\\FMR\\");
		strcat(file,fileName[iC]);
		fp=fopen(file,"rb");
		if(fp==NULL)
		{
			printf("\nFile not found\n");
		}
		else 
		{
			printf("\nfile open panniyachu thambi");
			fmrLength=0;
			fseek(fp,0,SEEK_END);
			fmrLength = ftell(fp);
			fseek(fp,0,SEEK_SET);
			fmrDataset = (unsigned char*)malloc(sizeof(unsigned char)*(fmrLength));
			fread(fmrDataset,1,fmrLength,fp);
			fclose(fp);
		}

		getXYTFromFMR(fmrDataset,fmrLength,iso2xytptr);

		if(fmrDataset != NULL)
		{
			free(fmrDataset);
			fmrDataset=NULL;
		}

		for(jC=2;jC<fileCount1;jC++)
		{
			GetCurrentDirectoryA(400,file1);

			strcat (file1,"\\FMR1\\");

			strcat(file1,fileName1[jC]);
			
			fp=fopen(file1,"rb");

			if(fp==NULL)
			{
				printf("\nFile not found\n");
			}
			else
			{
				fmrLength=0;
				fseek(fp,0,SEEK_END);
				fmrLength = ftell(fp);
				fseek(fp,0,SEEK_SET);
				fmrDataset = (unsigned char*)malloc(sizeof(unsigned char)*(fmrLength));
				fread(fmrDataset,1,fmrLength,fp);
				fclose(fp);
			}

			getXYTFromFMR(fmrDataset,fmrLength,iso2xytptr1);

			if(fmrDataset != NULL)
			{
				free(fmrDataset);
				fmrDataset=NULL;
			}

			struct min_pair* pair_list1=NULL;
			struct min_pair* pair_list2=NULL;


			vp_calc(iso2xytptr,&pair_list1);
			vp_calc(iso2xytptr1,&pair_list2);
			
			float score = compare(pair_list1,iso2xytptr->nrows,pair_list2,iso2xytptr1->nrows);
			
			printf ("\n%s : %s Match Score is  %f\n",fileName[iC],fileName1[jC],score);

			fp = fopen("Result.xls","a");
			fprintf(fp,"%s\t%s\t%f\n",fileName[iC],fileName1[jC],score);
			fclose(fp);

		}    

	}

	printf("Finished\n");

	getchar();

	return 0;
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






void vp_calc(struct iso2xyt_struct* xyt_data,struct min_pair** pair_list)
{
	float dis,ad; //dis is inter minutiae distance, ad is angle between two points
	float dmatrix[50]={ 0 };
	int amatrix[50]={ 0 };
	int nofNeigh=0;
	struct min_pair* current;

	for(int i=0; i<xyt_data->nrows-1; i++)
	{
		int delx = 0;
		int dely = 0;
		int distance = 0;

		if(i>0)
		{
			for(int k=0; k<i; k++)
			{
				delx = (xyt_data->xcol[i]-xyt_data->xcol[k]); // delx = x2-x1;
				dely = (xyt_data->ycol[i]-xyt_data->ycol[k]); // dely = y2-y1;
				distance = SQUARED(delx)+SQUARED(dely);
				dis = sqrt((double)distance); // sqrt((delx)^2+(dely)^2)
				if(distance>SQUARED(DM))
				{
					if(delx>DM)
					{
						break;
					}
					else
					{
						continue;
					}
				}
				else
				{
					ad = atan2((float)delx,(float)dely)*180/PI; //angle between two points atan2(delx/dely)*180/pi
					dmatrix[nofNeigh] = dis;
					amatrix[nofNeigh] = ad;
					nofNeigh++;
				}
			}
		}
		for(int j=i+1; j<xyt_data->nrows; j++)
		{	
			delx = (xyt_data->xcol[j]-xyt_data->xcol[i]); // delx = x2-x1;
			dely = (xyt_data->ycol[j]-xyt_data->ycol[i]); // dely = y2-y1;
			distance = SQUARED(delx)+SQUARED(dely);
			dis = sqrt((double)distance); // sqrt((delx)^2+(dely)^2)
			if(distance>SQUARED(DM))
			{
				if(delx>DM)
				{
					break;
				}
				else
				{
					continue;
				}
			}
			else
			{
				ad = atan2((float)delx,(float)dely)*180/PI; //angle between two points atan2(delx/dely)*180/pi
				dmatrix[nofNeigh] = dis;
				amatrix[nofNeigh] = ad;
				nofNeigh++;
			}
		}


		if(*pair_list == NULL)
		{
			(*pair_list) = (struct min_pair*)malloc(sizeof(struct min_pair));
			(*pair_list)->root = i;
			(*pair_list)->numofneigh = nofNeigh;
			(*pair_list)->dismat = (float*)malloc(sizeof(float)*nofNeigh);
			(*pair_list)->angmat = (int*)malloc(sizeof(int)*nofNeigh);
			memcpy((*pair_list)->dismat,dmatrix,nofNeigh);
			memcpy((*pair_list)->angmat,amatrix,nofNeigh);
			(*pair_list)->next = NULL;
			current = *pair_list;
		}
		else
		{
			if(current->next == NULL)
			{
				struct min_pair* temp = (struct min_pair*)malloc(sizeof(struct min_pair));
				temp->root = i;
				temp->numofneigh = nofNeigh;
				temp->dismat = (float*)malloc(sizeof(float)*nofNeigh);
				temp->angmat = (int*)malloc(sizeof(int)*nofNeigh);
				memcpy(temp->dismat,dmatrix,nofNeigh);
				memcpy(temp->angmat,amatrix,nofNeigh);
				temp->next=NULL;
				current->next=temp;
				current = temp;
			}
		}



		nofNeigh = 0;
	}

	//printf("\nOver to stage 1\n");

	/*for(int cq=0;cq<rows;cq++)
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
	//printf("\nvp_calc done\n");*/

}




float compare(struct min_pair* pair_list1,int nofMinutiae1, struct min_pair* pair_list2,int nofMinutiae2)
{
	struct min_pair* tmp_list1 = NULL;
	struct min_pair* tmp_list2 = NULL;

	float rden,ratiodiff,rmatchscore,countcm,thescore,angdiff;
	 
	rden=ratiodiff=rmatchscore=countcm=thescore=angdiff=0;



	tmp_list1 = pair_list1;

	while(tmp_list1 != NULL)
	{
		tmp_list2 = pair_list2;

		while(tmp_list2 != NULL)
		{
			for(int i=0; i<tmp_list1->numofneigh; i++)
			{
				for(int j=0; j<tmp_list2->numofneigh; j++)
				{
					ratiodiff=(pow((double)(tmp_list1->dismat[i]-tmp_list2->dismat[j]),(double)2));
					angdiff=(pow((double)(tmp_list1->angmat[i]-tmp_list2->angmat[j]),(double)2));

						if(ratiodiff<=2 || angdiff<=12*12)
						{
							countcm++;
							break;
						}
						else
							continue;
				}
			}
			if(tmp_list1->numofneigh<=tmp_list2->numofneigh)
					rden=(float)tmp_list1->numofneigh;
				else
					rden=(float)tmp_list2->numofneigh;
				 
				rmatchscore=countcm/rden;

			tmp_list2 = tmp_list2->next;
		}
		tmp_list1 = tmp_list1->next;
	}

	if(nofMinutiae1<=nofMinutiae2)
		thescore=countcm/(float)nofMinutiae1;
	else
		thescore=countcm/(float)nofMinutiae2;

	printf("Finished comparision\n");

	return thescore;
}