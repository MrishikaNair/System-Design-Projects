#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>

#define N 5
#define N_b 4
#define think 2
#define hungry 1
#define eat 0
#define l (phnum + 4) % N
#define r (phnum + 1) % N

sem_t mut;
sem_t S[N];

int state[N];
int phil[N] = { 0, 1, 2, 3, 4 };
int bowl[N_b]= {0, 1, 2, 3};

void check(int phnum)
{
	if (state[phnum] == hungry && state[l] != eat && state[r] != eat) {
		
		state[phnum] = eat;//mentioning that he is eating

		sleep(3);

		printf("Philosopher %d uses fork %d and %d\n", phnum + 1, l + 1, phnum + 1);
        if((phnum+1)<4)
        {
        printf("Philosopher %d uses bowl %d\n", phnum + 1);}

		printf("Philosopher %d is Eating\n", phnum + 1);

		sem_post(&S[phnum]);
	}
}

void take_fork(int phnum)
{
	sem_wait(&mut);

	state[phnum] = hungry; //mentioning that he is hungry

	printf("Philosopher %d is Hungry\n", phnum + 1);

	check(phnum);// eat if others not

	sem_post(&mut);

	sem_wait(&S[phnum]); // waiting

	sleep(1);
}

void take_bowl(int phnum)
{
	sem_wait(&mut);

	state[phnum] = hungry; //mentioning that he is hungry

	printf("Philosopher %d is Hungry\n", phnum + 1);

	check(phnum); // eat if others not

	sem_post(&mut);

	sem_wait(&S[phnum]); // waiting

	sleep(1);
}

void put_fork(int phnum)
{
	sem_wait(&mut);

	state[phnum] = think; //mentioning that he is thinking

	printf("Philosopher %d keeps fork %d and %d down\n",
		phnum + 1, l + 1, phnum + 1);
	printf("Philosopher %d is thinking\n", phnum + 1);

	check(l);
	check(r);
	sem_post(&mut);
}

void put_bowl(int phnum)
{
	sem_wait(&mut);

	state[phnum] = think; //mentioning that he is thinking
    if((phnum+1)<4)
    {
		printf("Philosopher %d keeps bowl %d down\n", phnum + 1, l + 1);
	}
	printf("Philosopher %d is thinking\n", phnum + 1);

	check(l);
	check(r);
	sem_post(&mut);
}

void* philos(void* num)
{
	while (1) {

		int* a = num;

		sleep(1);

		take_fork(*a);
        if(a<4)
            take_bowl(*a);
		sleep(0);

		put_fork(*a);
        if(a<4)
            put_bowl(*a);
	}
}

int main()
{

	int a;
	pthread_t id_th[N];

	sem_init(&mut, 0, 1); // semaphore initialisation

	for (a = 0; a < N; a++)

		sem_init(&S[a], 0, 0);

	for (a = 0; a < N; a++) {
        
		pthread_create(&id_th[a], NULL, philos, &phil[a]);

		printf("Philosopher %d is thinking\n", a + 1);
	}

	for (a = 0; a < N; a++)

		pthread_join(id_th[a], NULL);
}
