#include<bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <iostream>
#include <chrono>
#include "coloring.hpp"
using namespace std;

#ifdef __cplusplus
extern"C" {
#endif
#include "graphio.h"
#include "graph.h"
#ifdef __cplusplus
}
#endif

char gfile[2048];
ofstream myfiles;
void usage() {
	printf("./coloring <filename> -t=THREADS\n");
	exit(0);
}

/*
  You can ignore the ewgths and vwghts. They are there as the read function expects those values
  row_ptr and col_ind are the CRS entities. nov is the Number of Vertices
*/

/* ====== THE ALGORITHM ========
	The algorithm implemented in this project follows the methodologies proposed by Deveci et al.
					" Parallel Graph Coloring for Manycore Architectures
 */
void Store_Result(int result,string s)
{
	if(strcmp(gfile,"af_shell10.mtx")==0)
	{
		myfiles.open("af_shell10.txt",std::ios::app);
		myfiles<<result<<" "<<s<<"\n";
		myfiles.close();
	}
	else if(strcmp(gfile,"bone010.mtx")==0)
	{
		myfiles.open("bone010.txt",std::ios::app);
		myfiles<<result<<" "<<s<<"\n";
		myfiles.close();
	}
	else if(strcmp(gfile,"coPapersDBLP.mtx")==0)
	{
		myfiles.open("coPapersDBLP.txt",std::ios::app);
		myfiles<<result<<" "<<s<<"\n";
		myfiles.close();
	}	
	if(strcmp(gfile,"nlpkkt120.mtx")==0)
	{
		myfiles.open("nlpkkt120.txt",std::ios::app);
		myfiles<<result<<" "<<s<<"\n";
		myfiles.close();
	}

}
int main(int argc, char *argv[]) 
{
 
	string values=argv[2];
	etype *row_ptr;
	vtype *col_ind;
	ewtype *ewghts;
	vwtype *vwghts;
	vtype nov;
	int matrix[4][32];
	std::chrono::high_resolution_clock::time_point begin, end, initial = std::chrono::high_resolution_clock::now();

	if (argc < 2)
		usage();

	const char* fname = argv[1];
	strcpy(gfile, fname);

	begin = std::chrono::high_resolution_clock::now();
	if (read_graph(gfile, &row_ptr, &col_ind, &ewghts, &vwghts, &nov, 0) == -1) {
		printf("error in graph read\n");
		exit(1);
	}
	end = std::chrono::high_resolution_clock::now();
	cout << "Graph file read [" << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms]"
	 	<<endl<< "Starting graph coloring procedure" << endl;
	GraphColoring coloring(row_ptr, col_ind, nov);
	begin = std::chrono::high_resolution_clock::now();
	coloring.perform_coloring();
	end = std::chrono::high_resolution_clock::now();
	cout << "Coloring finished [" << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms]" << std::endl
		<< "Starting accuracy computation procedure" << std::endl;
	double accuracy = coloring.accuracy()*100;	
	end = std::chrono::high_resolution_clock::now();
	int result= std::chrono::duration_cast<std::chrono::milliseconds>(end - initial).count();
	cout<< "Coloring accuracy: " << accuracy << "%" <<endl << "Total time: "<< result << " ms" <<endl;
	Store_Result(result,values);
	cout <<endl;

	return 0;
}
