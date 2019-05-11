#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>
#include <omp.h>
#include <mpi.h>


#define BLOCK_LOW(i, n, k) ((i) * (n) / (k))
#define BLOCK_HIGH(i, n, k) (BLOCK_LOW((i) + 1, n, k) - 1)
#define BLOCK_SIZE(i, n, k) (BLOCK_LOW((i) + 1, n, k) - BLOCK_LOW(i, n, k))
#define BLOCK_OWNER(index, n, k) ((((k) * (index) + 1) - 1) / (n))
#define ROOT 0
using namespace std;




void sieveMPIAndOpenMP(unsigned long long n, unsigned int n_threads) {
	n= pow(2,n);

    // Get the number of processes
    int worldSize;
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    // Get the rank of the process
    int worldRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

    double timeCounter = 0;
    unsigned long long startBlockValue = 0;
    int numberOfPrimes = 0;

    if(worldRank == ROOT)
        timeCounter = -MPI_Wtime();

    unsigned long long blockSize = BLOCK_SIZE(worldRank, n-1, worldSize);
    unsigned long long blockLow = 2+BLOCK_LOW(worldRank, n-1, worldSize);
    unsigned long long blockHigh = 2+BLOCK_HIGH(worldRank, n-1, worldSize);

    vector<bool> primes(blockSize,false);
    for (unsigned long long k = 2;k*k <= n;) {

        if(k*k < blockLow){
            if(blockLow % k == 0){
                startBlockValue = blockLow;
            }else{
                startBlockValue = (blockLow + (k-(blockLow % k)));
            }
        }else{
            startBlockValue = k*k;
        }

        // Mark as false all multiples of k between k*k and n
        #pragma omp parallel for num_threads(n_threads)
        for (unsigned long long i = startBlockValue; i <= blockHigh; i += k){
            primes[i-blockLow] = true;
        }

        // Set k as the smaller urmarked number > k
        if(worldRank == ROOT){
            do {
				k++;
			} while (primes[k - blockLow] && k*k < blockHigh);
        }

        MPI_Bcast(&k, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
        
    }

    int blockNumberOfPrimes=0;
    for(unsigned long long i = 0; i < primes.size(); i++){
        if (!primes[i])
            blockNumberOfPrimes++;
    }

    MPI_Reduce(&blockNumberOfPrimes, &numberOfPrimes, 1, MPI_INT, MPI_SUM, ROOT, MPI_COMM_WORLD);
    
    if(worldRank == ROOT){
        timeCounter += MPI_Wtime();

        cout << "Tempo de execucao: " <<  timeCounter << " (s)" <<endl;

        cout << "Numero de primos: " << numberOfPrimes << endl;
    }

}

void sieveMPI(unsigned long long n) {
	n= pow(2,n);

    // Get the number of processes
    int worldSize;
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    // Get the rank of the process
    int worldRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

    double timeCounter = 0;
    unsigned long long startBlockValue = 0;
    int numberOfPrimes = 0;

    if(worldRank == ROOT)
        timeCounter = -MPI_Wtime();

    unsigned long long blockSize = BLOCK_SIZE(worldRank, n-1, worldSize);
    unsigned long long blockLow = 2+BLOCK_LOW(worldRank, n-1, worldSize);
    unsigned long long blockHigh = 2+BLOCK_HIGH(worldRank, n-1, worldSize);

    vector<bool> primes(blockSize,false);
    for (unsigned long long k = 2;k*k <= n;) {

        if(k*k < blockLow){
            if(blockLow % k == 0){
                startBlockValue = blockLow;
            }else{
                startBlockValue = (blockLow + (k-(blockLow % k)));
            }
        }else{
            startBlockValue = k*k;
        }

        // Mark as false all multiples of k between k*k and n
        for (unsigned long long i = startBlockValue; i <= blockHigh; i += k){
            primes[i-blockLow] = true;
        }

        // Set k as the smaller urmarked number > k
        if(worldRank == ROOT){
            do {
				k++;
			} while (primes[k - blockLow] && k*k < blockHigh);
        }

        MPI_Bcast(&k, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
        
    }

    int blockNumberOfPrimes=0;
    for(unsigned long long i = 0; i < primes.size(); i++){
        if (!primes[i])
            blockNumberOfPrimes++;
    }

    MPI_Reduce(&blockNumberOfPrimes, &numberOfPrimes, 1, MPI_INT, MPI_SUM, ROOT, MPI_COMM_WORLD);
    
    if(worldRank == ROOT){
        timeCounter += MPI_Wtime();

        cout << "Tempo de execucao: " <<  timeCounter << " (s)" <<endl;

        cout << "Numero de primos: " << numberOfPrimes << endl;
    }

}



int main(){
    MPI_Init( NULL, NULL );

    sieveMPIAndOpenMP(25,3);
	MPI_Finalize();
}







