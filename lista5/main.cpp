#include <iostream>
#include <random>
#include <ctime>
#include <mpi.h>

double pi(double N)
{
	int n = 0;

	std::mt19937 generator;
	generator.seed(std::time(0));

	std::uniform_real_distribution<double> random(0, 1);

	double x, y;
	 
	for (int i = 1; i <= N; i++)
	{
		x = random(generator);
		y = random(generator);

		if (pow(x, 2) + pow(y, 2) <= 1)
		{
			n++;
		}
	}

	return n;
}

int main()
{
	// inicjalizacja mechanizmu MPI
	// od momentu wywolania tej funkcji aplikacja staje sie wielowatkowa
	MPI_Init(NULL, NULL);

	int rank, size;

	// pobiera identyfikator procesu
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// pobiera ilosc dostepnych procesow
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int N = 100000000;
	double n = pi(N);

	// wstrzymuje prace procesu az do wywolania tej funkcji przez wszystkie procesy nalezace do grupy
	MPI_Barrier(MPI_COMM_WORLD);

	double g_n = 0.0;

	// obliczanie rownolegle, wynik operacji trafia do jednego wybranego procesu
	MPI_Reduce(&n, &g_n, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		double result = 4 * g_n / (N * static_cast<double>(size));
		std::cout << result << '\n';
	}

	// konczy prace w trybie MPI
	MPI_Finalize();
	
	return 0;
}
