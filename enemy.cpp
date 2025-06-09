#include <iostream> 
#include <string>
#include <cstdlib>
#include <ctime>
#include "skill.h"
#include "player.h"
#include "enemy.h"
using namespace std;  
//Constructor Definition
Enemy::Enemy(string name, string desc, int maxhp, int hp, int atk,int maxatk, int def,int maxdef, 
             int maxstamina, int stamina, int maxmana, int mana, char role, int xp,int level)
    : enemyname(name), enemydesc(desc), maxhp(maxhp), hp(hp), atk(atk),maxatk(maxatk), def(def), maxdef(maxdef),
      maxstamina(maxstamina), stamina(stamina), maxmana(maxmana), mana(mana), role(role), xp(xp), level(level) {}
Enemy::~Enemy(){
	//cout << "DEBUG: ENEMY DESTRUCTOR ENTERED" << '\n';
	for(int i = 0; i < 5; i++){
		if(availableEnemySkills[i] != nullptr){
			availableEnemySkills[i] = nullptr;
		}
	}
	//cout << "DEBUG: ENEMY DESTRUCTOR COMPLETED SUCCESFULLY" << '\n';
}
//Method Definitions 
string Enemy::getEnemyName() {
    return enemyname;}
string Enemy::getEnemyDesc() {
    return enemydesc;}
int& Enemy::getEnemyHp() {
    return hp;}
void Enemy::setEnemyHp(int newhp) {
    hp = newhp;}
int& Enemy::getEnemyMaxHp(){
	return maxhp;}
int& Enemy::getEnemyAtk() {
    return atk;}
int& Enemy::getEnemyStamina() {
    return stamina;}
void Enemy::setEnemyStamina(int newstamina) {
    stamina = newstamina;}
int& Enemy::getEnemyMaxStamina() {
    return maxstamina;}
int& Enemy::getEnemyMana() {
    return mana;}
void Enemy::setEnemyMana(int newmana) {
    mana = newmana;}
int& Enemy::getEnemyMaxMana() {
    return maxmana;}
int& Enemy::getEnemyDef() {
    return def;}
char& Enemy::getEnemyRole() {
    return role;}
int& Enemy::getEnemyXp() {
    return xp;}
//Combat effects
bool& Enemy::getEnemyStun(){
	return stun;}
void Enemy::setEnemyStun(bool stunstate){
	stun = stunstate;}
bool& Enemy::getEnemyBuff(){
		return buff;}
void Enemy::setEnemyBuff(bool buff){
		buff = buff;}
int& Enemy::getEnemyBuffCounter(){
		return buffcounter;}
void Enemy::setEnemyBuffCounter(int counts){
		buffcounter = counts;}
bool Enemy::getEnemyBleeding(){
	return bleeding;}
void Enemy::setEnemyBleeding(bool status){
		bleeding = status;}
int& Enemy::getEnemyBleedCounter(){
		return bleedcounter;}
void Enemy::setEnemyBleedCounter(int counts){
		bleedcounter = counts;}
bool& Enemy::getEnemyDebuff(){
		return debuff;}
void Enemy::setEnemyDebuff(bool buff){
		debuff = buff;}
int& Enemy::getEnemyDebuffCounter(){
		return debuffcounter;}
void Enemy::setEnemyDebuffCounter(int counts){
		debuffcounter = counts;}

    
//Combat functions
void Enemy::enemyBasicAttack(Player& player){
	int damage;
	int damagetaken;
	if(stamina >= 10 && mana >= 10){
		stamina += -10;
		mana += -10;
		cout << "Your opponent attacks!" << '\n';
		if(checkhit()){
				if(checkcrit()){
					cout << "Critical hit!" << '\n';
					damage = atk *2;
					damagetaken = damage - player.getPlayerDef();
					if(damagetaken <= 0){damagetaken = 0;}
					player.setPlayerHp(player.getPlayerHp() - damagetaken);}
				else{
					cout << "Hit!" << '\n';
					damage = atk;
					damagetaken = damage - player.getPlayerDef();
					if(damagetaken <= 0){damagetaken = 0;}
					player.setPlayerHp(player.getPlayerHp() - damagetaken);
				}
				cout << "Damage dealt: " << damagetaken << '\n';
	}
	
		else{cout << "Your opponent missed!" << '\n';}
	}
	else{cout << "The enemy attempts an attack, but is to tired to do it!" << '\n';}

}
	
		

void Enemy::applyEnemyBleed(){
	if(bleeding == true){
			hp = (hp - 10);
			cout << "Blood drips from the enemy's wounds." << '\n';
		}
		else{bleedcounter += -1;}
	if(bleedcounter == 0){
		cout << "Your opponent's bleeding stopped" << '\n';
		}
}
void Enemy::applyEnemyBuff(){
	int skillbuff;
	if(buff == true){
		cout << "The enemy looks stronger." << '\n';
		//Second wind 
		//+15% atk
		atk = maxatk * 1.15;
		buffcounter += -1;	
		if(buffcounter <= 0){
			buff = false;
			atk = maxatk;}
	}
}
void Enemy::applyEnemyDebuff(Player& player, string name){
	int skilldebuff;
	if(debuff == true){
	switch(player.getPlayersRole()){
		case 'w':
			//Defensive Stance
			//-25% damage taken
			skilldebuff = maxatk * 0.25;
			atk = atk - skilldebuff;
			cout << "Your enemy is having a hard time penetrating your defensive stance!" << '\n';
			break;
		case 'm':
			//Arcane Barrier
			//-30% damage taken
			skilldebuff = maxatk *0.30;
			atk = atk - skilldebuff;
			cout << "Your enemy is having a hard time penetrating your arcane barrier!" << '\n';
			break;
		case 'h':
			//Precise Shot
			// ignores 15% of armor
			if(!name.empty() && name == "Precise Shot"){
				skilldebuff = maxdef * 0.15;
				def = def - skilldebuff;
			}
			//Evasion
			//increase 35% evasion 
			else{
				skilldebuff = accuracy * 0.35;
				accuracy = accuracy - skilldebuff;
				cout << "Your enemy is having a hard time trying to hit you!" << '\n';
			}
			break;
			
		}
	debuffcounter += -1;
	if(debuffcounter <= 0){
		debuff = false;
		atk = maxatk;
		def = maxdef;
		accuracy = 85;
		}
	}
}

bool Enemy::checkhit(){
	return rand() %100 < accuracy;}
bool Enemy::checkcrit(){
	return rand() %100 < crit;}

void Enemy::skipturn() {
    stamina = (min(stamina + 15, maxstamina));
    mana = (min(mana + 15, maxmana));
    cout << enemyname << " takes a moment to recover. Stamina and mana restored slightly." << endl;
}
int Enemy::locateLastSkillPos(){
	int x = 0;
	while(availableEnemySkills[x] != nullptr && x != 5){
		x += 1;
	}
	x -= 1;
	if(x < 0){
	x = 0;}
	return x;
	}
string Enemy::enemyTurn(Player& player, Enemy* enemy) {
    int probSkillX, probSkillXMinus1, probSkillXMinus2;
    int probBasicAttack, probSkill1, probSkill0;
    string skillname; 
    int x;
    int randomnumber = rand() % 101;

    x = locateLastSkillPos(); // Determina la última habilidad disponible

    // Ajustamos probabilidades según la vida del enemigo
    if (enemy->getEnemyStamina() > enemy->getEnemyMaxStamina() * 0.2 && 
        enemy->getEnemyMana() > enemy->getEnemyMaxMana() * 0.25) {
        
        if (enemy->getEnemyHp() > enemy->getEnemyMaxHp() * 0.75) {
            probSkillX = 40; probSkillXMinus1 = 30; probSkillXMinus2 = 15;
            probBasicAttack = 15; probSkill0 = 0; probSkill1 = 0;
        }
        else if (enemy->getEnemyHp() >= enemy->getEnemyMaxHp() * 0.40) {
            probSkillX = 15; probSkillXMinus1 = 20; probSkillXMinus2 = 15;
            probSkill0 = 15; probSkill1 = 20; probBasicAttack = 15;
        }
        else {
            probSkillX = 5; probSkillXMinus1 = 15; probSkillXMinus2 = 25;
            probSkill0 = 15; probSkill1 = 20; probBasicAttack = 20;
        }

        // Intentamos ejecutar una habilidad en orden de prioridad
        if (randomnumber <= probSkillX && availableEnemySkills[x] != nullptr && x >= 0) {
            availableEnemySkills[x]->enemyUseSkill(player, enemy);
            skillname = availableEnemySkills[x]->getSkillName();
        } 
        else if (randomnumber <= probSkillXMinus1 + probSkillX && (x - 1) >= 0 && availableEnemySkills[x-1] != nullptr) {
            availableEnemySkills[x-1]->enemyUseSkill(player, enemy);
            skillname = availableEnemySkills[x-1]->getSkillName();
        } 
        else if (randomnumber <= probSkillXMinus2 + probSkillXMinus1 + probSkillX && (x - 2) >= 0 && availableEnemySkills[x-2] != nullptr) {
            availableEnemySkills[x-2]->enemyUseSkill(player, enemy);
            skillname = availableEnemySkills[x-2]->getSkillName();
        } 
        else if (randomnumber <= probBasicAttack + probSkillXMinus2 + probSkillXMinus1 + probSkillX) {
            enemyBasicAttack(player);
            skillname = "basicattack";
        } 
        else if (randomnumber <= probBasicAttack + probSkillXMinus2 + probSkillXMinus1 + probSkillX + probSkill0 && availableEnemySkills[0] != nullptr) {
            availableEnemySkills[0]->enemyUseSkill(player, enemy);
            skillname = availableEnemySkills[0]->getSkillName();
        } 
        else if (randomnumber <= probBasicAttack + probSkillXMinus2 + probSkillXMinus1 + probSkillX + probSkill0 + probSkill1 && availableEnemySkills[1] != nullptr) {
            availableEnemySkills[1]->enemyUseSkill(player, enemy);
            skillname = availableEnemySkills[1]->getSkillName();
        } 
        else {
           //cout << "DEBUG: No valid skill or attack was chosen, enemy defaults to basic attack!" << endl;
            enemyBasicAttack(player);
            skillname = "basicattack";
        }
    }
else{
	skipturn();
	skillname = "skipturn";}
    //cout << "DEBUG: Enemy action chosen -> " << skillname << '\n';
    return skillname;
}
void Enemy::resetStats(){
	hp = maxhp;
	stamina = maxstamina;
	mana = maxmana;
 	bool stun = false;
    	bool buff = false;
    	int buffcounter = 0;
    	bool bleeding = false;
    	int bleedcounter = 0;
	}
void Enemy::initializeEnemySkills(){
	for(int i = 0; i < 5; i++){
		availableEnemySkills[i] = nullptr;
	}
	switch(role){
		case 'w':
			switch(level){
				case 1:
					availableEnemySkills[0] = new ShatteringBlow();
					break;
				case 2:
					availableEnemySkills[0] = new ShatteringBlow();
					availableEnemySkills[1] = new RelentlessCharge();
					break;
				case 3:
					availableEnemySkills[0] = new ShatteringBlow();
					availableEnemySkills[1] = new RelentlessCharge();
					availableEnemySkills[2] = new FlurryOfBlows();
					break;
				case 4:
					availableEnemySkills[0] = new ShatteringBlow();
					availableEnemySkills[1] = new RelentlessCharge();
					availableEnemySkills[2] = new FlurryOfBlows();
					availableEnemySkills[3] = new SecondWind();
					break;
				case 5:
					availableEnemySkills[0] = new ShatteringBlow();
					availableEnemySkills[1] = new RelentlessCharge();
					availableEnemySkills[2] = new FlurryOfBlows();
					availableEnemySkills[3] = new SecondWind();
					availableEnemySkills[4] = new BleedingSlash();
					break;
				}
			break;
		case 'h':
				switch(level){
				case 1:
					availableEnemySkills[0] = new LurkingStrike();
					break;
				case 2:
					availableEnemySkills[0] = new LurkingStrike();
					availableEnemySkills[1] = new BarbedShot();
					break;
				case 3:
					availableEnemySkills[0] = new LurkingStrike();
					availableEnemySkills[1] = new BarbedShot();
					availableEnemySkills[2] = new EvasiveManeuver();
					break;
				case 4:
					availableEnemySkills[0] = new LurkingStrike();
					availableEnemySkills[1] = new BarbedShot();
					availableEnemySkills[2] = new EvasiveManeuver();
					availableEnemySkills[3] = new SmokeBomb();
					break;
				case 5:
					availableEnemySkills[0] = new LurkingStrike();
					availableEnemySkills[1] = new BarbedShot();
					availableEnemySkills[2] = new EvasiveManeuver();
					availableEnemySkills[3] = new SmokeBomb();
					availableEnemySkills[4] = new PiercingStrike();
					break;
				}
			break;
			
		case 'm':
				switch(level){
				case 1:
					availableEnemySkills[0] = new Drain();
					break;
				case 2:
					availableEnemySkills[0] = new Drain();
					availableEnemySkills[1] = new ArcaneTrap();
					break;
				case 3:
					availableEnemySkills[0] = new Drain();
					availableEnemySkills[1] = new ArcaneTrap();
					availableEnemySkills[2] = new ArcaneBurst();
					break;
				case 4:
					availableEnemySkills[0] = new Drain();
					availableEnemySkills[1] = new ArcaneTrap();
					availableEnemySkills[2] = new ArcaneBurst();
					availableEnemySkills[3] = new WarpingCurse();
					break;
				case 5:
					availableEnemySkills[0] = new Drain();
					availableEnemySkills[1] = new ArcaneTrap();
					availableEnemySkills[2] = new ArcaneBurst();
					availableEnemySkills[3] = new WarpingCurse();
					availableEnemySkills[4] = new ArcaneDetonation();
					break;
					}

			break;
	}

	}

Enemy* enemylist[16] = {nullptr};
void initializeenemys(){
 for (int i = 0; i < 16; i++) {
        if (enemylist[i] == nullptr) {  // Only create if it doesn't exist
            switch (i) {
                case 0: enemylist[i] = new Enemy("Goblin Grunt", "A hunched, green skinned creature with jagged teeth and filthy claws.", 100, 100, 23,23, 15,15, 170, 170, 75, 75, 'w', 60,2); break;
                case 1: enemylist[i] = new Enemy("Goblin Assassin", "A wiry goblin with sharp yellow eyes, wrapped in tattered black cloth.", 90, 90, 25,25, 10,10, 200, 200, 75, 75, 'h', 65,2); break;
                case 2: enemylist[i] = new Enemy("Goblin Shaman", "A frail goblin with cracked skin and glowing tribal markings.", 110, 110, 26,26, 7, 7, 45, 45, 140, 140, 'm',60, 2); break;
                case 3: enemylist[i] = new Enemy("Bandit Thug", "A scarred, wild-eyed brute with a permanent sneer and rough, unkempt hair.", 120, 120, 30,30, 14,14, 230,230, 100, 100, 'w', 100,3); break;
                case 4: enemylist[i] = new Enemy("Bandit Sharpshooter", "A long-haired rogue with a sharp gaze and a twisted smirk.", 110, 110, 35,35, 12,12, 250, 250, 75, 75, 'h', 90,3); break;
                case 5: enemylist[i] = new Enemy("Bandit Sorcerer", "Cloaked in a ragged mage robe, enchanted for protection.", 105, 105, 36,36, 10,10, 100,100, 200, 200, 'm', 100,3); break;
                case 6: enemylist[i] = new Enemy("City Guard", "Clad in standard-issue plate armor, built for endurance.", 140, 140, 42, 42, 18, 18, 240, 240, 110, 110, 'w', 120, 3); break;
                case 7: enemylist[i] = new Enemy("City Watchman", "Wears a scout’s reinforced tunic, perfect for agility.", 130, 130, 46, 46, 16, 16, 260, 260, 100, 100, 'h', 120, 3); break;
                case 8: enemylist[i] = new Enemy("Ork Warrior", "A hulking green brute covered in battle scars, with tusks bared in a snarl wearing thick iron plates.", 180, 180, 50, 50, 30, 30, 270, 270, 100, 100, 'w', 120, 4); break;
                case 9: enemylist[i] = new Enemy("Ork Warchief", "A towering massive ork with war paint streaked across his face, covered in reinforced battle armor.", 250, 250, 60, 60, 40, 40, 290, 290, 80, 80, 'w', 200, 4); break;
                case 10: enemylist[i] = new Enemy("Ork Hunter", "A savage Ork archer, draped in leather and bone armor.", 170, 170, 55, 55, 25, 25, 275, 275, 150,150, 'h', 120, 4); break;
                case 11: enemylist[i] = new Enemy("Golem", "A towering mass of cracked stone, ancient runes pulsing along its surface.", 400, 400, 70, 70, 60,60, 500, 500, 120, 120, 'w', 270, 5); break;
                case 12: enemylist[i] = new Enemy("Abomination", "A grotesque fusion of swollen limbs and rotting flesh, stitched together unnaturally.", 200, 200, 35, 35, 15, 15, 330, 330, 100, 100, 'w', 170, 4); break;
                case 13: enemylist[i] = new Enemy("Rogue Hunter", "Clad in dark reinforced leather, allowing unmatched speed and precision.", 300, 300, 70, 70, 40, 40, 350, 350, 250, 250, 'h', 300, 5); break;
                case 14: enemylist[i] = new Enemy("Rogue Warrior", "Wearing an unbreakable set of battle-worn plate armor, forged for war.", 350, 350, 75, 75, 55, 55, 370, 370, 140, 140, 'w', 300, 5); break;
                case 15: enemylist[i] = new Enemy("Rogue Mage", "Draped in ancient enchanted robes, radiating immense magical power.", 290, 290, 65, 65, 85, 85, 200, 200, 250, 250, 'm', 300, 5); break;
            }
        } else {
            // Reset stats instead of creating new instance
            enemylist[i]->resetStats();
        }
    }
}
void cleanenemylist(){
for(int i = 0; i < 16; i++){
	if(enemylist[i] != nullptr){
		delete enemylist[i];
		enemylist[i] = nullptr;
		}
	}
}

