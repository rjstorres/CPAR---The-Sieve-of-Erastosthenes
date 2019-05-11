#include "OpenMPISieve.h"


#define BLOCK_LOW(i, n, k) ((i) * (n) / (k))
#define BLOCK_HIGH(i, n, k) (BLOCK_LOW((i) + 1, n, k) - 1)
#define BLOCK_SIZE(i, n, k) (BLOCK_LOW((i) + 1, n, k) - BLOCK_LOW(i, n, k))
#define BLOCK_OWNER(index, n, k) ((((k) * (index) + 1) - 1) / (n))
#define ROOT 0
using namespace std;

void openMPISieve(unsigned long long n) {
    MPI_Init( NULL, NULL );
    n = pow(2, n);
	double openMPITime = 0;
	
	unsigned long long startBlockValue, counter = 0, numberOfPrimes = 0;

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	// calculate the lower and higher values of each process and its offset
	unsigned long long blockSize = BLOCK_SIZE(world_rank, n - 1, world_size);
	unsigned long long lowValue = BLOCK_LOW(world_rank, n - 1, world_size) + 2;
	unsigned long long highValue = BLOCK_HIGH(world_rank, n - 1, world_size) + 2;

	// initializes the list
	bool *primes = new bool[blockSize];

	MPI_Barrier (MPI_COMM_WORLD);

	if(world_rank == ROOT) {

		openMPITime = -MPI_Wtime();
	}

	for (unsigned long long k = 2; k*k <= n;) {
		// calculate the start block value to each process
	    if(k*k < low_value){

            if(low_value % k == 0)
                startBlockValue = low_value;
            else
                startBlockValue = (low_value + (k-(low_value % k)));

		} else {
			startBlockValue = k*k;
		}

		 // Mark as true all multiples of k between k*k and n
		for (unsigned long long i = startBlockValue; i <= highValue; i += k)
			primes[i - lowValue] = true;

		// get the next prime to broadcast it to the other processes
		if (world_rank == ROOT) {
			do {
				k++;
			} while (primes[k - lowValue] && k*k < highValue);
		}

		MPI_Bcast(&k, 1, MPI_LONG, 0, MPI_COMM_WORLD);
	}

	if(world_rank == ROOT) {
		openMPITime += MPI_Wtime();
        //return OpenMpiTime
	}

	// count all the Primes
	for (unsigned long long i = 0; i < blockSize; i+=2)
		if (!primes[i])
			counter++;

	// reduce the counter to multiple processes
	if (world_size > 1)
		MPI_Reduce(&counter, &numberOfPrimes, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
	else
		numberOfPrimes = counter;

	if(world_rank == ROOT)
		openMPITime += MPI_Wtime();
        
    MPI_Finalize();
	free(primes);
}





