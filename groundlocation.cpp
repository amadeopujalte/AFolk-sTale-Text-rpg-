#include <iostream>
#include <string>
#include "groundlocation.h"
#include "item.h"
const string Location::locationsdesc[Location::locnum] = {
    "Standing in the heart of Dunmar's bustling square, south of the city's entrance, you see the place alive with movement, you notice the chatter of merchants, calling out their wares and travelers passing through.\n"
    "You also notice your favourite small store tucked between stone buildings, its sign swaying gently in the breeze.",
    "You stand in a lush green field, the tall grass swaying gently in the breeze.\n"
    "To the north, the forest looms dense and impenetrable, its shadows forbidding any passage. Turning south, you glimpse the towering walls of Dunmar rising proudly in the distance.\n"
    "Narrow paths cut through the field to the east and west, worn by travelers over time.",
    "You find yourself on a rugged, rocky beach, the air filled with the salty tang of the sea.\n"
    "To the west, the dense forest stretches to the horizon, while to the east, waves crash against the jagged shoreline. The beach continues north, where a river cuts through the rocks, its waters rushing toward the sea.\n"
    "To the south, the beach softens into a green field, where a small building stands in quiet solitude.",
    "You stand in a green field that gently slopes toward a sandy beach to the east, where the sea shimmers under the sunlight.\n"
    "To the north lies a rocky beach, its jagged edges contrasting with the soft grass underfoot.\n"
    "Southward, the field stretches further, meeting the sea.\n"
    "A well worn path leads west, but your eyes are drawn to a house not far, its large sign weathered yet legible.'The Traveler's Fortune'.",
    "You stand at a crossroads deep within the forest, the towering trees casting dappled shadows on the dirt path.\n"
    "The trail splits here: one branch veers north, its sign jagged and crossed out, shrouding its destination in mystery. To the east, the sign reads 'Dunmar', promising the bustling city nearby. The path to the west is marked 'Auralis', though you know it whispers of a long and arduous journey ahead. The forest’s quiet hum surrounds you, urging you to choose your direction.",
    "The forest around you has grown darker, the vibrant greens replaced by withered branches and lifeless undergrowth.\n"
    "The path has faded into nothingness, swallowed by the encroaching wilderness, a clear sign that no one treads this way anymore. In the distance, the faint outline of a village emerges, a cluster of decaying rooftops and leaning structures, shrouded in an eerie silence. The air feels heavier here...",
    "You stand on rugged, rocky terrain, the air filled with the sound of rushing water. A wide, unforgiving river stretches across your path to the north, barring any attempt to cross.\n"
    "To the south lies the rocky beach, its sharp edges glinting in the light. To the west, the ground grows harsher and more uneven, but the forest beyond appears navigable.",
    "The ground beneath your feet feels more like ash than dirt, soft and unsettling with every step. The terrain grows steeper, and a dark, foreboding forest encircles you, its twisted trees casting long, unnatural shadows. Looking to the north, the immense silhouette of Mount Solace looms over the landscape, its jagged peaks disappearing into the clouds. The path ahead is clear—you’ll have to climb.",
    "You are high up the mountain, the air thinner and colder with each step.\n" 
    "From here, the entire landscape stretches out before you Dunmar City, the rocky beach, the sea, and the vast, impassable forest. Yet none of it holds your attention. In front of you stands a massive stone gate, its surface etched with intricate carvings resembling roots that twist and weave along its edges. At its center lies a small crevice, its purpose unknown but unmistakably deliberate."};
const int Location::locations[Location::locnum] = {0, 1, -1, -2, 4, 5, 7, 10,11};

 // Method to get the description of a location
string Location::getLocationsDesc(int index) {
    return locationsdesc[index];
}

// Method to get the location's ID
int Location::getLocations(int index) {
    return locations[index];
}
 void Ground::initializematrix(){
			for(int i; i < 9; i++){
				front[i] = 0;}}
 void Ground::insertmatrix(int col, Item* object) {
    			//cout << "DEBUG: Entered insertmatrix\n";
  	  		 //Validate column index
    			//if (col < 0 || col >= 9) {
        			//cout << "ERROR: Column out of bounds in insertmatrix().\n";
        			//return;}
    			// Validate item pointer
			//if (object == nullptr) {
        		//cout << "ERROR: Trying to insert a nullptr item into Ground::matrix!\n";
      	 		//return;}
    			// Ensure front[col] is within bounds (0 to 3)
    		if (front[col] < 0 || front[col] >= 4) {
        	//	cout << "ERROR: front[col] is out of bounds (" << front[col] << "). Resetting to 0.\n";
        		front[col] = 0;
   	 }
    		// Store the item in the matrix (overwrite if necessary)
    		//cout << "DEBUG: Storing " << object->getItemName() << " in Ground::matrix at row " << front[col] << " and col " << col << '\n';
    		// Check if we're overwriting an existing item
    		//if (matrix[front[col]][col] != nullptr) {
        	//	cout << "WARNING: Overwriting existing item: " << matrix[front[col]][col]->getItemName() << '\n';
    		//}
    		// Insert the new item
  	  	matrix[front[col]][col] = object;
    		// Verify the item was stored
    		//if (matrix[front[col]][col] == object) {
        		//cout << "DEBUG: Successfully stored " << matrix[front[col]][col]->getItemName() << " in Ground::matrix.\n";
    		//} else {
        	//	cout << "ERROR: Storage failed for " << object->getItemName() << "\n";
    		//}
    		// Increment front[col] circularly
    		front[col] = (front[col] + 1) % 4;
    		//cout << "DEBUG: Updated front[" << col << "] to " << front[col] << '\n';
		}
 void Ground::removematrix(int col){
 // Ajustar índice circularmente hacia atrás
    front[col] = (front[col] + 3) % 4;  // equivalente a (front[col] - 1 + 4) % 4
    Item* toDelete = matrix[front[col]][col];
    if (toDelete != nullptr) {
        delete toDelete;
        matrix[front[col]][col] = nullptr;
    }
}
Item* Ground::matrix[4][9] = {nullptr};
int Ground::front[9] = {0};

