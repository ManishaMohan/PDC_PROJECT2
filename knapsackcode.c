#include<iostream>

#include <stdio.h>

#include <stdlib.h>

#include<pthread.h>

#include<time.h>

#include<exception>

using namespace std;


const int p = 4; //number of processors kept constant due to lack of resources


//Structure for knapsack problem

//to be sent as argument to thread

//m: number of elements

//c: capacity of knapsack

//wt[i]: weight of ith element

//val[i]: value of ith element

struct arg{

int m,c;

short *wt;

short *val;

};

//Structure for combining algorithm

//to be sent as argument to thread

//size: the capacity of knapsack

//left[i] and right[i] are the profit array

//obtained from left and right processors in the process tree struct arg2{
 
int size;

int *left;

int *right;

};

//Random test case generator

void testcase(arg &G){

G.wt = new short[G.m];

G.val = new short[G.m];

for(int i=0;i<G.m;i++){

G.val[i] = rand()%100;

G.wt[i] = rand()%(G.c­1)+1;

}

}

//Function to calculate max of 2 numbers

int max(int a, int b) {

return (a > b)? a : b;
 

}

//dynamic solution for knapsack

//each thread solves this at base level

//returns the profit array instead of solution

void* knapSack(void *argu){

arg *Q = (arg*)argu;
 
//argu converted back to arg to obtain
 

data

int m = Q­>m,c = Q­>c;

short *wt = Q­>wt,*val = Q­>val;

int i, w;

int f[m+1][c+1]; //profit function


for (i = 0; i <= m; i++){

for (w = 0; w <= c; w++){

if (i==0 || w==0)

f[i][w] = 0;

else if (wt[i­1] <= w){
 
f[i][w] = max(val[i­1] + f[i­1][w­wt[i­1]], f[i­1][w]);

}

else

f[i][w] = f[i­1][w];

}
 

}

int *a = new int[c+1];

for(int i=0;i<=c;i++){

a[i]=f[m][i];
 


//initializing the profit vector
 

}

/*for(w=0;w<=c;w++) cout<<"a"<<w<<" = "<<a[w]<<"\n";

for(w=0;w<=c;w++){

for(i=0;i<=m;i++){

a[w] = max(a[w],f[i][w]);

}

}*/

pthread_exit(a);

}

//combine the profit array of 2 processes

//returns the overall profit array

void *combine(void *argu){

arg2 *Q = (arg2*)argu;

int size = Q­>size;

int *left = Q­>left;

int *right = Q­>right;

int *a = new int[size+1];

for(int i=0;i<=size;i++){

a[i] = 0;

for(int j=0;j<=i;j++){

a[i] = max(a[i],left[j]+right[i­j]);

}

}
 
pthread_exit(a);

}

int main()

{


srand (time(NULL));//for generating the test cases pthread_t PR[p],t;//declaring the threads

void *a[4],*b;//to hold the returned array from each thread int *A[4],*B;//to convert the returned array to <int>

arg G,g[4];//G takes the initial knapsack problem and g[i] take parts of the problem

G.c = 50;//Set capacity

G.m = 40;//Set number of elements

testcase(G);


clock_t T = clock();//for calculating the runtime

//solving the problem directly without parallel programming

pthread_create(&t,NULL,knapSack,(void*)&G);

pthread_join(t,&b);

B = (int*)b;

cout<<"max profit = "<<B[G.c];

printf("\nTime taken using single processor: %.5fs\n", (double)(clock() ­ T)/CLOCKS_PER_SEC);

T = clock();//resetting the clock for parallel programming


//Divide and Conquer

//creating subproblems from initial problem //and start threads to solve them for(int i=0;i<4;i++){

g[i].c = G.c;

g[i].m = G.m/p;

g[i].wt = new short[g[i].m];
 
g[i].val = new short[g[i].m];

for(int j=0;j<g[i].m;j++){

g[i].wt[j] = G.wt[i*G.m/p+j];

g[i].val[j] = G.val[i*G.m/p+j];

}

pthread_create(&PR[i],NULL,knapSack,(void*)&g[i]);

}


for(int i=0;i<4;i++){

pthread_join(PR[i],&a[i]);

delete[] g[i].wt;

delete[] g[i].val;

A[i] = (int*)a[i];

}

//preparing the profit arrays for combining process

arg2 g21[2],g22;

g21[0].size = G.c;

g21[1].size = G.c;

g22.size = G.c;

void *g22l,*g22r;

g21[0].left = A[0];

g21[0].right = A[1];

g21[1].left = A[2];

g21[1].right = A[3];


//combining the profit arrays in pairs

//then collecting the arrays and combinging again

//until a single profit array is left

pthread_create(&PR[0],NULL,combine,(void*)&g21[0]);

pthread_create(&PR[1],NULL,combine,(void*)&g21[1]);

pthread_join(PR[0],&g22l);

pthread_join(PR[1],&g22r);

g22.left = (int*)g22l;
 
g22.right = (int*)g22r;

pthread_create(&PR[0],NULL,combine,(void*)&g22);

pthread_join(PR[0],&g22l);

int *A2 = (int*)g22l;

cout<<A2[G.c];

printf("\nTime taken using single processor: %.5fs\n", (double)(clock() ­ T)/CLOCKS_PER_SEC);

return 0;

}
