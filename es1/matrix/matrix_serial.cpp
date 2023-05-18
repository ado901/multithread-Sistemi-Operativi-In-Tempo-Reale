/* matrix_serial.c */

#include <vector> 
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include "matrix.h"
#include <vector>
/* dimensione standard della matrice */
const size_t STD_SIZE = 10;

/* dimensione max per stampare le matrici a video */
const size_t MAX_PRINT_SIZE = 30;

/* Questa funzione moltiplica una riga di M1 per una colonna di M2
   posizionando il risultato in Res[row][column]
*/
void mult(size_t row, size_t column, const matrix & M1, const matrix & M2, matrix & Res)
{
	Res(row,column) = 0;

	for(size_t position = 0; position < Res.size(); ++position)
	{
		Res(row,column) += M1(row,position) * M2(position,column);
	}
}
void multrow(int row, const matrix & M1, const matrix & M2, matrix & Res)
{
	for (size_t j = 0; j < Res.size(); ++j)
		mult(row, j, M1, M2, Res);
			
}


int main(int argc, char * argv[])
{
	auto cores = std::thread::hardware_concurrency();
	std::cout << "Cores: " << cores << std::endl;
	size_t size = STD_SIZE;

	if (argc > 1)
	{
		std::istringstream iss(argv[1]);
		iss >> size;
	}

	matrix MA(size), MB(size), MC(size);

	/* Riempimento delle matrice MA e MB: valori di esempio */
	for (size_t i = 0; i < size; ++i)
		for (size_t j = 0; j < size; ++j)
		{
			MA(i,j) = 1;
			MB(i,j) = i + j + 1;
		}

	if (size < MAX_PRINT_SIZE)
	{
		std::cout << "Matrix Ma:" << std::endl << MA;
		std::cout << "Matrix Mb:" << std::endl << MB;
	}
	std::vector <std::thread> mythreads; 
	auto start= std:: chrono::high_resolution_clock::now();
	/* Calcolo del prodotto .............................. */
	for(size_t i = 0; i < size; ++i)
		mythreads.push_back(std::thread(multrow, i, std::ref(MA), std::ref(MB), std::ref(MC))); //potrei fare un move (anzi meglio per come funzione push_back) visto che thread non è copiabile
		//potrei usare emplace_back per dire esplicitamente di costruire un oggetto thread

	for (auto & t : mythreads)
		t.join();
	/* ................................................... */
	auto end= std:: chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = end-start;
	std::cout << "Time: " << diff.count() << " s" << std::endl;
	if (size < MAX_PRINT_SIZE)
	{
		std::cout << "Matrix Mc (Ma + Mb):" << std::endl << MC;
	}

	return 0;
}

