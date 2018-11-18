#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NMAX 50

typedef struct s_product{
	int id_customer;
	int data;
}Product;

//Finished products array
Product products[NMAX] = { 0 };//products[i] : ith customer's product
int pipes[NMAX][2] = { 0 };
int numW = 0, numC = 0;
int workers[NMAX];
int customers[NMAX];

void worker(int num);
void customer(int num);

int main(int argc; char **argv){
	//Preparation
	time_t t;

	srand((unsigned)time(&t));

	if (argv != 3){
		fprintf(stderr, "Usage : %s num_workers num_customers");
		exit(9);
	}

	sscanf(argv[1],"%d",&numW);
	sscanf(argv[2],"%d",&numC);

	//numW-1 pipes for chainworking
	for (int i = 0 ; i < numW ; ++i){
		if (pipe(pipes[i]) == -1){
			perror("Pipe error");
			exit(8);
		}
	}

	//create workers and customers processes
	for (int i = 0 ; i < numW ; ++i){
		if ((workers[i] = fork()) == -1){
			perror("Fork error");
			exit(99);
		}else if (workers[i] == 0){
			worker(i);
		}
	}

	for (int i = 0 ; i < numC ; ++i){
		if ((customers[i] = fork()) == -1){
			perror("Fork error");
			exit(99);
		}else if (customers[i] == 0){
			customer(i);
		}
	}

	//wait for all childs to finish.
	wait(NULL);

	return 0;
}

void worker(int num){
	//General traitment
	close(pipes[num][1]);
	Product p;
	if (num > 0 && num < numW-1){
		//get data from previous worker
		read(pipes[num-1][1], &p, sizeof(p));
		p.data += rand()%100;
		write(pipes[num][0], &p, sizeof(p));
		close(pipes[num-1][1]);
		close(pipes[num][0]);
	}else if (!num){
		//first one must create the product
		p = 
		p.data = rand()%100;
		write(pipes[num][0], &p, sizeof(p));
		close(pipes[num][0]);
		//waits for next customer
	}else{
		read(pipes[num-1][1], &p, sizeof(p));
		p.data += rand()%100;
		products[p.id_customer] = p;
		close(pipes[num-1][1]);
	}
}

void customer(int num){

}