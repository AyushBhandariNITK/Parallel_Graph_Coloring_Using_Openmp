Parallel Graph Coloring <br>
In large-scale parallel applications a graph coloring is often carried out to schedule computational tasks.Graph coloring problem is to assign colors to certain elements of a graph subject to certain constraints. The algorithm operates in an iterative fashion, in each round vertices are speculatively colored based on limited information, and then a set of incorrectly colored vertices, to be recolored in the next round, is identified.Parallel speedup is achieved in part by using openmp.

<br>
<br>Objective<br>
1) To study parallelism in graph coloring algorithm by using Distance 1 coloring .<br>
2) Analysis of no. of colors used and the  time  required with variation of densities of graph  as well as varying the number of threads in process. <br>
3) Plot the graph for the performance comparison with different graphs and different no.of thread used. <br>
<br>
Region of Parallelism<br>
1) Assigning colors to vertices.<br>
2) Detecting conflicts between adjacent vertices.<br>
3) Forbid the coloring.<br> 
<br>
Input files<br>
 Download the input files via search of input file from https://sparse.tamu.edu/ <br>  
<br>
Execution<br>
Run the following command: <br>
make -f Makefile  <br>
<br>
For parallel <br>
./coloring inputfile.mtx no.ofthreads <br>
eg:  ./coloring bone010.mtx 4 <br>
<br>
For series <br>
./coloring inputfile.mtx <br>


