#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <limits>
#include "player.h"
#include "item.h"
#include "skill.h"
#include "enemy.h"
#include "groundlocation.h"
using namespace std;
//player class definitions
        	//methods
		string Player::getPlayerName(){
			return playername;}
		void Player::setPlayerName(string name){
			playername = name;}
		char Player::getPlayersRole(){
			return playersrole;}
		void Player::setPlayersRole(char role){
			playersrole = role;}
		int& Player::getPlayerLoc(){
			return playerloc;}
		void Player::setPlayerLoc(int loc){
			playerloc = loc;}
		int Player::getLoccount(int index){
			return loccount[index];}
		int& Player::getPlayerHp(){
			return hp;}
		void Player::setPlayerHp(int newhp){
			hp = newhp;}
		int& Player::getPlayerMaxHp(){
			return maxhp;}
		int& Player::getPlayerAtk(){
			return atk;}
		int& Player::getPlayerDef(){
			return def;}
		int& Player::getPlayerStamina(){
			return stamina;}
		void Player::setPlayerStamina(int newstamina){
			stamina = newstamina;}
		int& Player::getPlayerMaxStamina(){
			return maxstamina;}
		int& Player::getPlayerMana(){
			return mana;}
		void Player::setPlayerMana(int newmana){
			mana = newmana;}
		int& Player::getPlayerMaxMana(){
			return maxmana;}
		int& Player::getPlayerWeight(){
				return weight;}
		int& Player::getPlayerAccuracy(){
				return accuracy;}
		int& Player::getPlayerCrit(){
				return crit;}
		int& Player::getPlayerLevel(){
				return level;}
		int& Player::getPlayerXp(){
				return xp;}
		bool Player::getRune(){
			return rune;}
		void Player::setRune(bool hasrune){
			rune = hasrune;}
			//Status effects
		bool& Player::getPlayerStun(){
				return stun;}
		void Player::setPlayerStun(bool stunstate){
				stun = stunstate;}
		bool& Player::getPlayerBuff(){
				return buff;}
		void Player::setPlayerBuff(bool buff){
				buff = buff;}
		int& Player::getPlayerBuffCounter(){
				return buffcounter;}
		void Player::setPlayerBuffCounter(int counts){
				buffcounter = counts;}
		bool& Player::getPlayerBleeding(){
				return bleeding;}
		void Player::setPlayerBleeding(bool status){
				bleeding = status;}
		int& Player::getPlayerBleedCounter(){
				return bleedcount;}
		void Player::setPlayerBleedCounter(int counts){
				bleedcount = counts;}
		bool& Player::getPlayerDebuff(){
			return debuff;}
		void Player::setPlayerDebuff(bool buff){
			debuff = buff;}
		int& Player::getPlayerDebuffCounter(){
			return debuffcounter;}
		void Player::setPlayerDebuffCounter(int counts){
			debuffcounter = counts;}
		
		//Equippedset
		Item* Player::getEquippedWeapon(){
			return equippedweapon;}
		void Player::setEquippedWeapon(Item* weapon){
			equippedweapon = weapon;}
		Item* Player::getEquippedArmor(string type){
			if(type == "head"){return equippedhead;}
			else if(type == "vest"){return equippedvest;}
			else{return equippedleg;}}
		void Player::setEquippedArmor(string type,Item* armor){
			if(type == "head"){ equippedhead = armor;}	
			else if(type == "vest"){equippedvest = armor;}
			else{equippedleg = armor;}}
		//combat functions
int Player::playerBasicAttack(string weapon, Enemy* enemy){
	srand(time(0));
	int validattack = 0;
	int damage;
	int damagetaken;
	int random;
	if(getEquippedWeapon() != nullptr && weapon == getEquippedWeapon()->getItemName()){
		if(checkhit()){
		if(checkcrit()){
			cout << "Critical hit!" << '\n';
			damage = atk *2;
			damagetaken = damage - enemy->getEnemyDef();
			if(damagetaken <= 0){damagetaken = 0;}
			enemy->setEnemyHp(enemy->getEnemyHp() - damagetaken);
		}
			else{
				cout << "Hit!" << '\n';
				damage = atk;
				damagetaken = damage - enemy->getEnemyDef();
				if(damagetaken <= 0){damagetaken = 0;}
				enemy->setEnemyHp(enemy->getEnemyHp() - damagetaken);
		}
			cout << "Damage dealt: " << damagetaken << '\n';
	}
		else{cout << "You missed!" << '\n';}
		}
	else{cout << "You do not have that weapon equipped!" << '\n';
		validattack = -1;
	}		
		if(enemy->getEnemyHp() <= 0){
			switch(getPlayersRole()){
				case 'w':
					random = rand() % 4;
					break;
				case 'h':
					random = rand() % (9 - 6) + 5;
					break;
				case 'm':
					random = rand() % (14 - 11) + 10;
					break;
			}
			cout << basicAttackFinishers[random] << '\n';
		}
	return validattack;
	}

void Player::skipturn(){
	stamina = (min(stamina + 25, maxstamina));
    	mana = (min(mana + 25, maxmana));
    	cout << playername << " takes a moment to recover. stamina and mana restored slightly." << '\n';
}
void Player::applyPlayerBleed(){
	int bleeddmg = 0;
	if(bleeding == true){
		//For both skills is 5
		bleeddmg = 5;	
		hp = hp - bleeddmg;
		bleedcount += -1;
		cout << "You lose " << bleeddmg << " hp due to bleeding" << '\n';
		if(bleedcount <= 0){
			bleeding = false;}
		}
	}
void Player::applyPlayerBuff(){
	int skillbuff;
	if(buff == true){
		//War Cry
		// Def increase 40%
		// Atk decrease 15%
		def = def * 1,4;
		skillbuff = atk * 0,15;
		atk = atk - skillbuff;
		buffcounter += -1;
		cout << "Your war cry still echoes in your spirit!" << '\n';
		if(buffcounter <= 0){
			buff == false;}
		}
	}
void Player::applyPlayerDebuff(Enemy* enemy){
	int skilldebuff;
	if(debuff == true){
		switch(enemy->getEnemyRole()){
			case 'w':
				//Shattering Blow 
				//-20% defense for 2 turns
				skilldebuff = maxdef *0,2;
				def = def - skilldebuff;
				cout << "The fractures in your armor weaken your defenses!" << '\n';
				break;
			case 'h':
				//EvasiveManeuver
				//+20% evasion == -20% accuracy
				skilldebuff = accuracy * 0,2;
				accuracy = accuracy - skilldebuff;
				cout << "Your enemy keeps moving rapidly, making it harder to hit!" << '\n';
				break;
			case 'm':
				//Warping Curse
				//-50% attack damage for 1 turn
				skilldebuff = maxatk * 0,5;
				atk = atk - skilldebuff;
				break;
		}
		debuffcounter += -1;
		if(debuffcounter <= 0){
			debuff == false;
			debuffcounter = 0;
			atk = maxatk;
			def = maxdef;
			accuracy = 85;
		}
		}
	}

//Skill Functions
void Player::unlockSkill(int level){
	int x = 0;
	while(availableskills[x] != nullptr){
		x += 1;}
	switch(level){
		case 1:
			switch(playersrole){
				case 'w':
					availableskills[x] = new BrutalStrike();
					break;
				case 'h':
					availableskills[x] = new TripleShot();
					break;
				case 'm':
					availableskills[x] = new Fireball();
					break;
			}
			break;
		case 2:
		switch(playersrole){
				case 'w':
					availableskills[x] = new DefensiveStance();
					break;
				case 'h':
					availableskills[x] = new PreciseShot();
					break;
				case 'm':
					availableskills[x] = new IceSpike();
					break;
			}
	
			break;
		case 3:
		switch(playersrole){
				case 'w':
					availableskills[x] = new CripplingBlow();
					break;
				case 'h':
					availableskills[x] = new CrimsonStrike();
					break;
				case 'm':
					availableskills[x] = new ArcaneBarrier();
					break;
			}
	
			break;
		case 4:
			switch(playersrole){
				case 'w':
					availableskills[x] = new WarCry();
					break;
				case 'h':
					availableskills[x] = new Evasion();
					break;
				case 'm':
					availableskills[x] = new ArcaneDischarge();
					break;
			}

			break;
		case 5:
			switch(playersrole){
				case 'w':
					availableskills[x] = new UnbreakableWill();
					break;
				case 'h':
					availableskills[x] = new DeathsGamble();
					break;
				case 'm':
					availableskills[x] = new Kamehameha();
					break;
			}
			break;
	}
	cout << "New skill unlocked!" << '\n';
	cout << "Name: " << availableskills[x]->getSkillName() << '\n';
	cout << "Description: " << availableskills[x]->getSkillDesc() << '\n';
	cout << "Damage bonus: " << availableskills[x]->getSkillDmg() << '\n';
	cout << "Stamina cost: "<< availableskills[x]->getSkillStmCost() << '\n';
	cout << "Mana cost: " << availableskills[x]->getSkillMcost() << '\n';
}

int Player::calculateNextLevelXP(int level) {
    return 100 + (level - 1) * 150;  
}
void Player::checknewlevel(int &level, int &xp, int nextLevelXp){
	int x = 0;
    while (xp >= nextLevelXp && level < 5) {  
        level++;
        xp -= nextLevelXp;  
        nextLevelXp = calculateNextLevelXP(level); 
        cout << "You feel stronger! You have reached Level " << level << "!\n";
	updatestats(30,15,45,45,10,0);
	hp = maxhp;
	mana = maxmana;
	stamina = maxstamina;
	cout << "Your stats have increased" << '\n';
	unlockSkill(level);
    }
}

void Player::checkskills(){
	for(int i = 0; i < 6; i++){
		if(availableskills[i] != nullptr){
			cout << "name: " << availableskills[i]->getSkillName() << '\n';
			cout << "description: " << availableskills[i]->getSkillDesc() << '\n';
			cout << "damage bonus: " << availableskills[i]->getSkillDmg() << '\n';
			cout << "stamina cost: " << availableskills[i]->getSkillStmCost() << '\n';
			cout << "mana cost: " << availableskills[i]->getSkillMcost() << '\n';
		}
		}
	}
			
		void Player::displaycombatstats(){
			int barlenght = 10;
			int displayedmana = 0;
			int displayedstamina = 0;
			if(stamina < 0){
				displayedstamina = 0;}
			else{displayedstamina = stamina;}
			if(mana < 0){
				displayedmana = 0;}
			else{displayedmana = mana;}

			cout << "-------------------------------------------------------------" << '\n';
			//hp bar
			int hpfilledblocks = (hp * barlenght) / maxhp;
			string hpbar = string(hpfilledblocks,'#') + string(barlenght - hpfilledblocks, '-');
			cout << "hp: " << hpbar << " (" << hp << "/" << maxhp << ")" << '\n';
			//mp bar 
			int manafilledblocks = (mana * barlenght) / maxmana;
			string mpbar = string(manafilledblocks,'#') + string(barlenght - manafilledblocks,'-');
			cout << "mp: " << mpbar << " (" << displayedmana << "/" << maxmana << ")" << '\n';
			//stp bar
			int stfilledblocks = (stamina * barlenght) / maxstamina;
			string stbar = string(stfilledblocks,'#') + string(barlenght - stfilledblocks, '-');
			cout << "stp: " << stbar << " (" << displayedstamina << "/" << maxstamina << ")" << '\n';
			cout << "--------------------------------------------------------------" << '\n';
		}
		// muestra los objetos de la tienda.
	 	Item* Player::displayitems[7] = {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};	
		int Player::getPlayerGold(){
			return playergold;}
		void Player::setPlayerGold(int gold){
			playergold += gold;}
		// inventory functions
		Item* Player::getInventory(int index){
			return inventory[index];}
		void Player::setInventory(int index, Item* object){
			inventory[index] = object;}
		int Player::freeslot(){
		int flag = 0;
		int i = 0;
		int x = 0;
		while(flag == 0 && x < 10){
			if(getInventory(x) == nullptr){
				i = x;
				flag = 1;}
			else{x += 1;}
			}
		if(flag == 0){cout << "inventory full" << '\n';
			i = -1;}
		return i;
		}
			
		int Player::detloccode(int playerlocation){
		int locationcode;
		switch(playerlocation){
                                case -1:
                                        locationcode = 2;
                                        break;
                                case -2:
                                        locationcode = 3;
                                        break;
                                case 7:
                                        locationcode = 6;
                                        break;
                                case 10:
                                        locationcode = 7;
                                        break;
                                case 11:
                                        locationcode = 8;
                                        break;
				default:
					locationcode = playerlocation;
					break;
                                }
			return locationcode;}
		// parametised constructor
		Player::Player(int playerloc,int maxhp,int hp, int atk, int maxatk, int maxstamina, int stamina, int maxmana, int mana, int def, int maxdef, int weight, int xp,int gold)
	: playerloc(playerloc), maxhp(maxhp), hp(hp), atk(atk), maxatk(maxatk), maxstamina(maxstamina),  stamina(stamina), maxmana(maxmana), mana(mana), def(def), maxdef(maxdef), weight(weight), xp(xp), playergold(gold){}
		//default constructor
		Player::Player(): playerloc(0), maxhp(100),hp(100),atk(0),maxstamina(100),stamina(100),maxmana(100),mana(100),def(0), maxdef(0),weight(0),xp(0),playergold(150){
			 for (int i = 0; i < 10; i++) {
            inventory[i] = nullptr;  // initialize each slot to null (empty)
	}
	
basicAttackFinishers[0] = "Your strike was the last thing they ever saw, as their head was separated from their body.";
basicAttackFinishers[1] = "Steel met flesh, and they collapsed, their blood staining the ground beneath them.";
basicAttackFinishers[2] = "With sheer force, your blow shattered their defenses, leaving them no chance to recover.";
basicAttackFinishers[3] = "Your weapon cleaved through them with ease, severing them in a single, decisive strike.";
basicAttackFinishers[4] = "Their body crumbled under the weight of your blow, unable to withstand your power.";
basicAttackFinishers[5] = "Swift and precise, your attack found its mark, and they fell, lifeless and unmoving.";
basicAttackFinishers[6] = "They never stood a chance against your strength, their body torn apart with a single blow.";
basicAttackFinishers[7] = "A well-placed strike, and they dropped to the ground, their life fading away instantly.";
basicAttackFinishers[8] = "With perfect aim, your weapon found its target, and they collapsed in defeat.";
basicAttackFinishers[9] = "You took them down with a surge of magic, their body dissipating into a puff of smoke.";
basicAttackFinishers[10] = "Your magic-infused strike ended them, their form disintegrating in a flash of arcane energy.";
basicAttackFinishers[11] = "A single precise hit, and their body burned away in the wake of your magical power.";
basicAttackFinishers[12] = "Even without spells, your strike proved deadly, their body crumbling under the sheer force.";
basicAttackFinishers[13] = "Your weapon surged with energy as it delivered the final blow, their form vanishing in a burst of light.";
basicAttackFinishers[14] = "Arcane power flowed through your strike, and they were consumed by its force, leaving nothing behind.";	
		equippedweapon = nullptr;
		equippedhead = nullptr;
		equippedvest = nullptr;
		equippedleg = nullptr;
}

		// destructor
		Player::~Player() {
		//	cout << "DEBUG: PLAYER DESTRUCTOR ENTERED" << '\n';
			equippedweapon = nullptr;
			equippedhead = nullptr;
			equippedvest = nullptr;
			equippedleg = nullptr;
			for(int i; i < 10; i++){
				inventory[i] = nullptr;	
			}
			for(int i; i < 6; i++){
				availableskills[i] = nullptr;
			}
	
			
		}
		//game mechanics
		void Player::help(){
		cout << "head (direction), (north,south,east,west)" << '\n';
		cout << "check surrounds" << '\n';
		cout << "check inventory" << '\n';
		cout << "check stats" << '\n';
		cout << "inspect itemname" << '\n';
		cout << "use itemname" << '\n';		
		cout << "equip itemname" << '\n';
		cout << "drop itemname" << '\n';
		cout << "loot" << '\n';
		cout << "check skills" << '\n';
		cout << "go to store" << '\n';
		}
bool Player::checkhit(){
	return rand() %100 < accuracy;}
bool Player::checkcrit(){
	return rand() %100 < crit;}
// Quest Orkish Sword
bool Player::quest1(int counts){
	bool questcomplete = false; 
	int flag = 0;
	int flag2 = 0;
	char action;
	if(counts == 1 && !quest1triggered){
	quest1triggered = true;
	static char action;
	cout << "As you step into the shop, the store owner looks up from behind the counter and smiles warmly. 'Ah, " << playername << " it's been a while since you've passed through,' he says, his eyes lighting up. 'I was beginning to think you'd forgotten about me and my wares.'" << '\n';
	cout << "-'Actually, I’ve got a job for you, if you’re interested.'It’s nothing too dangerous, but it could be worth your time..." << '\n';
	delay(6000);
	cout << "-'Im sure you have heard of the increasing amount of those greenskins around Dunmar.'" << '\n';
	cout << "-'Unfortunate as it may be it presents a fine opportunity. For years ive been in search for one of their swords, it we'll be a fine addition to my collection and will surely bring more fame to the best store in town.'" << '\n';
	delay(4000);
	cout << "-'Well finally that moment has come! You are a pretty resourceful person, could you retrieve for me one orkish sword?'" << '\n';
	cout << "-'I would pay you handsomely of course!, lets say... 150 gold for your troubles.'" << '\n';
	delay(2000);
	cout << "He looks at you, grinning expectantly" << '\n';
	cout << "What will you do?" << '\n';
	cout << "(a) accept." << '\n';
	cout << "(b) refuse." << '\n';
		while(action != 'a' && action != 'b'){
			cin >> action;}
		if(action == 'a'){
			cout << "You accept."<< '\n';
			cout << "-'Great!, well then ill be waiting.'" << '\n';
			cout << "-'Now matters aside what have you come here for?'" << '\n';
			return questcomplete;
		}
		else{ cout << "You refuse." << '\n';
		cout << "-'Really?, fine no worries i will find someone else for this task.' He says clearly upset." << '\n'; }
	}
	else{
		if(flag2 == 0){
		cout << "-'" << playername << " just the one i wanted to see!'" << '\n';
		cout << "-'Have you acquired what i asked?'" << '\n';
		delay(500);
		for(int i = 0; i < 10; i++){
			if(inventory[i]!= nullptr && inventory[i]->getItemName() == "orkish sword" && flag == 0){
				cout << "-'Seems you have one!'" << '\n';
				cout << "-'Is it for me?'" << '\n';
				cout << "(y) Yes. " << '\n';
				cout << "(n) No." << '\n';
				while(action != 'y' && action != 'n'){
				cin >> action;}
				if(action == 'y'){
				questcomplete = true;
				cout << "-'Splendid! i knew i could count on you.'" << '\n';
				cout << "-'Let me get that from you and here is your due reward.'" << '\n';
				delete inventory[i];
				inventory[i] = nullptr;
				flag = 1;
				cout << "Item eliminated from inventory." << '\n';
				playergold = playergold + 150;
				cout << "150 gold added" << '\n';
				xp = xp + 200;
				cout << "200 xp earned" << '\n';
				checknewlevel(level, xp, calculateNextLevelXP(level));
				cout << "-'Now, are you interested in anything else?'" << '\n';
				flag2 = 0;
				return questcomplete;

				}
			else{cout << "-'Oh, ok ill keep waiting then." << '\n';
		    		cout << "For what have you come then?" << '\n';
		    		flag = 1;
		    		return questcomplete;
		    }	
				}
			}
		if(flag == 0){ cout << "You havent found it yet?, shame." << '\n';
			       cout << "For what have you come then?" << '\n';
		}
		}
 return questcomplete;
}
}
bool Player::killshopkeeper(){
 string description;
 int questionsAsked = 0;  // Track how many questions the player asks
   static bool alive = true;
    char choice;
    int pos = -1;
    // Remind the player of the previous encounters
    string memory = "As you stand in the shop, you can’t help but remember the wounded man lying on the ground, his cryptic words still echoing in your mind. You recall the quicksand, the feeling of sinking, and the urgency to escape.\n";
    typeWriterEffect(memory);

    // Ask the player if they want to start the conversation
    cout << "The shopkeeper looks at you, his shady demeanor ever present. 'What can I do for you?' he asks, cleaning a glass bottle behind the counter.\n";
    cout << "Do you want to ask him something? (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        while (questionsAsked < 4) {
            cout << "\nWhat will you ask? (choose one of the following):\n";
            cout << "(a) Why is there so much sand here?\n";
            cout << "(b) Where do you get your goods?\n";
            cout << "(c) Do many people pass through here?\n";
            cout << "(d) Have you not had any problem with greenskins?\n";
            cout << "(e) No more questions.\n";
            cin >> choice;

            if (choice == 'a') {
                cout << "'Ah, yes... the sand... it's an odd thing. It comes and goes, as if the winds themselves bring it in from the beach. Not sure why, but it’s a common problem around here. Just part of living in this area,' the shopkeeper says, looking out the window, clearly lost in thought.\n";
                questionsAsked++;
            }
            else if (choice == 'b') {
                cout << "'My goods? I get my goods from travelers, mostly,' he replies with a shrug. 'Some of them have quite the stories to tell. Occasionally, I'll trade with local merchants, but it's not often.'\n";
                questionsAsked++;
            }
            else if (choice == 'c') {
                cout << "'Not as many as you might think,' the shopkeeper says with a faint chuckle. 'This place doesn’t see much foot traffic. But when people do come by, they’ve usually got something to sell or trade.'\n";
                questionsAsked++;
            }
            else if (choice == 'd') {
                cout << "'Greenskins, huh?' He pauses, then shakes his head. 'I’ve had my share of run-ins with them in the past. Lately, though, they’ve been keeping their distance. Perhaps they’ve found other places to haunt.'\n";
                questionsAsked++;
            }
            else if (choice == 'e') {
                cout << "You decide not to ask any more questions. The shopkeeper nods, sensing your decision.\n";
                break;
            }
            else {
                cout << "Invalid option. Please choose a valid question.\n";
            }

            // If the player asks more than 2 questions, they are given the option to kill the shopkeeper
            if (questionsAsked >= 3) {
                cout << "\nYou've asked enough questions. The shopkeeper seems a bit uneasy. He looks at you warily and says, 'I’ve answered your questions, friend. What more do you seek?'\n";
                cout << "Do you want to end this conversation... or do something more? (k) Kill the shopkeeper, (q) end the conversation ";
                cin >> choice;

                if (choice == 'k' || choice == 'K') {
                    string description =  "You draw your weapon and approach the shopkeeper, who tries to back away. -'I know what you did' you say face filled with disgust. In one swift motion, you end his life. His body crumples to the ground, and the shop falls silent.\n"
                     "You search through his wares, most are dirty with sand. Probably of someone who got trapped in the quicksand. "
		     "You step behind the counter and into a small, dimly lit room. The air is thick with dust and the smell of decay. In the corner, two corpses lie in an unsettling stillness—one almost naked, the other dressed in fancy clothes, both covered in sand. As you approach, your eyes catch something glinting in the corner: a small pile of gold coins and a unique weapon, its intricate design catching the faint light from the room.\n"
		     "You grab it.\n";
		    typeWriterEffect(description);
		    delay(1000);
		    cout << "200 gold added to your inventory" << '\n';
		    playergold += 200;
		    cout << "150 xp earned" << '\n'; 
		    xp += 150;
		    unlockSkill(level);
		    pos = freeslot();
		    if(pos != -1){
		    	switch(playersrole){
				case 'w':
					inventory[pos] = storeweapon[4];
					break;
				case 'h':
					inventory[pos] = storeweapon[6];
					break;
				case 'm':
					inventory[pos] = storeweapon[14];
					break;
				}
					
		    cout << "Item added to inventory" << '\n';
		    cout << "As you leave the store you grab lantern and throw it to the wooden house." << '\n';
		    cout << "You watch it burn as you mourn the dead, feeling lighter now that justice has been done." << '\n';
	            alive = false;
					}
                    break;
				}
                else if (choice == 'q' || choice == 'Q') {
                    cout << "You turn away and look whats in store, the shopkeeper watches you, his face unreadable.\n";
                    break;
                }
                else {
                    cout << "Invalid option. Choose again.\n";
                }
            }
        
	 else {
        cout << "You decide to remain silent. Looking through the shop without asking any questions.\n";
		
    }
}
}
return alive;
}
void Player::sell(string itemname){
	if(itemname != "-1"){
		int i = 0;
		int flag = 0;
		while( i < 9 && flag == 0){
			if(inventory[i] != nullptr && inventory[i]->getItemName() == itemname){
				cout << "item sold!" << '\n';
				flag = 1;
				setPlayerGold(inventory[i]->getItemValue());
				inventory[i] = nullptr;
				cout << "your current amount of gold: " << getPlayerGold() << '\n';
				}
		i += 1;
		}
		if(flag == 0){cout << "item not found in inventory" << '\n';}
		}
	}
void Player::buy(string itemname) {
    char answer;
    int posd = -1;  // index of item in displayitems[]
    int posi;       // index of item in inventory[]
    int flag = 0;
    cout << "Your gold: " << playergold << '\n';
    // remove leading and trailing spaces from item name
    itemname.erase(0, itemname.find_first_not_of(" \t"));
    itemname.erase(itemname.find_last_not_of(" \t") + 1);

    // find the item in the store
    for (int i = 0; i < 7; i++) {
        if (displayitems[i] != nullptr && itemname == displayitems[i]->getItemName()) {
            flag = 1;
            posd = i;
            break;
        }
    }

  //  cout << "DEBUG: INSIDE BUY FUNCTION, CHECKED IF ITEM IN STORE" << '\n';
    // if item is not found, return
    if (flag == 0 || posd == -1 || displayitems[posd] == nullptr) {
        cout << "item not found." << '\n';
	//cout << "DEBUG: LEAVING BUY FUNCTION" << '\n';
        return;
    }
    else{
   // cout << "ENTERED BUYING PROCESS" << '\n';
    // show item info
    cout << displayitems[posd]->getItemDesc() << '\n';
    cout << "value: " << displayitems[posd]->getItemValue() << " coins\n";
    cout << "weight: " << displayitems[posd]->getItemWeight() << " kg\n";

    switch (displayitems[posd]->getItemCode()) {
        case 'w': {
            Weapon* w = static_cast<Weapon*>(displayitems[posd]);
            cout << "atk bonus: " << w->getAtkBonus() << " points\n";
            cout << "stamina bonus: " << w->getStaminaBonus() << " points\n";
            cout << "mana bonus: " << w->getManaB() << " points\n";
            break;
        }
        case 'a': {
            Armor* a = static_cast<Armor*>(displayitems[posd]);
            cout << "def bonus: " << a->getDefBonus() << " points\n";
            cout << "hp bonus: " << a->getHpa() << " points\n";
            break;
        }
        case 'p': {
            Potion* p = static_cast<Potion*>(displayitems[posd]);
            cout << "hp points: " << p->getHpBonus() << '\n';
            cout << "mana points: " << p->getManaBonus() << '\n';
	    cout << "stamina points: " << p->getStaminaBonus() << '\n';

            break;
        }
		}
    // ask for confirmation
    cout << "buy item? y/n\n";
    cin >> answer;
    if (answer == 'y') {
        if (getPlayerGold() >= displayitems[posd]->getItemValue()) {
            posi = freeslot();
            if (posi != -1) {
                // create a copy of the item in inventory
                if (displayitems[posd]->getItemCode() == 'w') {
                    inventory[posi] = new Weapon(*static_cast<Weapon*>(displayitems[posd]));
                } else if (displayitems[posd]->getItemCode() == 'a') {
                    inventory[posi] = new Armor(*static_cast<Armor*>(displayitems[posd]));
                } else if (displayitems[posd]->getItemCode() == 'p') {
                    inventory[posi] = new Potion(*static_cast<Potion*>(displayitems[posd]));
                }
                setPlayerGold(-(displayitems[posd]->getItemValue()));
                cout << "item added to inventory.\n";
		delete displayitems[posd];
		displayitems[posd] = nullptr;
            } else {
                cout << "-ahh..seems you dont have enough space in you bags." << '\n'; 
            }
        } else {
            cout << "you don't have enough gold!\n";
        }
    }
	}	
}

void Player::gostore(int playerlocation) {
    static bool alive = true;
    static int cstore1 = 0;
    static int cstore2 = 0;
    string itemname;
    int option = 0;
    // reset store before adding new items
    for (int i = 0; i < 7; i++) {
        if (displayitems[i] != nullptr) {
            delete displayitems[i];
            displayitems[i] = nullptr;
        }
    }

    // fill store with new random items
    for (int i = 0; i < 2; i++) {
        int number0 = rand() % 15;
        if (storeweapon[number0] != nullptr) {
            displayitems[i] = new Weapon(*storeweapon[number0]);
        }
    }

    for (int i = 2; i < 4; i++) {
        int number1 = rand() % 27;
        if (storearmor[number1] != nullptr) {
            displayitems[i] = new Armor(*storearmor[number1]);
        }
    }

    for (int i = 4; i < 7; i++) {
        int number2 = rand() % 9;
        if (storepotion[number2] != nullptr) {
            displayitems[i] = new Potion(*storepotion[number2]);
        }
    }
string encounter;
static bool quest1done = false;
    // store interaction
    if (playerlocation == 0 || playerlocation == -2) {	
        cout << "you enter the store.\n";
    	if(playerlocation == 0){
		cstore1 += 1;
		if(quest1done == false){
		quest1done = quest1(cstore1);
			}
		}
   	else{cstore2 += 1;
		if(playerlocation == -2 && cstore2 == 1){
		string encounter = "You step into a small wooden house, its creaky floorboards groaning underfoot. The air smells of aged wood and leather, and the shelves around the room are stacked with various items, armor, clothes, weapons, and tool kits fill every nook. As your eyes wander, you notice something odd: despite the building being tightly closed off from the outside, there's sand scattered across the floor, as if the desert itself has found its way inside. The strange combination of the store's goods and the sand gives the place an almost eerie feel, like it’s been untouched by time yet still carries the marks of distant lands. "			
				   "Behind the counter stands the shopkeeper, a figure with a shady look, his eyes narrow and calculating as he watches you step in. His unsettling gaze makes you pause for a moment, but you quickly dismiss it, perhaps it’s just the dim lighting and dusty atmosphere of the place. With a low, gravelly voice, he greets you, -'Welcome to my humble shop. Take your time, I’m sure you’ll find something... interesting.'\n";
		typeWriterEffect(encounter);}
		else if(cstore2 > 1 && alive && loccount[detloccode(7)] >= 1){
		alive = killshopkeeper();
			return;}
	        else if(!alive){
			cout << "You enter the burned shop. Nothing remains but ash. you leave" << '\n';
			return;
			}
		}
while (option != 3) {
        cout << "1. buy items\n";
	cout << "2. sell items\n";
        cout << "3. leave\n";
        cout << "select number: ";
        // Read menu option and clear buffer
        if (!(cin >> option)) {
            cin.clear(); // Clear error flags if non-integer entered
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue; // Skip to next iteration
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear leftover newline

        switch (option) {
            case 1: {
                cout << "write the item's name to inspect and buy (-1 to cancel)\n";
                    // display available items
                    for (int i = 0; i < 7; i++) {
                        if (displayitems[i] != nullptr) {
                            cout << i << ". " << displayitems[i]->getItemName() << '\n';
                        }
                    }
	  bool buying = true;
    	  while (buying) {
//		cout << "DEBUG: INSIDE WHILE BUYING LOOP" << '\n';
        	cout << "enter item name (-1 to cancel): ";
        	getline(cin, itemname);

        	// Trim whitespace
        	itemname.erase(0, itemname.find_first_not_of(" \t"));
        	itemname.erase(itemname.find_last_not_of(" \t") + 1);
	
        	if (itemname == "-1") {
        	    buying = false; // Exit buy menu
        	}
        	else if (!itemname.empty()) {
        	    buy(itemname);
           	 buying = false; // Exit after purchase attempt (success or failure)
//		cout << "DEBUG: LEAVING WHILE BUYING LOOP " << '\n'; 
        	}
    	}
    	break;
	}
                case 2: {
                    cout << "write the item's name you wish to sell (-1 to cancel)\n";
                    checkinventory();
                    getline(cin, itemname);
                    if (!itemname.empty() && itemname != "-1") {
                        sell(itemname);
                    }
                    break;
                }

                case 3:
                    cout << "you say goodbye and leave.\n";
                    break;

                default:
                    cout << "invalid option. try again.\n";
            }
        }
    } else {
        cout << "there is no store here.\n";
    }

    // clean up store inventory after leaving
    for (int i = 0; i < 7; i++) {
        if (displayitems[i] != nullptr) {
            delete displayitems[i];
            displayitems[i] = nullptr;
        }
    }
}

void Player::loot(int playerlocation) {
    int number;
    int pos;
    int itemindices[4];  // assuming max 4 items per location
    int itemcount = 0;
    // iterate through all rows for the given location
    for (int i = 0; i < 4; i++) {
        if (Ground::matrix[i][playerlocation] != nullptr) {
            cout << itemcount << ". " << Ground::matrix[i][playerlocation]->getItemName() << '\n';
            itemindices[itemcount] = i;  // store the row index of the item
            itemcount++;
        }
    }
    if (itemcount == 0) {
        cout << "no items to loot" << '\n';
        return;
    }
    cout << "select item number to loot" << '\n';
    cout << "(-1) to cancel" << '\n';
    while (!(cin >> number) || number < -1 || number >= itemcount) {
        cin.clear();  // clear the error state
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // discard invalid input
        cout << "invalid input. please enter a number between 0 and " << itemcount - 1 << " or -1 to cancel." << '\n';
    }
    if (number == -1) {
        return;  // cancel looting
    }
    if (number >= 0 && number < itemcount) {
        int row = itemindices[number];  // get the actual row index from the itemindices array
        if (Ground::matrix[row][playerlocation] != nullptr) {
            pos = freeslot();
            if (pos == -1) {
                cout << "no free inventory slots" << '\n';
            } else {
	if (Ground::matrix[row][playerlocation] != nullptr) {
    		Item* copied = nullptr;
   	 // Detectá dinámicamente el tipo del item
    	if (dynamic_cast<Weapon*>(Ground::matrix[row][playerlocation])) {
     	   	copied = new Weapon(*dynamic_cast<Weapon*>(Ground::matrix[row][playerlocation]));
    	} else if (dynamic_cast<Armor*>(Ground::matrix[row][playerlocation])) {
        	copied = new Armor(*dynamic_cast<Armor*>(Ground::matrix[row][playerlocation]));
    	} else if (dynamic_cast<Potion*>(Ground::matrix[row][playerlocation])) {
        	copied = new Potion(*dynamic_cast<Potion*>(Ground::matrix[row][playerlocation]));
    }

    if (copied != nullptr) {
        setInventory(pos, copied);
                Ground::matrix[row][playerlocation] = nullptr;  // remove the item from the ground
                cout << "item added to inventory" << '\n';
            }
         else {
            cout << "there's no item to loot" << '\n';
        }
    } else {
        cout << "invalid selection" << '\n';
    }
}
}
	}
}
void Player::drop(string name) {
    int flag = 0;
    for (int i = 0; i < inventoryspace; i++) {
        if (inventory[i] != nullptr && name == inventory[i]->getItemName() && flag == 0) {
            flag = 1;
           // cout << "debug: found item to drop: " << inventory[i]->getItemName() << '\n';
           // cout << "debug: dropping " << inventory[i]->getItemName() << " at location " << getplayerloc() << '\n';
            // si el item estaba equipado, lo quitamos
            if (equippedweapon == inventory[i]){ 
                    if (equippedweapon != nullptr) {
                        updatestats(0, -(static_cast<Weapon*>(equippedweapon)->getAtkBonus()),
                                   -(static_cast<Weapon*>(equippedweapon)->getStaminaBonus()),
                                   -(static_cast<Weapon*>(equippedweapon)->getManaB()), 0, 0);
			equippedweapon = nullptr;}
					}
	    else if (equippedhead == inventory[i]){
                updatestats(-(static_cast<Armor*>(equippedhead)->getHpa()), 0, 0, 0, -(static_cast<Armor*>(equippedhead)->getDefBonus()), 0);
		equippedhead = nullptr;	
	 	}	
	    else if (equippedvest == inventory[i]){ 
                        updatestats(-(static_cast<Armor*>(equippedvest)->getHpa()), 0, 0, 0, -(static_cast<Armor*>(equippedvest)->getDefBonus()), 0);
			equippedvest = nullptr;}
	    else if(equippedleg == inventory[i]){ 
                        updatestats(-(static_cast<Armor*>(equippedleg)->getHpa()), 0, 0, 0, -(static_cast<Armor*>(equippedleg)->getDefBonus()), 0);				
			equippedleg = nullptr;}
            // verificar que `insertmatrix` recibe un item válido
           // if (inventory[i] == nullptr) {
             //   cout << "error: trying to insert a nullptr item into insertmatrix()!\n";
               // return;
            // }
            Ground::insertmatrix(getPlayerLoc(), inventory[i]);
            if (Ground::matrix[(Ground::front[getPlayerLoc()] - 1 + 4) % 4][getPlayerLoc()] == inventory[i]) {
             //   cout << "debug: confirmed " << inventory[i]->getItemName() << " was stored correctly.\n";
                inventory[i] = nullptr;  // ✅ solo borramos si el item fue guardado correctamente
            } //else {
               // cout << "error: insertmatrix() failed, item not removed from inventory.\n";
            //}
            cout << "item dropped\n";
        }
    }
    if (flag == 0) {
        cout << "item not found in inventory.\n";
    }
}
void Player::equip(string name) {
    int flag = 0;
    for (int i = 0; i < inventoryspace; i++) {
        if (inventory[i] != nullptr && name == inventory[i]->getItemName() && flag == 0) {
            flag = 1;
            if (inventory[i]->getItemCode() == 'w') {  // equipping weapons
                if (static_cast<Weapon*>(inventory[i])->getClassCode() == getPlayersRole()) {
                    if (equippedweapon != nullptr) {
                        updatestats(0, -(static_cast<Weapon*>(equippedweapon)->getAtkBonus()),
                                   -(static_cast<Weapon*>(equippedweapon)->getStaminaBonus()),
                                   -(static_cast<Weapon*>(equippedweapon)->getManaB()), 0, 0);
                    }
                    updatestats(0, static_cast<Weapon*>(inventory[i])->getAtkBonus(),
                               static_cast<Weapon*>(inventory[i])->getStaminaBonus(),
                               static_cast<Weapon*>(inventory[i])->getManaB(), 0, 0);
                    equippedweapon = static_cast<Weapon*>(inventory[i]);
                    cout << "weapon equipped" << '\n';
		    //cout << "DEBUG: WEAPONEQUIPPED: " << getEquippedWeapon()->getItemName() << '\n';
                } else {
                    cout << "cannot equip this weapon" << '\n';
                }
            } else if (inventory[i]->getItemCode() == 'a') {  // equipping armor
                Armor* newarmor = static_cast<Armor*>(inventory[i]);

                // check if it's a helmet
                if (newarmor->getArmorType() == "head" && newarmor->getClassCode() == playersrole) {
                    if (equippedhead != nullptr) {
                        updatestats(-(static_cast<Armor*>(equippedhead)->getHpa()), 0, 0, 0, -(static_cast<Armor*>(equippedhead)->getDefBonus()), 0);
                    }
                    updatestats(newarmor->getHpa(), 0, 0, 0, newarmor->getDefBonus(), 0);
                    equippedhead = newarmor;
                    cout << "helmet equipped" << '\n';
                }

                // check if it's chest armor
                else if (newarmor->getArmorType() == "vest" && newarmor->getClassCode() == playersrole) {
                    if (equippedvest != nullptr) {
                        updatestats(-(static_cast<Armor*>(equippedvest)->getHpa()), 0, 0, 0, -(static_cast<Armor*>(equippedvest)->getDefBonus()), 0);
                    }
                    updatestats(newarmor->getHpa(), 0, 0, 0, newarmor->getDefBonus(), 0);
                    equippedvest = newarmor;
                    cout << "chest armor equipped" << '\n';
                }

                // check if it's leg armor
                else if (newarmor->getArmorType() == "leg" && newarmor->getClassCode() == playersrole) {
                    if (equippedleg != nullptr) {
                        updatestats(-(static_cast<Armor*>(equippedleg)->getHpa()), 0, 0, 0, -(static_cast<Armor*>(equippedleg)->getDefBonus()), 0);
                    }
                    updatestats(newarmor->getHpa(), 0, 0, 0, newarmor->getDefBonus(), 0);
                    equippedleg = newarmor;
                    cout << "leg armor equipped" << '\n';
                }

                else {
                    cout << "unknown armor type!" << '\n';
                }
            } else {
                cout << "item not equipable or your class cannot equip this item" << '\n';
            }
        }
    }
    if (flag == 0) {
        cout << "item not found" << '\n';
    }
}
void Player::use(string name){
	int flag = 0;
	// flag to controll that if the player has the same item two times, the function wont use in the two of them.
	for (int i = 0; i < inventoryspace; i++){
		if (inventory[i] != nullptr && name == inventory[i]->getItemName() && flag == 0){
			flag = 1;
			if(inventory[i]->getItemCode() == 'p'){
				hp += static_cast<Potion*>(inventory[i])->getHpBonus();
				mana += static_cast<Potion*>(inventory[i])->getManaBonus();
				stamina += static_cast<Potion*>(inventory[i])->getStaminaBonus();
				if(maxhp < hp){hp = maxhp;}
				if(maxmana < mana){mana = maxmana;}
				if(maxstamina < stamina){stamina = maxstamina;}
				delete inventory[i];
				inventory[i] = nullptr;
				cout << "you drink the potion" << '\n';
			}
			else{cout << "use on what?" << '\n';}
		}
		}
	if(flag == 0){cout << "item not found in inventory" << '\n';}
	}

void Player::inspectitem(string name) {
    int flag = 0;
    for (int i = 0; i < inventoryspace; i++) {
        if (inventory[i] != nullptr && name == inventory[i]->getItemName() && flag == 0) {
            cout << inventory[i]->getItemDesc() << '\n';
            cout << "value: " << inventory[i]->getItemValue() << " coins" << '\n';
            cout << "weight: " << inventory[i]->getItemWeight() << " kg" << '\n';
            flag = 1;

            switch (inventory[i]->getItemCode()) {
                case 'w':  // weapon
                    cout << "atk bonus: " << static_cast<Weapon*>(inventory[i])->getAtkBonus() << " points" << '\n';
		    cout << "stamina bonus: " << static_cast<Weapon*>(inventory[i])->getStaminaBonus() << " points" << '\n';
		    cout << "mana bonus: " << static_cast<Weapon*>(inventory[i])->getManaB() << " points" << '\n';
                    break;
                case 'a':  // armor
                    cout << "def bonus: " << static_cast<Armor*>(inventory[i])->getDefBonus() << " points" << '\n';
		    cout << "hp bonus: " << static_cast<Armor*>(inventory[i])->getHpa() << " points" << '\n';
                    break;
                case 'p':  // potion
                    cout << "hp points: " << static_cast<Potion*>(inventory[i])->getHpBonus() << '\n';
                    cout << "mana points: " << static_cast<Potion*>(inventory[i])->getManaBonus() << '\n';
		    cout << "stamina points: " << static_cast<Potion*>(inventory[i])->getStaminaBonus() << '\n';
                    break;
            }
		}
	}
    if (flag == 0) {
        cout << "item not found in inventory" << '\n';}
}

		 void Player::checkstats(){
			cout << "health: " << hp << "/" << maxhp << '\n';
			cout << "stamina: " << stamina << "/" << maxstamina <<'\n';
			cout << "mana: " << mana << "/" << maxmana << '\n';
			cout << "defense: " << def << '\n'; 
			cout << "attack: " << atk << '\n';
			// total weight
			int totalweight = 0;
			for(int i = 0; i < 9; i++){
				if(inventory[i] != nullptr){
					totalweight += inventory[i]->getItemWeight();
				}
			}
			cout << "total weight: " << totalweight << " kg" << '\n';
			}
		
        	void Player::checkinventory(){
			cout << "gold: " << getPlayerGold() << '\n';
                	for (int i = 0; i < inventoryspace; i++){
				if(inventory[i] != nullptr){
                	cout << inventory[i]->getItemName() << '\n';}
			else{cout << "empty slot" << '\n';}}
		}

		void Player::checksurrounds(int playerlocation){
			int lcode;
			lcode = detloccode(playerlocation);
			cout << Location::getLocationsDesc(lcode) << '\n';} 
	//movement system
	bool Player::availablemove(int playerlocation){
			int flag = 0;
			for(int i = 0; i < 9; i++){
				if(playerlocation == Location::getLocations(i)){
					flag = 1;}	
				}
			if(flag == 1){
				return true;
			}
			else{return false;}
			}

int Player::movenorth(int &playerlocation){
    if (playerlocation == -1) {
        playerlocation += 8;
        updateloccount(playerlocation);
	cout << "you head north" << '\n';
    }
    else {
        if(availablemove(playerlocation + 1)){
            playerlocation += 1;
            updateloccount(playerlocation);
            cout << "you head north" << '\n';} 
	else { 
            cout << "cant go that way!" << '\n';
        }
    }
    return playerlocation;}

int Player::movewest(int &playerlocation){
		if(playerlocation == 4){cout << "cant go that way!" << '\n';}
		else if(availablemove(playerlocation +3)){
			playerlocation += 3;
			updateloccount(playerlocation);
			cout << "you head west" << '\n';}
		else{cout << "cant go that way!" << '\n';}
	return playerlocation;}
int Player::moveeast(int &playerlocation){
		if (playerlocation == 7){ cout << "cant go that way!" << '\n';}
		else if(availablemove(playerlocation -3)){
			playerlocation += -3;
			updateloccount(playerlocation);
			cout << "you head east" << '\n';}
		else{cout << "cant go that way!" << '\n';}
	return playerlocation;}

int Player::movesouth(int &playerlocation){
    if (playerlocation == 7) {
        playerlocation -= 8;
        updateloccount(playerlocation);
	cout << "you head south" << '\n';}
    else if(playerlocation == 0){ cout << "cant go that way!" << '\n';}
	else {
        if(availablemove(playerlocation - 1)){
            playerlocation -= 1;
            updateloccount(playerlocation);
            cout << "you head south" << '\n';} 
	else { 
            cout << "cant go that way!" << '\n';
        }
    }
    return playerlocation;
}

void Player::updatestats(int earnhp, int earnatk, int earnstamina, int earnmana, int earndef, int earnxp){
    maxhp += earnhp;  
    hp += earnhp;
    atk += earnatk;
    maxatk += earnatk;
    maxstamina += earnstamina;
    stamina += earnstamina;
    maxmana += earnmana;
    mana += earnmana;
    def += earndef;
    maxdef += earndef;
    xp += earnxp;
}

void Player::updateloccount(int playerlocation){  
    int lcode = detloccode(playerlocation); 
    loccount[lcode] += 1;
}


