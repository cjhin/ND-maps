/* Window:  Interface
 * Interface for data members and member functions for the Window class
 *
 * Programmers:  Pat Raycroft, Ashley Taylor
 *
 * Updated:  5/2/12
 *
 * This file contains the interface for the Window class and includes all data members and the prototypes of the member functions
 *
*/

#ifndef WINDOW_H
#define WINDOW_H
#include "mapND.h"
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QDockWidget>
#include <QMainWindow>
#include <QLineEdit>
#include <QAbstractButton>
#include <QPushButton>
#include <map>
using namespace std;

class QAction;
class QListWidget;
class QMenu;
class QTextEdit;

class Window : public QMainWindow {
	Q_OBJECT

public :
	Window();
public slots:
	void mapRoute();
protected:
    QWidget *mapWidget;
	
private:
	void createDockWindows();
	void menus();
	void toolBars();
	void actions();
    void readLocationsFile(); //reads in the Locations.txt file and parses the data to a nodesVector object

    map<string, long int> locationsMap;

    MapND mapWid; // Creates a map object mapWid
	QDockWidget *dock;
	QDockWidget *menu;
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *viewMenu;
	QMenu *helpMenu;
	QToolBar *fileToolBar;
	QToolBar *editToolBar;
	QAction *newForm;
	QAction *printMap;
	QAction *save;
	QAction *undo;
	QAction *about;
	QAction *quit;
	QAction *edit;
    QLineEdit *start; //*start and *end are possibly deprecated from the input fields?
	QLineEdit *end;
	QComboBox *startbox;
	QComboBox *endbox;
	QPushButton *actionBtn;
};

#endif
