all:
	g++ -O3 seqSieve.cpp -o sieve -fopenmp -Wall
	mpic++ -O3 openMPI.cpp -o sievempi -fopenmp -Wall

clean:
	rm -r Sieve SieveMPI SieveMPI_OMP

run:
	./sieve

normalmpi1:
	for p in  2 3 4 5 6 7 8; do \
		for n in 25 26 27 28 29 30 31 32 ; do \
			mpirun --hostfile hostfile -np $$p sievempi $$n 1 $$p; \
			mpirun --hostfile hostfile -np $$p sievempi $$n 1 $$p; \
			mpirun --hostfile hostfile -np $$p sievempi $$n 1 $$p; \
		done; \
	done; \

normalmpi2:
		for n in 25 26 27 28 29 30 31 32; do \
			mpirun --hostfile hostfile2 -np 8 sievempi $$n 1 8; \
			mpirun --hostfile hostfile2 -np 8 sievempi $$n 1 8; \
			mpirun --hostfile hostfile2 -np 8 sievempi $$n 1 8; \
		done; \

normalmpi3:
		for n in 25 26 27 28 29 30 31 32; do \
			mpirun --hostfile hostfile3 -np 16 sievempi $$n 1 16; \
			mpirun --hostfile hostfile3 -np 16 sievempi $$n 1 16; \
			mpirun --hostfile hostfile3 -np 16 sievempi $$n 1 16; \
		done; \

normalmpi4:
		for n in 25 26 27 28 29 30 31 32; do \
			mpirun --hostfile hostfile4 -np 32 sievempi $$n 1 32; \
			mpirun --hostfile hostfile4 -np 32 sievempi $$n 1 32; \
			mpirun --hostfile hostfile4 -np 32 sievempi $$n 1 32; \
		done; \

ompmpi1:

			for n in 25 26 27 28 29 30 31 32; do \
				mpirun --hostfile hostfile -np 2 sievempi $$n 2 2 6; \
				mpirun --hostfile hostfile -np 2 sievempi $$n 2 2 6; \
				mpirun --hostfile hostfile -np 2 sievempi $$n 2 2 6; \
			done; \


ompmpi2:

			for n in 25 26 27 28 29 30 31 32; do \
				mpirun --hostfile hostfile -np 4 sievempi $$n 2 4 4; \
				mpirun --hostfile hostfile -np 4 sievempi $$n 2 4 4; \
				mpirun --hostfile hostfile -np 4 sievempi $$n 2 4 4; \
			done; \	


ompmpi3:
		
			for n in 25 26 27 28 29 30 31 32; do \
				mpirun --hostfile hostfile -np 6 sievempi $$n 2 6 2; \
				mpirun --hostfile hostfile -np 6 sievempi $$n 2 6 2; \
				mpirun --hostfile hostfile -np 6 sievempi $$n 2 6 2; \
			done; \	



