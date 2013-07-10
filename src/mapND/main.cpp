/*  Driver:  mapND
 *  The driver for the mapND program
 *  
 *  Programmers:  Pat Raycroft, Ashley Taylor
 *
 *  Updated:  5/2/12
 *
 *  This file contains the driver for the mapND program
 *  Creates a Window object and uses QApplication and the QMainWindow member function show() to open the GUI
 *
 *	The purpose of the mapND program is to create an interactive map of the University of Notre Dame campus that a user can interact with
 *  to find the shortest routes between any two locations on campus.  The program accomplishes this goal through a variety of means,
 *  utilizing the qt graphics framework to create the windows, as well as the Nodes objects created by our team in order to efficiently utilize the
 *  paths on campus.  
 *
*/

#include <QApplication>
#include "window.h"
using namespace std;

int main(int argc, char *argv[]){
    cout << endl<<argv[0]<<endl; //returns the package path of the exe, useful for placing nodes.txt in the right place
	QApplication app(argc, argv);
	Window win;
	win.show();
	return app.exec();
}
