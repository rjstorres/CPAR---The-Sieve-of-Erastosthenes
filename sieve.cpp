#include <iostream>
#include <iomanip>
#include <math.h>
#include <limits.h>
#include <time.h>

using namespace std;
#define SYSTEMTIME clock_t

void sieveSequential(){
	long long n;
    
    cout << "Power of 2: ";
    cin >> n;
 
    n = pow(2,n);
	cout << " -> " << n << endl;
    
    cout <<  "\nCalculating prime numbers..." << endl;
    
    bool *primes = new bool[n];
    long long k = 3;

	SYSTEMTIME Time1, Time2;	
	char st[100];

	Time1 = clock();
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
    Time2 = clock();
	sprintf(st, "Time: %3.3f seconds\n", (double)(Time2 - Time1) / CLOCKS_PER_SEC);
	cout << st;
    
    cout << "\nCounting prime numbers..." << endl;
	Time1 = clock();
    long long primesFound = 0;
    for (long long i=1; i<n; i+=2)
        if (!primes[i>>1])
            //cout << i << " ";
			primesFound++;
	Time2 = clock();
	sprintf(st, "Time: %3.3f seconds\n", (double)(Time2 - Time1) / CLOCKS_PER_SEC);
	cout << st;
	cout << "\nFound " << primesFound << " prime numbers." << endl;
}


int main (int argc, char *argv[])
{
	sieveSequential();
}

