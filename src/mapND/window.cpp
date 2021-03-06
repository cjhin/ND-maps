/* Window:  Implementation
 * Implementation of member functions for the Window class
 *
 * Programmers:  Pat Raycroft, Ashley Taylor
 *
 * Updated:  5/2/12
 *
 * This file contains the implementations of the Window constructor along with the member functions menus(), toolBars(), createDockWindows(), and actions()
 *
*/

#include "window.h"
#include "mapND.h"
#include <QMainWindow>
#include <QLabel>
#include <QComboBox>
#include <QtGui>
#include <QLineEdit>
#include <QWidget>
#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
using namespace std;

Window::Window() // Constructor for a Window object
{
    readLocationsFile();
	setWindowTitle(tr("MapND"));
	resize(1200, 800);
	actions();
	menus();
	toolBars();
	createDockWindows();
}

void Window::createDockWindows(){ // Creates the dock windows (the Menu bar and the Map) using QDockWidget objects
    // Create "Start" Form
    QDockWidget *menu =new QDockWidget(tr(""), this);
	menu->setAllowedAreas(Qt::LeftDockWidgetArea);
	menu->setFeatures(QDockWidget::NoDockWidgetFeatures);
	menu->setFixedSize(250, 75);

    menu = new QDockWidget( this);
    menu->setAllowedAreas(Qt::LeftDockWidgetArea);
    menu->setFeatures(QDockWidget::NoDockWidgetFeatures);
    menu->setFixedSize(250, 50);
    QString startboxTxt("Choose your starting location:");
    QLabel *startboxLabel = new QLabel (startboxTxt,menu);
    menu -> setWidget(startboxLabel);
    addDockWidget(Qt::LeftDockWidgetArea, menu);

	menu = new QDockWidget( this);
	menu->setAllowedAreas(Qt::LeftDockWidgetArea);
	menu->setFeatures(QDockWidget::NoDockWidgetFeatures);
	menu->setFixedSize(250, 50);
    startbox = new QComboBox(menu);
	menu -> setWidget(startbox);
	addDockWidget(Qt::LeftDockWidgetArea, menu);

    //Create "End" Form
    menu = new QDockWidget( this);
    menu->setAllowedAreas(Qt::LeftDockWidgetArea);
    menu->setFeatures(QDockWidget::NoDockWidgetFeatures);
    menu->setFixedSize(250, 50);
    QString endboxTxt ("Choose your ending location:");
    QLabel *endboxLab = new QLabel (endboxTxt,menu);
    menu -> setWidget(endboxLab);
    addDockWidget(Qt::LeftDockWidgetArea, menu);

    menu = new QDockWidget( this);
    menu->setAllowedAreas(Qt::LeftDockWidgetArea);
    menu->setFeatures(QDockWidget::NoDockWidgetFeatures);
    menu->setFixedSize(250, 50);
    endbox = new QComboBox(menu);
	menu -> setWidget(endbox);
	addDockWidget(Qt::LeftDockWidgetArea, menu);

    //populate both dropdown menus with the keys of locationsMap
    map<string, long int>::iterator mapIt;
    for(mapIt = locationsMap.begin(); mapIt != locationsMap.end(); mapIt++) {
        startbox ->addItem((*mapIt).first.c_str());
        endbox ->addItem((*mapIt).first.c_str());
    }

    //Create Start Button
    menu = new QDockWidget(this);
    menu->setFeatures(QDockWidget::NoDockWidgetFeatures);
    menu->setFixedSize(250, 50);
    QString btnTxt("Map Route");
    actionBtn = new QPushButton(btnTxt, menu);
    menu->setWidget(actionBtn);
    addDockWidget(Qt::LeftDockWidgetArea, menu);

    connect(actionBtn, SIGNAL(clicked()), this, SLOT(mapRoute()));

    //Create Map using the Map object mapWid
    QDockWidget *dock = new QDockWidget(tr("Map of Campus of the University of Notre Dame"), this);
    mapWidget = new QWidget(dock);
    mapWid.setParent(mapWidget);
    mapWidget->setFixedSize(700, 700);
    dock->setAllowedAreas(Qt::RightDockWidgetArea);
    dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    dock->setWidget(mapWidget);
	addDockWidget(Qt::RightDockWidgetArea, dock);
}

void Window::mapRoute()
{
    QString current=startbox->currentText();
    QString destination=endbox->currentText();

    long int a = locationsMap[current.toStdString()];
    cout << "Source: " << a << endl;
    long int b = locationsMap[destination.toStdString()];
    cout << "Dest: " << b << endl;

    mapWid.initShortestPath(a, b);
}

void Window::actions() // Create actions to input into Menus and ToolBars (unconnected)
{
    newForm = new QAction(tr("&New Form"), this);
    newForm->setStatusTip(tr("Start a new map route"));

    save = new QAction(tr("&Save"), this);
    save->setStatusTip(tr("Save this route"));

    printMap = new QAction(tr("&Print"), this);
    printMap->setStatusTip(tr("Print this route"));

    edit = new QAction(tr("&Edit"), this);
    edit->setStatusTip(tr("Edit this route"));

    undo = new QAction(tr("&Undo"), this);
    undo->setStatusTip(tr("Undo this route"));

    quit = new QAction(tr("&Quit"), this);
    quit->setStatusTip(tr("Quit the application"));

    about = new QAction(tr("&About"), this);
    about->setStatusTip(tr("About this application"));
}

void Window::menus() // Create Top Menu
{
    // Create File Menu
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(quit);

    // Create Help Menu
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(about);
}

void Window::toolBars() // Create ToolBar
{
    // Create File Toolbar
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newForm);

}

//read in the locations file
void Window::readLocationsFile()
{
    ifstream infile ("locations.txt");

    if (infile.is_open())
    {
        while(infile.good())
        {
            string garbage;
            string name;
            string ID;

            getline(infile, name, ',');
            getline(infile, garbage, ' ');
            getline(infile, ID, ',');
            getline(infile, garbage);

            //add this location to the locationsMap for use in the program
            if(!name.empty())
            {
                locationsMap.insert( pair<string, long int>(name, atol(ID.c_str())));
                //cout << "reading in: "<<name <<" "<< ID<<endl;
            }
        }
    }
    else {
        cout<<"ERROR: File not found or unable to open."<<endl;
    }
    infile.close(); //close instream
}
