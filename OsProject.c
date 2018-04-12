#include <string.h> 
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

sem_t studentReady;
sem_t teacherReady;

pthread_mutex_t mutex;

int WRChair[3];
int waitingStudent = 0;
int nextChair = 0;
int next_to_teach = 0;

#define seats 3

#define SLEEP_MAX 5

void randomWait(void);
void* studentExecution(void* stu_id);
void* teachingAssisTAidnt();

void randomWait(void)
{
	
	int time = rand() % SLEEP_MAX + 1;
	sleep(time);
}
void* teachingAssisTAidnt()
{
	while(1)
	{
		sem_wait(&studentReady);	
		
		pthread_mutex_lock(&mutex);
		
		printf("\n\nTA is teaching student %d\n",WRChair[next_to_teach]);	
		WRChair[next_to_teach]=0;
		waitingStudent--;
		
		printf("\nwaiting Student_Id :");
		
		if(WRChair[0]>0)
		printf("\nWaiting On Chair One Id: %d",WRChair[0]);
		else
		printf("\nWaiting On Chair One Id: NoBody");
		if(WRChair[1]>0)
		printf("\nWaiting On Chair Two Id: %d",WRChair[1]);
		else
		printf("\nWaiting On Chair Two Id: NoBody");
		if(WRChair[2]>0)
		printf("\nWaiting On Chair Three Id: %d",WRChair[2]);
		else
		printf("\nWaiting On Chair Three Id: NoBody");
	
			next_to_teach = (next_to_teach + 1) % seats;
		
		randomWait();
		
		if(WRChair[0]==0 && WRChair[1]==0 && WRChair[2]==0)
		{
				printf("\n\nDAY ENDS.EVERYONE HOME");
				exit(0);
		}
		pthread_mutex_unlock(&mutex);

		sem_post(&teacherReady);
	}	
}


void* studentExecution(void* stu_id)
{
	int id = *(int*)stu_id;

	printf("\nStudent %d is programming\n",id);		
	
	while(1)
	{
		randomWait();

		pthread_mutex_lock(&mutex);

		if(waitingStudent < seats)	
		{
			WRChair[nextChair] = id;
			waitingStudent++;
			
			printf("\n\nStudent %d is waiting for doubts to be cleared",id);
			printf("\nwaiting Students are as follows :");
			
			if(WRChair[0]>0)
			printf("\n Student is Waiting on chair no. 1: %d",WRChair[0]);
			else
			printf("\n Student Waiting on chair no .1 : No_One");
			if(WRChair[1]>0)
			printf("\n Student is Waiting on chair no. 2: %d",WRChair[1]);
			else
			printf("\n Student Waiting on chair no .2 : No_One");
			if(WRChair[2]>0)
			printf("\n Student is Waiting on chair no. 3: %d",WRChair[2]);
			else
			printf("\n Student Waiting on chair no .3 : No_One");
			
			nextChair = (nextChair+1) % seats;
			
			pthread_mutex_unlock(&mutex);
	
			sem_post(&studentReady);
			sem_wait(&teacherReady);
		}
		else
		{
			pthread_mutex_unlock(&mutex);
			
			printf("\nNo more WRChairs. student %d goes back to coding\n\n",id);		
		}
	}				
}
int main()
{
	
	printf("\n\n************************ SLEEPING TEACHING ASSISTANT PROBLEM ************************\n");
	pthread_t *Student_Id;
	pthread_t TAid;
	
	int* SID;
	int no_of_students;
	
	int i;

	printf("\nEnter no of Students : ");
	scanf("%d", &no_of_students);

	printf("\nTEACHING ASSISTANT SLEEPING INITIALLY\n");
	
	Student_Id = (pthread_t*)malloc(sizeof(pthread_t) * no_of_students);
	
	SID = (int*)malloc(sizeof(int) * no_of_students);

	memset(SID, 0, no_of_students);

	sem_init(&studentReady,0,0);
	sem_init(&teacherReady,0,1);
	
	srand(time(NULL));
	
	pthread_mutex_init(&mutex,NULL);
	
	pthread_create(&TAid,NULL,teachingAssisTAidnt,NULL);
	
	for(i=0; i<no_of_students; i++)
	{
		SID[i] = i+1;
		pthread_create(&Student_Id[i], NULL, studentExecution, (void*) &SID[i]);
	} 
	
	pthread_join(TAid, NULL);
	
	for(i=0; i<no_of_students;i++)
	{
		pthread_join(Student_Id[i],NULL);
	}	
	
	return 0;
}
