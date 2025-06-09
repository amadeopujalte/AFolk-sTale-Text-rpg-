#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include "groundlocation.h"
#include "item.h"
#include "player.h"
#include "enemy.h"
#include "skill.h"
using namespace std;

Player player1;


void delay(int milliseconds) {
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

void typeWriterEffect(const string& text, int delay_ms) {
    for (char c : text) {
        cout << c << flush;  // Print character by character
        this_thread::sleep_for(chrono::milliseconds(delay_ms));  // Add delay
    }
    cout << endl;
}

void clearScreen() {
    // Clear the screen depending on the operating system
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void credits() {
    string roles[] = {
        "Game made by: Booster Studios",
        "CEO",
	"CFO",
        "Narrative Director",
        "Design Director", 
        "Lead Developer", 
        "Artist",  
        "Tester",
        "Project Manager"
    };

    string name = "Amadeo Pujalte";

    int totalCredits = sizeof(roles) / sizeof(roles[0]);

    // Display the credits with a sliding effect
    for (int i = 0; i < totalCredits; ++i) {
        // Clear the screen before displaying new credit
        clearScreen();

        // Display the current role and name sliding down
        for (int j = 0; j <= i; ++j) {
            cout << roles[j] << ": " << name << endl;
        }

        // Delay for a smooth sliding effect
        this_thread::sleep_for(chrono::milliseconds(1500));
    }
    cout << "I hope you enjoyed/will enjoy" << '\n';
    // After the credits end, give a final pause before closing
    this_thread::sleep_for(chrono::milliseconds(2000));
}

void droploot(int location){
int itemsToDrop = rand() % 3 + 1;  // Random number between 1 and 3
    for (int i = 0; i < itemsToDrop; i++) {
        // Randomly choose an item type (weapon, armor, or potion)
        int itemType = rand() % 3;  // Random number between 0, 1, or 2
        // Randomly select an item from the chosen type
        Item* itemToDrop = nullptr;
        if (itemType == 0) {
            // Weapon: Randomly choose a weapon from the weapon array
            int weaponIndex = rand() % 15;  // Random index within weapon array size
            itemToDrop = storeweapon[weaponIndex];
        }
        else if (itemType == 1) {
            // Armor: Randomly choose armor from the armor array
            int armorIndex = rand() % 27;  // Random index within armor array size
            itemToDrop = storearmor[armorIndex];
        }
        else {
            // Potion: Randomly choose a potion from the potion array
            int potionIndex = rand() % 6;  // Random index within potion array size
            itemToDrop = storepotion[potionIndex];
        }
        // Insert the randomly selected item into the ground matrix at the specified location
        if (itemToDrop != nullptr) {
            Ground::insertmatrix(location, itemToDrop);  // Insert item into the matrix at the location
        }
    }	
}
//Combat functions
bool determineFirstTurn() {
	return rand() % 2;
}
string extractItemName(const string& command) {
    string itemName;

    if (command.find("attack with") == 0) {
        itemName = command.substr(11);
    } else if (command.find("use") == 0) {
        itemName = command.substr(3);
    } else {
        return "";
    }

    // Trim leading and trailing whitespace
    itemName.erase(0, itemName.find_first_not_of(' '));
    itemName.erase(itemName.find_last_not_of(' ') + 1);

    // ✅ Ya está limpio, devolver aunque sea una sola palabra
    return itemName;
}
int combatreader(string command,Player& player, Enemy* enemy){
	int flag = 0;
	int validcommand = 0;
	string itemName = extractItemName(command);
	//cout << "DEBUG: Item name = " << itemName << '\n';
 	if(command == "skip turn"){player.skipturn();}
	else if(command == "check inventory"){player.checkinventory();}
	else if(command == "check skills"){player.checkskills();}
	else if(command.find("attack with") == 0 && !itemName.empty()) {
        // Call attack function if the command is "attack with"
		validcommand = player.playerBasicAttack(itemName,enemy);
    } else if (command.find("use") == 0 && !itemName.empty()) {
        // Call use function if the command is "use"
        player.use(itemName);
    } else{
	for(int i = 0; i < 5; i++){
		if(player.availableskills[i] != nullptr){
		if(command == player.availableskills[i]->getSkillName()){
		flag = 1;
		player.availableskills[i]->playerUseSkill(player,enemy);}
		}
		}
	if(flag == 0){
        // Invalid command or missing item name
        cout << "Invalid command or missing item name." << endl;
	validcommand = -1;
	}
    }
return validcommand;
}

void combat(Player& player,Enemy* enemy){
	cin.ignore();
	cin.clear();
	string lastSkill = " ";
	bool turn;
	int validaction = 0;
	string action;
	int gold = rand() % 50;
	enemy->initializeEnemySkills();
	turn =  determineFirstTurn();
	//Reset of conditions
	player.setPlayerStun(false);
	player.setPlayerBleeding(false);
	player.setPlayerBuff(false);
	player.setPlayerDebuff(false);
	enemy->setEnemyStun(false);
	enemy->setEnemyBleeding(false);
	enemy->setEnemyBuff(false);
	enemy->setEnemyDebuff(false);

	turn =  determineFirstTurn();
	cout << "----Entering combat----" << '\n';
	cout << "You face a " << enemy->getEnemyName() << "." << " You scan him, taking his form: " << enemy->getEnemyDesc() << '\n';
	
	if(turn){
		cout << "You start first!" << '\n';}
	else{cout << "The enemy starts first!" << '\n';}
	//Combat Loop
	while(player.getPlayerHp() > 0 && enemy->getEnemyHp() > 0){	
		if(turn && player.getPlayerStun() == false){
			enemy->setEnemyStun(false);
			cout << "Your turn." << '\n';
			enemy->applyEnemyBleed();
			player.displaycombatstats();
			cout << "Options:" << '\n';
		 	cout << "- attack with (weaponname)" << '\n';
			cout << "- skillname"  << '\n';
			cout << "- use (itemname)" << '\n';
			cout << "- check inventory" << '\n';
			cout << "- check skills" << '\n';
			cout << "- skip turn" << '\n';
			cout << "What will you do?" << '\n';
			do{
			getline(cin,action);
			lastSkill = action;
			validaction = combatreader(action, player, enemy);
			//Avoid negative values
			if(player.getPlayerMana() < 0){player.setPlayerMana(0);}
			if(player.getPlayerStamina() < 0){player.setPlayerStamina(0);}
			//cout << "DEBUG: ENEMY HEALTH: " << enemy->getEnemyHp() << '\n';
			//cout << "DEBUG: ENEMY STM: " << enemy->getEnemyStamina() << '\n';
			//cout << "DEBUG: ENEMY MANA: " << enemy->getEnemyMana() << '\n';
			} while(action == "check inventory" || action == "check skills" || validaction == -1);
			player.applyPlayerBuff();
			enemy->applyEnemyDebuff(player,lastSkill);
			//cout << "DEBUG: BUFFCOUNT: " << player.getPlayerBuffCounter() << '\n';
			//cout << "DEBUG: DEBUFFCOUNT: " << player.getPlayerDebuffCounter() << '\n';
			if(enemy->getEnemyHp() <= 0){	
				cout << "You are victorious." << '\n';
				cout << "Xp earned: " << enemy->getEnemyXp() << '\n';
				player.updatestats(0,0,0,0,0,enemy->getEnemyXp());
				cout << "Gold earned: " << gold <<  '\n';
				player.setPlayerGold(gold);
				break;
			}
			turn = false;
			}
		//Enemy Turn
		else{	//cout << "DEBUG: ENTERED ENEMY TURN" << '\n';
			enemy->applyEnemyBuff();
			player.applyPlayerDebuff(enemy);
			if(player.getPlayerStun() == true){
				cout << "You are stunned!" << '\n';
				player.setPlayerStun(false);}
			if(enemy->getEnemyStun() == false){
				player.applyPlayerBleed();
				cout << "Enemy turn." << '\n';
				 delay(1000);
				lastSkill = enemy->enemyTurn(player, enemy);
			//	cout << "DEBUG: ENEMYDMG = " << enemy->getEnemyAtk(); 
				//Avoid negative values
				if(player.getPlayerMana() < 0){player.setPlayerMana(0);}
				if(player.getPlayerStamina() < 0){player.setPlayerStamina(0);}
				if(enemy->getEnemyMana() < 0){enemy->setEnemyMana(0);}
				if(enemy->getEnemyStamina() < 0){enemy->setEnemyStamina(0);}
				//cout << "DEBUG: Skill used: " << lastSkill << '\n';
				delay(1000);
				
			}
			else{cout << "Enemy is stunned!" << '\n';}
			if(player.getPlayerHp() <= 0){
					cout << "You have fallen... and the echoes of your tale fade into whispers, forgotten by time." << '\n';
					cout << "Returning to game menu..." << '\n';
					return;
			}

			turn = true;}
}
	player.checknewlevel(player.getPlayerLevel(),player.getPlayerXp(), player.calculateNextLevelXP(player.getPlayerLevel()));
	droploot(player.getPlayerLoc());
	enemy->resetStats();
		}
void combattutorial(){
	char option;
	delay(1000);
	player1.unlockSkill(player1.getPlayerLevel());
	delay(3000);
	cout << "Welcome to your first battle!" << '\n';
	cout << "With this small tutorial you will be able to defeat every foe!" << '\n';
	cout <<	"While in combat, you shall not be able to use the same commands as in outside of combat, for example, you cannot equip items nor loot." << '\n';
	cout << "Before every combat, you'll have the time to mentally prepare, just press 'enter' and you will start the battle." << '\n';
	cout << "You cannot escape from combat, so every battle is to the the death." << '\n';
	cout << "In order to do a basic attack the command is: " << "attack with weaponname" << '\n';
	cout << "NOTE: the attack is only valid with your equipped weapon, so be sure to carry one always." << '\n';
	cout << "Last but not least, to use abilities you only need to put its exact name. These abilities cost mana and stamina, so be careful with that." << '\n';
	cout << "In order to regenerate both mana and stamina you can use the 'skip turn' command to rest." << '\n';
	cout << "The more you fight the more xp you will earn, which will result in a level increase that will make you stronger. Also after every fight remember to loot your fallen foes, they might carry interesting things..." << '\n';
	cout << "Good luck adventurer! and dont die!!" << '\n';
	cout << "Ready?" << '\n';
	cout << "(y/n)" << '\n';
	cin >> option;
	}	
void normalreader(string command){
	if(command == "head north"){player1.movenorth(player1.getPlayerLoc());}
	else if(command == "head west"){player1.movewest(player1.getPlayerLoc());}
	else if(command == "head east"){player1.moveeast(player1.getPlayerLoc());}
	else if(command == "head south"){player1.movesouth(player1.getPlayerLoc());}
	else if(command == "check surrounds"){player1.checksurrounds(player1.getPlayerLoc());}
	else if(command == "check inventory"){player1.checkinventory();}
	else if(command == "check stats"){player1.checkstats();}
	else if(command == "help"){player1.help();}
	else if(command == "loot"){player1.loot(player1.getPlayerLoc());}
	else if(command == "quit"){}
	else if(command == "go to store"){player1.gostore(player1.getPlayerLoc());}
	else if(command == "check skills"){player1.checkskills();}
	else if(command == "getequippedweapon"){cout << player1.getEquippedWeapon()->getItemName();}
	else if(command == "level"){cout << player1.getPlayerLevel() << '\n';}
    	else if(command.find("inspect ") == 0 ||
             command.find("use ") == 0 ||
             command.find("drop ") == 0 ||
             command.find("equip ") == 0) {
         //Find the space position to extract the command and item name
         size_t spacePos = command.find(' ');
         if (spacePos != string::npos && spacePos + 1 < command.length()) {
          string action = command.substr(0, spacePos);
          string itemName = command.substr(spacePos + 1);

             //Call the appropriate function based on action
             if (action == "inspect") {
                player1.inspectitem(itemName);
              } else if (action == "use") {
                player1.use(itemName);
            } else if (action == "drop") {
                player1.drop(itemName);
            } else if (action == "equip") {
                player1.equip(itemName);
            }
        } else {
            cout << "Invalid command format. Please provide an item name.\n";
        }
      }	
	else{cout << "Invalid command!" << '\n';}
}
	
// Story functions
char chooseclass(){
	char role;
	while(role != 'w' && role != 'h' && role != 'm'){
	cout << "(w) Warrior,(medium damage, high armor, medium stamina, low mana. Uses swords, axes and plate armor)"<< '\n';
	cout << "(h) Hunter, (high damage, medium armor, high stamina, low mana. Uses bows and daggers and light armor)" << '\n';
	cout << "(m) Mage,   (high damage, low armor, medium stamina, high mana. Uses staffs, books and cloth armor)" << '\n';
	cin >> role;}
	player1.setPlayersRole(role);
	switch (role){
		case 'w':
			player1.updatestats(0,0,0,-25,10,0);
			cout << "So in order to achieve this, "<< player1.getPlayerName() << " joined the city's watch." << '\n';
			break;
		case 'h':
			player1.updatestats(0,5,5,-25,5,0);
			cout << "So in order to achieve this, " << player1.getPlayerName() << " joined the hunter's guild." << '\n';
			break;
		case 'm':
			player1.updatestats(0,10,0,25,0,0);
			cout << "So in order to achieve this " << player1.getPlayerName() << " signed up in the town's school of magic." << '\n';
			break;
			}
	return role;
}
void option1(char role){
	char response1;
	while(response1 != 'a' && response1 != 'b'){
	switch (role){
                	case 'w':
                        	cout << "(a) training, mastering his use of heavy armor." << " (+10 stamina,+10 def ,+15 xp)" << '\n';
				cout << "(b) dueling against all who would dare face him." << " (+10 atk,+10 stamina,+15 xp)" << '\n';
                        	cin >> response1;
				if (response1 == 'a'){
					player1.updatestats(0,0,0,0,10,15);}
				else {
					player1.updatestats(0,10,0,0,0,15);}
				break;
                	case 'h':
                        	cout << "(a) learning how to survive in the wild." << "(+10 hp,+ 5 def,+15 xp)" << '\n';
                        	cout << "(b) refining his precision." << "(+10 stamina,+5 atk+15 xp)" << '\n';
				cin >> response1;
				if (response1 == 'a'){
					player1.updatestats(10,0,0,0,5,15);}
				else {
					player1.updatestats(0,5,10,0,0,15);}
                        	break;
                	case 'm':
				cout << "(a) expanding his knowledge, reading tomes and scrolls." << "(+20 mana,+10 hp,+15 xp)" << '\n';
                                cout << "(b) mastering elemental magic." << "(+10 atk,+5 def ,+15 xp)" << '\n';
                                cin >> response1;
                                if (response1 == 'a'){
                                        player1.updatestats(10,0,0,20,0,15);}
                                else{
                                        player1.updatestats(0,10,0,10,0,15);}
                                break;
                        }}
}
void option2(char role){
	 char response2 = ' ';
	while(response2 != 'a' && response2 != 'b'){ 
        switch (role){
                        case 'w':
                                cout << "(a) keeping the city's outskirts safe, joining skirmishes and scout missions." << "(+10 stamina,+5 def,+10 xp)" << '\n';
                                cout << "(b) training new recruits and patrolling the streets." << "(+10 hp ,+5 atk ,+10 xp)" << '\n';
                                cin >> response2;
                                if (response2 == 'a'){
                                        player1.updatestats(0,0,10,0,5,10);}
                                else{
                                        player1.updatestats(10,5,0,0,0,10);}
                                break;
                        case 'h':
                                cout << "(a) seeking wisdom in the wild, learning its essence, balance, and strength." << "(+15 mana,+5 atk,+10 xp)" << '\n';
                                cout << "(b) mercenary tasks, bound by no lord but gold as he honed his skills." << "(+10 atk,+5 hp,+10xp)" << '\n';
                                cin >> response2;
                                if (response2 == 'a'){
                                        player1.updatestats(0,5,0,15,0,10);}
                                else{
                                        player1.updatestats(5,10,0,0,0,10);}
                                break;
                        case 'm':
                                cout << "(a) exploring forbidden tomes and ancient ruins, seeking truths his professors had only dared to whisper about." << "(+10 mana,+5 def,+10 xp)" << '\n';
                                cout << "(b) embracing the path of the battle mage, transforming his knowledge into weapons of unmatched power." << "(+10 atk,+5 stamina,+10 xp)" << '\n';
                                cin >> response2;
                                if (response2 == 'a'){
                                        player1.updatestats(0,0,0,10,5,10);}
                                else{
                                     player1.updatestats(0,10,5,0,0,10);}
                                break;
                        }}
	cout << "Loading choices.." << '\n';
	cout << "----------------------------------------------------------------------------------------------------------" << '\n'; 
}
void gameintroduction(char playerrole){
	// For test mode put typewriter effect in comments.
	switch (playerrole){ 
                        case 'w':{
	string warriorStory = 
	"You wake up to rumbling guards in the barracks. The morning air sharp and cold, as you put your uniform on, the commanding officer's voice echoes through the halls, calling all guards to assemble immediately.\n\n"
        "You quickly join your brothers in the courtyard, where the commander stands tall, his presence commanding absolute silence.\n\n"
        "-'We have received urgent orders from the High Council itself. Our mission is to journey to Mount Solace and retrieve a sword of great importance. This is not just a task; the success of this mission is critical.'\n"
        "You feel the tension in the air, your brothers exchanging uneasy glances. The way the commander spoke was alarmingly unusual.\n"
        "-'A squad has been selected, " + player1.getPlayerName() + ". You will be the captain, Julius sergeant and the rest are Lucian, Valrik, Thomas, Cent, Foyd and Kaelen.'\n"
        "'We have received numerous reports of green skins prowling the area and we believe that others will be in search of the sword.'\n"
        "-'Your orders are clear; retrieve it and bring it back to the city at all costs. Eliminate anyone who stands in your way and DO NOT touch the sword. It possesses magical properties that we do not yet understand.\n"
        "'To ensure this, you will be provided with a containment structure, specially designed to secure the artifact without compromising anyone’s safety.\n"
        "- " + player1.getPlayerName() + ", the party must leave in an hour, buy some supplies with this money and get ready to leave. And " + player1.getPlayerName() + ", do not stop at anything.\n"
        "You take the coin and exit the courtyard while you hear the commander chanting instructions.\n";
        typeWriterEffect(warriorStory);
        break;
			}
                        case 'h':{
				string hunterStory = "The forest is mute, no rustling leaves, no distant birds, only the faint crackle of twigs beneath your feet.\n"
                                "You crouch low, drawing your bowstring taut, your breath steady. Before you, a majestic stag moves gracefully, unaware of the arrow aimed at its heart. "
                                "Just as you prepare to release, a flicker of movement catches your eye, a disturbance so small it could be the wind shifting grass. Instinct takes over. "
                                "You spin and release the arrow toward the unseen threat. Silence. You rise cautiously, eyes fixed where the arrow flew, and then you see it, a ghostly figure, flickering and insubstantial, as if it might dissolve with the breeze. "
                                "Its voice, layered and alien, whispers riddles you cannot grasp. Suddenly, it grows louder, its form swelling, and one word cuts through the haze: Mount Solace. "
                                "The figure vanishes, and you wake with a start, heart pounding. The name Mount Solace echoes in your mind, vivid and inescapable. "
                                "You don’t know what it means, but you can’t shake the feeling that it’s calling to you. "
				"The name lingers. Mount Solace. It burrows into your thoughts, persistent as an old wound. "
				"Shaking off the dream, you dress swiftly and step out of your quarters into the crisp morning air, the scent of damp wood and parchment filling your lungs. "
				"At the Hunter’s Guild Hall, a murmur of voices draws your gaze. A crowd huddles near the contract board, whispers uneasy. You move closer, scanning the usual bounties: beasts, escorts, routine work."
				"Then, your eyes catch something unusual.\n"
				"A charred scrap of parchment, its edges blackened, as if it had barely escaped the flames. The words, scrawled in uneven strokes, send a chill down your spine: "
				"'For those without fear, seek the blade. Claim power beyond measure or riches untold.' "
				"Your breath stills. You don’t know how, but this is connected. Mount Solace. The sword. The vision. You tighten your grip on the worn leather of your gloves. Whatever this is, it isn't chance. It’s a call.\n"
				"And you’ve already decided, you’re going. "
                                "So with the remaining coin of the last contract, you decide to buy some supplies for the journey to Mount Solace.\n";
            			typeWriterEffect(hunterStory);
				break;
			}
                        case 'm': {
				string mageStory = "The halls of the Academy of Magic is eerily quiet at this hour.\n"
                              "You sat in the study chamber, surrounded by towering shelves of books and scrolls. The faint scent of old parchment and candle wax filled the air as you poured over a weathered tome. "
                              "As you trace a line of text with your finger, something shifted, a ripple in the unseen currents of magic. Suddenly the air grows heavy, charged with an energy that sends a chill down your spine. The flame of the nearby candle flickered unnaturally, and a faint whisper, almost imperceptible, brushed against your mind. "
                              "You freeze, closing your eyes to focus. The disturbance pulses again, a dissonance in the flow of magic. You reach out instinctively, weaving threads of arcane energy to attune yourself to the source. Images begin to coalesce in your mind: a gleaming blade, its edges shimmering with unlimited raw power, and a jagged mountain rising against a storm darkened sky. \n"
			      "Mount Solace, the name surfacing unbidden from the depths of the disturbance. "
                              "Whatever power lays hidden in that blade, it had unsettled the very fabric of magic itself. You cant ignore it. "
                              "You stood in your chamber, your satchel packed with the essentials: spell scrolls, protective talismans, and a few weathered journals. The vision of the sword still lingered in your mind, its power pulsing faintly like a second heartbeat. A blade capable of manipulating the fabric of magic itself... the mere thought made your hands tremble with equal parts excitement and dread. You had to study it, to understand it. Such an artifact could reshape everything you knew about the arcane. Just as you were about to leave, a knock echoed at your door. You turned to see your apprentice, Erynn, standing there, his youthful face a mixture of curiosity and determination. "
                              "-'Master' he said, stepping inside. 'Where are you going?'\n "
                              "You hesitate, your mind racing. The journey ahead will be perilous, and Erynn, bright as he is, is far from ready. -'Nowhere that concerns you', you say firmly, hoping your tone would end the conversation. "
                              "But Erynn crossed his arms, his expression unwavering. -'Come on " + player1.getPlayerName() + " you can trust me!, I'm sure I can be of great use.' \n"
                              "You sigh. He isnt wrong, he had shown remarkable potential, more than you ever anticipated. And yet, the thought of him facing the dangers you might encounter fills you with unease. Still after a moment, you relent. -'Fine, you said at last. But you will do exactly as I say. No deviations, no impulsive decisions. This isn’t just another lesson, Erynn. This is real.'\n"
                              "His face lits up. -'Thank you, Master I promise, I won’t let you down.'\n"
			      "-'If you’re coming, you’ll wait for me at the town gates. I need to gather supplies before we set out, and I don’t have time to watch over you while I prepare. Meet me there at dusk and don’t be late.' "
                              "Erynn nods eagerly, practically bouncing out the door. You feeling the weight of the journey ahead pressing even heavier on your shoulders. If the sword is as powerful as you believe, you have to uncover its secrets... and now, you’d have to protect your apprentice along the way.";
            			typeWriterEffect(mageStory);
         			break;
                        }}
}
void tutorial(){
	cout << '\n';
	cout << "You are outside" << '\n';
	cout << "From now on, you will have to use commands in order to move, fight and interact with things" << '\n';
	cout << "You can see the list of commands writing (help). NOTE: commands must be written exactly like in the list." << '\n';
	cout << "If you were to write the command wrong or try an unexisting one the message: Invalid command! will be displayed" << '\n';
	cout << "An important command you should almost always use is: check surrounds." << '\n';
	cout << "This command will provide you with important information of your surroundings, like where you can go and if there is anything interesting your eyes can see" << '\n';
	cout << "Now set forth adventurer and reclaim the sword!!" << " You should probably go to the store first." << '\n';
}
// Crear los objetos, asignarlos al inventario y equiparlos.
void startgear(char role){
	Weapon* startweapon = nullptr;
	Armor* starthead = nullptr;
	Armor* startvest = nullptr;
	Armor* startleg = nullptr;
	switch(role){
		case 'w':
			startweapon = storeweapon[0];
			starthead = storearmor[0];
			startvest = storearmor[1];
			startleg = storearmor[2];
			player1.setInventory(0,startweapon);
			player1.setInventory(1,starthead);
			player1.setInventory(2,startvest);
			player1.setInventory(3,startleg);
			player1.equip("recruit blade");
			player1.equip("iron helm");
			player1.equip("iron vest");
			player1.equip("iron greaves");
			break;

		case 'h':
			startweapon = storeweapon[5];
			starthead = storearmor[9];
			startvest = storearmor[10];
			startleg = storearmor[11];
			player1.setInventory(0,startweapon);
			player1.setInventory(1,starthead);
			player1.setInventory(2,startvest);
			player1.setInventory(3,startleg);
			player1.equip("wooden bow");
			player1.equip("leather hood");
			player1.equip("leather vest");
			player1.equip("leather boots");
			break;
		case 'm':
			startweapon = storeweapon[10];
			starthead = storearmor[18];
			startvest = storearmor[19];
			startleg = storearmor[20];
			player1.setInventory(0,startweapon);
			player1.setInventory(1,starthead);
			player1.setInventory(2,startvest);
			player1.setInventory(3,startleg);
			player1.equip("wooden staff");
			player1.equip("cloth hat");
			player1.equip("cloth robe");
			player1.equip("cloth sandals");
			break;

}
}
			
void newgame(){
	cout << "Starting new game..." << '\n';
	string charactername;
	cout << "Select your character name: " << '\n';
	cin.ignore();
	getline(cin,charactername);
	player1.setPlayerName(charactername);
	cout << "Throughout all the lands of Eryndor a folk's tale is told, that of a young lad from the city of ashes, the old city of Dunmar. His name was " << player1.getPlayerName() << "." << '\n';
	cout << "As a small child he was determined to become a..." << '\n';
	player1.setPlayersRole(chooseclass());
	cout << "In his teenage years " << player1.getPlayerName() << " spent most of his time..." << '\n';
	option1(player1.getPlayersRole());
	cout << "And in its adulthood he focused himself on..." << '\n';
	option2(player1.getPlayersRole());
	startgear(player1.getPlayersRole());
	gameintroduction(player1.getPlayersRole());
	tutorial(); 
}
//Encounters functions


void startadventure(char playerclass){
	static bool eventdone;
	if(eventdone != true){
		switch(playerclass){
			case 'w': {
				 string warriorintro = 	"You depart from Dunmar City with your fellow companions, leading the platoon… The weight of the containment structure slows their pace, shoulders straining beneath its burden. Boots press deep into the dirt, and their breaths comes heavy, but they march on—disciplined, determined. The mission is clear, yet a shadow of doubt lingers in your mind. This will not be easy. "
						       	"Beyond the towering city walls, the air grows thick with tension. The road ahead narrows, flanked by jagged rocks and skeletal trees. Then chaos. "
						   	"Arrows hiss through the air like vengeful spirits. A soldier cries out as a shaft finds its mark. -'Drop the structure! Shields up! Form ranks!.'"
							"Your voice roars over the sudden fray. Steel clashes against stone as the platoon scrambles into position."
							"From the shadows, they come...greenskins, their guttural howls tearing through the night. Orks and goblins, their wicked blades glinting in the dying light, rush forward in a tide of filth and fury. You tighten your grip on your weapon as two Orks charge; you sidestep and cut them down. But as they fall, you see your brothers being overwhelmed by the attack."
				 			" Then, through the shifting lines, a goblin locks eyes with you. Malice twists its wretched face. It lunges...";
				 typeWriterEffect(warriorintro);
				 delay(1000);
				 combattutorial();
				 combat(player1,enemylist[0]);
				if(player1.getPlayerHp() < 0){return;}

				 string warrioroutro = 	"Your foe lies dead."
							"You scan the battlefield, bodies strewn across the dirt, blood soaking into the earth. Only you stand. "
							"One by one, you search your fallen companions. Lifeless eyes stare back, their journeys ended. Then, you find Julius, barely clinging to life, his breath ragged, wounds deep. "
							"-'You'll be fine', you say, kneeling beside him. -'I'll carry you back to Dunmar.' "
							"He grips your arm, shaking his head. -'No, he rasps.-You must go on.' "
							"You see it in his eyes, acceptance, resolve. He knows his time is short. You want to argue, to refuse, but the words die in your throat. "
							"Your gaze shifts to the containment structure. Too heavy for one person. Too much to take alone. You exhale, the weight of choice settling in. "
							"You must leave it behind.\n"
							"Julius gives you a final nod, a faint smile before his breath stills. "
							"Grief threatens to pull you under, but there is no time to mourn. Dunmar still stands. Dunmar still needs you. You will not let their sacrifice be in vain.\n"
							"You tighten your grip on your weapon, steel and fire surging through your veins. The battle is not over. The mission is not lost. "
							"Alone now, you rise. The road ahead is yours to walk.";
				 typeWriterEffect(warrioroutro);
				 break; 
				}
			case 'h': {
				 string hunterintro = 	"You depart from Dunmar City, the contract weighing on your mind as much as the bow at your back. The parchment bore no name, no sigil only the promise of wealth and power for a task left deliberately vague. Who left it? And why? The questions coil in your thoughts, but answers must wait. "
				 			"The city walls fade behind you as you press into the forest, where the canopy sways gently, whispering secrets between the branches. Then movement. Faint, but unmistakable. You drop low, fingers already tracing the fletching of an arrow as your eyes scan the undergrowth. "
							"A band of greenskins. Five, maybe six. Their guttural speech carries through the trees, but their words are lost to you. They seem at ease, unaware of the hunter lurking in the shadows. Then, a shift—two break away, vanishing deeper into the woods. The remaining three settle, tossing down their packs and kindling a small fire. A mistake. "
							"You draw. The string hums as the first arrow finds a throat, silencing the goblin before it can even gasp. The second follows in a heartbeat, burying itself into the chest of another. A short, ragged breath escapes him before he collapses. "
							"Only one remains.\n"
							"Eyes wide with terror, the goblin stares at his fallen kin. Then, in a frantic burst, he snatches a jagged dagger and lunges at you...";
				typeWriterEffect(hunterintro);
				delay(1000);
				combattutorial();
				combat(player1,enemylist[0]);
				if(player1.getPlayerHp() < 0){return;}
				string hunteroutro =	"You scan through the dead. "
							"Orks this close to the city... unusual, unsettling. They weren’t just wandering raiders. They were searching for something. The same thing you are. "
							"The realization settles in. This journey will be harder than you thought. Others seek the sword, and they are willing to kill for it. "
							"The other greenskins must be lurking somewhere around. You know you must move.\n"
							"You tighten your grip on your bow. The road ahead is yours to walk.";
				typeWriterEffect(hunteroutro);
				break;
			}
			case 'm': {
				 string mageintro = 	"As you walk already out of the city walls, Erynn bombards you with questions, his curiosity relentless. You answer, though your mind drifts to the pulse you felt. Did every mage sense it? Could this weapon shift the balance of power, even spark war between mages and the academies? The thought tightens your chest."
							" Then...an ambush."
							" Five greenskins encircle you, grinning, blades glinting. A knife presses against your throat, another at Erynn’s. You stall, words measured, hands subtly weaving a spell into your plea. Their snarls waver, suspicion dulled. When they stand down, rifling through your belongings, Erynn moves, sudden, reckless. "
							"His staff ignites an orc in flames. "
							"An arrow finds him. "
							"Shock seizes you, but instinct takes over. A shield flares, catching the next volley. Another arrow strikes, and with a surge of fury, you unleash a shockwave, hurling one attacker into the trees. The rest flee, taking everything but what you carry. "
							"You turn to Erynn. His eyes stare blankly, breath still. Gone. "
							"Rising, your grief is cut short, a goblin, dagger in hand, lunges...";
				 typeWriterEffect(mageintro);
				 delay(1000);
				 combattutorial();
				 combat(player1,enemylist[0]);
				if(player1.getPlayerHp() < 0){return;}
				string mageoutro = 	"You go to Erynn. "
							"Dead.\n"
							"Anger surges through you. You knew this journey would be dangerous, yet you let him accompany you. You should have protected him. Should have stopped him. "
							"But now, it’s too late. Unless...\n"
							"Your mind races. If the sword is as powerful as the legends claim, perhaps it can undo your mistakes. Perhaps it can bring him back. "
							"You rise, fists clenched, resolve hardening. The road ahead is yours to walk. ";
				 typeWriterEffect(mageoutro);
				 break;
			}
		}
	eventdone = true;
	}
}
bool banditEncounter() {
bool battle = false;	
 string encounter = 
        "As you walk through the woods, a faint smell of smoke drifts through the air.\n"
        "Curious, you follow the scent and soon spot a small clearing. There, around a crackling campfire, sit three figures, chatting quietly.\n"
        "They seem armed, each with a weapon resting nearby, two swords and a bow, their gear rough but functional.\n"
        "As you step closer, one of them notices you and nudges the others. The conversation stops.\n"
        "-'Well, well... looks like we have a guest,' one of them calls out, his voice gruff but friendly.\n"
        "Another smiles, adjusting their weapon casually. -'Come closer, friend. We're not looking for trouble.'\n"
        "The third person, a woman, eyes you warily but doesn't reach for her weapon. -'What brings you to our fire?' she asks.\n"
        "You hesitate for a moment, considering your options.\n";
	static bool eventdone = false;
	if(eventdone != true){
	eventdone = true;
    typeWriterEffect(encounter);

    cout << "What will you do?" << '\n';
    cout << "(a) Approach and greet them." << '\n';
    cout << "(b) Stay cautious, keeping your distance, and ask what they are doing here." << '\n';
    cout << "(c) Attack." << '\n'; 
    
    char response;
    int trustScore = 0;  // Initialize trust score

    while (response != 'a' && response != 'b' && response != 'c') {
        cin >> response;
	cin.clear();
	cin.ignore();
	}
        switch (response) {
            case 'a':
                // Friendly interaction, increase trust score
                trustScore++;
                cout << "You approach the campfire, offering a friendly greeting. The group nods, welcoming you into their circle.\n";
                cout << "-'We're travelers, much like yourself,' one of the men says. -'The woods can be a dangerous place. Care for a meal?'\n";
                cout << "You accept the meal, and as you sit down you eye the party more thoroughly, and notice that the woman has some dried blood on her glove." << '\n';
                cout << "What do you do?" << '\n';
                cout << "(a) Ask about the blood." << '\n';
                cout << "(b) Ask where they are headed." << '\n';
                cout << "(c) Attack." << '\n';

                char response2;
		while(response2 != 'a' && response2 != 'b' && response2 != 'c'){
                cin >> response2;
		cin.clear();
		cin.ignore();
				}
                switch (response2) {
                    case 'a':
                        cout << "-'Oh this? Must have been from the hare we hunted earlier, forgot to wash it,' said the woman." << '\n';
                        break;
                    case 'b':
			trustScore++;
                        cout << "The man looks around uneasily. 'We came from Auralis, just passing through. The journey’s been long, but we're not in any rush.'\n";
                        cout << "You notice they have no bags or any sign of long, term travel, just their weapons and the clothes on their backs.\n";
                        cout << "The others glance at each other, clearly uneasy.\n";
                        cout << "You can tell something’s off. What do you say?" << '\n';
                        cout << "(a) Point out that they don’t look like they've traveled much, with barely any supplies." << '\n';
                        cout << "(b) Remain silent." << '\n';

                        char response3;
			while(response3 != 'a' && response3 != 'b' && response3 != 'c'){
                        cin >> response3;
			cin.clear();
			cin.ignore();
					}
                        switch (response3) {
                            case 'a':
                                cout << "The man shifts uncomfortably. 'We’ve had... difficulties on the road,' he says quickly.\n";
                                break;
                            case 'b':
                                trustScore++;  // Increase trust score for staying silent
                                cout << "You remain silent, watching them closely. The tension seems to ease slightly, and they begin talking more openly.\n";
                                break;
                        }
                        break;
                    case 'c':
                        cout << "Without warning, you draw your weapon and charge toward them!\n";
                        cout << "The group reacts quickly, drawing their weapons and preparing to fight!\n";	
			battle = true;			
                        combat(player1, enemylist[4]);
			if(player1.getPlayerHp() < 0){return battle;}
                        combat(player1, enemylist[3]);
			if(player1.getPlayerHp() < 0){return battle;}
                        combat(player1, enemylist[5]);
			if(player1.getPlayerHp() < 0){return battle;}
			return battle;
                        break;
                }
                break;

            case 'b':
                // Cautious interaction, increase trust score
                trustScore++;
                cout << "You remain cautious, keeping your hand near your weapon as you ask what they are doing in these woods.\n";
                cout << "- 'Just passing through,' says the woman, her eyes never leaving you. 'We’ve heard rumors of bandits nearby. We’re just laying low for the night.'\n";
                cout << "The man looks around  a little nervous. 'We’ve been traveling from Auralis, but haven't really gone far. The roads have been... difficult lately.'\n";
                cout << "You suspect there's more to their story. What do you do?" << '\n';
                cout << "(a) Subtly inquire about what's happening in the area. What's going on with Solace and the surrounding lands?" << '\n';
                cout << "(b) Ask why they’re traveling." << '\n';
                cout << "(c) Attack." << '\n';

                char response4;
		while(response4 != 'a' && response4 != 'b' && response4 != 'c'){
                cin >> response4;
		cin.clear();
		cin.ignore();
		}
                switch (response4) {
                    case 'a':
                        cout << "-'What’s happening?' one of the men chuckles nervously. -'There’s been talk of strange things near Mount Solace... rumors of a hidden power.'\n";
                        cout << "The others exchange uneasy glances. '-We don’t know much, but travelers haven’t been returning from there.'\n";
                        cout << "-'Orks have been appearing more often,' he continues, his eyes narrowing. 'Strange, too, how they’ve been growing bolder as the nights grow colder.'\n";
                        cout << "-'At night, if you listen closely, you can hear howls coming from the abandoned village north of here,' adds the woman, her tone dropping. 'Not sure what’s causing it, but it’s not something we want to get caught up in.'\n";
                        cout << "The third party member shifts uncomfortably, as if he’s been listening to these whispers too long. 'Travelers say the village is cursed... anyone who goes there doesn’t return.'\n";
                        cout << "They all exchange uneasy glances, as if they all share the same unsettling thought.\n";
                        cout << "-'We try to keep away from that area,' says the man, his voice serious now. 'But these Orks... they don’t care for the boundaries anymore.'\n";
                        trustScore++;  // Increase trust score for gathering more information
                        break;
                    case 'b':
                        cout << "-'We’re just making our way, same as everyone else,' says the woman, still eyeing you warily.\n";
                        cout << "You sense they're avoiding the question, but they don’t elaborate.\n";
                        break;
                    case 'c':
			battle = true;
                        cout << "Without warning, you draw your weapon and charge toward them!\n";
                        cout << "The group reacts quickly, drawing their weapons and preparing to fight!\n";
                        combat(player1, enemylist[4]);
			if(player1.getPlayerHp() < 0){return battle;}
                        combat(player1, enemylist[3]);
			if(player1.getPlayerHp() < 0){return battle;}
                        combat(player1, enemylist[5]);
			if(player1.getPlayerHp() < 0){return battle;}
			return battle;
                        break;
                }
                break;

            case 'c':
                // Immediate attack, no trust points added
                cout << "Without warning, you draw your weapon and charge toward them!\n";
                cout << "The group reacts quickly, drawing their weapons and preparing to fight!\n";
		battle = true;
                combat(player1, enemylist[4]);
		if(player1.getPlayerHp() < 0){return battle;}	
                combat(player1, enemylist[3]);
		if(player1.getPlayerHp() < 0){return battle;}		
                combat(player1, enemylist[5]);
		if(player1.getPlayerHp() < 0){return battle;}
		return battle;
                break;

            default:
                cout << "Invalid option. Please select a valid choice.\n";
		break;
        }
cout << "The group looks at you with curious eyes as one of the men speaks up.\n";
cout << "- 'What’s a lone adventurer like yourself doing in these dangerous lands?' he asks, his tone wary but intrigued.\n";
cout << "You feel their eyes on you, waiting for an answer.\n";
cout << "What will you do?\n";
cout << "(a) Explain honestly why you’re here, sharing your true purpose.\n";
cout << "(b) Leave out the important details, only tell part of your story.\n";
cout << "(c) Make up an excuse, say you're just passing through.\n";
cout << "(d) Attack. You don't trust them.\n";
cin.ignore();
char response7;
while (response7 != 'a' && response7 != 'b' && response7 != 'c' && response7 != 'd') {
    cin >> response7;
    
    cin.clear();  // Ensure that cin is cleared.
    cin.ignore(); // Ignore the rest of the input.
}
//cout << "DEBUG: char recibido: [" << response7 << "]\n";
switch (response7) {
    case 'a':
        cout << "- 'I’m here to complete an important task,' you say, keeping your voice steady. 'There are forces at work, and I have a responsibility to stop them. I cannot say more, but it's a matter of life and death.'\n";
        trustScore++;  // Gain trust for being honest
        break;
    case 'b':
	trustScore++;
        cout << "- 'Oh, I’m on a task, just passing through, you know, taking the long road around,' you say, avoiding the truth.\n";
        cout << "They seem to buy your story, but you notice they’re still suspicious. The woman eyes you carefully, as if she knows you’re hiding something.\n";
        break;
    case 'c':
        cout << "- 'I’m just passing through, avoiding the main roads,' you say quickly, trying to downplay the situation. 'I’ve heard rumors about these lands, but I’m just here for the journey, nothing too dangerous.'\n";
        cout << "The man raises an eyebrow. 'A traveler, huh? Well, this place is more dangerous than most realize... stay cautious.'\n";
        break;
    case 'd':
        cout << "Without hesitation, you draw your weapon and charge toward the group.\n";
        cout << "The group quickly reacts, weapons drawn, preparing for the inevitable confrontation!\n";
        combat(player1, enemylist[3]);  // Start combat with the group
        if(player1.getPlayerHp() <= 0) { return battle; }
        combat(player1, enemylist[4]);
        if(player1.getPlayerHp() <= 0) { return battle; }
        combat(player1, enemylist[5]);
        if(player1.getPlayerHp() <= 0) { return battle; }
	return battle;
        break;
}

if (response7 == 'a' || response7 == 'b') {
    cout << "- 'What do you seek on this task?' one of the men asks, eyeing you curiously.\n";
    cout << "- 'I seek a powerful artifact,' you reply, your voice steady. 'It lies within the heart of Mount Solace, a place of untold power. I must find it before it falls into the wrong hands.'\n";
    cout << "The group falls silent for a moment, their eyes flicking to one another. You sense their wariness, but they don't press the matter further.\n";    
    cout << "What will you do?\n";
    cout << "(a) Tell the whole truth about the artifact and Mount Solace.\n";
    cout << "(b) Say you can’t reveal your purpose.\n";
    cout << "(c) Lie, say you’re just traveling further north to meet a merchant.\n";
    char response8 = 'd';
  //  cout << "DEBUG: char8 recibido: " << response8 << '\n';
    while (response8 != 'a' && response8 != 'b' && response8 != 'c') {
        cin >> response8;
        cin.clear();  // Clear any unwanted input.
        cin.ignore(); // Ignore the rest of the input.
    }

    switch (response8) {
        case 'a':
            cout << "- 'I seek a powerful artifact,' you reply, your voice steady. 'It lies within the heart of Mount Solace, a place of untold power. I must find it before it falls into the wrong hands.'\n";
            cout << "The group falls silent for a moment, their eyes flicking to one another. You sense their wariness, but they don't press the matter further.\n";
            break;
        case 'b':
            cout << "- 'I... cannot say,' you reply, your eyes narrowing slightly. 'There are things better left unsaid.'\n";
            cout << "The man nods slowly, as if he understands. 'A man with secrets, eh? Alright, then.'\n";
            break;
        case 'c':
            trustScore -= 1;
            cout << "- 'I just need to go further north,' you explain, giving a shrug. 'There’s a merchant I need to meet. I’m just here for business.'\n";
            cout << "The group exchanges glances, clearly skeptical, but they say nothing more about it. You feel like they might know you're hiding something.\n";
            break;
    }
}

else{
trustScore += -1;
cout << "-'You don't look much like a traveler,' one of the men remarks, squinting at you. 'You’ve got no bags, no proper supplies... just your clothes and weapon. What kind of traveler doesn't prepare for a journey?' he asks, eyeing you with suspicion.\n";
		}
string endstory =  "-'Interesting..., says the woman, Well i guess we should warn you about the village up north.\n"
"-'The village,' one of the men begins, his voice low, as if the very mention of it carries weight. 'It's been abandoned for years, but recently, there have been rumors. Strange things happen there at night.'\n"
"'And recently... it’s become more active. You can feel it, like something stirring in the air.'\n"
"They exchange a look, and you notice a subtle shift in their posture.\n"
"-'But it’s not just the village that’s troubling. It’s what’s been happening around it, the Orks and their strange movements.' says the man, his voice dropping to a whisper.\n"
"-'It’s almost like they’re being drawn to something there,' adds the third person, looking towards the horizon.\n"
"The woman nods, her expression grave. '- You’d be a fool to venture too close,' she adds, glancing at you, then at the weapon at your side.\n"
"'But you seem like you know what you’re doing,' the man continues. 'Perhaps you’ve come across something valuable there? perhaps?'\n"
"The group exchanges a look, a brief flash of something unreadable passing between them. You sense that they know something more. \n"
"You can’t help but feel the weight of their gaze, their curiosity about your mission.\n"
"The conversation falls to silence, you feel the tension rise, you decide that is time to leave and start preparing.\n"
"As you prepare to leave, one of the men steps forward, his expression turning hard.\n"
"-'Leaving so soon? I thought you’d want to share more about your journey,' he says, a slight edge to his voice. 'Seems like we’re all headed toward the same... direction, don’t you think? Some of us know more about what’s ahead than others, but who’s keeping track, right?'\n"
"The group exchanges brief, knowing glances, and the air grows heavier. You get the sense that they might know more than they’re letting on, and it’s making you uneasy.\n";
typeWriterEffect(endstory);
    // If trust score is less than 3, there is combat after the encounter
    if (trustScore < 2) {
        cout << "Sensing the tension in the air, you see the party reach for their weapons, hands shifting toward their swords and bow.\n";
        cout << "You realize these are not mere travelers, and the situation is escalating quickly.\n";
        cout << "You draw your blade, preparing for a fight.\n";
        cout << "The party, now on edge, draw their weapons and get ready for combat.\n";
	battle = true;
        combat(player1, enemylist[4]);
        if(player1.getPlayerHp() < 0){return battle;}
        combat(player1, enemylist[3]);
	if(player1.getPlayerHp() < 0){return battle;}
        combat(player1, enemylist[5]);
	if(player1.getPlayerHp() < 0){return battle;}
	
    } else {
        // If trust score is 3 or higher, peaceful resolution
        cout << "The party nods and says, 'Take care, friend. Safe travels.'\n";
        cout << "You bid them farewell, and they remain seated around the campfire, continuing their conversation.\n";
    }
	eventdone = true;	
	}
return battle;
}
void villageEncounter(){ 
    string story;
    static bool eventdone = false;
	if(!eventdone){
    string encounter = 
        "As you walk deeper into the woods, the trees begin to thin out. Ahead, you can see the remains of a small village. Abandoned, its buildings standing eerily still. "
        "A strange hum reaches your ears, faint at first, but growing louder with each step you take. It’s almost as if the very ground beneath you is alive, resonating with some unseen force.\n"
        "The houses, once filled with the life of its people, are now empty shells. The windows are dark, and the doors hang ajar, their hinges creaking in the wind. "
        "You venture further into the village, the hum intensifying, vibrating through your bones. The village seems... unnaturally still, almost as if it’s waiting.\n"
        "The air grows thick with a strange, suffocating energy. You can’t quite pinpoint what it is, but something feels horribly wrong about this place.\n"
        "As you reach the center of the village, you stop and survey the surroundings. The once-lively streets are now deserted, no signs of life, nothing but the faint echo of the hum. "
        "It’s then that you notice something: the village seems... darkened, as though it’s being swallowed by an unseen shadow, but there’s no source for it. The sun still hangs overhead, yet the light seems diminished.\n"
        "Before you can gather your thoughts, the hum reaches a deafening crescendo. Suddenly, the ground trembles beneath your feet.\n"
        "In the blink of an eye, something monstrous appears before you, its form shifting and writhing. A grotesque abomination, its twisted features barely visible in the dim light, lets out a chilling scream as it charges straight at you.\n";

    // Typewriter effect to create suspense
    typeWriterEffect(encounter);

    cout << "The creature's scream echoes in your ears as it lunges toward you, its grotesque body warping with every movement.\n";
    combat(player1,enemylist[12]);
    if(player1.getPlayerHp() < 0){return;}
    else{
	string story = 
	"Baffled by what you just faced you decide to search the houses.\n"
        "One by one you search for any clues of what happened. Nothing, empty houses.\n"
        "As you enter the leader's house, the air is thick with the scent of decay and dust. Unlike any other structure in the village, this one stands untouched, its walls scarred with age.\n"
        "The once-magnificent house now lies in ruins. The furniture is smashed to pieces, scattered on the floor, as though a violent force had wreaked havoc here long ago.\n"
        "In the center of the room, on the floor, lies a stone statue, a human form that has fallen to the ground. Its body is twisted, with hands raised defensively, as if trying to protect himself from a great danger.\n"
        "There is something unsettling about the statue, his face full of dread and fear. Like if he had just seen a monster.\n"
        "You step cautiously around the wreckage of the house, your eyes drawn to a piece of paper half-hidden under some debris. It’s a letter, yellowed with age, and written in a hand that shakes as if from fear.\n"
        "You unfold it and begin to read its cryptic contents:\n"
        "'The Rune... must remain hidden, never given to another, forgotten by time itself. Its power is beyond reckoning...'\n"
        " Parts of the letter are blurred and unreadable, as though time itself had erased it.\n"
        "'It is too powerful for any man to control. It must not fall into the wrong hands. The mere thought of that thing...'\n"
        "The last part of the letter becomes impossible to read, smudged as if something or someone tried to wipe away its very words.\n"
        "The air feels oppressive as you finish reading, the presence of something unnatural weighing on your mind.\n"; 
	 typeWriterEffect(story); 	

    cout << "What will you do?" << '\n';
    cout << "(a) Inspect the statue closely." << '\n';
    cout << "(b) Search the house for any other clues." << '\n';
    cout << "(c) Leave the house." << '\n';
    char choice;
    int flag1 = 0;
    int flag2 = 0;
    while(choice != 'c'){
    cin >> choice;
    cin.clear();
    cin.ignore();
    switch (choice) {
        case 'a': {
	    if(flag1 == 0) {
            // Inspecting the statue, which causes the player to break it and reveal the rune
 	string encounter = 
        "You step closer to the statue, inspecting it carefully. Your hand grazes the stone, and the statue’s form feels strangely cold.\n"
        "As you trace the defensive posture of the statue, something gives way, and the stone statue falls and breaks apart.\n"
        "Inside, hidden within the stone, you find a glowing rune. It pulses softly, a faint energy emanating from it.\n"
        "The air grows colder as you hold it, and you can feel the weight of its power, as if the very essence of it presses down on your mind.\n"
        "The power of this rune is undeniable... and it now rests in your pocket.\n";
	    typeWriterEffect(encounter);
	    player1.setRune(true);
		cout << "What will you do?" << '\n';
		cout << "(a) Inspect the statue closely." << '\n';
    		cout << "(b) Search the house for any other clues." << '\n';
    		cout << "(c) Leave the house." << '\n';		
	    flag1 = 1;}
	    else{ cout << "You inspect it again searching for something more and find nothing" << '\n';}
            break;
				}

        case 'b':
            // Searching the house yields nothing of interest besides the broken furniture
            cout << "You begin searching the wreckage of the house, hoping to find something more useful. But everything seems to have been either broken or discarded long ago.\n";
            cout << "There’s nothing here that offers more answers. Only the darkened atmosphere and the eerie presence of the statue remain.\n";
            cout << "What will you do?" << '\n';
	    cout << "(a) Inspect the statue closely." << '\n';
    	    cout << "(b) Search the house for any other clues." << '\n';
    	    cout << "(c) Leave the house, feeling uneasy." << '\n';						
            break;

        case 'c':
            // Leaving the house feeling uneasy
            cout << "You decide this place is far too unsettling to remain in. The presence of the statue, the strange letter, and the rune are more than enough for now.\n";
            cout << "You turn and leave the house, the ominous feeling of what you just discovered still weighing heavily on you.\n";
            break;

        default:
            cout << "Invalid option. Please select a valid choice.\n";
    }
		}
cout << "As you leave through the front door two of those creatures stand in you path, as if waiting for you" << '\n';
delay(1000);		
combat(player1,enemylist[12]);
    if(player1.getPlayerHp() < 0){return;}
combat(player1,enemylist[12]);
    if(player1.getPlayerHp() < 0){return;}
}
		
	}
eventdone = true;
}	

void banditEncounter2(){
static bool eventdone = false;
string talk3;
string talk4;
string talk5;
if(eventdone != true){
eventdone = true;
string encounter =
        "As you walk through the woods again, you hear voices ahead. The same group of travelers you encountered before sits near their campfire.\n"
        "This time, their demeanor is different. They are less open, but still show a level of curiosity about you. Their eyes are keen, and they study you as you approach.\n"
        "-'Back again?' one of the men says, his voice still friendly, but with a hint of interest. -'We heard the howls from the village. Did you hear them too?'\n"
        "The woman looks at you carefully. -'We’ve been hearing those sounds all day. They’re not normal, and not something we’ve encountered before.'\n"
        "The third man leans forward slightly, as if intrigued. -'The village… what did you find there? We’ve heard rumors, but they’re all just whispers.'\n"
        "They are clearly curious about your time in the village, but they don’t seem aggressive, just eager for any piece of information you might offer.\n";
    typeWriterEffect(encounter);

    cout << "What will you do?" << '\n';
    cout << "(a) Tell them the whole truth, you explored the village, fought those monsters and found a strange rune." << '\n';
    cout << "(b) Tell them you explored the village, fought the creatures but didn’t find anything." << '\n'; 
    cout << "(c) Lie, say you didn’t find anything, just heard the howls." << '\n';
    char response;
    int trustScore = 1;  // Start with 1 due to prior interaction
    cin >> response;
    cin.clear();
    cin.ignore();
    switch (response) {
        case 'a':
 	 encounter = 
        	"- 'You found a rune?' one of the men asks, his voice barely above a whisper.\n"
        	"The group suddenly becomes very quiet, all eyes on you, studying your every word.\n"
        	"- 'Where did you find it?' the woman asks, her tone turning sharp. 'Can we see it?'\n"
        	"The thought of giving them the rune bothers you, but you keep your composure.\n"
        	"- 'Yes, let's see if it is worth something,' the third man says, eyes scanning your face as if trying to read your intentions.\n"
        	"The atmosphere grows tense as they wait for your answer. You feel like they are trying to elicit more information from you.\n"
        	"Do you give them the rune, or keep it for yourself?\n"; 
            cout << "(a) Give them the rune." << '\n';
            cout << "(b) Refuse." << '\n';
            
            char response2;
	    while(response2 != 'a' && response2 != 'b'){
            cin >> response2;
	    cin.clear();
	    cin.ignore();
	    }
            switch (response2) {
                case 'a': 
                    cout << "As your about to hand over the rune, you feel a strange ache in your body and instinctly flinch back .\n";
                    cout << "But the woman’s eyes flashes with greed, though she doesn’t express it directly. 'We understand more than you think. Hand it over now!' she demands.\n";
                    cout << "They unsheathe their weapons.\n";
                    combat(player1, enemylist[4]);
		    if(player1.getPlayerHp() <= 0){return;}
                    combat(player1, enemylist[3]);
		     if(player1.getPlayerHp() <= 0){return;}				
                    combat(player1, enemylist[5]);
		     if(player1.getPlayerHp() <= 0){return;}
		    return;
                    break;
                case 'b':
                    cout << "You refuse, and the party starts to look at each other. In unison, they nod.\n";
		    cout << "They instantly unsheathe their weapons. -'Give it. Snarled the woman." << '\n';
		    cout << "- 'And what you carry as well.' said another smiling." << '\n';
		    delay(300);
                    cout << "What will you do?.\n";
		    cout << "(a) Try to intimidate them." << '\n';
		    cout << "(b) Try to reason with them." << '\n';
		    cout << "(c) Attack." << '\n';
		    char response3;
		    while(response3 != 'a' && response3 != 'b' && response3 != 'c'){   
		    cin >> response3;
		    cin.clear();
	            cin.ignore();
			}
		    switch(response3){
				case 'a':
					cout << "You grab your weapon firmly. Stand tall and say: " << '\n';
					cout << "-'I have defeated those abominations. You are nothing to me.'" << '\n';
					cout << "-'You still have a chance to walk away. Take it, or you'll regret ever crossing me." << '\n';
					delay(200);		
					cout << "For a second no one moves. Then suddenly one man drops his weapons and runs away." << '\n';
					cout << "The other clearly shocked advance." << '\n';
					delay(1000);
					combat(player1, enemylist[4]);
					 if(player1.getPlayerHp() <= 0){return;}
                    			combat(player1, enemylist[5]);
					 if(player1.getPlayerHp() <= 0){return;}
					return;
					break;
					
				case 'b':
					cout << "You tell them that there is no reason for violence. That there is no need for this and that they might come to an understanding." << '\n';
					cout << "They do not seem to hear you, their gaze fixed on your pocket where the rune remains." << '\n';
					delay(500);
					cout << "One of them attacks." << '\n';
					delay(500);
					cout << "You dodge" << '\n';
     			                combat(player1, enemylist[4]);
					 if(player1.getPlayerHp() <= 0){return;}	
                    			combat(player1, enemylist[3]);
					 if(player1.getPlayerHp() <= 0){return;}			
                    			combat(player1, enemylist[5]);
				          if(player1.getPlayerHp() <= 0){return;}
					return;
                    			break;
				case 'c':
					cout << "In a quick flash you draw your weapon and charge at them!" << '\n';
					delay(200);
					combat(player1, enemylist[4]);
					 if(player1.getPlayerHp() <= 0){return;}	
                    			combat(player1, enemylist[3]);
					 if(player1.getPlayerHp() <= 0){return;}			
                    			combat(player1, enemylist[5]);
				          if(player1.getPlayerHp() <= 0){return;}
					return;
                    			break;
					}
			break; 
            }
            

        case 'b':
		trustScore++;
		talk3 = "- 'Really? You have found nothing?'\n"
        	"She shakes her head, a knowing look in her eyes.\n"
        	"- 'There is a tale of that village over there. Once, something powerful resided there, an artifact whose value was worth a whole kingdom. But greed consumed the people, and they abandoned the place in haste.'\n"
        	"Her voice drops to a whisper, as if the mere mention of it could draw attention.\n"
        	"- 'Some say those howls they can hear at night are the former residents of that place... their bodies consumed, twisted by the artifact’s strange power.'\n"
        	"She pauses, letting the weight of her words hang in the air for a moment.\n"
        	"The man, deep in thought, responds:\n"
        	"- 'They might be guarding it.'\n"
        	"The group falls silent, their eyes fixed on you. The atmosphere is thick with expectation, as if they’re waiting for you to reveal something crucial.\n";
   		typeWriterEffect(talk3);
    		cout << "What will you do?" << '\n';
    		cout << "(a) Remain silent." << '\n';
    		cout << "(b) Continue saying you didn’t find anything, just heard the howls." << '\n';
    		cout << "(c) Shift the conversation and ask them what they know about the artifact." << '\n';
    		char choice;
		while(choice != 'a' && choice != 'b' && choice != 'c'){
    		cin >> choice;
		cin.clear();
         	cin.ignore();
		}
    switch (choice) {
        case 'a':
            // Player remains silent
            cout << "You stay silent, offering nothing more. The tension in the air thickens, but the group says nothing more either.\n";
            cout << "They seem to accept your silence for now, but you feel their eyes on you, studying you.\n";
            break;
        case 'b':
	    trustScore++;
            // Player denies finding anything again
            cout << "'I told you, I didn’t find anything,' you repeat, your voice growing firmer. 'Just the howls and an empty village.'\n";
            cout << "The woman’s eyes narrow. - 'Strange... you keep saying that.'\n";
            break;

        case 'c':
            // Player redirects the conversation
            cout << "- 'What about you?' you ask. - 'What do you know about the artifact? What’s really going on in this area?'\n";
            cout << "The group shifts slightly, clearly surprised by your question.\n";
            cout << "-'We’re just trying to piece things together,' the man says, his tone cautious. -'What we know is that people have been drawn to the village for some time. But most of them don’t return...'\n";
            cout << "The woman looks at you with a new sense of interest. -'You’re asking the right questions... but be careful who you trust, Many still seek this artifact.'\n";
            break;
    }
    cout << "Silence fills the woods." << '\n';
    cout << "What will you do?" << '\n';
    cout << "(a) Ask their true purpose here." << '\n';
    cout << "(b) Leave." << '\n';
    char response4;
    char response5;
    char response6;
    char response7;
    char response8;
    while(response4 != 'a' && response4 != 'b'){
	cin >> response4;
	cin.clear();
	cin.ignore();
				}
    switch(response4){
	case 'a':
		talk4 = "-'You havent figured it out yet?' says a man laughing.'\n'"
			"-'We are looking for the same thing as you are.' adds the woman. "
			"-'But that does not mean we have to be enemies right? As long as you dont stand in our way we wont stand in yours...'\n"
			"-'The problem is that I believe you may have something that we need. And if im right i cannot let you go that easily.'\n"
			"-'So why dont you just show me what you got in your pockets eh?' she says with a smile.\n";
		typeWriterEffect(talk4);
	        delay(100);
		cout << "What will you do?" << '\n';
		cout << "(a) show them your things." << '\n';
		cout << "(b) refuse." << '\n';
		cout << "(c) attack." << '\n';
 		while(response5 != 'a' && response5 != 'b'){
		cin >> response5;
		cin.clear();
		cin.ignore();
				}
		switch(response5){
			case 'a':
				if(player1.getRune()){
				cout << "-'I knew it.' says the woman her eyes wide." << '\n';
				cout << "-'Give it or you will perish.' she says with her weapon unsheathed." << '\n';
				cout << "-'And your coin as well.' says another." << '\n';
				delay(500);
				cout << "What will you do?" << '\n';
				//copy paste de la primera parte
				cout << "(a) Give them the rune." << '\n';
            			cout << "(b) Refuse." << '\n';
	    while(response6 != 'a' && response6 != 'b'){
            cin >> response6;
	    cin.clear();
	    cin.ignore();
	    }
            switch (response6) {
                case 'a': 
                    cout << "As your about to hand over the rune, you feel a strange ache in your body and instinctly flinch back .\n";
                    cout << "But the woman’s eyes flashes with greed, though she doesn’t express it directly. 'We understand more than you think. Hand it over now!' she demands.\n";
                    cout << "They unsheathe their weapons.\n";
                    combat(player1, enemylist[4]);
		    if(player1.getPlayerHp() <= 0){return;}
                    combat(player1, enemylist[3]);
		     if(player1.getPlayerHp() <= 0){return;}				
                    combat(player1, enemylist[5]);
		     if(player1.getPlayerHp() <= 0){return;}
		    return;
                    break;
                case 'b':
                    cout << "You refuse, and the party starts to look at each other. In unison, they nod.\n";
		    cout << "They instantly unsheathe their weapons. -'Give it. Snarled the woman." << '\n';
		    cout << "- 'And what you carry as well.' said another smiling." << '\n';
		    delay(300);
                    cout << "What will you do?.\n";
		    cout << "(a) Try to intimidate them." << '\n';
		    cout << "(b) Try to reason with them." << '\n';
		    cout << "(c) Attack." << '\n';
		    while(response7 != 'a' && response7 != 'b' && response7 != 'c'){   
		    cin >> response7;
		    cin.clear();
	            cin.ignore();
			}
		    switch(response7){
				case 'a':
					cout << "You grab your weapon firmly. Stand tall and say: " << '\n';
					cout << "-'I have defeated those abominations. You are nothing to me.'" << '\n';
					cout << "-'You still have a chance to walk away. Take it, or you'll regret ever crossing me." << '\n';
					delay(200);		
					cout << "For a second no one moves. Then suddenly one man drops his weapons and runs away." << '\n';
					cout << "The other clearly shocked advance." << '\n';
					delay(1000);
					combat(player1, enemylist[4]);
					 if(player1.getPlayerHp() <= 0){return;}
                    			combat(player1, enemylist[5]);
					 if(player1.getPlayerHp() <= 0){return;}
					 return;
					break;
					
				case 'b':
					cout << "You tell them that there is no reason for violence. That there is no need for this and that they might come to an understanding." << '\n';
					cout << "They do not seem to hear you, their gaze fixed on your pocket where the rune remains." << '\n';
					delay(500);
					cout << "One of them attacks." << '\n';
					delay(500);
					cout << "You dodge" << '\n';
     			                combat(player1, enemylist[4]);
					 if(player1.getPlayerHp() <= 0){return;}	
                    			combat(player1, enemylist[3]);
					 if(player1.getPlayerHp() <= 0){return;}			
                    			combat(player1, enemylist[5]);
				          if(player1.getPlayerHp() <= 0){return;}
					return;
                    			break;
				case 'c':
					cout << "In a quick flash you draw your weapon and charge at them!" << '\n';
					delay(200);
					combat(player1, enemylist[4]);
					 if(player1.getPlayerHp() <= 0){return;}	
                    			combat(player1, enemylist[3]);
					 if(player1.getPlayerHp() <= 0){return;}			
                    			combat(player1, enemylist[5]);
				          if(player1.getPlayerHp() <= 0){return;}
					return;
                    			break;
					}
			break; 
            }

								}
				else{
				     cout << "-'Oh, seems i was wrong.'" << '\n';
				     cout << "-'Well then you are free to leave, goodbye.'" << '\n';
				     return;
								}
				break;
		
						}
		break;
	case 'b':
		talk4 = "You give a small nod at them and start to head to the crossroads. As you finish taking your third step you hear: -'One more thing!' says the woman.\n"
			"-'While you were there, did you feel any sort of strange pull?' asks the woman.\n";
		typeWriterEffect(talk4);
		 delay(100);
		cout << "What will you do?" << '\n';
		cout << "(a) Say yes." << '\n';
		cout << "(b) Say no (lie)." << '\n';
 		while(response8 != 'a' && response8 != 'b'){
		cin >> response8;
		cin.clear();
		cin.ignore();
		}
		if(response8 == 'a'){
			cout << "-'I knew it!' screams the woman." << '\n';
			cout << "-'You must have it, then, the tales say the artifact pulls the men's hearts.'" << '\n';
			cout << "-'Boys, attack!'" << '\n';
			delay(1000);
   		    combat(player1, enemylist[4]);
		    if(player1.getPlayerHp() <= 0){return;}
                    combat(player1, enemylist[3]);
		     if(player1.getPlayerHp() <= 0){return;}				
                    combat(player1, enemylist[5]);
		     if(player1.getPlayerHp() <= 0){return;}
		    return;
						}
		else{cout << "-'Mhm'" << '\n';}
		
		break;
						
			}
    break;
		break;

        case 'c':
		talk4 = 
        "-'You didn’t find anything?' The man narrows his eyes, looking at you carefully.\n"
        "-'That’s... strange,' the woman says. 'So, you only heard the howls, and nothing more?'\n"
        "Her suspicion grows, but they still don’t make any hostile move.\n"
        "-'The howls were certainly odd,' the man continues, eyeing you. 'But there’s more to this. We know something is happening.'\n"
        "The group continues to probe for information. They’re clearly trying to know something more.\n"
        "-'We’re just trying to understand what’s going on,' the woman adds.\n"
        "-'But be careful who you trust in these woods. There are others who will kill for this knowledge,' says the third man.\n"
        "They continue their questioning, but it’s clear that they’re trying to figure out if you know anything else.\n";
	typeWriterEffect(talk4);
	delay(200);
	cout << "So you quickly say goodbye to them and start to walk." << '\n';					
	cout << "Then suddenly. -'Wait! ' says the woman." << '\n';
	delay(600);
	    break;
		}
            if (trustScore < 2) {
		cout << "-'I dont believe you.'" << '\n';
		cout << "-'I will have it, even if it means taking it off your corpse.'" << '\n';
                cout << "The tension grows. The woman looks at the others, and you know that they’re now ready to act.\n";
		delay(2000);
                cout << "Suddenly, the group lunges toward you, weapons drawn!\n";
		delay(500);		
                combat(player1, enemylist[4]);
 		if(player1.getPlayerHp() <= 0){return;}
                combat(player1, enemylist[3]);
 		if(player1.getPlayerHp() <= 0){return;}
                combat(player1, enemylist[5]);
		if(player1.getPlayerHp() <= 0){return;}
		return;
            } else {
                cout << "-'Whats your name?'" << '\n';
		cout << "-" << player1.getPlayerName() << '\n'; 
		delay(500);
                cout << "-'Well good luck on your journey'" << player1.getPlayerName() << '\n';
                cout << "They remain seated, watching you leave, but their eyes never stray far from you.\n";
            }

           
	}
	eventdone = true;
}
void villageEncounter2(){
cout << "You walk through the abandoned village, not a single living creature passes." << '\n';
cout << "The strange darkness looms above you." << '\n';
cout << "Then, ahead of you, you see two abominations rushing towards you," << '\n';
combat(player1,enemylist[12]);
if(player1.getPlayerHp() <= 0){return;}
combat(player1,enemylist[12]);
if(player1.getPlayerHp() <= 0){return;}
if(player1.getRune() == false){
cout << "You walk up to the great house. You can feel a strange pull coming from the house." << '\n';
cout << "You enter." << '\n';
cout << "What will you do?" << '\n';
cout << "(a) Inspect the statue closely." << '\n';
cout << "(b) Search the house for any other clues." << '\n';
cout << "(c) Leave the house, feeling uneasy." << '\n';
char choice;
int flag1 = 0;
int flag2 = 0;
while(choice != 'c'){
    cin >> choice;
    cin.clear();
    cin.ignore();
    switch (choice) {
        case 'a': {
	    if(flag1 == 0) {
            // Inspecting the statue, which causes the player to break it and reveal the rune
 	string encounter = 
        "You step closer to the statue, inspecting it carefully. Your hand grazes the stone, and the statue’s form feels strangely cold.\n"
        "As you trace the defensive posture of the statue, something gives way, and the stone statue falls and breaks apart.\n"
        "Inside, hidden within the stone, you find a glowing rune. It pulses softly, a faint energy emanating from it.\n"
        "The air grows colder as you hold it, and you can feel the weight of its power, as if the very essence of it presses down on your mind.\n"
        "The power of this rune is undeniable... and it now rests in your pocket.\n";
	    typeWriterEffect(encounter);
	    player1.setRune(true);
	    flag1 = 1;}
	    else{ cout << "You inspect it again searching for something more and find nothing" << '\n';}
            break;
				}

        case 'b':
            // Searching the house yields nothing of interest besides the broken furniture
            cout << "You begin searching the wreckage of the house, hoping to find something more useful. But everything seems to have been either **broken** or **discarded** long ago.\n";
            cout << "There’s nothing here that offers more answers. Only the darkened atmosphere and the eerie presence of the statue remain.\n";
            break;

        case 'c':
            // Leaving the house feeling uneasy
            cout << "You decide this place is far too unsettling to remain in. The presence of the statue, the strange letter, and the rune are more than enough for now.\n";
            cout << "You turn and leave the house, the ominous feeling of what you just discovered still weighing heavily on you.\n";
            break;

        default:
            cout << "Invalid option. Please select a valid choice.\n";
    }
		}		
	}
	else{cout << "You walk a while longer looking for anything of interest." << '\n';
	     cout << "You found nothing." << '\n';}		
}

void quicksand(){
// 5 pasos para salir
static bool eventdone = false;
int stepstaken = 0;
// 6 para ser comido
string command;
float sinkmeter = 0;
string encounter = 
        "As you walk along the coast, the sound of crashing waves fills the air. The sand beneath your feet feels solid at first, "
        "but suddenly, as you try to lift your foot, you notice it's stuck. A sinking feeling spreads through you, and you look "
        "down to see your boot partially submerged in thick, sticky mud. The more you struggle, the deeper your foot sinks into the "
        "quicksand, the cold, viscous substance creeping up your legs. Panic rises as you realize the earth beneath you is pulling you in.\n";    	
if(!eventdone){
eventdone = true;
typeWriterEffect(encounter);
delay(500);
cout << "With growing fear you stay still. You can see that you are 5 steps from the edge" << '\n';
cout << "The sinking feeling is growing. You need to keep moving or risk being consumed. What will you do?" << '\n';
while(stepstaken < 6 && sinkmeter != 6){
	getline(cin,command);
	if(command == "keep moving" || command == "move" || command == "walk" || command == "step"){
		cout << "You take a step and are closer to the edge." << '\n';
		stepstaken++;
		if(player1.getPlayerWeight() < 15){ 
		cout << "You barely feel the sand shift beneath your feet." << '\n';
		sinkmeter = sinkmeter + 0.5;			
		}
		else if(player1.getPlayerWeight() >= 35){
			cout << "The quicksand pulls you down fast, as if the earth is intent on consuming you." << '\n';
			sinkmeter = sinkmeter + 2;
				}
		else{cout << "You feel the pull of the quicksand, making it harder to move." << '\n';
		     sinkmeter = sinkmeter + 1;}
			}
	else if(command == "stay still" || command == "stay" || command == "dont move" || command == "stop moving"){
		cout << "You stay still, the quicksand does not pull you further but you know you must do something." << '\n';}
	else if(command == "check inventory"){player1.checkinventory();}
	else if(command == "check stats"){player1.checkstats();}
	else if(command == "help"){player1.help();}
	else if(command == "loot"){player1.loot(player1.getPlayerLoc());}
	else if(command == "check skills"){player1.checkskills();}
    	else if(command.find("inspect ") == 0 ||
             command.find("use ") == 0 ||
             command.find("drop ") == 0 ||
             command.find("equip ") == 0) {
         //Find the space position to extract the command and item name
         size_t spacePos = command.find(' ');
         if (spacePos != string::npos && spacePos + 1 < command.length()) {
          string action = command.substr(0, spacePos);
          string itemName = command.substr(spacePos + 1);

             //Call the appropriate function based on action
             if (action == "inspect") {
                player1.inspectitem(itemName);
              } else if (action == "use") {
                player1.use(itemName);
            } else if (action == "drop") {
                player1.drop(itemName);
            } else if (action == "equip") {
                player1.equip(itemName);
            }
        } else {
            cout << "Invalid command format. Please provide an item name.\n";
        }
      }	
	else{cout << "Invalid command!" << '\n';}
	//End of commandreader
	if(stepstaken == 5){
		cout << "With a final surge of strength, you use your arms to claw your way out of the quicksand. Each movement feels like a battle as you pull yourself free, your legs sinking deeper with every second. Finally, with one last desperate push, you drag yourself onto solid ground, gasping for air. You’ve escaped, but the tight grip of the quicksand still haunts your muscles." << '\n';
		cout << "You'll tread more carefully from now on." << '\n';		
			return;}
	if(sinkmeter == 2){cout << "The sand covers your foot completely." << '\n';}
	else if (sinkmeter == 3){cout << "The sand covers you up to your ankles making in it quite difficult." << '\n';}
	else if (sinkmeter == 4) {cout << "The sand is way past your ankles. Its hard but you can still move, you know that soon you shall not be able to move." << '\n';}
	else if (sinkmeter == 5) {cout << "The sand is up to your knees. Taking a step requires great effort, your strength waning. You can feel it, one more wrong step, and you’ll be completely consumed." << '\n';}			
	else if (sinkmeter >= 6){cout << "The sand is up to your waist now, and no matter how hard you try, your legs refuse to lift." << '\n';
				 delay(500);
				 cout << "The quicksand has a firm grip on you, pulling you deeper with each breath. Your strength is gone. You can no longer move, you drown." << '\n';
				 player1.setPlayerHp(0);
		 	         cout << "You have fallen... and the echoes of your tale fade into whispers, forgotten by time." << '\n';
				 cout << "Returning to game menu..." << '\n';
				 return;
			}	
		}
}
}
bool orkEncampment(){
static bool eventdone = false;
string encounter =
        "As you continue your journey north, the wind blowing strong. Ahead, the path is obstructed by a small "
        "Ork encampment. Instinctively, you duck behind a nearby rock, pressing yourself low to the ground. The camp before you seems "
        "almost deserted, with only a few Orks scattered around a dying fire, muttering quietly among themselves. The remnants of what must "
        "have once been a bustling encampment lie in disarray—broken crates, discarded weapons, and tattered tents tell of a larger force now "
        "absent. Two goblins stand guard, their eyes scanning the area lazily, unaware of your presence. It’s clear that the once thriving group "
        "has either left in a hurry or been scattered, but the camp still hums with an eerie, unsettled energy.\n"
 	"You quickly realize that there is no way around the camp without being noticed. The only way forward is through the encampment, and any "
        "attempt to avoid it will likely lead to you being spotted.\n";
string attack;
if(!eventdone){
    typeWriterEffect(encounter);
char action;
cout << "What will you do?" << '\n';
while(action != 'a' && action != 'b'){	
	cout << "(a) attack." << '\n';
	cout << "(b) leave. " << '\n';
	cin >> action;
	cin.clear();
	cin.ignore();
	}
if(action == 'a'){
	switch(player1.getPlayersRole()){
		case 'w':
			attack = "You move swiftly, taking advantage of the element of surprise. With a single powerful swing, your blade cleaves through the two goblins, sending them tumbling to the ground.\n"
				 "You step deeper into the camp, and with a roar, your sword slashes through the first Ork, its edge biting deep into his flesh. He collapses instantly. But the remaining two Orks by the fire grab their weapons, eyes narrowing as they prepare to face you. The tension in the air thickens, and you hear the heavy steps of another Ork approaching. You steel yourself, ready for the battle.\n";
			typeWriterEffect(attack);
			break;
		case 'h':
			attack = "You move like a shadow, striking quickly. The goblins never see you coming, and you dispatch them with swift, silent strikes. Entering the camp, you find the first Ork and, with deadly precision, your weapon cuts him down before he can even react. The other two Orks by the fire completely unaware of your presence react quickly, pulling their weapons and preparing for a fight.\n"
				 "The tension grows as you hear the distant sound of another Ork approaching. But you're ready. You narrow your focus, calculating your next move as the Orks near the fire stare you down, preparing to strike.\n";
			typeWriterEffect(attack);	
			break;
		case 'm':
			attack = "With a surge of magic, you take down the two goblins in a flash, their bodies slumping to the ground before they can even make a sound. You step into the camp, the air crackling with arcane energy, and you unleash a burst of magic that strikes the first Ork. He crumples to the ground, unable to resist the force of your spell.\n"
				  "The two remaining Orks by the fire draw their weapons, but they hesitate, unsure of your next move. Then you hear the sound of heavy footsteps drawing closer, the unmistakable sound of another Ork entering the fray. You brace yourself, ready to face them with all the arcane power at your disposal.\n";
			typeWriterEffect(attack);
			break;
	}
	combat(player1,enemylist[8]);
	if(player1.getPlayerHp() <= 0){return eventdone;}	
	combat(player1,enemylist[10]);
	if(player1.getPlayerHp() <= 0){return eventdone;}	
	encounter = "Breathing hard as the two orks lie dead, you see a massive and imposing ork standing a few feet in front of you.\n"
		    "Its face marked with what seems paint or blood. His massive axe glints in the light, dripping with the blood of fallen soldiers."
		    "The ground shakes with each step he takes, his war cry echoing through the air as he raises his weapon, ready to face you in a battle for supremacy.\n";
	typeWriterEffect(encounter);	
	combat(player1,enemylist[9]);
	if(player1.getPlayerHp() <= 0){return eventdone;}	
	encounter = "The Warchief is dead, the camp empty.\n"
		    "You decide to burn this place to the ground. If other orks return they will be forced to move elsewhere.\n"
		    "You grab a piece of cloth from your fallen foe, head to the still crackling fire and start spreading the fire through the tents. Within a few minutes the camp starts ablaze and you leave with a thick cloud of smoke billows into the sky, rising like a dark storm from the ground.\n";
	typeWriterEffect(encounter);
	eventdone = true;		 
	}
else{ cout << "You retreat carefully and follow your steps back." << '\n';
	cout << "You head south" << '\n';
	player1.setPlayerLoc(-2);	
}
	}
return eventdone;
}
void abominationEncounter(){
	static bool eventdone = false;
	if(!eventdone){
	  string encounter = 
        "You continue your journey through the ashen land, the ground beneath your feet rocky and uneven. The air is thick with the scent "
        "of burnt earth, and the landscape feels harsh, devoid of the life and green you’re used to. It’s a stark contrast to the fertile "
        "fields of Dunmar, and as you walk, a strange sense of unease creeps over you. The mountains to the north loom large, but there is "
        "something unsettling about the sight of Mount Solace. Its towering peak is shrouded in a thick, swirling mist that seems to cover "
        "its immense height, as if the mountain itself is hiding something from your view.\n"
        "As you look toward the mountain, a cold shiver runs down your spine. You can’t shake the feeling that something is terribly wrong "
        "with this place. The air feels heavier, thicker, as if it’s pressing down on you. You’re about to look away when a chilling, inhuman "
        "scream pierces the air, echoing off the rocky cliffs. Your heart races as you freeze in place, trying to pinpoint the source of the "
        "sound. And then you see them.\n"
        "From the misty shadows, three abominations stagger toward you. Their twisted forms are barely humanoid, grotesque shapes lurching with "
        "an unsettling rhythm. Their pale, mottled skin glistens with some sort of slimy residue, and their glowing eyes lock onto you with an "
        "unblinking hunger. They move in unison, their inhuman screams growing louder, their clawed hands reaching for you as they close the "
        "distance. There’s no time to waste—you know they’ll be on you within moments.\n";
	typeWriterEffect(encounter);
	combat(player1,enemylist[12]);
	if(player1.getPlayerHp() <= 0){return;}
	combat(player1,enemylist[12]);
	if(player1.getPlayerHp() <= 0){return;}
	combat(player1,enemylist[12]);
	if(player1.getPlayerHp() <= 0){return;}
	string story = "As the last abomination crumbles to ash, a strange silence fills the air. Standing alone, you survey the clearing, your breath still heavy from the battle. The threat has passed, but the sense of foreboding lingers.\n"
			"Only one thing now separates you from your purpose a towering, treacherous climb to the lonely mountain that looms in the distance. Its peak is still shrouded in mist, calling you forward, but the path to it is not for the faint of heart.";
	typeWriterEffect(story);
	eventdone = true;
	}
}
void skirmish(){
 static bool eventdone = false;
 string battle;
if(!eventdone){
eventdone = true;
 string encounter =
        "As you walk through the lush, green fields a few miles near the entrance of Dunmar, the peaceful serenity of the landscape feels "
        "almost out of place. The soft rustle of grass in the wind is interrupted by the unmistakable sound of clashing weapons and "
        "shouts. You approach cautiously, your eyes narrowing as you make out a bloody skirmish unfolding before you.\n"
        "The City Watch are locked in fierce combat with a group of greenskins. Their weapons flash in the sunlight, blood "
        "spraying the ground beneath them. The greenskins are wild and brutal, attacking with ferocity, while the watch fights with the "
        "discipline of trained soldiers. The battle rages near the outskirts, and you can see the chaos unfolding in the once peaceful field.\n"
        "A few wounded lie on the ground, but the fight is far from over. The sight of the skirmish fills you with unease as the sound of "
        "clashing metal echoes in the air.\n";
typeWriterEffect(encounter);
cout << "What will you do?" << '\n';
cout << "(a) join the battle." << '\n';
cout << "(b) walk past." << '\n';
char action;
char action1;
char action2;
while(action != 'a' && action != 'b'){
	cin >> action;
	cin.clear();
	cin.ignore();
}
if(action == 'a'){
	battle = "You charge into the fray, your weapon raised high. With a swift motion, you strike down an Ork that was about to deliver a fatal blow to a soldier. The Ork crumples to the ground, and the soldier looks at you with wide eyes, nodding in gratitude.\n" 
		 "The battle rages on, but your presence has turned the tide at least for now.\n";
	typeWriterEffect(battle);
	cout << "A goblin charges at you." << '\n';
	delay(600);
	combat(player1,enemylist[1]);
	if(player1.getPlayerHp() <= 0){return;}
        battle = "You quickly scan the battlefield after eliminating the goblin, noting the chaos still unfolding. Neither side has gained the upper hand yet, but your focus sharpens as you spot an Ork rushing toward you, weapon raised, ready to continue the fight.\n";
	typeWriterEffect(battle);
	combat(player1,enemylist[8]);
	if(player1.getPlayerHp() <= 0){return;}
	battle = "After eliminating the Ork, you catch sight of the Squadron Commander, his back pressed against a rocky outcrop as he fends off two Orks. He’s holding his ground well, but you can tell he won’t be able to resist much longer. Across the battlefield, you spot two soldiers being encircled by a group of goblins, their desperate struggle growing more dire by the second.";
	typeWriterEffect(battle);
	cout << "What will you do?" << '\n';
	cout << "(a) aid the commander." << '\n';
	cout << "(b) aid the encircled soldiers." << '\n';
	while(action2 != 'a' && action2 != 'b'){
		cin >> action2;
		cin.clear();
		cin.ignore();
	}
	delay(200);
	battle = "You rush of on a sprint, avoiding slashes and thrusts. Swinging you weapon to aid in what you can those who are your path.\n"
		 "Suddenly in the sprint you crash with a goblin, going both down to the ground. As you recover from the impact you see the goblin above you just about to stab you right in the chest.\n"
		 "In a desperate flash of energy, you land a punch squarely on his mouth, knocking him back and freeing yourself. You rise quickly, seizing your weapon, while the goblin, enraged and blood dripping from his battered face, snarls in fury.\n";
	typeWriterEffect(battle);
	combat(player1,enemylist[2]);
	if(player1.getPlayerHp() <= 0){return;}
	 if (action2 == 'a') {
        // First part for 'a' action: rushing to help the commander
        battle = "You rush forward again, your steps heavy but determined. As the commander struggles to fend off the two Orks, slowly and tiredly retreating, you see your chance.";
        typeWriterEffect(battle);
        if (player1.getPlayersRole() == 'm') {
            // If the player is a mage, incinerate the Ork
            battle = " With a swift motion, you raise your hand, casting a fiery spell that bursts from your fingertips. The Ork closest to you is engulfed in flames, screaming in agony as it crumbles to the ground.\n";
	    typeWriterEffect(battle);			
        } else {
            // If the player is not a mage, stab the Ork
            battle = " You move silently, taking advantage of the confusion. With a swift motion, you drive your weapon into the heart of the closest Ork from behind, its body collapsing with a final gasp.\n";
	    typeWriterEffect(battle);				
        }

        battle = "Both the commander and the remaining Ork are shocked by your swift action. But the Ork, enraged, delivers a brutal punch to the commander, knocking him out cold."
                 " The Ork roars at you in fury, its eyes burning with hatred as it prepares to strike again.\n";
        typeWriterEffect(battle); // Print the battle description
	combat(player1,enemylist[8]);
	if(player1.getPlayerHp() <= 0){return;}		
	battle = "After killing the ork you take a quick scan of the battlefield the greenskins are starting to flee.\n"
		 "You turn to see the commander, now awake. You help him to stand up.\n";
    } else {
        // Else part for action2 != 'a'
        battle = "You rush forward, determination in your eyes. The surprise of your sudden movement catches the goblin off guard.\n";
        typeWriterEffect(battle);
        if (player1.getPlayersRole() == 'm') {
            // If the player is a mage, incinerate the goblin
            battle = "With a flick of your wrist, you unleash a powerful wave of fire, engulfing the goblin in a blaze of flame. The goblin screams in agony, falling to the ground as its body turns to ash.\n";
	    typeWriterEffect(battle);			
        } else {
            // If the player is not a mage, stab the goblin
            battle = "You strike swiftly, your blade plunging through the goblin’s heart with ease. The goblin lets out a surprised gasp, its body slumping to the ground.\n";
	    typeWriterEffect(battle);
        }

        battle = "The element of surprise works in your favor. As the goblin crumbles, the soldiers around you start to break free, fighting back with renewed strength.\n";
        battle = "You turn to face the remaining threat and find yourself facing two goblins, their faces twisted in eager smiles as they prepare to attack.\n";
        typeWriterEffect(battle);  // Print the battle description
	combat(player1,enemylist[1]);
	if(player1.getPlayerHp() <= 0){return;}	
	combat(player1,enemylist[2]);
	if(player1.getPlayerHp() <= 0){return;}
	battle = "After killing the last one you take a quick scan of the battlefield. The soldiers are now safe and you can see that the greenskins are starting to flee.\n";	  
    }
	battle += "You have won the battle.\n"
		   "The soldiers erupt into cheers, their voices filled with triumph and relief as the last of the Orks fall. The sound of their victory reverberates through the battlefield, a chorus of shouts and laughter, their weariness forgotten for a moment. Some begin checking the wounded, offering help to those who can still stand, while others begin the grim task of collecting the Ork heads, spiking them onto tall pikes as a gruesome reminder of their victory.\n" 
		   "The air smells of sweat and blood, but the soldiers' spirits are high, their faces a mix of exhaustion and satisfaction. Amidst the aftermath, a soldier approaches you, his face serious but grateful.\n" 
		   "-'The captain sends his thanks for your bravery,' he says, handing you a pouch heavy with gold. -'Your service will not go forgotten. Here's 200 gold for your efforts.'\n";
	if(player1.getPlayersRole() == 'w'){
		battle += "As the soldier realizes who you are, his posture straightens, and a look of recognition flashes across his face. 'Sir!, we thought you were dead!,' he says, his voice filled with joy. 'I didn’t realize it was you. The High Commander is searching for you.' You nod and respond, 'Tell the Commander I’m still in pursuit of the artifact, and that he should send another squad to reinforce me. But I’m afraid there’s no time to stop; much is happening. So i shall continue on my own.' The soldier nods in understanding before quickly turning and leaving, his footsteps fading as he heads off to relay your message.\n";
		typeWriterEffect(battle);
		}
	else{typeWriterEffect(battle);}
	player1.setPlayerGold(200);
}
	
else{cout << "You move on, leaving the soldiers to their fate." << '\n'; 
}
	}
return;
}
void attackedCaravan(){
static bool eventdone = false;
char action;
int pick;
int pos;
Item* items[5];
if(!eventdone){
eventdone = true;
string encounter =  "As you make your way along the dusty road, the sounds of struggle reach your ears. You push forward and come across a scene that stops you dead in your tracks. A caravan lies in the middle of the path, its wheels turned askew. Thieves have surrounded the cart, tearing through the goods, their movements quick and frantic.\n"
        "The driver, an older man, struggles helplessly on the ground, pinned by one of the thieves. You can see a blade in the air, aimed at his chest, ready to end his life.\n"
        "The thieves haven’t noticed you yet, but the man’s gasps for breath and the threatening sneers of the attackers make it clear: the driver won’t survive if you don’t act soon.\n";
typeWriterEffect(encounter);
cout << "What will you do?" << '\n';
cout << "(a) save the caravan driver." << '\n';
cout << "(b) leave him to his death." << '\n';
while(action != 'a' && action != 'b'){
		cin >> action;
		cin.clear();
		cin.ignore();
}
if(action == 'a'){
	if(player1.getPlayersRole() == 'm'){
		encounter =   "With a sharp focus, you raise your hand, muttering the incantation under your breath. A cold wind sweeps through the air as a bolt of ice forms in your palm. Without hesitation, you launch the ice bolt with precision, watching as it streaks through the air and pierces the thief’s heart. He crumples to the ground instantly, his breath stolen by the icy shard lodged deep within him. The remaining thieves freeze for a moment, eyes wide with fear as they realize the power you wield. Three remain, their weapons drawn, ready to fight.\n";
			} 
	else{encounter = "Without hesitation, you leap into the fray, your weapon drawn and ready. With a quick slash, you cut down the thief holding the driver, his body collapsing to the ground. The others turn in surprise, eyes widening in shock as they realize you’re standing between them and their prize. Three thieves remain, snarling and brandishing their weapons, ready for a fight. The air is thick with tension as you prepare to face them, the driver’s life hanging in the balance.\n";}			
	typeWriterEffect(encounter);
	combat(player1,enemylist[4]);
	if(player1.getPlayerHp() <= 0){return;}
	combat(player1,enemylist[3]);
	if(player1.getPlayerHp() <= 0){return;}
	combat(player1,enemylist[4]);
	if(player1.getPlayerHp() <= 0){return;}
	encounter = "With the bandits lying defeated at your feet, the air grows still. You wipe the sweat from your brow, the weight of the battle lifting from your shoulders. As the last of the attackers falls, you turn to the driver, who is barely standing but alive.\n" 
		    "His face is pale, but a faint smile crosses his lips as he looks at you with gratitude.\n"
		    "-'You saved my life,' he says hoarsely, still catching his breath. -'Take what you need from the caravan. I owe you more than I can repay, but these are yours now.'\n"
		    "He gestures toward the scattered crates and supplies, offering you the chance to pick three items from the caravan. With a nod, you move forward, scanning the goods—whatever you choose, you know you’ve earned it.\n"
		    "Inside the caravan, you see simple supplies bundles of food, linen-made goods, and weathered tools. In the back corner, half-hidden beneath a cloth, you notice something that catches your eye, a large chest with intricate carvings, its contents a mystery.\n";
	typeWriterEffect(encounter);
	cout << "You open it and find:" << '\n';
  for (int i = 0; i < 5; i++) {
            items[i] = nullptr;
    }

    // fill the chest with new random items
    for (int i = 0; i < 2; i++) {
        int number0 = rand() % 15;
        if (storeweapon[number0] != nullptr) {
            items[i] = new Weapon(*storeweapon[number0]);
        }
    }
    for (int i = 2; i < 4; i++) {
        int number1 = rand() % 27;
        if (storearmor[number1] != nullptr) {
            items[i] = new Armor(*storearmor[number1]);
        }
    }
        //Gives Big Hp potion
        if (storepotion[2] != nullptr) {
            items[4] = new Potion(*storepotion[2]);
        }		
    cout << "You may select three items." << '\n';
    cout << "Select the number to pick." << '\n';
    for(int i = 0; i < 5; i++){
		cout << i + 1 << ". Pick " << items[i]->getItemName() << '\n';}
		
    for(int i = 0; i < 2; i++){
	pick = -1;
	while(pick <= 0 || pick >= 5){
	    cout << i << "." << '\n';				
	    if(!(cin >> pick)){
   	    cin.clear(); // Clear error flags if non-integer entered
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	pos = player1.freeslot();
	if(pos != -1){
		if(items[pick] != nullptr){
		player1.setInventory(pos,items[pick]);
		cout << "Item added to inventory" << '\n';
		}
	}
			}
			}
	delay(500);
	cout << "After picking and helping the old man with the caravan, both part ways." << '\n';	
	
		}
else{cout << "You quietly leave the place and carry on your journey." << '\n';}	
}
}
void quest2(){
string encounter = "As you make your way through the desolate area, you stumble upon a wounded man lying on the ground. His body is battered, blood soaking through his clothes, and his eyes flutter in delirium. His mutterings make little sense, the words slipping from his mouth in broken fragments. 'He’s lying... he... he steals... everything... don't... trust him...' he mumbles, his breath shallow and strained.\n"			
"You step closer, trying to make sense of his ramblings, but he suddenly stops, looking directly at you with fevered eyes. 'Kill him,' he whispers, his voice hoarse and urgent. Confused, you ask, 'Who?'"
"The man’s eyes flicker with madness before he continues, his words tumbling out in a disjointed fit. 'He’s lying... he grabs the corpses... steals their things... Avenge us... he has my things. They are yours if you kill him...'"
"As his voice fades, a cold shiver runs down your spine. The man gives you one last pained look before his body goes limp, and with his final breath, he whispers, 'Kill him... Avenge us..'";
typeWriterEffect(encounter);
	 }
bool endgame(){
bool end = false;
char action;
string story2;
//We put the player in full stats
player1.setPlayerHp(player1.getPlayerMaxHp());
player1.setPlayerStamina(player1.getPlayerMaxStamina());
player1.setPlayerMana(player1.getPlayerMaxMana());
string story = 
        "As you step into the mountain’s entrance, the air grows colder, and an eerie silence surrounds you. Carefully, you make your way through the darkness, each step measured as you feel your way through the unknown. Your foot brushes against something cold and metallic, and with a soft click, a mechanism is triggered. Suddenly, the chamber flickers to life, torches lining the walls igniting with a burst of flame, casting long shadows across the stone.\n"
        "Before you stands a vast, circular chamber, the floor covered in dust. In the center of the room, a stone figure kneels, its body frozen in a position of eternal suffering. A massive, intricately designed sword blackened by age is plunged deep into the figure’s chest, its hilt resting just above the ground. The sight strikes you with a wave of unease, and your heart begins to beat faster, a primal instinct telling you that somethinending vibrations through the stone walls. The sound echoes in your ears, and from behind you, a clash of weapons fills the air, two figures are fighting. You turn, startled, and as the dust settles, you finally see them: two shadowy figures, locked in combat, their movements swift and deadly.\n";		
    typeWriterEffect(story); 
string fight;		
switch(player1.getPlayersRole()){
	case 'w':
         	fight = "As the dust settles, you watch the two figures locked in combat. The first, a  hunter, moves with unmatched speed and precision. His dark, reinforced leather armor seems to meld with the shadows, his every movement fluid as he strikes with lethal accuracy. You can almost feel the sharpness of his blade as he strikes quickly and retreats into the shadows, his form barely visible in the dim light.\n"
        		"The second figure, the mage, stands at the ready, his body cloaked in ancient enchanted robes that shimmer faintly with arcane energy. He raises his hand, and you can feel the immense magical power radiating from him as spells are cast with a flick of his fingers. Each movement is deliberate, controlled, and you sense that his magic could easily overwhelm any opponent, if given the chance.\n";
		break;
	case 'h':
 		 fight = "The warrior, clad in heavy armor, stands firm as he faces the mage. His sword is raised, ready to strike, but his opponent is quick with magic. The Mage's robes shimmer with arcane power, and every movement is an incantation. With a mighty swing, the warrior charges, but the mage raises his hands, conjuring a barrier of magic to absorb the blow. The warrior's strikes are powerful, but the mage's agility and magic keep him at bay.\n"
        		 "The warrior’s movements are slow but deliberate, his heavy armor weighing him down. Still, each strike seems like a final decision, and his resolve is unyielding. His focus remains, but the mage dances around him, casting spells in quick succession, each one more powerful than the last. The warrior’s shield is battered, but his endurance holds firm.\n";
		break;
	case 'm':
      		fight = "Against the hunter, the warrior faces a challenge. The hunter is swift, his movements blurring as he darts around the warrior, attacking from the shadows. The warrior swings his sword, but the hunter is already gone, reappearing behind him. The hunter’s leather armor gives him unparalleled mobility, while the warrior’s heavy plate makes him a slow-moving target. The hunter’s precision is unmatched, but the warrior’s resilience and sheer power are something else entirely. Every attack from the hunter is dodged, parried, or absorbed, but each miss costs the hunter valuable time. It's a battle of speed vs strength, and the outcome remains uncertain.\n";		
		break;
		}
    fight += "They seek the sword as well.\n";
    // Final sentence when they notice the player
    fight += "Then suddenly, another tremble shakes the chamber. They stop and start to look at you, their expressions shifting from intense concentration to amazement. The battle halts as their eyess seem to lock on you.\n"
	     "Confused for their sudden stop, you turn around and see that behind you stands an enourmous mass of cracked stone, its form rough and jagged. The stone seems to pulse with crackling energy as ancient runes etched deep into its surface glow, casting an unsettling blue light in the dim chamber. Its presence feels both imposing and otherworldly, as if it was guarding the sword.\n";			
    typeWriterEffect(fight);		
    cout << "What will you do?" << '\n';
    cout << "(a) attack the golem." << '\n';
    cout << "(b) attack the rogue warriors." << '\n';
    while(action != 'a' && action != 'b'){
			cin >> action;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
	}
		if(action == 'a'){
			fight = "The Golem’s stone eyes lock onto you, its immense form looming over the battlefield. The air grows heavy as you step forward, your heart pounding in your chest. It moves with a slow, deliberate grace, its massive stone fists raised in preparation for a crushing blow. You know that any mistake could be your last.\n"
				"Behind you, the sound of clashing fills the air, the rogues still locked in their deadly dance. They seem oblivious to the Golem's wrath, their battle continuing with lethal intensity.\n"
				"But now, the Golem focuses solely on you, and its overwhelming power is undeniable. The ground shakes with its every step, as its stone fist swings toward you, an unstoppable force. There’s no turning back, the battle is yours, and you must survive, while the rogues continue their chaotic fight in the background.\n";
			typeWriterEffect(fight);
			delay(500);
			combat(player1,enemylist[11]);
			if(player1.getPlayerHp() <= 0){return end;}
			fight = "With a final, powerful strike, the Golem crumbles into a pile of rubble, its stone form shattered beyond recognition. As the dust settles, you catch your breath, but there’s no time to rest. The two rogues stand before you, their weapons drawn and eyes filled with determination. They exchange a glance, knowing the battle is far from over. The real fight begins now.\n";
			typeWriterEffect(fight);
			switch(player1.getPlayersRole()){
				case 'w':
					combat(player1,enemylist[15]);
					if(player1.getPlayerHp() <= 0){return end;}
					combat(player1,enemylist[13]);
					if(player1.getPlayerHp() <= 0){return end;}
					 break;
				case 'h':
					combat(player1,enemylist[15]);
					if(player1.getPlayerHp() <= 0){return end;}
					combat(player1,enemylist[14]);
					if(player1.getPlayerHp() <= 0){return end;}
					 break;
				case 'm':
					combat(player1,enemylist[14]);
					if(player1.getPlayerHp() <= 0){return end;}
					combat(player1,enemylist[13]);
					if(player1.getPlayerHp() <= 0){return end;}
					 break;

			}
		}
		else{ fight = "You face the rogue warriors.\n";
		      fight +="They shift glances between you and the Golem. And then, in a sudden, coordinated move, the Golem charges, its stone fists crashing down upon the three of you. Each of the rogues narrowly dodged the massive blows, their movements swift as they evade the Golem's wrath. With a low, grinding roar, the Golem's gaze locks onto one of them, and it shifts its focus, determined to take down the chosen target. You and the other step back, wary, as the battle intensifies. Now, the duel begins only you and your opponent, the air filled with dust as the Golem's thunderous steps shake the chamber.\n";
		      typeWriterEffect(fight);
		      switch(player1.getPlayersRole()){
				case 'w':
					combat(player1,enemylist[15]);
					if(player1.getPlayerHp() <= 0){return end;}
					combat(player1,enemylist[13]);
					if(player1.getPlayerHp() <= 0){return end;}
					 break;
				case 'h':
					combat(player1,enemylist[15]);
					if(player1.getPlayerHp() <= 0){return end;}
					combat(player1,enemylist[14]);
					if(player1.getPlayerHp() <= 0){return end;}
					 break;
				case 'm':
					combat(player1,enemylist[14]);
					if(player1.getPlayerHp() <= 0){return end;}
					combat(player1,enemylist[13]);
					if(player1.getPlayerHp() <= 0){return end;}
					 break;
			}
		   fight = "With the rogues lying defeated at your feet, their bodies crumpling to the ground, you stand tall, breathing heavily from the fight. But as the dust settles, you hear a low rumble. The Golem—its massive stone form still standing—now turns its attention to you. The battle isn’t over. The Golem’s stone eyes lock onto you, and with a thunderous step, it charges. Your heart races; it’s time to face the true test.\n";
		   typeWriterEffect(fight);
		   combat(player1,enemylist[11]);
		   if(player1.getPlayerHp() <= 0){return end;}
		}
story = "With the last of your foes defeated, a quiet silence falls over the chamber, the remnants of battle scattered across the stone floor. Fear still lingers in your chest, but it quickly fades as your hand wraps around the cold hilt of the sword, still lodged deep in the stone form. With a strained grunt, you pull, the stone groaning in resistance, but with a final, desperate tug, the sword slides free.\n"
	"As the weight of the weapon settles in your hands, a surge of energy floods your body. You feel it first as a faint tingle, then as a roar of power coursing through your veins. Strength like you’ve never known fills your muscles, every fiber of your being awakened, every movement suddenly effortless. But it’s not just physical; your mind sharpens, your thoughts clearer than ever. The arcane responds to your will, spells once distant and incomprehensible, now lay before you like tools you can control.\n"
	"With the sword fully in hand, you step back from the stone figure, your heart pounding, not from fear, but from the raw power now surging through you. You exit the cave, the light of the outside world blinding you at first. As you look out at the horizon, the vast world before you, something stirs in your mind. You remember your purpose, the reason you came here. The sword is more than a weapon. It is a key to your destiny, and you are no longer the same. You feel invincible, a force of nature, both in strength and arcane mastery. The world seems small before you, and you know that nothing will stand in your way now.\n";
// switch para cada uno en cual es su destino y objetivo.
switch(player1.getPlayersRole()){
	case 'w':
		story += "As you stand at the edge watching the lands from above, the weight of the sword still heavy in your hands, memories of your mission flood back. You were tasked with retrieving this blade and delivering it to the High Council in Dunmar,a simple duty. But now, as its power hums through you, a thought settles in your mind. You’ve been loyal to them, true to your mission, but you’re certain that you, as the wielder of this sword, can achieve whatever plan they schemed. There’s no reason to give it up. Not now, not after all you went through. The blade is yours, and you know that no one else could wield it as you can.\n"
			 "You will become the strongest soldier, the people's unwavering shield, the one who stands as their greatest defense against whatever threats may come.\n";
		break;
	case 'h':
		story += "As you stand on the cliff, the wind brushing against your face, you gaze out over the sprawling lands below. The vastness of the world stretches before you, endless and full of possibilities. But then, something strange happens. A vision flickers before your eyes, a glimpse of yourself, standing in Dunmar, bathed in light, surrounded by glory. The scene is vivid, almost as if the gods themselves are showing you the path ahead. You feel the pull, a sense of destiny calling you forward."
			 "In that moment, you know—this is not mere chance. This is the will of the gods. And as you look down at the land, you realize that you, before a mercenary, is now capable of anything. Your fate lies before you, and nothing can stand in your way. With newfound determination, you turn and begin your journey toward Dunmar, knowing that the future is yours to shape.\n";	
		break;
	case 'm':
		story += "You stand on the cliff, gazing at the lands below, but the beautiful sight does little to capture your attention. Instead, your focus shifts, and you see them—fine blue tendrils, weaving through the very air around you. It's as if the fabric of the world itself is now within your reach, threads of magic visible to your eyes, pulsing with power. The realization hits you like a thunderclap. The mistake you made, bringing Erynn into this journey, weighs heavily on your mind. But in this moment, you also see your purpose more clearly than ever before.\n"
			"You must return to Dunmar—to learn, to master this new ability, and to correct your mistake. With the power you've now touched, you have the ability to end wars, to reshape the world. You will become the most powerful mage of all time, and from there, do what you see fit for the sake of the world.\n";
		break;
	}
story += "With your newfound strength, you sprint down the mountain, each step lighter, faster, and more powerful than before. The wind rushes past you, but the world blurs around you as you descend with incredible speed. When you finally reach the bottom, you skid to a halt, eyes locking onto a large group of Orks standing in your path. Their eyes glow with an unsettling yellow color, filled with a strange and menacing energy.\n"
	 "They have come for the sword\n";	
typeWriterEffect(story);
//Update the stats of the player cause of the sword
player1.setPlayerHp(player1.getPlayerMaxHp());
player1.setPlayerStamina(player1.getPlayerMaxStamina());
player1.setPlayerMana(player1.getPlayerMaxMana());
player1.updatestats(50,50,50,50,20,0);
combat(player1,enemylist[8]);
if(player1.getPlayerHp() <= 0){return end;}
combat(player1,enemylist[2]);
if(player1.getPlayerHp() <= 0){return end;}
combat(player1,enemylist[10]);
if(player1.getPlayerHp() <= 0){return end;}
combat(player1,enemylist[8]);
if(player1.getPlayerHp() <= 0){return end;}
combat(player1,enemylist[0]);
if(player1.getPlayerHp() <= 0){return end;}
combat(player1,enemylist[9]);
if(player1.getPlayerHp() <= 0){return end;}
combat(player1,enemylist[1]);
if(player1.getPlayerHp() <= 0){return end;}
combat(player1,enemylist[9]);
if(player1.getPlayerHp() <= 0){return end;}
story = "With a pile of green bodies littered before you, the remaining Orks turn and flee, their eyes wide with horror. Without hesitation, you slash through the air, your blade cutting the wind with a powerful wave of energy. The force of your strike rips through the before impassable forest, carving a path straight into the other side. There's no time to waste, you stride forward, knowing your path is now clear.\n";
story += "As you approach the towering gates of Dunmar, the weight of your journey begins to settle in. The massive walls loom above, and you can almost feel the city’s heartbeat from here. As you step forward, a detachment of the city watch steps into your path, their eyes scanning you warily. At their head stands the High Commander, his sharp gaze fixed on you, as if weighing your every move. The tension in the air is thick, and the city’s watchful eyes are on you as you arrive.\n";
switch(player1.getPlayersRole()){
	case 'w':
   story +=
        "You greet them with a friendly nod, raising the sword high. -'Mission complete,' you say, stepping forward to your comrades. But the commander stops you.\n"
        "-'The sword must go in the containment structure,' he orders. As he speaks, you notice the same yellow eyes you've seen before through their helmets, watching greedily. Their gaze is sharp, filled with something dark and hungry, as if they crave the very power you hold.\n"
        "You refuse, confident in your newfound strength. -'There's no need,' you reply, your voice steady. -'I feel stronger than ever, and I’m the one who has earned the right to wield it.'\n"
        "As you attempt to advance, the sharp points of long spears suddenly block your path. The soldiers around you move with precise, practiced steps. The tension in the air grows, and your anger flares.\n"
        "With a scowl, you snap, -'I did this all by myself. I deserve the credit and the right to wield this sword.' The commander looks at you coldly, his voice unwavering as he repeats, -'The council has ordered for me to bring it to them.'\n"
        "In his eyes, you see greed, a desire for power. This isn’t about protection; it’s about control. You feel your resolve harden, your anger boiling over. With a single, powerful swing, you break past the spears, sending the soldiers stumbling back. They attack\n";		
		break;
	case 'h':
 	story += "You are halted by the detachment, their weapons raised in front of you. -'I just want to enter as a citizen of Dunmar,' you state calmly, but the commander’s voice cuts through the tension. -'You are allowed entry only if you relinquish the sword.'\n"
        "You refuse, your grip tightening around the hilt as you meet his gaze. The same yellow eyes you've seen before in the helmets stare back at you, full of something dark. It's not just suspicion, it's desire. You feel a chill run down your spine as the spears rise in a final warning, their points glinting in the light.\n"
        "But you won’t back down. -'I’ve earned this,' you declare, the weight of the sword in your hands reminding you of the trials you’ve faced. With one swift motion, you break through the spears, and the battle begins as they move to attack.\n";	
		break;
	case 'm':
	story += "The detachment blocks your way, weapons raised in unison. -'I am a member of the academy,' you assert, your voice unwavering. -'I demand to be let through as I’ve been sent on important business.'\n"
        "The commander’s face is unreadable, but his response is cold. -'You may pass only if you relinquish the sword.'\n"
        "You shake your head. -'That is for study. You have no authority over it.' You feel your frustration mounting as you notice the same yellow eyes you've seen before staring at you from behind the helmets, their hunger for the power growing more apparent.\n"
        "The spears rise again in a final warning. You’ve had enough. You won’t give up the sword, not after everything. With a single swing, you break the spears and charge forward, the battle beginning as the soldiers attack.\n";		
		break;
	}	
typeWriterEffect(story);
combat(player1,enemylist[6]);
if(player1.getPlayerHp() <= 0){return end;}
combat(player1,enemylist[6]);
if(player1.getPlayerHp() <= 0){return end;}
combat(player1,enemylist[7]);
if(player1.getPlayerHp() <= 0){return end;}
combat(player1,enemylist[6]);
if(player1.getPlayerHp() <= 0){return end;}
combat(player1,enemylist[7]);
if(player1.getPlayerHp() <= 0){return end;}
combat(player1,enemylist[6]);
if(player1.getPlayerHp() <= 0){return end;}
story = "After eliminating the last one, you see as the commander is ordering the troops to retreat. You let them, hoping that they will stop and let you be.\n"
	"Then after some minutes you continue you path to the heart of Dunmar, with your purpose still in mind. As you reach the now desolate square you see that the people of Dunmar are hidden in their homes, clearly not knowing what is happening. You also see in the roofs archers ready to shoot, soldiers from all sides looking at you expectantly.\n"
	"Your inspection of the area is cut by the commander's voice. -'This is your last chance, you are surrounded, surrender and give up the sword.'\n"
	"After a long second you answer...\n"	
	"-'All im surrounded by, is fear and dead men.'\n"
	"-'KILL HIM!' yells the commander\n";
typeWriterEffect(story);

combat(player1,enemylist[7]);
if(player1.getPlayerHp() <= 0){return end;}
combat(player1,enemylist[6]);
if(player1.getPlayerHp() <= 0){return end;}
combat(player1,enemylist[6]);
if(player1.getPlayerHp() <= 0){return end;}
combat(player1,enemylist[6]);
if(player1.getPlayerHp() <= 0){return end;}
combat(player1,enemylist[7]);
if(player1.getPlayerHp() <= 0){return end;}
combat(player1,enemylist[6]);
if(player1.getPlayerHp() <= 0){return end;}
combat(player1,enemylist[7]);
if(player1.getPlayerHp() <= 0){return end;}
combat(player1,enemylist[6]);
if(player1.getPlayerHp() <= 0){return end;}
combat(player1,enemylist[6]);
if(player1.getPlayerHp() <= 0){return end;}
story = "Amidst the chaos, you fend off countless soldiers, your blade cutting through the air with precision. Every blow and arrow aimed at you is narrowly dodged, your body moving almost instinctively, despite the growing weight of exhaustion. Your strength is beginning to slowly fade, but with each passing moment, you feel the power still surging within you, almost unstoppable, as if your will alone is keeping you standing.\n"
	"A single arrow grazes your side, just enough to cause you to loose your balance before the soldiers rush in, throwing themselves at you. As you fall, overwhelmed, you catch sight of the sword still gripped in your hand. The jewel in its pommel begins to glow, faint at first, but growing brighter with each passing second. The light pulses, then suddenly stops. Instinctively, you close your eyes, and in the next moment, an explosion of energy rips through the air, though you don’t feel its force. When you open your eyes, you’re met with the sight of the soldiers lying motionless on the ground, their bodies covered by something dark, unnatural.\n"	
	"In the silence, a voice echoes in your head: -'You're welcome...' A strange, echoing presence speaks directly into your thoughts, its voice low but powerful.\n"
	 "'I am Octavius, creator and former owner of the sword you now wield. All the power you desire can be yours, but you must first become my apprentice. To do so, you need only one thing... plunge the sword into the earth. By doing this, you will return me to the physical world, where your power will grow beyond anything you can imagine.\n"
	 "You hesitate, and ask, -'What will it do?'\n"
	 "-'It will open the path back, unlocking your potential to wield the unimaginable.'\n"
	 "Looking around, you see the destruction: bodies scattered, eyes normal, but the city is grey and ruined. You walk toward the fountain where the commander lies dead, his eyes wide with horror. Staring into the water, you see your reflection, pale, dark, your eyes flickering between yellow and black. All of this cause by just one weapon.\n" 
	"Memories of the stone figure kneeling in the chamber flood your mind its sword plunged into its chest. The weight of your next decision presses down on you.\n"
	"What will you do?\n";
typeWriterEffect(story);
cout << "(a) Plunge the sword into the earth." << '\n';
cout << "(b) Plung the sword into your chest." << '\n';
action = 'd';
while(action != 'a' && action != 'b'){
	cin >> action;
	cin.clear();
	cin.ignore();

	}
if(action == 'a'){
story = "As you raise the sword, a deep resolve fills you. With a powerful thrust, you plunge the blade deep into the earth. As the sword meets the ground, a single pulse emanates from the earth like a shockwave, spreading outward. The air around you seems to darken, and the sky above is suddenly consumed by what appears to be a storm, swirling in ominous clouds. The energy is palpable, the power of the sword surging through the very land.\n" 
"You stand still, feeling the weight of what you’ve just unleashed. Octavius's voice echoes in your mind, his words like a dark affirmation.\n"
"-'Well done, my apprentice... We will achieve great things together...'\n";
delay(1000);
	}
else{
	story = "Determined, you return to the place where the explosion had torn apart the world. You stand amidst the destruction, the aftermath of your actions painfully clear. Everything—completely destroyed. Everyone—dead, their lives lost because of your hunger for power. Your vision blurs with the weight of your guilt and the consequences of your choices.\n"
		"In that moment, you feel the weight of all your mistakes, the unbearable truth sinking in. Without hesitation, you plunge the sword deep into your chest, the cold steel biting through your flesh. You push it as far as it will go, the agony coursing through you, until you can feel nothing but pain.\n"
		"Inside your mind, a voice echoes with a bitter sneer: \n"
		"-'YOU FOOL, YOU HAD EVERYTHING WITHIN YOUR GRASP.'\n"
		"The pain is overwhelming. Your strength fades, your body falters, and you kneel, the world spinning around you.\n" 
		"As your vision dims, everything fades to black, leaving only the silence of your regret.\n";
	}
story2 = "...And that is how the tale ends...";
typeWriterEffect(story);
delay(1000);
typeWriterEffect(story2);
delay(2000);
end = true;
credits();
return end;
		    }
bool mountSolace(){
	char action;
	bool gameended;
	string encounter = "After hours of climbing the steep, jagged slopes of Mount Solace, you reach a plateau. There, before you, stands a massive stone gate, its imposing presence commanding the mountain’s harsh landscape. The surface is etched with intricate carvings, resembling roots that twist and weave, winding their way along the gate’s edges. At its center, you notice a small crevice, narrow yet deliberate in its design. The purpose of the crevice remains a mystery, but its precision suggests it is no accident. You feel a pull toward it, as if the gate itself is waiting for something.\n";
	typeWriterEffect(encounter);
	if(player1.getRune()){
		encounter = "Suddenly, you feel a strange warmth radiating from your pocket. You reach inside, and to your surprise, the rune begins to shine brightly, casting a blinding, pulsating light. The glow intensifies as if responding to the ancient energy of the stone gate before you. Heart pounding, you pull the rune from your pocket and step closer to the gate, the crevice beckoning you. As you carefully press the rune into the small gap at the center, the stone hums with life. With a deep rumble, the massive gate shudders, and slowly, it begins to open, revealing whatever lies beyond.\n";
		typeWriterEffect(encounter);
		cout << "What will you do?" << '\n';
		cout << "(a) enter." << '\n';
		cout << "(b) wait and enter another time." << '\n';
		while(action != 'a' && action != 'b'){
			cin >> action;
			cin.clear();
			cin.ignore();}
		if(action == 'a'){
			cout << "You enter..." << '\n';
			gameended = endgame();
			return gameended;
		}
		else{cout << "You stay put trying to see whats inside but darkness covers everything." << '\n';
		    cout << "Whatever is inside you know is probably best to be well prepared" << '\n';
		 return true;
		    }
	}
	else{
		encounter = "You brace yourself against the massive gate, pushing with all your might. The stone feels cold and unyielding beneath your hands, and despite your best efforts, it doesn’t budge. With a grunt of frustration, you shift your stance and throw yourself against the gate, hoping to break it open, but it remains firm, as if the gate itself is imbued with an ancient strength. No matter how hard you push, it’s clear this gate is not going to open by brute force.\n";
	        return false;
		}
	}				
bool eventchecker(int playerlocation,char playerclass){	
//Variable to determine if the player has battled against the bandits.
static bool battle;
static bool encampmentclear;
static bool gateopened;
static bool gameended = false;
	// First message when going out of the city. Tutorial battle where you lose your companions.
	if(playerlocation == 1 && player1.getLoccount(player1.detloccode(playerlocation)) == 1){
		startadventure(playerclass);
		}
	//Encounter with human party (bandits) Loc 2
	if(playerlocation == 4 && player1.getLoccount(player1.detloccode(playerlocation)) == 1 && battle == false){
		battle = banditEncounter();
	}
	//Second encounter with human party if not dead
	if(playerlocation == 4 && player1.getLoccount(player1.detloccode(playerlocation)) == 2 && battle == false && player1.getLoccount(player1.detloccode(5)) <= 1){
		 banditEncounter2();
	}
	//Encounter if player goes back to the village
	if(playerlocation == 5 && player1.getLoccount(player1.detloccode(playerlocation)) > 1){
		villageEncounter2();
	}
	//Quicksand encounter Loc -1
	if(playerlocation == -1 && player1.getLoccount(player1.detloccode(playerlocation)) == 1){
		quicksand();
	//Ork Encampment blocking passage Loc -1
		encampmentclear = orkEncampment();
	}
	else if(playerlocation == -1 && encampmentclear == false){
		encampmentclear = orkEncampment();}
	//Encounter with Abominations Gets Rune Loc 5
	if(playerlocation == 5 && player1.getLoccount(player1.detloccode(playerlocation)) == 1){
		villageEncounter();
	}
	//Encounter with Skirmish Loc 1 
	if(playerlocation == 1 && player1.getLoccount(player1.detloccode(playerlocation)) == 3){
		skirmish();
		}
	//Encounter with Attacked Caravan Loc -2
	if(playerlocation == -2 && player1.getLoccount(player1.detloccode(playerlocation)) == 3){
		attackedCaravan();}
	//Encounter Abominations Loc 10
	if(playerlocation == 10 && player1.getLoccount(player1.detloccode(playerlocation)) == 1){
		abominationEncounter();
	} 
	if(playerlocation == 7 && player1.getLoccount(player1.detloccode(playerlocation)) == 1){
		quest2();}
	
	// Gate blocking passage Loc 11
	if(playerlocation == 11 && player1.getLoccount(player1.detloccode(playerlocation)) == 1){
		gateopened = mountSolace();}
	// Endgame() Loc 11
	char action;
	if(playerlocation == 11 &&  player1.getLoccount(player1.detloccode(playerlocation)) > 1){
		if(gateopened){
			cout << "You climb again and find that the gate is still opened, the rune still on the crevice." << '\n';
			cout << "What will you do?" << '\n';
			cout << "(a) enter." << '\n';
			cout << "(b) wait and enter another time." << '\n';
			while(action != 'a' && action != 'b'){
				cin >> action;
				cin.clear();
				cin.ignore();}
				if(action == 'a'){
					cout << "You enter..." << '\n';
			gameended = endgame();
			return gameended;
				}
				else{cout << "You stay put trying to see whats inside but darkness covers everything." << '\n';
		    			cout << "Whatever is inside you know is probably best to be well prepared." << '\n';
				return false;
				     }
		}
		else{mountSolace();
		}

	}
return gameended;
}
// Game Loop
int main() {
	srand(time(NULL));
	initializestoreitems();
	initializeenemys();
	char option;
	string action;
	bool endgame;
	do{
        	cout << "|     " << "Welcome to A Folk`s Tale"<< "     |" << '\n';
        	cout << "|  " << "Press 'w' to start game" << "         |" << '\n';
        	cout << "|  " << "Press 'r' to see options" <<      "        |" << '\n';
        	cout << "|  " << "Press 't' to see credits" <<  "        |" << '\n';
		cout << "|  " << "Press 'q' to quit the game" << "      |" << '\n';
        	cin >> option;

        	switch (option) {
                	case 'w':
                        	// function new game
                        	cout << "loading..." << '\n';
				endgame = false;
				cleanupStoreItems();
				cleanenemylist();
				initializestoreitems();
				initializeenemys();
				newgame();
                        	break;
                	case 'r':
                        	cout << "This is a text based rpg, what setting do you want to change???" << '\n';
                        	cout << "If you were thinking to change the difficulty, you have dissapointed me." << '\n';
				break;
                	case 't':
                        	// function credits
                        	credits();
                        	break;
			case 'q':
				cout << "Quitting game" << '\n';
				option = 'q';
				action = "quit";
				break;
                        }
			}
		while(option != 'q' && option != 'w');
	while( action != "quit" && player1.getPlayerHp() > 0 && endgame == false){
		getline(cin,action);
		if(!action.empty()){ 
		normalreader(action);}
		endgame = eventchecker(player1.getPlayerLoc(),player1.getPlayersRole());} 
		
	//cout << "DEBUG: END OF WHILE LOOP" << '\n';
	cleanupStoreItems();
	//cout << "DEBUG: CLEANUPSTOREITEMS COMPLETED SUCCESFULLY" << '\n';
	cleanenemylist();
	//cout << "DEBUG: CLEANENEMYLIST COMPLETED SUCCESFULLY" << '\n';
        return 0; 
	}





                                                                   
