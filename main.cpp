#include <iostream>
#include <winbgim.h>
#include "redes_neurais.h"

int main() {
	
	int n; //number of neurons
		
	connectivity_matrix mat;
//	mat.initialize_identity(5);
//	mat.print_layers();
//	mat.print_matrix();
//	mat.create_new_layer(10);
//	mat.print_layers();
//	mat.print_matrix();
//	mat.create_new_layer(3);
//	mat.connect_layers(1, 2);
//	mat.connect_layers(1, 0);
//	mat.print_layers();
//	mat.print_matrix();
//	mat.draw_network();
//	
//	mat.print_coordinate_vector();
//	
//	mat.record_network("network.txt");
	mat.load_network("netw.txt");
	
	return 0;
}
