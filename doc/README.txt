**********************************************************************
**********************************************************************
******			ND Mapquest (v1.0)		  ************	
******							  ************
******		         Patrick Raycroft		  ************
******		           Ashley Taylor		  ************
******			   "Chas" Jhin			  ************
******			  Matt Fitzgerald		  ************
******							  ************
**********************************************************************
**********************************************************************
Overview:

ND Mapquest provides users with an easy way to efficiently travel across the Notre Dame campus. Features include:
	-Easy directions between any two buildings on campus
	-Double-click feature allows users to travel between any two sidewalks on campus
	-Map zooming and panning to enhance the user experience


ND Mapquest Features and User's Manual:

The program must be compiled and run using the Qt application framework. Upon running the program, the program presents a view of the sidewalks traversing the campus of the University of Notre Dame. On the right can be found the interactive map, and on the left are two drop down menus which list all the significant buildings on Notre Dame's campus.  
To find the shortest path between two buildings, select two different buildings in each of the drop down menus and click the "run" button.  The shortest path between the front doors of each building will then be displayed in red. 
The double-click function can be used when the user wishes to find a path between two specific sidewalk intersections.  The user must first double click on a node around which a blue circle is drawn.  Nodes are located wherever two sidewalks intersect.  Once the user double clicks on a second node, the program will draw a green circle around it and draw the shortest path between the two points in red.  It is important to make sure that when selecting nodes, the user selects two nodes that are connected.  This is important because both buildings and sidewalks are drawn with nodes, but the building nodes are not connected with the sidewalk nodes.  If two unconnected nodes are selected, the program will not be able to calculate the shortest path between them.  Instead, the user will be able to select another node by double clicking, and if it is connected to the other point then the shortest path will be calculated.  
The map has panning and zooming functionality to allow for more control over the map view.  The zooming is controlled by scrolling up and down, and the panning is controlled by clicking and dragging the map in the direction which the user wishes to pan.
The nodes were generated using a website called openstreetmap.org.  The sidewalks and buildings were traced by hand, and then the information was parsed into an xml file (.osm).  This allowed for the dynamic addition of nodes if the map were to be expanded or new buildings were to be built.



Known Issues:

-Once it is successfully calculated, the shortest path is not drawn on the map until it is zoomed.  This seems to be an issue on the Linux machines but not on Mac or Windows machines.
-Since the buildings are drawn using nodes that are unconnected to the sidewalks, a shortest path cannot be calculated when the user tries to connect a building node to a sidewalk node.  This issue could be addressed by going through the data by hand and flagging the nodes which comprise buildings.
-Carroll Hall was not able to be connected because the lake trails were not connected with the sidewalks on the rest of campus.  But who would want to go to Carroll anyway?  If someone did, for some unknown reason, this could be fixed by connecting the lake paths to the sidewalks on openstreetmap.org.
-Sometimes the double click functionality does not select the exact point that the user clicked on, but instead selects a different node within 5 pixels. It is believed that this is because when there are multiple nodes within 5 pixels of the double click, the program picks the first one it recognizes as opposed to the one which is closest to the mouse pointer.  This can be fixed by adding a function which checks that the selected node is closest.



License:

Copyright (c) 2012 ND Mapquest
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.