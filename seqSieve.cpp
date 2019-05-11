#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

void sequentialSieve (unsigned long long n)
{
 
    n = pow(2,n);
    
    bool *primes = new bool[n];
    
    long long k = 3;
    clock_time = (float)clock();
    do
    {
        for (long long j = k*k ; j<n ; j+=2*k)
        {   primes[j>>1]=true;
        }
        
        do
        {
            k+=2;
        }while (k*k <= n && primes[k>>1]);
        
    } while (k*k <= n);
    clock_time = (float)((clock() - clock_time)/CLOCKS_PER_SEC);

    int numberOfPrimes;
    for (unsigned long long i=1; i<n; i+=2)
        if (!primes[i>>1])
            numberOfPrimes++;

    cout << numberOfPrimes << endl;
}

void sequentialSieveOpenMp(unsigned long long n,  int n_threads){
    n = pow(2,n);
    
    bool *primes = new bool[n]; //initialized as false
    
    long long k = 3;
    clock_time = (float)omp_get_wtime();
    do
    {
        #pragma omp parallel for num_threads(n_threads)
        for (long long j = k*k ; j<n ; j+=2*k)
        {   primes[j>>1]=true;
        }
        
        do
        {
            k+=2;
        }while (k*k <= n && primes[k>>1]);
        
    } while (k*k <= n);

    clock_time = (float)((omp_get_wtime() - clock_time));

    int numberOfPrimes;
    for (int i=1; i<n; i+=2)
        if (!primes[i>>1])
            numberOfPrimes++;

    cout << numberOfPrimes << endl;
}

