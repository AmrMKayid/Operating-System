#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>

struct processes {
   int  id;
   int  arrive;
   int  total;
   int  tickets;
   int complete;
   int brust;
}; 


void delay(int ms) {
    long halt;
    clock_t current, previous;

    halt = ms * (CLOCKS_PER_SEC / 1000);
    current = previous = clock();

    while ((current - previous) < halt)
        current = clock();
}



const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}




int main(){

	int lines = 0;

	FILE * fPointer;
	
	fPointer = fopen("input1.in","r");
	
	

	
	while(!feof(fPointer))
	{
		char ch = fgetc(fPointer);
			if(ch == '\n')
			{
			lines++;
			}
	}
	
	
	struct processes record[lines-2];

	char singleline[500];	

	
	fPointer = fopen("input1.in","r");

	
	int k = 0;

	char sentences[lines][500];
	
	while(!feof(fPointer))
	{
		fgets(singleline,500,fPointer);
		strcpy(sentences[k], singleline);   

			k = k+1;
	}
	
	fclose(fPointer);



	int quantum = atoi(sentences[0]);
	int total = atoi(sentences[1]);


	for(int i = 0; i < lines-1; i++)
	{
		char temp[500];
		strcpy(temp,sentences[i+2]);
		record[i].id = atoi(getfield(temp,1));
				strcpy(temp,sentences[i+2]);

		record[i].arrive = atoi(getfield(temp,2));
				strcpy(temp,sentences[i+2]);

		record[i].total = atoi(getfield(temp,3));
				strcpy(temp,sentences[i+2]);
				
		record[i].brust = record[i].total;

		record[i].tickets = atoi(getfield(temp,4));

	}				
	
	int n = sizeof(record)/sizeof(record[0]);
	
	
	char all [total];
	int x = 0;
	for(int i = 0; i < sizeof(all)/sizeof(all[0]); i++)
	{
		all[i] = record[x].id-1;
		record[x].tickets = record[x].tickets - 1;
		
		if(record[x].tickets == 0)
		{
			x = x+1;
		}
	}
	

	FILE *fp;
	fp = fopen("Output.txt", "w");
	int sizeOfAll = sizeof(all)/sizeof(all[0]);
	int time = 0;

	while(sizeOfAll > 0)
	{
		int random = rand() % (sizeOfAll);
		int myNumber = all[random];
		int myProc = record[myNumber].id;
		int myProcTime = record[myNumber].total;
		
		
		printf("At time ");
		fprintf(fp, "At time ");
		printf("%d",time);
		fprintf(fp,"%d",time);
		printf(" : p");
		fprintf(fp," : p");
		printf("%d",myProc);
		fprintf(fp,"%d",myProc);
		printf(" Entering quantum\n");
		fprintf(fp, " Entering quantum\n");
		


		if(myProcTime <= quantum)
		{
			time = time + myProcTime;
			delay(myProcTime);
			record[myNumber].total = 0;
			record[myNumber].complete = time;
			for(int i=0; i < sizeOfAll; i++)
			{
				if(all[i] == myNumber)
				{
					for(int j=i; j < sizeOfAll; j++)
					{
						all[j] = all[j+1];
					}
					
					sizeOfAll = sizeOfAll - 1;
			
					i = i - 1;
				}
			}
		}
		else
		{
			time = time + quantum;
			delay(quantum);
			record[myNumber].total = record[myNumber].total - quantum;
			for (int c = random - 1 ; c < sizeOfAll - 1 ; c++ )
			{
				all[c] = all[c+1];
			}
			sizeOfAll = sizeOfAll - 1;
		}
		
		
	}
	
	int count = 1;
	int tt = 0;
	int wt = 0;
	double ans = 0;
	int z;
	for(z = 0; z < sizeof(record)/sizeof(record[0]); z++)
	{
		tt = tt + (record[z].complete - record[z].arrive);
		wt = wt + ((record[z].complete - record[z].arrive) - record[z].brust);
		count = count + 1;
	}
		
	printf("\n");
	fprintf(fp,"\n");
	printf("Average Turnarround Time = ");
	fprintf(fp,"Average Turnarround Time = ");
	ans = (double)tt/(double)count;
	printf("%lf\n",ans);
	fprintf(fp,"%lf\n",ans);
	printf("Average Waiting Time = ");
	fprintf(fp,"Average Waiting Time = ");
	ans = (double)wt/(double)count;
	printf("%lf\n",ans);
	fprintf(fp,"%lf\n",ans);

	

	
	
	
	
	
	
	
	

	return 0;




}
