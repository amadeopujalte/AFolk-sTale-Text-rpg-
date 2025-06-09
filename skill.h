#ifndef SKILL_H
#define SKILL_H

#include <string>
#include <iostream>
class Player;
class Enemy;
using namespace std;
class Skill{
	private:
		int dmg;
		int stmcost;
		int mcost;
		string skillname;
		string skilldesc;
		string skillcastdesc;
		char skillclass;
		int skillevel;
	public:
		 string basicAttackFinishers[14];
		Skill(string name,string desc,string castdesc,int dmg,int stamina,int mana,int level,char skillclass);
		~Skill();
		string getSkillName();
		string getSkillDesc();
		string getSkillCastDesc();
		int getSkillevel();
		int getSkillDmg();
		int getSkillStmCost();
		int getSkillMcost();
		char getSkillClass();
		void playerDamageCalc(Player& player, Enemy* enemy, int dmg);
		void enemyDamageCalc(Player& player, Enemy* enemy, int dmg);
		void playerBasicAttack(Player& player, string weapon, Enemy& enemy);
		void enemyBasicAttack(Enemy* enemy, Player& player);

				
		};
class PlayerSkill : public Skill {
public:
	PlayerSkill(string name,string desc,string castdesc,int dmg,int stamina,int mana,int level,char skillclass);
	virtual void playerUseSkill(Player& player, Enemy* enemy) = 0;
	//Pure virtual function
};
class EnemySkill : public Skill {
public:
	EnemySkill(string name,string desc,string castdesc,int dmg,int stamina,int mana,int level,char skillclass);
	virtual void enemyUseSkill(Player& player, Enemy* enemy) = 0;
};
class Fireball : public PlayerSkill {
public:
	Fireball();
	void playerUseSkill(Player& player, Enemy* enemy) override;
	};
class IceSpike : public PlayerSkill {
public:
	IceSpike();
	void playerUseSkill(Player& player, Enemy* enemy) override;
	};
class ArcaneBarrier : public PlayerSkill {
public:
    ArcaneBarrier();
    void playerUseSkill(Player& player, Enemy* enemy) override;
};
class ArcaneDischarge : public PlayerSkill {
public:	
	ArcaneDischarge();
	void playerUseSkill(Player& player, Enemy* enemy) override;
};

class Kamehameha : public PlayerSkill {
public:
    Kamehameha();
    void playerUseSkill(Player& player, Enemy* enemy) override;
};

class TripleShot : public PlayerSkill {
public:
    TripleShot();
    void playerUseSkill(Player& player, Enemy* enemy) override;
};

class PreciseShot : public PlayerSkill {
public:	
    PreciseShot();
    void playerUseSkill(Player& player, Enemy* enemy) override;
};

class CrimsonStrike : public PlayerSkill {
public:	
    CrimsonStrike();
    void playerUseSkill(Player& player, Enemy* enemy) override;
};

class Evasion : public PlayerSkill {
public:	
    Evasion();
    void playerUseSkill(Player& player, Enemy* enemy) override;
};

class DeathsGamble : public PlayerSkill {
public:	
    DeathsGamble();
    void playerUseSkill(Player& player, Enemy* enemy) override;
};

class BrutalStrike : public PlayerSkill {
public:	
    BrutalStrike();
    void playerUseSkill(Player& player, Enemy* enemy) override;
};

class DefensiveStance : public PlayerSkill {
public:	
    DefensiveStance();
    void playerUseSkill(Player& player, Enemy* enemy) override;
};

class WarCry : public PlayerSkill {
public:	
    WarCry();
    void playerUseSkill(Player& player, Enemy* enemy) override;
};

class CripplingBlow : public PlayerSkill {
public:	
    CripplingBlow();
    void playerUseSkill(Player& player, Enemy* enemy) override;
};

class UnbreakableWill : public PlayerSkill {
public:	
    UnbreakableWill();
    void playerUseSkill(Player& player, Enemy* enemy) override;
};
// Enemy skills
// Warrior
class ShatteringBlow : public EnemySkill {
public:	
    ShatteringBlow();
    void enemyUseSkill(Player& player, Enemy* enemy) override;
};

class RelentlessCharge : public EnemySkill {
public:
    RelentlessCharge();
    void enemyUseSkill(Player& player, Enemy* enemy) override;
};

class FlurryOfBlows : public EnemySkill {
public:	
    FlurryOfBlows();
    void enemyUseSkill(Player& player, Enemy* enemy) override;
};

class SecondWind : public EnemySkill {
public:	
    SecondWind();
    void enemyUseSkill(Player& player, Enemy* enemy) override;
};

class BleedingSlash : public EnemySkill {
public:	
    BleedingSlash();
    void enemyUseSkill(Player& player, Enemy* enemy) override;
};
//Hunter
class LurkingStrike : public EnemySkill {
public:	
    LurkingStrike();
    void enemyUseSkill(Player& player, Enemy* enemy) override;
};

class BarbedShot : public EnemySkill {
public:	
    BarbedShot();
    void enemyUseSkill(Player& player, Enemy* enemy) override;
};

class EvasiveManeuver : public EnemySkill {
public:	
    EvasiveManeuver();
    void enemyUseSkill(Player& player, Enemy* enemy) override;
};

class SmokeBomb : public EnemySkill {
public:	
    SmokeBomb();
    void enemyUseSkill(Player& player, Enemy* enemy) override;
};

class PiercingStrike : public EnemySkill {
public:	
    PiercingStrike();
    void enemyUseSkill(Player& player, Enemy* enemy) override;
};
//Mage
class Drain : public EnemySkill {
public:
    Drain();
    void enemyUseSkill(Player& player, Enemy* enemy) override;
};

class ArcaneTrap : public EnemySkill {
public:	
    ArcaneTrap();
    void enemyUseSkill(Player& player, Enemy* enemy) override;
};

class ArcaneBurst : public EnemySkill {
public:
    ArcaneBurst();
    void enemyUseSkill(Player& player, Enemy* enemy) override;
};

class WarpingCurse : public EnemySkill {
public:	
    WarpingCurse();
    void enemyUseSkill(Player& player, Enemy* enemy) override;
};
class ArcaneDetonation : public EnemySkill {
public:
	ArcaneDetonation();
	void enemyUseSkill(Player& player, Enemy* enemy) override;
};
#endif 
