#ifndef NEURAL_NETWORKS_H
#define NEURAL_NETWORKS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <winbgim.h>
#include <string>
#include "ponto2D.h"

using namespace std;

class connectivity_matrix {
	private:
		bool **m;		//the matrix. it's a square and simmetric one
		ponto2D<int> *coord;	//spatial coordinates of the nodes, when they are drawn. coords size == summatory of all elements of layers vector
		vector<int> layers;	//number of nodes at each layer
		int n;			//matrix number of rows and columns
		void set_size(int s);	//defines the number of matrix
		void add_size(int s);	//increases the number of nodes
		void increase_matrix(int s);	//expands the matrix
		void draw_lines();				//draw the lines connecting the nodes
		void initialize_coords();	//initializes the coordinates at the origin
		void increase_coords(int s);	//expands the coordinates vector
	public:		
		void initialize_empty(int n);	//initializes an n x n matrix with 0's
		void initialize_identity(int n); //creates an n x n  identity matrix 
		void create_new_layer(int n);	//create a layer with n nodes
		void print_matrix();		//print matrix at the screen
		void print_coordinate_vector();	//prints the coordinates of the nodes at the screen
		void draw_network();		//visualize the nodes
		void print_layers();		//print the layers vector
		int biggest_layer_size();	//returns the size of the largest layer
		void connect_layers(int l1, int l2);	//creates connections between all nodes of l1 and all nodes of l2
		void record_network(string output_file_name);
		void load_network(string input_file_name);
};

void connectivity_matrix::set_size(int s) {
	if (s > 0) {
		n = s;
	} else {
		cout << "\nInvalid size!\n" << endl;
	}
}

void connectivity_matrix::add_size(int s) {
	if (s > 0){
		n += s;
	} else {
		cout << "\nInvalid size!\n" << endl;
	}
}
	
void connectivity_matrix::increase_matrix(int s) {
	if(s > 0){
		bool **temp;				//creates a  temporary matrix
		temp =  new bool* [n+s];			//allocates memory
		
		//1st: allocates memory for the new matrix
		for(int i = 0; i < n+s; i++) {
			temp[i] = new bool [n+s];
		}
		
		//2nd: copy previous matrix
		for(int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
		        temp[i][j] = m[i][j];
	        }
	    }
	    
	    //3rd: set the new nodes as in an identity matrix
	    for(int i = 0; i < n+s; i++) {
			for (int j = (i>=n?0:n); j < n+s; j++) {	//the starting column to be filled depends on the line
		        if(i == j){
        			temp[i][j] = 1;
				} else {
					temp[i][j] = 0;
				}
	        }
	    }
	    
		//4th: establish the new matrix
		m =  new bool* [n+s];
		
		//4.1: allocates memory for the matrix and copy the new temp matrix created
		for(int i = 0; i < n+s; i++) {
			m[i] = new bool [n+s];
			for (int j = 0; j < n+s; j++) {
		        m[i][j] = temp[i][j];
	        }
		}
	}
}

void connectivity_matrix::draw_lines(){
	//if there is a connection between 2 nodes, a line will be draw between them
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(m[i][j]){	//m is a bool matrix
				if(i != j){	//prevents the program to draw a line between 2 identical points
					line(coord[i].x, coord[i].y, coord[j].x, coord[j].y);	
				}
			}
		}
	}	
}

void connectivity_matrix::initialize_coords(){
	coord = new ponto2D<int> [n];
	for (int i = 0; i < n; i++) {
        coord[i] = {0, 0};
	}
}

void connectivity_matrix::increase_coords(int s){
	ponto2D<int> *temp;		//creates a temporary vector
	temp = new ponto2D<int> [n+s];	//allocates memory
	
	//1st: copy previous vector
	for(int i = 0; i < n; i++) {	
		temp[i] = coord[i];
    }
    
    //2nd: set the new spatial nodes at origin
    for(int i = n; i < n+s; i++) {	
		temp[i] = {0, 0};
    }
    
    //3rd: establish the new vector
	coord = new ponto2D<int> [n+s];
	
	//4th: copy the contents of temp to coord
	coord = temp;   
}

void connectivity_matrix::initialize_empty(int n){
	set_size(n); 	//define the size of the matrix
	initialize_coords();	//initializes the spatial coordinates at the origin
	m = new bool* [n];	
	for (int i = 0; i < n; i++) {
        m[i] = new bool [n];
        for (int j = 0; j < n; j++) {
            m[i][j] = 0;
        }
	}
}

void connectivity_matrix::initialize_identity(int n){
	set_size(n); 	//define the size of the matrix
	initialize_coords(); //initializes the spatial coordinates at the origin
    layers.push_back(n); 
	m = new bool* [n];	
	for (int i = 0; i < n; i++) {
        m[i] = new bool [n];
        for (int j = 0; j < n; j++) {
        	if(i == j){
        		m[i][j] = 1;
			} else {
				m[i][j] = 0;
			}
        }
    }
}

void connectivity_matrix::create_new_layer(int n){
	if(n > 0){
		layers.push_back(n);
		increase_matrix(n);
		increase_coords(n);
		add_size(n);
	} else {
		cout << "\nInvalid input!" << endl;
	}
}

void connectivity_matrix::print_matrix(){
	cout << "\nCONNECTIVIY MATRIX" << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << m[i][j] << " ";
		}
		cout << endl;
	}	
}

void connectivity_matrix::print_coordinate_vector(){
	cout << "\nCOORDINATES VECTOR" <<endl;
	int aux = 0; //coord vector iterator
	for (int k = 0; k < layers.size(); k++) {
		for (int i = 0; i < layers[k]; i++){
			cout << " (" << coord[aux].x << ", " << coord[i].y << ") ";
			aux++;
		}
		cout << endl;
	}
	cout << endl;
}

//this function draw the network. each node is a circle, each layer is a rectangle around the nodes and there is a line between each connected node
void connectivity_matrix::draw_network() {
	if (layers.size() == 0) { 	//verify if there is some structure created
		cout << "\nThere is no layers\n!" << endl;
	} else {
		if (layers[0] != 0) {	//verify if there is some structure created
					
			int radius = 5;	//radius of nodes
			int x, y;		//node's center at screen
			int left, top;	//upper left corner of the rectangle envolving the layer
			int right, bottom;	//lower right corner of the rectangle envolving the layer
			int diameter = 2*radius; //auxiliar
			int in_border = diameter; //internal border size, measured in node's diameter. distance between node's edge and the rectangle
			int out_border = 5*diameter;	//distance between rectangle and screen
			int node_space = diameter; //vertical space between node's edges, measured in node's diameter
			int layer_space = 3*diameter; //space between layers
			int rect_width = 2*in_border + diameter;	//width of layer's rectangle
						
			int n_max = biggest_layer_size();	//biggest layer. needed to evaluate the size of the screen								
			int height = 2*out_border + 2*in_border + n_max*diameter + node_space*(n_max-1);	//screen height: 2 out_borders + 2 in_borders + diameters + spaces between nodes
			int width = 2*out_border + layers.size()*rect_width +  (layers.size()-1)*layer_space;	//screen width: 2*out_border + n*rect_width + (n-1)*layer_space
			initwindow(width, height);	//initialization of the canvas window
						
			int aux = 0; 	//iterator for coord vector
			for(int i = 0; i < layers.size(); i++){
				//drawing the rectangle
				left = out_border + i*layer_space +i*rect_width;
				top = out_border;
				right = left + rect_width;
				bottom = top + layers[i]*diameter + (layers[i]-1)*node_space + 2*in_border;	//rectangle lower coordinate: top + n*diameters + (n-1)*node_space + 2*in_border
				rectangle (left, top, right, bottom);				
				
				//defining node's circle x coordinate
				x = out_border + i*rect_width + i*layer_space + in_border + radius;	//horizontal position of node: outer border + n*innerborders + n*layerspaces + radius
				for(int j = 0; j < layers[i]; j++) {					
					y = out_border + in_border + j*diameter + j*node_space + radius; //vertical position of the node: out_border + in_border + previus diameters + spaces + 1 radius
					circle(x, y, radius);	//draws the node
					coord[aux] = {x, y};
					aux++;
				}	
			}
			draw_lines();
		    while (!kbhit()) {
		        delay(0);
		    }	
		}
	}
}

void connectivity_matrix::print_layers() {
	if (layers.size() == 0) {
		cout << "\nLayers vector is empty!" << endl;
	} else {
		cout << "\nLayers vector:";
		for(int i = 0; i < layers.size(); i++) {
			cout << "\n" << layers[i];
		}
	}
}

int connectivity_matrix::biggest_layer_size() {
	int max = 0;	//number of nodes of the biggest layer
	if (layers.size() == 0) {	//checking structure
		cout << "\nThere is no data!" << endl;
	} else {
		for(int i = 0; i < layers.size(); i++){ 	//scans the whole vector 
			if (layers[i] > max){
				max = layers[i];		//record the biggest value
			}
		}
	}
	return max;
}

void connectivity_matrix::connect_layers(int l1, int l2){
	//testing for consistency: the layers indexes must be bigger than 0. if they are equal, the program will create lateral connections
	if(l1 >= 0 && l2 >= 0){
		int aux1, aux2; //matrix indexes: aux1 is the 1st node of l1 layer and aux2 is the 1st node of l2 layer
		aux1 = aux2 = 0;
		
		//initializing indexes
		for(int i = (l1-1); i >= 0; i--){
			aux1 += layers[i];
		}
		for(int i = (l2-1); i >= 0; i--){
			aux2 += layers[i];
		}
			
			
		//creating the connections
		for(int i = aux1; i < aux1+layers[l1]; i++){
			for(int j = aux2; j < aux2 + layers[l2]; j++){
				m[i][j] = 1;
				m[j][i] = 1;  //m is simmetric
			}
		}
	}	
}

void connectivity_matrix::record_network(string output_file_name){
	ofstream output_stream;		//creates an output file stream object
	output_stream.open(output_file_name.c_str());	//creates an output file
	
	//1st: print the layers structure
	output_stream << "\nLAYERS_BEGIN" << endl;	
	for(auto iter = layers.begin(); iter != layers.end(); ++iter){
		output_stream << *iter << endl;
	}
	output_stream << "LAYERS_END" << endl;
	
	//2nd: print the matrix
	output_stream << "\nCONNECTIVIY_MATRIX_BEGIN" << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			output_stream << m[i][j] << " ";
		}
		output_stream << endl;
	}
	output_stream << "CONNECTIVIY_MATRIX_END" << endl;
		
	//3rd: close file and annouces it 
	output_stream.close();
	cout << "\nFile created!" << endl;	
}

void connectivity_matrix::load_network(string input_file_name){
	ifstream input_stream;	//creates an input file stream object
	input_stream.open(input_file_name.c_str());	//creates an input file
	
	//1st: check if the file exhists
	if(!input_stream.is_open()){
		cout << "\nERROR 1: file " << input_file_name << " not found!" << endl;
		return; //end function if criteria is not met
	}
	
	//2nd: check if the data is correct	
	//2.1: searches for labels into the file
	//2.1.1: declares variables used
	string line;	//temporary string, records each line read
	vector<string> lines;	//vector that will hold all text data read
	const string lb = "LAYERS_BEGIN";
	const string le = "LAYERS_END";
	const string cmb = "CONNECTIVIY_MATRIX_BEGIN";
	const string cme = "CONNECTIVIY_MATRIX_END";
	unsigned int flag_lb = 0;		//flag to identify if the LAYERS_BEGIN label was found
	unsigned int flag_le = 0;		//flag to identify if the LAYERS_END label was found
	unsigned int flag_cmb = 0;		//flag to identify if the CONNECTIVIY_MATRIX_BEGIN label was found
	unsigned int flag_cme = 0;		//flag to identify if the CONNECTIVIY_MATRIX_END label was found	
	unsigned int lb_pos; 	//register the position of the LAYERS_BEGIN label on the stream
	unsigned int le_pos;  	//register the position of the LAYERS_END label on the stream
	unsigned int cmb_pos;   //register the position of the CONNECTIVIY_MATRIX_BEGIN label on the stream
	unsigned int cme_pos;	//register the position of the CONNECTIVIY_MATRIX_END label on the stream
	
	//2.1.2: loop the file looking for labels
	while(getline(input_stream, line)){		//load the file until the end
		lines.push_back(line);	//store string
		if(!line.empty()){		//check if the line read is not empty
			if(line == lb){
			 	flag_lb = 1;	//register if the label was found
			 	lb_pos = lines.size() - 1;	//register the position of the label on the vector;
			}
			if(line == le){
			 	flag_le = 1;	//register if the label was found
			 	le_pos = lines.size() - 1;	//register the position of the label on the vector;
			}
			if(line == cmb){
				flag_cmb = 1;	//register if the label was found
				cmb_pos = lines.size() - 1;	//register the position of the label on the vector;
			}
			if(line == cme){
			 	flag_cme = 1;	//register if the label was found
			 	cme_pos = lines.size() - 1;	//register the position of the label on the vector;
			}	
		}
	}
	input_stream.close();	//closes the stream
	
	//cout << "\n" << lb_pos << " " << le_pos << " " << cmb_pos << " " << cme_pos << endl; 	//debug
	
	//2.2: if any of the labels weren't found, show an error message
	if((flag_lb+flag_le+flag_cmb+flag_cme) < 4){	//if any of the flags isn't == 1, one or more labels are missing. the function will ends
		cout << "ERROR 2.2: file labels incorrect. Check the ";	//show the error and specify it
		if(flag_lb == 0){
		 	cout << lb << " ";
		}
		if(flag_le == 0){
		 	cout << le << " ";
		}
		if(flag_cmb == 0){
			cout << cmb << " ";
		}
		if(flag_cme == 0){
		 	cout << cme << " ";
		}
		cout << " label(s)." << endl;
		return;
	}
	
	//2.3: check the labels order
	//2.3.1: check the LAYERS labels order
	if(le_pos - lb_pos < 0){
		cout << "\nERROR 2.3.1: LAYERS labels not in order" << endl;
		return;
	}	
	//2.3.2: check the CONNECTIVITY_MATRIX labels order
	if(cme_pos - cmb_pos < 0){
		cout << "\nERROR 2.3.2: CONNECTIVITY_MATRIX labels not in order" << endl;
		return;
	}
	
	//2.4: check if the labels aren't scrambled
	if((cmb_pos < le_pos) && (lb_pos < cme_pos)){
		cout << "ERROR 2.4: labels scrambled" << endl;
		return;
	}
	
	//2.5: check the layers data
	cout << le_pos << " " << lb_pos << endl;
	if(le_pos - lb_pos == 1){
		cout << "ERROR 2.5: there is no layers data" << endl;
	}
	
	for (int i = lb_pos; i < le_pos; i++){
		cout << lines[i] << endl;		
	}
	
	
//	int n_lines = 0;		//number of valid lines between LB and LE labels (lines with int numbers whithout spaces)
//	do {
//		getline(input_stream, line);	//store the current line from "input_stream" into the "line" string
//		cout << "2 " << line << endl;
//		if(line == le){		//check if the layers_end flag was reached
//			break;
//		} else {
//			if(line.empty()){	//2.5.1:check if line is empty
//				cout << "ERROR 2.5.1: empty line at layers data" << endl;
//				return;
//			} else {
//				n_lines++;
//				getline(input_stream, line);
//			}		
//		}
//	} while(line != le);

}

#endif
