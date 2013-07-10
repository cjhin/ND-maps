/*  Nodes: Implementation
 *  Implementation of member functions for the Nodes class
 *  
 *  Programmers:  Pat Raycroft, Chas Jhin
 *
 *  Updated:  5/1/12
 *
 *  This file contains the data for the Nodes objects and their contents.
 *  Nodes are integral to the mapND product, as they found the basis for the sidewalks on the map as well as the ability
 *  to calculate the shortest path between any two given points.
 *  Each Node contains a 9 digit ID, a latitude, a longitude, a vector of connections, and finally a corresponding vector of weights or 
 *  distances from this node to another node (in the connections vector).
 */

#include <iostream>
#include <fstream>
#include "nodes.h"
using namespace std;

Nodes::Nodes( long int i, double lt, double lng )
{
	ID = i;
	lat = lt;
	lon = lng;
}

//add a connection @ ID_next by adding the value to the connections vector
void Nodes::addConnection( long int ID_next, double ID_next_weight )
{
	connections.push_back(ID_next);
	connection_weights.push_back(ID_next_weight);
}

//return ID
long int Nodes::getID()
{
	return ID;
}

//return latitude value
double Nodes::getLat()
{
	return lat;
}

//return longitude value
double Nodes::getLon()
{
	return lon;
}

//return a vector of the connections (long ints)
vector<long int> Nodes::getConnections()
{
	return connections;
}

//return the weight (distance) between this and a connection.  return -1 if an invalid connection
double Nodes::getConnectionWeight( long int otherNodeID)
{
	for(int i=0; i<connections.size(); i++)
	{
		if(otherNodeID = connections[i]) //if in fact otherNodeID was a valid ID found in our connections vector
		{
			return connection_weights[i];
		}
	}
	return -1;
}