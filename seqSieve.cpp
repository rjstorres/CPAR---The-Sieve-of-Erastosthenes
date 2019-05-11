#include <iostream>
#include <iomanip>
#include <math.h>
#include <omp.h>
#include <fstream>
using namespace std;

double sequentialSieve (unsigned long long n)
{
 
    n = pow(2,n);
    
    bool *primes = new bool[n];
    
    unsigned long long k = 3;
    double timeCounter = -(double)clock();
    do
    {
        for (unsigned long long j = k*k ; j<n ; j+=2*k)
        {   primes[j>>1]=true;
        }
        
        do
        {
            k+=2;
        }while (k*k <= n && primes[k>>1]);
        
    } while (k*k <= n);

    timeCounter += (double)clock();
    timeCounter /= CLOCKS_PER_SEC;
    return timeCounter;
    /*int numberOfPrimes=0;
    for (unsigned long long i=1; i<n; i+=2)
        if (!primes[i>>1])
            numberOfPrimes++;*/

}

double sieveOpenMp(unsigned long long n,  int n_threads){
    n = pow(2,n);
    
    bool *primes = new bool[n]; //initialized as false
    
    unsigned long long k = 3;
    double timeCounter = -(double)omp_get_wtime();
    do
    {
        #pragma omp parallel for num_threads(n_threads)
        for (unsigned long long j = k*k ; j<n ; j+=2*k)
        {   primes[j>>1]=true;
        }
        
        do
        {
            k+=2;
        }while (k*k <= n && primes[k>>1]);
        
    } while (k*k <= n);

    timeCounter += (double)(omp_get_wtime());
    return timeCounter;
    /*
    int numberOfPrimes=0;
    for (unsigned long long i=1; i<n; i+=2)
        if (!primes[i>>1])
            numberOfPrimes++;*/

    
}

int main(){

    std::ofstream myfile;
	myfile.open("Benchmark1.csv");
    myfile << "Algorithm, Run Number, 2^n size, Time, num_threads\n";

    for (int j = 25; j <= 25; j ++ )
		for (int i = 0; i < 3; i++)
		{
			
			double time = sequentialSieve(j);
			
			myfile << "1," << i << "," << j << "," << setprecision(3) <<time <<",\n";
			cout << "1," << i << "," << j << "," << setprecision(3) <<time <<",\n";
		}
    for (int k=2; k<=3;k++)
        for (int j = 25; j <= 25; j ++ )
            for (int i = 0; i < 3; i++)
            {
                
                double time = sieveOpenMp(j,k);
                
                myfile << "1," << i << "," << j << "," << setprecision(3) <<time <<"," <<k<<",\n";
                cout << "1," << i << "," << j << "," << setprecision(3) <<time <<"," <<k<<",\n";
            }

    return 0;
}

