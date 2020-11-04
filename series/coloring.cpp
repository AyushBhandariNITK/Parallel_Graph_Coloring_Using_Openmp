#include "coloring.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <exception>
#include <chrono>
#include <iostream>
#include <algorithm>

#include <ctime>
#include <ratio>
#include <chrono>

#include <forward_list>

using namespace std;
using namespace std::chrono;

typedef unsigned int uint;

#define UNKNOWN 0

GraphColoring::GraphColoring(etype * row_ptr, vtype* col_ind, int nov) {
	num_vertices = nov;
	num_colored = 0;
	this->row_ptr = row_ptr;
	this->col_ind = col_ind;
	colors = new unsigned int[nov];
	unvisited_vertices.resize(nov);
	for (int i = 0; i < nov; i++) {
	  unvisited_vertices[i] = i;
	}
	unvisited_vertices_tail = nov;
	
	for (int i = 0; i < nov; i++) {
	  colors[i] = 0;
	}
	
	forbiddens.resize(nov);

	if (colors == nullptr) {
		throw std::bad_alloc();
	}
}

GraphColoring::~GraphColoring() {
	delete[] row_ptr;
	delete[] col_ind;
}

bool GraphColoring::allColored() const {
  bool allIsColored = true;
  uint num_colored = 0;
  for (int i = 0; i < num_vertices; i++) {
    if (colors[i] == UNKNOWN) {
      allIsColored = false;
    }
    num_colored++;
  }
  return allIsColored;
}

bool GraphColoring::accuracy() const {
	int false_colored = 0;
	for (int v_i = 0; v_i < num_vertices; v_i++) {
		for (int neighbor_ind = row_ptr[v_i]; neighbor_ind < row_ptr[v_i + 1]; neighbor_ind++) {
		  const int neighbor = col_ind[neighbor_ind];
			if (colors[neighbor] == colors[v_i]) {
				false_colored++;
			}
		}
	}
	false_colored /= 2;
	
	int color_with_max_id = 0;
	{
		int t_max = -1;
		for (int i = 0; i < num_vertices; i++) {
			t_max = std::max(t_max, static_cast<int>(colors[i]));
		}
		{
			color_with_max_id = std::max(t_max, color_with_max_id);
		}
	}
	std::cout << color_with_max_id << " color were used" << std::endl;
	cout << "neighbors having same colors: " << false_colored << std::endl;
	return ((num_vertices - false_colored) / static_cast<double>(num_vertices));
}

void GraphColoring::printColors() const {
	for (int i = 0; i < num_vertices; i++) {
		cout << "color " << i << ": " << colors[i] << endl;
	}
}

void GraphColoring::perform_coloring() {
	std::chrono::high_resolution_clock::time_point step_beg, step_end = std::chrono::high_resolution_clock::now();
	uint num_iterations = 0;
	bool coloringFinished = false;
	while (!coloringFinished) {
	  step_beg = std::chrono::high_resolution_clock::now();
	  assignColors();
	  step_end = std::chrono::high_resolution_clock::now();
	  cout << "assigned colors in " << std::chrono::duration_cast<std::chrono::milliseconds>(step_end - step_beg).count() << " s" << endl;
	  step_beg = std::chrono::high_resolution_clock::now();
	  detectConflicts();
	  step_end = std::chrono::high_resolution_clock::now();
	  cout << "detected conflicts in " << std::chrono::duration_cast<std::chrono::milliseconds>(step_end - step_beg).count() << " s" << endl;
	  step_beg = std::chrono::high_resolution_clock::now();
	  forbidColors();
	  step_end = std::chrono::high_resolution_clock::now();
	  cout << "forbid colors in " << std::chrono::duration_cast<std::chrono::milliseconds>(step_end - step_beg).count()  << " s" << endl;
	  num_iterations++;
	  coloringFinished = allColored();
	  cout << "ITERATION " << num_iterations << endl;
	}
}


void GraphColoring::assignColors() {
  for (uint vertex_ind = 0; vertex_ind < unvisited_vertices_tail; vertex_ind++) {
    const uint vertex = unvisited_vertices[vertex_ind];
    colors[vertex] = getNextColor(vertex);		
  }
}

void GraphColoring::detectConflicts() {
	// Traverse edges of the graph, for edges (u, v) where C[u] = C[v] reset color of min(u, v)
  for (unsigned int vertex_ind = 0; vertex_ind < unvisited_vertices_tail; vertex_ind++) {
    const uint vertex = unvisited_vertices[vertex_ind];
		for (unsigned int neighbor_ind = row_ptr[vertex]; neighbor_ind < row_ptr[vertex + 1]; neighbor_ind++) {
		  const uint neighbor_label = col_ind[neighbor_ind];
		    if (colors[neighbor_label] == colors[vertex]) {
		      const uint minLabel = std::min(neighbor_label, vertex);
		      colors[minLabel] = UNKNOWN;		   
		    }
		}
    }
    unvisited_vertices.clear();
    unvisited_vertices_tail = 0;

    for (int i = 0; i < num_vertices; i++) {
      if (colors[i] == UNKNOWN) {
	unvisited_vertices[unvisited_vertices_tail++] = i;
      }
    }
}  

void GraphColoring::forbidColors() {
	// Search for pair of vertices (u, v) such that exactly one of the vertices is colored. Then, forbid
	// the color of the one having the color on the other
  for (unsigned int vertex_ind = 0; vertex_ind < unvisited_vertices_tail; vertex_ind++) {
    const uint vertex = unvisited_vertices[vertex_ind];
    const uint vertex_color = colors[vertex];
    for (uint neighbor_ind = row_ptr[vertex]; neighbor_ind < row_ptr[vertex + 1]; neighbor_ind++) {
      const uint neighbor_label = col_ind[neighbor_ind];
      if (colors[neighbor_label] != UNKNOWN) {
	forbiddens[vertex].insert(colors[neighbor_label]);
      }		   
    }
  }		
}

unsigned int GraphColoring::getNextColor(const uint & vertex) const {
  uint minColor = 0;
  bool colorValid = false;
  while (!colorValid) {
    minColor++;
    colorValid = true;
    for (int neighbor_ind = row_ptr[vertex]; neighbor_ind < row_ptr[vertex + 1] && colorValid; neighbor_ind++) {
      const uint neighbor = col_ind[neighbor_ind];
      if (colors[neighbor] == minColor) {
	colorValid = false;
      }
    }
  }
  return minColor;
}


