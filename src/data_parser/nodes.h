/*  Nodes: Interface
 *  Interface for data and member functions for the Nodes class
 *  
 *  Programmers:  Pat Raycroft, Chas Jhin
 *
 *  Updated:  5/1/12
 *
 * This file containts the interface for the Nodes class and includes all data members and the prototypes of the member functions
 */

#ifndef NODES_H
#define NODES_H

#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class Nodes {
public:
	Nodes( long int = 0, double = 0, double = 0);
	void addConnection( long int ID_next, double ID_next_weight); //add a connection @ ID_next by adding the value to the connections vector
	long int getID(); //return ID
	double getLat(); //return latitude value
	double getLon(); //return longitude value
	vector<long int> getConnections(); //return a vector of the connections (long ints)
	double getConnectionWeight( long int ); //return the weight (distance) between this and another connection.  return -1 if an invalid connection
	
private:
	long int ID;
	double lat;
	double lon;
	vector<long int> connections;
	vector<double> connection_weights;
};

#endif
