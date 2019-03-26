#define N 5000
#include <stdio.h>
#include <sys/time.h>

double mysecond();

int main(){
  int i,j,k;
  double t1, t2; // timers
  double a[N], b[N], c[N]; // arrays

  // init arrays
  for (i = 0; i < N; i++){
    a[i] = 47.0;
    b[i] = 3.1415;
  }

  // avoid cold start
  for(i = 0; i < N; i++)
    c[i] = a[i]*b[i];


  // avoid interference
  #define INTEF_REPEAT 50
  double min = 1e10;
  double sum = 0;
  double dummy = 0;
  
  for(k = 0; k < INTEF_REPEAT; k++){
    
	// avoid clock granularity
    #define GRAN_REPEAT 100
    double totalTime = 0;

    for(j = 0; j < GRAN_REPEAT; j++){
      // measure performance
      t1 = mysecond();

      for(i = 0; i < N; i++)
        c[i] = a[i]*b[i];

      t2 = mysecond();
      totalTime += (t2 - t1);
    }

    // avoid lazy evaluation
   for(i = 0; i < N; i++)
      dummy+= c[i];

    double iteration_result = totalTime/(float)GRAN_REPEAT;
	
	if(iteration_result < min) min = iteration_result;
	sum+= iteration_result;
	
	
  }
  
  printf("result for avoiding lazy opt : %f\n", dummy);
  printf("min exec time = %11.8fs\n", min)
  printf("average exec time = %11.8fs\n", sum/(float)INTEF_REPEAT)
  
  return 0;
}

// function with timer
double mysecond(){
  struct timeval tp;
  struct timezone tzp;
  int i;

  i = gettimeofday(&tp,&tzp);
  return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}