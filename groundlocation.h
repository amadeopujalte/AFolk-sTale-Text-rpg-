#ifndef GROUND_LOCATION_H
#define GROUND_LOCATION_H
#include <string>
#include <iostream>
#include "item.h"
using namespace std;

class Location {
private:
    static constexpr int locnum = 9;              // Number of locations
    static const string locationsdesc[locnum];    // Descriptions of locations
    static const int locations[locnum];           // Location data (indices, or other relevant data)

public:
    // Method to get the description of a location
    static string getLocationsDesc(int index);

    // Method to get the location's ID
    static int getLocations(int index);
};
class Ground {
	public:
	static Item* matrix[4][9];
	static int front[9];
	static void initializematrix();
	static void insertmatrix(int col, Item* object);
	static void removematrix(int col);

};

#endif // GROUNDLOCATION_H

