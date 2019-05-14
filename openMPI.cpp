#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>
#include <omp.h>
#include <mpi.h>
#include <fstream>


#define BLOCK_LOW(i, n, k) ((i) * (n) / (k))
#define BLOCK_HIGH(i, n, k) (BLOCK_LOW((i) + 1, n, k) - 1)
#define BLOCK_SIZE(i, n, k) (BLOCK_LOW((i) + 1, n, k) - BLOCK_LOW(i, n, k))
#define BLOCK_OWNER(index, n, k) ((((k) * (index) + 1) - 1) / (n))
#define ROOT 0
unsigned numberOfProcessses=0;
using namespace std;




double sieveMPIAndOpenMP(unsigned exponent, unsigned int n_threads) {
    MPI_Init( NULL, NULL );
	unsigned long long n= pow(2,exponent);
    

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
    
    unsigned long long ksToSee= sqrt(n)-2;
    vector<bool> primesHelper(ksToSee,false);
    vector<unsigned long long> ks;
	unsigned long long kHelper=3;
	
	
	 do
    {
        for (unsigned long long j = kHelper*kHelper ; j<=ksToSee ; j+=2*kHelper)
        {   primesHelper[j>>1]=true;
        }
        
        do
        {
            kHelper+=2;
        }while (kHelper*kHelper <= ksToSee && primesHelper[kHelper>>1]);
        
    } while (kHelper*kHelper <= ksToSee);
    

    for (unsigned long long i=1; i<=ksToSee; i+=2)
        if (!primesHelper[i>>1])
            ks.push_back(i);
          
    vector<bool> primes(blockSize,false);
    for (unsigned long long i=1, k = ks[0]; i < ks.size() ;i++) {

        if(k*k < blockLow){
            if(blockLow % k == 0){
                startBlockValue = blockLow;
            }else{
                startBlockValue = (blockLow + (k-(blockLow % k)));
            }
        }else{
            startBlockValue = k*k;
        }

        // Mark as true all multiples of k between k*k and n
        for (unsigned long long i = startBlockValue; i <= blockHigh; i += k){
            primes[i-blockLow] = true;
        }
		
		k= ks[i];
		 
    }

    int blockNumberOfPrimes=0;
    for(unsigned long long i = 0; i < primes.size(); i++){
        if (!primes[i])
            blockNumberOfPrimes++;
    }

    MPI_Reduce(&blockNumberOfPrimes, &numberOfPrimes, 1, MPI_INT, MPI_SUM, ROOT, MPI_COMM_WORLD);
    
    if(worldRank == ROOT){
        timeCounter += MPI_Wtime();
        cout << numberOfPrimes << endl;
        /*std::ofstream myfile;
	    myfile.open("Benchmark2.csv", ofstream::out | ofstream::app);
        myfile << "4," << exponent << "," << setprecision(3) <<timeCounter <<"," <<numberOfProcessses <<"," <<n_threads<<",\n";*/
        cout << "4,"  << exponent << "," << setprecision(3) <<timeCounter << "," <<numberOfProcessses <<"," <<n_threads<<",\n";
        //myfile.close();

    }
    MPI_Finalize();
    return timeCounter;
}

double sieveMPI(unsigned exponent) {
    MPI_Init( NULL, NULL );
	unsigned long long n= pow(2,exponent);

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

        //cout <<  timeCounter << " (s)" <<endl;

        //cout << numberOfPrimes << endl;
        std::ofstream myfile;
	    myfile.open("Benchmark2.csv", ofstream::out | ofstream::app);
        myfile << "3," << exponent << "," << setprecision(3) <<timeCounter <<"," << numberOfProcessses<< ",\n";
        cout << "3,"  << exponent << "," << setprecision(3) <<timeCounter <<","<<numberOfProcessses<<",\n";
        myfile.close();
    }
    MPI_Finalize();
    return timeCounter;
    
}



int main(int argc, char** argv) {

    unsigned long exponent = (unsigned) atol(argv[1]);

	unsigned algNumber = (unsigned) atoi(argv[2]);

    numberOfProcessses = (unsigned) atoi(argv[3]);

	if(algNumber==1)
        sieveMPI(exponent);
    else if(algNumber==2){
        unsigned n_threads = (unsigned) atoi(argv[4]);
        sieveMPIAndOpenMP(exponent,n_threads);
    }

}







