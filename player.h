#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include "item.h"
#include "skill.h"
#include "enemy.h"
using namespace std;
void typeWriterEffect(const string& text, int delay_ms = 95);
void delay(int miliseconds);
class Player {
private:
    // Character stats
    	string playername;
    	char playersrole;
	int playerloc;
    	int maxhp;
    	int hp;
    	int atk;
	int maxatk;
    	int maxstamina;
    	int stamina;
    	int maxmana;
    	int mana;
   	int def;
	int maxdef;
    	int weight;
    	int xp;
    	int level = 1;
    	int inventoryspace = 10;
    	int loccount[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  	Item* inventory[10];
  	Item* equippedweapon;
    	Item* equippedhead;
    	Item* equippedvest;
    	Item* equippedleg;
    	int playergold = 150;
	bool rune;
	int accuracy = 85;
	int crit = 10;
	static Item* displayitems[7];
	//Combat effects
	bool stun = false;
	bool buff = false;
	bool debuff = false;
	int buffcounter = 0;
	int debuffcounter = 0;
	bool bleeding = false;
	int bleedcount = 0;
	bool quest1triggered = false;

public:
	PlayerSkill* availableskills[5];
	string basicAttackFinishers[14];
    // Methods
	// Player Stats
	// Getters and setters
    	string getPlayerName();
    	void setPlayerName(string name);
    	char getPlayersRole();
    	void setPlayersRole(char role);
   	int& getPlayerLoc();
	void setPlayerLoc(int loc);
    	int getLoccount(int index);
    	int& getPlayerHp();
    	void setPlayerHp(int newhp);
	int& getPlayerMaxHp();
    	int& getPlayerAtk();
    	int& getPlayerDef();
    	int& getPlayerStamina();
    	void setPlayerStamina(int newstamina);
    	int& getPlayerMaxStamina();
    	int& getPlayerMana();
    	void setPlayerMana(int newmana);
    	int& getPlayerMaxMana();
	int& getPlayerWeight();
	int& getPlayerAccuracy();
	int& getPlayerCrit();
	int& getPlayerLevel();
	int& getPlayerXp();
	bool getRune();
	void setRune(bool hasrune);
	// Status effects
	bool& getPlayerStun();
	void setPlayerStun(bool stunstate);
	bool& getPlayerBuff();
	void setPlayerBuff(bool buff);
	int& getPlayerBuffCounter();
	void setPlayerBuffCounter(int counts);
	bool& getPlayerBleeding();
	void setPlayerBleeding(bool bleeding);
	int& getPlayerBleedCounter();
	void setPlayerBleedCounter(int counts);
	bool& getPlayerDebuff();
	void setPlayerDebuff(bool debuff);
	int& getPlayerDebuffCounter();
	void setPlayerDebuffCounter(int counts);
	
	//Equipped items 
    	Item* getEquippedWeapon();
    	void setEquippedWeapon(Item* weapon);
    	Item* getEquippedArmor(string type);
    	void setEquippedArmor(string type, Item* armor);
    	void attack(string name, Enemy* enemy);
	//Skill Functions
	void unlockSkill(int level);
	int calculateNextLevelXP(int level);
	void checknewlevel(int &level, int &xp, int nextLevelXp);
	void checkskills();
	void displaycombatstats();
	//Quests
	bool quest1(int counts);
	bool killshopkeeper();
	//
	int getPlayerGold();
	void setPlayerGold(int gold);
	Item* getInventory(int index);
	void setInventory(int index, Item* object);
	int freeslot();
	int detloccode(int playerlocation);
	// Parametised constructor
	Player(int playerloc,int maxhp,int hp, int atk, int maxatk, int maxstamina, int stamina, int maxmana, int mana, int def, int maxdef, int weight, int xp,int gold);
	//Default constructor
	Player();
	//Cleaner
	//Destructor
	~Player();
	//Combat function
	int playerBasicAttack(string weapon, Enemy* enemy);
	bool checkhit();
	bool checkcrit();
	void skipturn();
	void applyPlayerBuff();
	void applyPlayerDebuff(Enemy* enemy);
	void applyPlayerBleed();
	//Inventory sistem
	void help();
	void sell(string itemname);
	void buy(string itemname);
	void gostore(int playerlocation);
	void loot(int playerlocation);
	void drop(string name);
	void equip(string name);
	void use(string name);
	void inspectitem(string name);
	void checkstats();
	void checkinventory();
	void checksurrounds(int playerlocation);
	//Movement system
	bool availablemove(int playerlocation);
	int movenorth(int &playerlocation);
	int movewest(int &playerlocation);
	int moveeast(int &playerlocation);
	int movesouth(int &playerlocation);
	void updatestats(int earnhp, int earnatk, int earnstamina, int earnmana, int earndef, int earnxp);
	void updateloccount(int playerlocation);
	};

#endif // PLAYER_H
	
	
