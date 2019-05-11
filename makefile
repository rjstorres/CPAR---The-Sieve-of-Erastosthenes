all:
	g++ -O3 seqSieve.cpp -o sieve -fopenmp -Wall
	mpic++ -O3 openMPI.cpp -o sievempi -fopenmp -Wall

clean:
	rm -r Sieve SieveMPI SieveMPI_OMP

run:
	./sieve

normalmpi1:
		for n in 25 26 27 28 29 30 31 32 ; do \
			mpirun --hostfile hostfile -np 4 sievempi $$n 1 4 ; \
			mpirun --hostfile hostfile -np 4 sievempi $$n 1 4; \
			mpirun --hostfile hostfile -np 4 sievempi $$n 1 4; \
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
		for t in 2 4 6 8; do \
			for n in 25 26 27 28 29 30 31 32; do \
				mpirun --hostfile hostfile -np 4 sievempi $$n 2 4 $$t; \
				mpirun --hostfile hostfile -np 4 sievempi $$n 2 4 $$t; \
				mpirun --hostfile hostfile -np 4 sievempi $$n 2 4 $$t; \
			done; \
		done; \

ompmpi2:
		for t in 2 4 6 8; do \
			for n in 25 26 27 28 29 30 31 32; do \
				mpirun --hostfile hostfile2 -np 8 sievempi $$n 2 8 $$t; \
				mpirun --hostfile hostfile2 -np 8 sievempi $$n 2 8 $$t; \
				mpirun --hostfile hostfile2 -np 8 sievempi $$n 2 8 $$t; \
			done; \	
		done; \

ompmpi3:
		for t in 2 4 6 8; do \
			for n in 25 26 27 28 29 30 31 32; do \
				mpirun --hostfile hostfile3 -np 16 sievempi $$n 2 16 $$t; \
				mpirun --hostfile hostfile3 -np 16 sievempi $$n 2 16 $$t; \
				mpirun --hostfile hostfile3 -np 16 sievempi $$n 2 16 $$t; \
			done; \	
		done; \

ompmpi4:
		for t in 2 4 6 8; do \
			for n in 25 26 27 28 29 30 31 32; do \
				mpirun --hostfile hostfile4 -np 32 sievempi $$n 2 32 $$t; \
				mpirun --hostfile hostfile4 -np 32 sievempi $$n 2 32 $$t; \
				mpirun --hostfile hostfile4 -np 32 sievempi $$n 2 32 $$t; \
			done; \	
		done; \


