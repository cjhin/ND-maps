/* MapND:  Implementation
 * Implementation of member functions for the MapND class
 *
 * Programmers:  Pat Raycroft, Ashley Taylor, Chas Jhin
 *
 * Updated:  5/2/12
 *
 * This file contains the implementations of the MapND constructor along with the member functions drawBackground(), setCenter(), mousePressEvent(), mouseReleaseEvent(), mouseMoveEvent(), wheelEvent(), and resizeEvent()
 *
 * This file is responsible for reading in the data from nodes.txt, drawing the paths/buildings as well as the currently calculated shortest path,
 * handling the interactive portions of the map (panning, zooming, double clicking to create new "shortest paths),
 * and finally handling calls for new shortest paths.
 */ 

#include "mapND.h"
#include "nodes.h"
#include <QtGui>
#include <iostream>
#include <fstream>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QWheelEvent>
#include <QApplication>
#include <QColor>
#include <math.h>
#include <cmath>
#include <vector>
#include <map>

//#define DEBUG
//#define DEBUG2

using namespace std;

MapND::MapND(QGraphicsView *parent)
    : QGraphicsView(parent) {

    readNodesFile(); //read in the data from nodes.txt

    //initShortestPath(181600147, 1650036165);

    setWindowTitle(tr("MapND"));
    resize(700, 700);
    QGraphicsScene *Scene = new QGraphicsScene(this); // Initializes the QGraphicsScene object
    setScene(Scene);
    setSceneRect(0, 0, 700, 700);
    srcNode = 0;
    dstNode = 0;

    setCenter(QPointF(350, 350)); // Sets the center of the view to the center of the scene
    setCursor(Qt::OpenHandCursor); // Sets the cursor to an "open hand"
}

//allow other files to create new shortest paths given a start and end ID
void MapND::initShortestPath(long int a, long int b)
{
    path = shortestPath(a, b);
    update();
}

//Draws the map as well as the currently calculated "shortest path"
void MapND::drawBackground(QPainter *painter, const QRectF &rect)
{
    //draw the background
    painter->setBrush(QBrush("#F7F6B0"));
    painter->drawRect(0, 0, 700, 700);

    painter->setPen(Qt::black);
    QPen pen;
    pen.setWidth(2);

    //paint the map by painting all the connections between Nodes as lines.

    //iterate through the nodesMap
    map<long int, Nodes>::iterator it;
    for(it = nodesMap.begin(); it != nodesMap.end(); it++)
    {
        Nodes currNode = it->second;

        vector<long int> connVec = currNode.getConnections();
        double origLat = convertLat(currNode.getLat());
        double origLon = convertLon(currNode.getLon());

        //for every connection of a given node
        for(int j = 0; j<connVec.size(); j++)
        {
            Nodes connNode = nodesMap[connVec.at(j)];

            double connLat = convertLat(connNode.getLat());
            double connLon = convertLon(connNode.getLon());

            //ensure they are valid points...
            if(connLat!=0 && connLon!=0)
            {
                painter->drawLine(origLon, origLat, connLon, connLat);
            }
        }
    }

    //draw the current "shortest path"
    painter->setPen(Qt::red);
    pen.setWidth(5);

    if(!path.empty())
    {
        double origLon = convertLon(nodesMap[path.at(0)].getLon());
        double origLat = convertLat(nodesMap[path.at(0)].getLat());
        double destLon;
        double destLat;

				//connect the current Node to the next node (starting at the first point and ending at the last point)
        for(int i=1; i<path.size(); i++)
        {
            destLon = convertLon(nodesMap[path.at(i)].getLon());
            destLat = convertLat(nodesMap[path.at(i)].getLat());

            painter->drawLine(origLon, origLat, destLon, destLat);

            origLon = destLon;
            origLat = destLat;
        }
    }

    //draw big green and blue circles where the src and dest nodes are
    painter->setPen(Qt::blue);
    painter->drawEllipse(convertLon(nodesMap[srcNode].getLon())-2, convertLat(nodesMap[srcNode].getLat())-2, 4, 4);

    painter->setPen(Qt::green);
    painter->drawEllipse(convertLon(nodesMap[dstNode].getLon())-2, convertLat(nodesMap[dstNode].getLat())-2, 4, 4);

    update();
}


QPointF MapND::getCenter() // Returns the current center point of the scene
{
    return centerPt;
}

void MapND::setCenter(const QPointF &center) // Sets the center of the scene
{
    QRectF visible = mapToScene(rect()).boundingRect();
    QRectF bound = sceneRect();
    // Sets the boundaries of the scene
    double boundX = visible.width() / 2.0;
    double boundY = visible.height() / 2.0;
    double width = bound.width() - 2.0*boundX;
    double height = bound.height() - 2.0*boundY;

    // Creates a QRectF object using these boundaries
    QRectF bounds(boundX, boundY, width, height);

    // Setting the current center point of the scene, centerPt
    if(bounds.contains(center)){
        centerPt = center;
    }
    else{
        if(visible.contains(bound)){
            centerPt = bound.center();
        }
        else{
            centerPt = center;
            if(center.x() > bounds.x() + bounds.width()){
                centerPt.setX(bounds.x() + bounds.width());
            }
            else if (center.x() < bounds.x()){
                centerPt.setX(bounds.x());
            }

            if(center.y() > bounds.y() + bounds.height()){
                centerPt.setY(bounds.y() + bounds.height());
            }
            else if (center.y() < bounds.y()){
                centerPt.setY(bounds.y());
            }
        }
    }
    // Uses the QGraphicsScene member function centerOn() to set the center of the scene to the value centerPt
    centerOn(centerPt);

}

void MapND::mousePressEvent(QMouseEvent *e) // Sets the last pan point value panPt to the position of the mousePressEvent; also sets the cursor to a "closed hand"
{
    panPt = e->pos();
    setCursor(Qt::ClosedHandCursor);
}

//allows the user to choose any arbitary intersections (nodes) on campus and try and find the shortest path between two points (if valid)
void MapND::mouseDoubleClickEvent(QMouseEvent *e)
{
    QRectF visible = mapToScene(rect()).boundingRect();
    QRectF bound = sceneRect();
    // Sets the boundaries of the scene
    double boundX = visible.width() / 2.0;
    double boundY = visible.height() / 2.0;
    double width = bound.width() - 2.0*boundX;
    double height = bound.height() - 2.0*boundY;

    long int holdNode;
    double mod = 7.0;
    vector<long int>::iterator pathIt;
    map<long int, Nodes>::iterator mapIt;
    locPt = e->pos();

    float xConv, yConv, xDiff = 999.0, yDiff = 999.0, x, y, yDiffHold, xDiffHold;
    yConv = (centerPt.y() - boundY) + locPt.y() * ((700.0-height)/700.0);
    xConv = (centerPt.x() - boundX) + locPt.x() * ((700.0-width)/700.0);

    for(mapIt = nodesMap.begin(); mapIt != nodesMap.end(); mapIt++){
        y = convertLat(mapIt->second.getLat());
        x = convertLon(mapIt->second.getLon());

        if (y  <= yConv + mod && y >= yConv - mod){
            if (x <= xConv + mod && x >= xConv - mod){
#ifdef DEBUG2
                cout << "y value converted: " << yConv << endl;
                cout << "x value converted: " << xConv << endl;
#endif
                yDiffHold = y - yConv;
                xDiffHold = x - xConv;
                //srcNode = mapIt->first;
                if (abs(yDiffHold) <= yDiff && abs(xDiffHold) <= xDiff){
                    //cout << (abs(yDiffHold) <= yDiff) << endl;
                    //cout << (abs(xDiffHold) <= xDiff) << endl;
                    holdNode = mapIt->first;
                    //cout << "holdNode: " << holdNode << endl;
                    yDiff = abs(yDiffHold);
                    xDiff = abs(xDiffHold);
                    //cout << yDiff << endl;
                    //cout << xDiff << endl;
                }
            }
        }
    }
    if (srcNode == 0 && holdNode != 0) srcNode = holdNode;
    else if (srcNode != 0 && dstNode == 0 && holdNode != 0) dstNode = holdNode;
    else if (srcNode != 0 && holdNode == srcNode) ;
    else if (srcNode != 0 && dstNode != 0 && holdNode != 0){
        srcNode = 0;
        dstNode = 0;
    }

    cout << "Source Node ID: " << srcNode << endl;
    cout << "Destination Node ID: " << dstNode << endl;
    cout << "Distance: " << nodesMap[srcNode].getConnectionWeight(dstNode)<<endl;

    if (srcNode != 0 && dstNode != 0){
#ifdef DEBUG
        cout << "Entering shortestPath() " << endl;
#endif
        initShortestPath(srcNode, dstNode);
#ifdef DEBUG
        for (pathIt = path.begin(); pathIt < path.end(); pathIt++){
            cout << *pathIt << endl;
        }
#endif
    }
    locPt = QPoint();
}

void MapND::mouseReleaseEvent(QMouseEvent *e) // set the last pan point value equal to 0; also sets the cursor to an "open hand"
{
    setCursor(Qt::OpenHandCursor);
    panPt = QPoint();
}

void MapND::mouseMoveEvent(QMouseEvent *e) // If the mouse moves while being pressed (i.e. panPt is not empty), changes panPt to the current mouse position and resets the center of the scene
{
    if (!panPt.isNull()){
        QPointF delta = mapToScene(panPt) - mapToScene(e->pos());
        panPt = e->pos();
        setCenter(getCenter() + delta);
    }
}

void MapND::wheelEvent(QWheelEvent *e) // Zooms in with a "forward" wheel turn and out with a "backward" wheel turn
{
    QPointF pt_init(mapToScene(e->pos())); // Sets the initial point to the location of the cursor
    QPointF center = getCenter(); // Gets the center of the scene

    double factor = 1.10; // Rate of zooming (must be greater than 1)
    if (e->delta() > 0){ // Zooming in
        scale(factor, factor);
    }
    else { // Zooming out
        scale(1.0/factor, 1.0/factor);
    }

    QPointF pt_final(mapToScene(e->pos())); // Sets the final point to the location of the cursor on the zoomed map
    QPointF delta = pt_init - pt_final; // Calculates the difference between the initial and final points on the map
    // Sets a new center of the scene based on the difference between the initial and final points
    QPointF newCenter = center + delta;
    setCenter(newCenter);
}

void MapND::resizeEvent(QResizeEvent *e) // Resizes the scene when zooming and panning
{
    QRectF visible = mapToScene(rect()).boundingRect();
    setCenter(visible.center());

    QGraphicsView::resizeEvent(e);
}

//convert the latitude into a graphable y coordinate
int MapND::convertLat(double lat)
{
    lat-=41.6834370; //subtracts minimum latitude value.  this gives us a range from 0-1
    lat*=30000;  //results in a number from 0-700
    lat = 800-lat;      //flips the coordinates, so that 0 is on the upper x axis instead of the bottom one
    return lat;
}

//convert the longitude into a graphable y coordinate
int MapND::convertLon(double lon)
{
    lon+=86.2503982; //adds (because it's negative) the minimum longitude value.  this gives us a range from 0-1
    lon*=30000;  //results in a number from 0-700
    return lon;
}


//read in the nodes.txt file to populate the nodesMap
void MapND::readNodesFile()
{
    ifstream infile ("nodes.txt");

    if (infile.is_open())
    {
        while(infile.good())
        {
            string garbage;

            string ID;
            string lat;
            string lon;
            string con; //number of connections
            string ID_next;
            string ID_next_weight;

            getline(infile, ID, ',');
            getline(infile, garbage, ' ');
            getline(infile, lat, ',');
            getline(infile, garbage, ' ');
            getline(infile, lon, ',');
            getline(infile, garbage, ' ');
            getline(infile, con, ',');
            getline(infile, garbage, ' ');

            //create a temp Node using the ID, lat, lon
            Nodes tempNode(atol(ID.c_str()), atof(lat.c_str()), atof(lon.c_str()));

            //cout << "\ncurrID: " <<ID<< " conn: ";

            //adds on the IDs of the connections
            for (int i=0; i<atoi(con.c_str()); i++)
            {
                getline(infile, ID_next, ',');
                getline(infile, garbage, ' ');
                getline(infile, ID_next_weight, ',');
                getline(infile, garbage, ' ');
                tempNode.addConnection(atol(ID_next.c_str()), atof(ID_next_weight.c_str()));
            }

            //add this node to the nodesMap for use in the program
            nodesMap.insert( pair<long int, Nodes>(atol(ID.c_str()), tempNode));

            getline(infile, garbage);
        }
    }
    else {
        cout<<"ERROR: File not found or unable to open."<<endl;
    }
    infile.close(); //close instream
}

//given the nodesMap, calculates the shortest path between the src and dst, and returns a vector<long int> containing the ID's, in order from src to dst, of the shortest path
vector<long int> MapND::shortestPath( long int src, long int dst )
{
    map<long int, long int> predMap; //key is the node, value is the "best predecessor" (determined so far)
    map<long int, double> distMap; //key is the node, value is the shortest distance to it from the src (determined so far)

    map<long int, Nodes>::iterator mapIt;
    // Initialize distances to 9999(infinity) and predecessors to zero, using the IDs in the nodesMap to salt the keys
    for(mapIt = nodesMap.begin(); mapIt != nodesMap.end(); mapIt++) {
        predMap.insert( pair<long int, long int>((*mapIt).first, 0));
        distMap.insert( pair<long int, double>((*mapIt).first, 9999.0));
    }

    distMap[src] = 0.0; //however, the source node's distance should be set to 0
    long int currNode = src; //also, the "currID", or first node to look at, should be the src

    vector<long int> solved;
    vector<long int>::iterator conn_it;
    vector<long int> finalPath;

    int counter = 0;

    bool done = false;
    while (!done && counter<nodesMap.size()/2) // Iterates while destination has not been found, or path is not possible
        //while(solved.size()<nodesMap.size())
    {
        counter++;
#ifdef DEBUG
        cout<< "size of solved array " << solved.size()<< " i = " <<counter<<endl;
        cout << "curr Node "<<currNode<<endl;
#endif

        vector<long int> connect = nodesMap[currNode].getConnections(); // Get all connections for current node

        // if connect is empty, then you are at an isolated point.  This should theoretically only happen if you've tried to set the src to an isolated node.  at which point you should just return an error
        if (connect.empty()) {
            cout << "ERROR: your source point (or possibly the destination?) is an invalid/isolated point";
            return finalPath;
        }

        for (conn_it = connect.begin(); conn_it != connect.end(); conn_it++) //iterates through connections for the currNode
        {
#ifdef DEBUG
            cout << " connection "<<*conn_it;
#endif DEBUG
            // If the given connection is already solved, ignore it
            if(distMap[*conn_it] == -1)
            {
#ifdef DEBUG
                cout << " node already solved"<< endl;
#endif DEBUG
                continue;
            }

            // Get distance between currNode and conn_it Node, and add total distance to our currNode
            // for the new "tenative total distance" to the unsolved conn_it Node
            double distComp = nodesMap[currNode].getConnectionWeight(*conn_it)+distMap[currNode]; //distMap[currNode] should always return a number less than 9999, as it should theoretically be solved.... but its possible this might mess up?

#ifdef DEBUG
            cout << " distComp =" <<distComp<<endl;
#endif

            if (distComp < distMap[*conn_it]){ // If the distance is less the current "shortest dist" to the conn_it node, update the distMap and predMap
                distMap[*conn_it] = distComp;
                predMap[*conn_it] = currNode;
            }
        }

        //add the currID to the solved
        solved.push_back(currNode);

        //Once a node has been solved, set the distMap to -1, this tells the program to ignore it from now on
        distMap[currNode] = -1;

        if(currNode == dst) // if the dst was just added, then you've reached the end!
            break;

        //otherwise find the unsolved node with the shortest distance to it (distMap value), this will become the new currID
        long int nextNode;
        double nextNodeDist = 9999;
        map<long int, double>::iterator distIt;
        for(distIt = distMap.begin(); distIt != distMap.end(); distIt++) {
            //if not a solved node, then see if its the shortest, and set nextNode appropriately
            if(distIt->second != -1 && distIt->second < nextNodeDist)
            {
                nextNode = distIt->first;
                nextNodeDist = distIt->second;
            }
        }
        currNode = nextNode;
    }

    if(counter>=nodesMap.size()) //an error occured and the path is unsolvable
    {
        cout << "ERROR: Unsolvable.  Its probable that the nodes are not connected."<<endl;
        return finalPath;
    }

    //Use the predMap to create the final path vector
    long int lastID = dst;
    while (lastID != src){ // Enters in the path from destination to source using the predecessors vector
        finalPath.push_back(lastID);
        lastID = predMap[lastID]; //increment one step backwards along graph (equivalent to the predecessor)
        if(lastID==0) //if somehow something messed up
        {
            cout << "ERROR in creating finalPath"<<endl;
            return finalPath;
        }
    }
    finalPath.push_back(src);
#ifdef DEBUG
    cout << "finished"<<endl;
#endif

    return finalPath;
}
