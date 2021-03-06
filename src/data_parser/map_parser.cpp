/*  Driver:  map_parser
 *  The driver for the map_parser program
 *  
 *  Programmers:  Chas Jhin, Matt Fitzgerald
 *
 *  Updated:  4/21/12
 *
 *  This file contains the auxiliary map_parser program
 *  The purpose of this program is to read in the .osm file (xml) from openstreetmaps.org, pull the relevant information
 *  and parse it into nodes objects that can then be written to the nodes.txt file.
 *  This program allows us to easily update the nodes.txt file given any necessary updates to the online data @ openstreetmaps.org
 *  which gives us flexibility in addressing bugs etc.
 *  This nodes.txt file will then be used in the main mapND program in order to populate the map data.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "Nodes.h"
#include <vector>
#include <math.h>

using namespace std;

void readFile();
void writeFile();
void makeConnection(string ID, string ID_next);

vector<Nodes> nodeVector;

//call the other functions in map_parser.cpp to properly parse the xml file
int main( void )
{
	readFile();
	writeFile();
}

//read in the .osm file, grab the relevant data, create nodes objects, and then add the nodes objects to the nodeVector
void readFile()
{
	ifstream infile ("map.osm");
	string line;

	if (infile.is_open())
	{
		while(infile.good())
		{
			string temp;
			
			string ID;
			string ID_next;
			string lat;
			string lon;
			
			string garbage;
			
			getline(infile, garbage, '<');
			getline(infile, temp, ' ');
						
			//if the tag signifies a node, then create a node with the important data and add ito the nodeVector
			if(temp.compare("node")==0)
			{
				getline(infile, garbage, '"');
				getline(infile, ID, '"');
				getline(infile, garbage, '"');
				getline(infile, lat, '"');
				getline(infile, garbage, '"');
				getline(infile, lon, '"');
				getline(infile, garbage);
				
				Nodes tempNode(atol(ID.c_str()), atof(lat.c_str()), atof(lon.c_str()));
				nodeVector.push_back(tempNode);
			}
						
			//if the tag signifies a way, or set of connections to make between vectors, then make connections between nodes instead of adding new nodes to the nodeVector
			//for a group of nodes in a way, each node has a bidirectional connection to the next node in the list
			else if(temp.compare("way")==0)
			{
				//throw away the rest of the line
				getline(infile, garbage);
				getline(infile, garbage, '<');
				getline(infile, temp, ' ');
								
				//while still nodes left in the way
				while(temp.compare("nd")==0)
				{
					getline(infile, garbage, '"');
					getline(infile, ID_next, '"');
					getline(infile, garbage);
					getline(infile, garbage, '<');
					getline(infile, temp, ' ');
					
					//if not the first one
					if(ID.empty()==false)
					{
						//make a connection from ID -> ID-next
						makeConnection(ID, ID_next);
					}
					
					//increment
					ID=ID_next;
				}			
			}
		}				
	}	
		
	infile.close(); //close instream
}

//write the data from nodeVector to nodes.txt
void writeFile()
{
	ofstream outfile ("nodes.txt");
	
	//iterate through the nodeVector
	for(int i = 0; i < nodeVector.size(); i++)
	{
		vector<long int> tempConn = nodeVector.at(i).getConnections();
		
		char tempbuff[40+tempConn.size()*12];
		sprintf(tempbuff, "%li, %lf, %lf, %i", nodeVector.at(i).getID(), nodeVector.at(i).getLat(), nodeVector.at(i).getLon(), (int) tempConn.size());
		
		//add the connections
		for(int j = 0; j<tempConn.size(); j++)
		{
			char tempbuff2[12];
			sprintf(tempbuff2, ", %li", tempConn.at(j));
			
			strcat(tempbuff, tempbuff2);
			
			//find the vector object of the connection, so as to get the lat and lon
			for(int k = 0; k < nodeVector.size(); k++)
			{
				if(nodeVector.at(k).getID() == tempConn.at(j))
				{
					//also calculate and add the weight of the connection
					double latDiff = nodeVector.at(i).getLat() - nodeVector.at(k).getLat();
					double lonDiff = nodeVector.at(i).getLon() - nodeVector.at(k).getLon();
					double dist = sqrt(pow(latDiff, 2) + pow(lonDiff, 2)); //calcuate the distance
					
					sprintf(tempbuff2, ", %lf", dist);
					
					strcat(tempbuff, tempbuff2);
				}
			}
		}
		
		strcat(tempbuff, ", ");
		
		outfile << tempbuff <<endl;	
	}

	outfile.close();
}

//given two ID's, connect the first to the second by filling the appropriate connections vector
void makeConnection(string ID, string ID_next)
{
	//iterate through the nodes vector
	for(int i = 0; i < nodeVector.size(); i++)
	{		
		//find the node with corresponding ID, then add to the connections vector
		if (nodeVector.at(i).getID() == atol(ID.c_str()))
		{
			nodeVector.at(i).addConnection(atof(ID_next.c_str()), 0);
		}
		//also find and add the opposite direction so as to make the connection bidirectional
		else if(nodeVector.at(i).getID() == atol(ID_next.c_str()))
		{
			nodeVector.at(i).addConnection(atof(ID.c_str()), 0);
		}
	}
}
