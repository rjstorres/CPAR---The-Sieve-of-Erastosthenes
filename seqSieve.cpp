#include <iostream>
#include <iomanip>
#include <math.h>
#include <omp.h>
using namespace std;

void sequentialSieve (unsigned long long n)
{
 
    n = pow(2,n);
    
    bool *primes = new bool[n];
    
    unsigned long long k = 3;
    float clock_time = (float)clock();
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
     clock_time = (float)((clock() - clock_time)/CLOCKS_PER_SEC);

    int numberOfPrimes=0;
    for (unsigned long long i=1; i<n; i+=2)
        if (!primes[i>>1])
            numberOfPrimes++;

    cout << numberOfPrimes << endl;
}

void sieveOpenMp(unsigned long long n,  int n_threads){
    n = pow(2,n);
    
    bool *primes = new bool[n]; //initialized as false
    
    unsigned long long k = 3;
    float clock_time = (float)omp_get_wtime();
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

    clock_time = (float)((omp_get_wtime() - clock_time));

    int numberOfPrimes=0;
    for (unsigned long long i=1; i<n; i+=2)
        if (!primes[i>>1])
            numberOfPrimes++;

    cout << numberOfPrimes << endl;
}

int main(){
    sieveOpenMp(25,2);
    sieveOpenMp(32,2);
    return 0;
}

