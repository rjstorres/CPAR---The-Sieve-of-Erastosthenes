#include <iostream>
#include <iomanip>
#include <math.h>
#include <omp.h>
#include <fstream>
#include <vector>
using namespace std;

double sequentialSieve (unsigned long long n)
{
 
    n = pow(2,n);
    
    vector<bool> primes(n,false);
    
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

    int numberOfPrimes=0;
    for (unsigned long long i=1; i<n; i+=2)
        if (!primes[i>>1])
            numberOfPrimes++;

    timeCounter += (double)clock();
    timeCounter /= CLOCKS_PER_SEC;
    return timeCounter;


}

double sieveOpenMp(unsigned long long n,  int n_threads){
    n = pow(2,n);
    
    vector<bool> primes(n,false); //initialized as false
    
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

    int numberOfPrimes=0;
    for (unsigned long long i=1; i<n; i+=2)
        if (!primes[i>>1])
            numberOfPrimes++;

    timeCounter += (double)(omp_get_wtime());
    return timeCounter;


    
}

int main(){

    std::ofstream myfile;
	myfile.open("Benchmark1.csv");
    myfile << "Algorithm, Run Number, 2^n size, Time, num_threads\n";

    for (int j = 25; j <= 32; j ++ )
		for (int i = 0; i < 3; i++)
		{
			
			double time = sequentialSieve(j);
			
			myfile << "1," << i << "," << j << "," << setprecision(3) <<time <<",\n";
			cout << "1," << i << "," << j << "," << setprecision(3) <<time <<",\n";
		}
    for (int k=2; k<=8;k++)
        for (int j = 25; j <= 32; j ++ )
            for (int i = 0; i < 3; i++)
            {
                
                double time = sieveOpenMp(j,k);
                
                myfile << "1," << i << "," << j << "," << setprecision(3) <<time <<"," <<k<<",\n";
                cout << "1," << i << "," << j << "," << setprecision(3) <<time <<"," <<k<<",\n";
            }

    return 0;
}

