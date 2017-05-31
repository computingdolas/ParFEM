#include<iostream>
#include<omp.h>


int main () {


int i = 0 ;
double a = 0.0 ;

double *data = new double [20] ;

#pragma omp for 
for (i =0 ; i < 20 ; ++i){

 a = a* 2 ; 


}


return 0 ; 


}


