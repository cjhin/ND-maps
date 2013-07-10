/* MapND:  Interface
 * Interface for data and member functions for the MapND class
 *
 * Programmers:  Pat Raycroft, Ashley Taylor, Chas Jhin
 *
 * Updated:  5/2/12
 *
 * This file containts the interface for the Map class and includes all data members and the prototypes of the member functions
 *
*/

#ifndef MAPND_H
#define MAPND_H
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include "nodes.h"
#include <vector>
#include <map>

using namespace std;

class MapND : public QGraphicsView {
	Q_OBJECT

public:
	MapND(QGraphicsView *parent = 0);
	void initShortestPath(long int a, long int b); //set the new shortest path

protected:
	QPointF getCenter();
	virtual void drawBackground(QPainter *painter, const QRectF &rect);
	virtual void setCenter(const QPointF &center);
	virtual void mousePressEvent(QMouseEvent *e);
	virtual void mouseDoubleClickEvent(QMouseEvent *e);
	virtual void mouseReleaseEvent(QMouseEvent *e);
	virtual void mouseMoveEvent(QMouseEvent *e);
	virtual void wheelEvent(QWheelEvent *e);
	virtual void resizeEvent(QResizeEvent *e);

	void readNodesFile(); //reads in the Nodes.txt file and parses the data to a nodesVector object
	int convertLat(double lat);  //convert the lat into a graphable y coordinate
	int convertLon(double lon);   // convert the lon into a graphable x coordinate

	vector<long int> shortestPath( long int, long int ); //calculate the sortest path given two nodes.
	vector<long int> path; //contains the path to be drawn, specifically the IDs
	map<long int, Nodes> nodesMap;

	long int srcNode;
	long int dstNode;
	
	QPointF centerPt;
	QPoint panPt;
	QPoint locPt;	
};

#endif

