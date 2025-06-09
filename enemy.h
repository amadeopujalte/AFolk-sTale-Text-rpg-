#ifndef ENEMY_H
#define ENEMY_H
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "skill.h"
#include "player.h"
class Skill; //Foward Declaration
class Player; //Foward Declaration
using namespace std;  

class Enemy {
private:
    string enemyname;
    string enemydesc;
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
    char role;
    int accuracy = 85;
    int crit = 10;
    int level;
    //Combat effects
    bool stun = false;
    bool buff = false;
    bool debuff = false;
    int buffcounter = 0;
    int debuffcounter = 0;
    bool bleeding = false;
    int bleedcounter = 0;
    int xp;
public:
    EnemySkill* availableEnemySkills[5];

public:
    // Constructor declaration
    Enemy(string name, string desc, int maxhp, int hp, int atk,int maxatk, int def,int maxdef, 
          int maxstamina, int stamina, int maxmana, int mana, char role, int xp,int level);
    // Destructor declaration
     ~Enemy();
    // Getter and Setter Methods declarations
    string getEnemyName();
    string getEnemyDesc();
    int& getEnemyHp();
    int& getEnemyMaxHp();
    void setEnemyHp(int newhp);
    int& getEnemyAtk();
    int& getEnemyStamina();
    void setEnemyStamina(int newstamina);
    int& getEnemyMaxStamina();
    int& getEnemyMana();
    void setEnemyMana(int newmana);
    int& getEnemyMaxMana();
    int& getEnemyDef();
    char& getEnemyRole();
    int& getEnemyXp();
    bool checkhit();
    bool checkcrit();
    // Combat effects
    bool& getEnemyStun();
    void setEnemyStun(bool stunstate);
    bool& getEnemyBuff();
    void setEnemyBuff(bool buff);
    int& getEnemyBuffCounter();
    void setEnemyBuffCounter(int counts);
    bool getEnemyBleeding();
    void setEnemyBleeding(bool status);
    int& getEnemyBleedCounter();
    void setEnemyBleedCounter(int counts);
    bool& getEnemyDebuff();
    void setEnemyDebuff(bool status);
    int& getEnemyDebuffCounter();
    void setEnemyDebuffCounter(int counts);
	
    // Methods
	// Combat
    void enemyBasicAttack(Player& player);
    void skipturn();
    void resetStats();
    int locateLastSkillPos();
    string enemyTurn(Player& player, Enemy* enemy);
    void setEnemyDebuff();
    void setEnemyDebuffCounter();
    void applyEnemyBuff();
    void applyEnemyBleed();
    void applyEnemyDebuff(Player& player, string name);
    void initializeEnemySkills();
};
extern Enemy* enemylist[16];
void initializeenemys();
void cleanenemylist();
#endif 
