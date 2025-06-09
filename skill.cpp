#include "skill.h"
#include "player.h"
#include "enemy.h"
#include "item.h"
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>
#include <cstdlib>
using namespace std;
Skill::Skill(string name,string desc,string castdesc,int dmg,int stamina,int mana,int level,char skillclass)
		: skillname(name),skilldesc(desc),skillcastdesc(castdesc),dmg(dmg),stmcost(stamina),mcost(mana),skillevel(level),skillclass(skillclass){
}
Skill::~Skill(){
//	cout << "DEBUG: SKILL DESTRUCTOR ENTERED" << '\n';
}
string Skill::getSkillName(){
	return skillname;}
string Skill::getSkillDesc(){
	return skilldesc;}
string Skill::getSkillCastDesc(){
	return skillcastdesc;}
int Skill::getSkillevel(){
	return skillevel;}
int Skill::getSkillDmg(){
	return dmg;}
int Skill::getSkillStmCost(){
	return stmcost;}
int Skill::getSkillMcost(){
	return mcost;}
char Skill::getSkillClass(){
	return skillclass;}
void Skill::playerDamageCalc(Player& player, Enemy* enemy, int dmg){
	int damage;
	int damagetaken;
	if(player.checkhit()){
	if(player.checkcrit()){
				cout << "Critical hit!" << '\n';
				damage = (dmg + getSkillDmg()) *2;
				damagetaken = damage - enemy->getEnemyDef();
				if(damagetaken <= 0){damagetaken = 0;}
				enemy->setEnemyHp(enemy->getEnemyHp() - damagetaken);
		}
			else{
				cout << "Hit!" << '\n';
				damage = dmg + getSkillDmg();
				damagetaken = damage - enemy->getEnemyDef();
				if(damagetaken <= 0){damagetaken = 0;}
				enemy->setEnemyHp(enemy->getEnemyHp() - damagetaken);
		}
			cout << "Damage dealt: " << damagetaken << '\n';
	}
		else{cout << "You missed!" << '\n';}
		}

void Skill::enemyDamageCalc(Player& player, Enemy* enemy, int dmg){
		int damage;
		int damagetaken;
			if(enemy->checkhit()){
			if(enemy->checkcrit()){
				cout << "Critical hit!" << '\n';
				damage = (dmg + getSkillDmg()) *2;
				damagetaken = damage - player.getPlayerDef();
				if(damagetaken <= 0){damagetaken = 0;}
				player.setPlayerHp(player.getPlayerHp() - damagetaken);}
			else{
				cout << "Hit!" << '\n';
				damage = dmg + getSkillDmg();
				damagetaken = damage - player.getPlayerDef();
				if(damagetaken <= 0){damagetaken = 0;}
				player.setPlayerHp(player.getPlayerHp() - damagetaken);
		}
			cout << "Damage dealt: " << damagetaken << '\n';
	}
		else{cout << "Your opponent missed!" << '\n';}
}
//PlayerSkill Class
PlayerSkill::PlayerSkill(string name, string desc, string castdesc, int dmg, int stamina, int mana, int level, char skillclass)
        : Skill(name, desc, castdesc, dmg, stamina, mana, level, skillclass) {}
//EnemySkill Class
EnemySkill::EnemySkill(string name, string desc, string castdesc, int dmg, int stamina, int mana, int level, char skillclass)
        : Skill(name, desc, castdesc, dmg, stamina, mana, level, skillclass) {}
//Fireball class
Fireball::Fireball() : PlayerSkill("Fireball","Launches a fiery projectile. No additional effects.","You gather fire and release a burning orb.",10,0,20,1,'m'){}
void Fireball::playerUseSkill(Player& player, Enemy* enemy)  {
		if(player.getPlayerMana() >= getSkillMcost() && player.getPlayerStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		player.setPlayerMana(player.getPlayerMana() - getSkillMcost());
		player.setPlayerStamina(player.getPlayerStamina() - getSkillStmCost());
		playerDamageCalc(player,enemy,player.getPlayerAtk() + getSkillDmg());
			if(enemy->getEnemyHp() <= 0){
				cout << "He was incinerated." << '\n';}
	}
		else{cout << "You dont have enough mana or stamina!" << '\n';}
}
//IceSpike class
IceSpike::IceSpike() : PlayerSkill("Ice spike","Freezes an enemy temporarily. Stuns for 1 turn.", "You summon an ice shard and launch it.",0,0,15,2,'m'){}
void IceSpike::playerUseSkill(Player& player, Enemy* enemy)  {
	int damage = 0;
	int damagetaken = 0;
	if(player.getPlayerMana() >= getSkillMcost() && player.getPlayerStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
	 	player.setPlayerMana(player.getPlayerMana() - getSkillMcost());
  	        player.setPlayerStamina(player.getPlayerStamina() - getSkillStmCost());
	if(player.checkhit()){
	if(player.checkcrit()){
				cout << "Critical hit!" << '\n';
				damage = (player.getPlayerAtk() + getSkillDmg()) *2;
				damagetaken = damage - enemy->getEnemyDef();
				if(damagetaken <= 0){damagetaken = 0;}
				enemy->setEnemyHp(enemy->getEnemyHp() - damagetaken);
		}
			else{
				cout << "Hit!" << '\n';
				damage =  player.getPlayerAtk() + getSkillDmg();
				damagetaken = damage - enemy->getEnemyDef();
				if(damagetaken <= 0){damagetaken = 0;}
				enemy->setEnemyHp(enemy->getEnemyHp() - damagetaken);
		}
			cout << "Damage dealt: " << damagetaken << '\n';
			enemy->setEnemyStun(true);
			if(enemy->getEnemyHp() <= 0){
				cout << "Frozen solid, he shattered like glass." << '\n';}
	}
		else{cout << "You missed!" << '\n';}
	}
		else{cout << "You dont have enough mana or stamina!" << '\n';}
}
	
// Arcane Barrier class
ArcaneBarrier::ArcaneBarrier() : PlayerSkill("Arcane barrier", "Protects from damage. Reduces damage taken by 30% for 2 turns.", 
    "You raise a magical shield around yourself.", 0, 0, 40, 3, 'm') {}
void ArcaneBarrier::playerUseSkill(Player& player,Enemy* enemy) {
	if(player.getPlayerMana() >= getSkillMcost() && player.getPlayerStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		player.setPlayerMana(player.getPlayerMana() - getSkillMcost());
		player.setPlayerStamina(player.getPlayerStamina() - getSkillStmCost());		
		player.setPlayerBuff(true);
		player.setPlayerBuffCounter(2);
	}
	else{cout << "You dont have enough mana or stamina!" << '\n';}

}
//Arcane Discharge class
ArcaneDischarge::ArcaneDischarge() : PlayerSkill("Arcane discharge", "Unleashes stored magical energy in an explosive burst. Deals random  passive damage between 30-60.", 
    "You release unstable arcane energy in a chaotic blast.", 40, 0, 45, 4, 'm') {}
void ArcaneDischarge::playerUseSkill(Player& player, Enemy* enemy)  {
	int passivedmg;
	if(player.getPlayerMana() >= getSkillMcost() && player.getPlayerStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		player.setPlayerMana(player.getPlayerMana() - getSkillMcost());
		player.setPlayerStamina(player.getPlayerStamina() - getSkillStmCost());
		passivedmg = rand() % 31;
		playerDamageCalc(player,enemy,getSkillDmg() + passivedmg);
		if(enemy->getEnemyHp() <= 0){
			cout << "Nothing remained after the beam passed through them." << '\n';}
	}
		else{cout << "You dont have enough mana or stamina!" << '\n';}
	}

// Kamehameha class
Kamehameha::Kamehameha() : PlayerSkill("Kamehameha", "A devastating beam of concentrated mana. No additional effects.", 
    "You gather arcane energy in your hand then yell: KAMEHAMEHAAA!", 40, 30, 70,5, 'm') {}
void Kamehameha::playerUseSkill(Player& player, Enemy* enemy)  {
	if(player.getPlayerMana() >= getSkillMcost() && player.getPlayerStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		player.setPlayerMana(player.getPlayerMana() - getSkillMcost());
		player.setPlayerStamina(player.getPlayerStamina() - getSkillStmCost());		
		playerDamageCalc(player,enemy,player.getPlayerAtk() + getSkillDmg());
		if(enemy->getEnemyHp() <= 0){
			cout << "Nothing remained after the beam passed through them." << '\n';}
	}
		else{cout << "You dont have enough mana or stamina!" << '\n';}
	}
// Triple Shot class
TripleShot::TripleShot() : PlayerSkill("Triple shot", "Fires 3 quick attacks at the target. No additional effects.", 
    "You rapidly fire three projectiles in succession.", 0, 15, 0, 1, 'h') {}
void TripleShot::playerUseSkill(Player& player, Enemy* enemy)  {
	if(player.getPlayerMana() >= getSkillMcost() && player.getPlayerStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		player.setPlayerMana(player.getPlayerMana() - getSkillMcost());
		player.setPlayerStamina(player.getPlayerStamina() - getSkillStmCost());
		//cout << "DEBUG: RAW DMG = " << player.getPlayerAtk() + getSkillDmg() << '\n';
		//cout << "DEBUG PLAYERDMG = " << player.getPlayerAtk() << '\n';
		//cout << "DEBUG: ENEMYDEF = " << enemy->getEnemyDef() << '\n';
		for(int i = 0; i < 3; i++){
		playerDamageCalc(player,enemy,player.getPlayerAtk() + getSkillDmg());}
			if(enemy->getEnemyHp() <= 0){
			cout << "Pierced by arrows, he collapsed lifelessly." << '\n';}
	}
		else{cout << "You dont have enough mana or stamina!" << '\n';}
	}
// Precise Shot class
PreciseShot::PreciseShot() : PlayerSkill("Precise shot", "A well-aimed attack that cannot miss and ignores 15% of the target's armor.", 
    "You carefully aim and release a deadly strike.", 15, 20, 0, 2, 'h') {}
void PreciseShot::playerUseSkill(Player& player, Enemy* enemy)  {
	int damagetaken;
	int damage;
		if(player.getPlayerMana() >= getSkillMcost() && player.getPlayerStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		player.setPlayerMana(player.getPlayerMana() - getSkillMcost());
		player.setPlayerStamina(player.getPlayerStamina() - getSkillStmCost());
			if(player.checkcrit()){
				cout << "Critical hit!" << '\n';
				damage = player.getPlayerAtk() + getSkillDmg() *2;
				damagetaken = damage - enemy->getEnemyDef() *(1-0.15);
				if(damagetaken <= 0){damagetaken = 0;}
				enemy->setEnemyHp(enemy->getEnemyHp() - damagetaken);}
			else{
				cout << "Hit!" << '\n';
				damage = player.getPlayerAtk() + getSkillDmg();
				damagetaken = damage - enemy->getEnemyDef() * (1-0.15);
				if(damagetaken <= 0){damagetaken = 0;}
				enemy->setEnemyHp(enemy->getEnemyHp() - damagetaken);}
			cout << "Damage dealt: " << damagetaken << '\n';
			if(enemy->getEnemyHp() <= 0){
				cout << "One perfect shot. One silent death." << '\n';}
		}
		else{cout << "You dont have enough mana or stamina!" << '\n';}
}

// Crimson Strike class
CrimsonStrike::CrimsonStrike() : PlayerSkill("Crimson strike", "A strike that causes the target to bleed for 3 turns.", 
    "You coat your weapon in poison and strike.", 5, 15, 0, 3, 'h') {}
void CrimsonStrike::playerUseSkill(Player& player, Enemy* enemy)  {
		if(player.getPlayerMana() >= getSkillMcost() && player.getPlayerStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		player.setPlayerMana(player.getPlayerMana() - getSkillMcost());
		player.setPlayerStamina(player.getPlayerStamina() - getSkillStmCost());
		playerDamageCalc(player,enemy,player.getPlayerAtk() + getSkillDmg());
		enemy->setEnemyBleeding(true);
		enemy->setEnemyBleedCounter(3);
		if(enemy->getEnemyHp() <= 0){
			cout << "They bled out, their strength fading into nothing." << '\n';}	
	}
		else{cout << "You dont have enough mana or stamina!" << '\n';}
}
// Evasion class
Evasion::Evasion() : PlayerSkill("Evasion", "Increases dodge chance by 35% for 2 turns.", 
    "You focus your senses, ready to avoid incoming attacks.", 0, 20, 0, 4, 'h') {}
void Evasion::playerUseSkill(Player& player,Enemy* enemy)  {
	if(player.getPlayerMana() >= getSkillMcost() && player.getPlayerStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		player.setPlayerMana(player.getPlayerMana() - getSkillMcost());
		player.setPlayerStamina(player.getPlayerStamina() - getSkillStmCost());		
		enemy->setEnemyDebuff(true);
		enemy->setEnemyDebuffCounter(2);
	}
	else{cout << "You dont have enough mana or stamina!" << '\n';}
}

// Death's Gamble class
DeathsGamble::DeathsGamble() : PlayerSkill("Death gamble", "Executes the enemy.if their HP is below 60%, but leaves you at 1 HP.", 
    "You take a deep breath and go for the kill, knowing the risk.", 999, 40, 25, 5, 'h') {}
void DeathsGamble::playerUseSkill(Player& player,Enemy* enemy)  {
	if(player.getPlayerMana() >= getSkillMcost() && player.getPlayerStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		if(enemy->getEnemyHp() <= enemy->getEnemyMaxHp()*0.6){
			enemy->setEnemyHp(0);
			player.setPlayerHp(1);
			cout << "And just like that… it's over." << '\n';}
		else{
			cout << "You miscalculated" << '\n';
			enemy->setEnemyHp(enemy->getEnemyHp() -1);
			player.setPlayerHp(player.getPlayerHp() -1);
		}
	}
	else{cout << "You dont have enough mana or stamina!" << '\n';}

}
// Brutal Strike class
BrutalStrike::BrutalStrike() : PlayerSkill("Brutal strike", "A powerful overhead attack that deals high damage. No additional effects.", 
    "You swing your weapon with full force.", 15, 25, 0, 1, 'w') {}
void BrutalStrike::playerUseSkill(Player& player, Enemy* enemy)  {
	if(player.getPlayerMana() >= getSkillMcost() && player.getPlayerStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		player.setPlayerMana(player.getPlayerMana() - getSkillMcost());
		player.setPlayerStamina(player.getPlayerStamina() - getSkillStmCost());
		playerDamageCalc(player,enemy,player.getPlayerAtk() + getSkillDmg());
			if(enemy->getEnemyHp() <= 0){
			cout << "With one mighty swing, you split him apart." << '\n';}
	}
		else{cout << "You dont have enough mana or stamina!" << '\n';}
	}
	

// Defensive Stance class
DefensiveStance::DefensiveStance() : PlayerSkill("Defensive stance", "Reduces incoming damage. Reduces damage taken by 25% for 3 turns.", 
    "You take a firm defensive posture.", 0, 25, 0, 2, 'w') {}
void DefensiveStance::playerUseSkill(Player& player,Enemy* enemy) {
	if(player.getPlayerMana() >= getSkillMcost() && player.getPlayerStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		player.setPlayerMana(player.getPlayerMana() - getSkillMcost());
		player.setPlayerStamina(player.getPlayerStamina() - getSkillStmCost());		
		enemy->setEnemyDebuff(true);
		enemy->setEnemyDebuffCounter(3);
	}
	else{cout << "You dont have enough mana or stamina!" << '\n';}
}

// War Cry class
WarCry::WarCry() : PlayerSkill("War cry", "Temporarily increases attack power but lowers defense. +30% attack, -10% defense for 3 turns.", 
    "You let out a fierce battle shout.", 0, 20, 0, 3, 'w') {}

void WarCry::playerUseSkill(Player& player,Enemy* enemy) {
	if(player.getPlayerMana() >= getSkillMcost() && player.getPlayerStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		player.setPlayerMana(player.getPlayerMana() - getSkillMcost());
		player.setPlayerStamina(player.getPlayerStamina() - getSkillStmCost());		
		player.setPlayerBuff(true);
		player.setPlayerBuffCounter(3);
		playerDamageCalc(player,enemy,getSkillDmg());
	}
	else{cout << "You dont have enough mana or stamina!" << '\n';}
}

// Crippling Blow class
CripplingBlow::CripplingBlow() : PlayerSkill("Crippling blow", "A precise strike that momentarily stuns the enemy. Chance to stun for 1 turn.", 
    "You target a weak spot to disable the enemy", 10, 35, 0, 4, 'w') {}
void CripplingBlow::playerUseSkill(Player& player, Enemy* enemy)  {
	int damage = 0;
	int damagetaken = 0;
	if(player.getPlayerMana() >= getSkillMcost() && player.getPlayerStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		player.setPlayerMana(player.getPlayerMana() - getSkillMcost());
		player.setPlayerStamina(player.getPlayerStamina() - getSkillStmCost());	
		if(player.checkhit()){
			if(player.checkcrit()){
				cout << "Critical hit!" << '\n';
				damage = (player.getPlayerAtk() + getSkillDmg()) *2;
				damagetaken = damage - enemy->getEnemyDef();
				enemy->setEnemyStun(true);
				if(damagetaken <= 0){damagetaken = 0;}
				enemy->setEnemyHp(enemy->getEnemyHp() - damagetaken);
		}
			else{
				cout << "Hit!" << '\n';
				damage = player.getPlayerAtk() + getSkillDmg();
				damagetaken = damage - enemy->getEnemyDef();
				enemy->setEnemyStun(true);
				if(damagetaken <= 0){damagetaken = 0;}
				enemy->setEnemyHp(enemy->getEnemyHp() - damagetaken);
		}
			cout << "Damage dealt: " << damagetaken << '\n';
	}
		else{cout << "You missed!" << '\n';}
			}

		else{cout << "You dont have enough mana or stamina!" << '\n';}
	}

// Unbreakable Will class
UnbreakableWill::UnbreakableWill() : PlayerSkill("Unbreakable will", "A devastating counterattack when near death. Deals extra damage based on missing HP (1% per missing HP).", 
    "You focus all your energy into a final strike.", 15, 40, 25, 5, 'w') {}
void UnbreakableWill::playerUseSkill(Player& player,Enemy* enemy)  {
	if(player.getPlayerMana() >= getSkillMcost() && player.getPlayerStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		player.setPlayerMana(player.getPlayerMana() - getSkillMcost());
		player.setPlayerStamina(player.getPlayerStamina() - getSkillStmCost());		
		int passivedmg = getSkillDmg() * (player.getPlayerHp()/player.getPlayerMaxHp());
		playerDamageCalc(player,enemy,( player.getPlayerAtk() + getSkillDmg() + passivedmg));
		if(enemy->getEnemyHp() <= 0){
			cout << "Even at death’s door, your will was stronger." << '\n';}
	}
		else{cout << "You dont have enough mana or stamina!" << '\n';}
	}
//Enemy Skills
//Warrior
ShatteringBlow::ShatteringBlow() : EnemySkill("Shattering Blow", "A strike so powerful it weakens the player’s defense.", 
    "The enemy delivers a crushing blow, cracking your armor.", 10, 30, 0, 1, 'w') {}
void ShatteringBlow::enemyUseSkill(Player& player, Enemy* enemy)  {
	if(enemy->getEnemyMana() >= getSkillMcost() && enemy->getEnemyStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		enemy->setEnemyMana(enemy->getEnemyMana() - getSkillMcost());
		enemy->setEnemyStamina(enemy->getEnemyStamina() - getSkillStmCost());
		//Debuff
		player.setPlayerDebuff(true);
		player.setPlayerDebuffCounter(2);
		enemyDamageCalc(player,enemy, enemy->getEnemyAtk() + getSkillDmg());
	}
	else{cout << "Your opponent tries to do something but is to tired to do it!" << '\n';}
}
	 

RelentlessCharge::RelentlessCharge() : EnemySkill("Relentless Charge", "Rushes forward, knocking the player down and forcing them to lose a turn.", 
    "The enemy charges with brutal force, slamming into you.", 20, 35, 0, 2, 'w') {}
void RelentlessCharge::enemyUseSkill(Player& player, Enemy* enemy)  {
	int damage;
	int damagetaken;
	if(enemy->getEnemyMana() >= getSkillMcost() && enemy->getEnemyStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		enemy->setEnemyMana(enemy->getEnemyMana() - getSkillMcost());
		enemy->setEnemyStamina(enemy->getEnemyStamina() - getSkillStmCost());
		//Stun
	if(enemy->checkhit()){
	if(enemy->checkcrit()){
				cout << "Critical hit!" << '\n';
				damage = getSkillDmg() *2;
				damagetaken = damage - player.getPlayerDef();
				if(damagetaken <= 0){damagetaken = 0;}
				player.setPlayerHp(player.getPlayerHp() - damagetaken);
		}
			else{
				cout << "Hit!" << '\n';
				damage =  getSkillDmg();
				damagetaken = damage - player.getPlayerDef();
				if(damagetaken <= 0){damagetaken = 0;}
				player.setPlayerHp(player.getPlayerHp() - damagetaken);
		}
			cout << "Damage dealt: " << damagetaken << '\n';
			player.setPlayerStun(true);
	}
		else{cout << "Your opponent missed!" << '\n';}
	}
	else{cout << "Your opponent tries to do something but is to tired to do it!" << '\n';}
}
	 

FlurryOfBlows::FlurryOfBlows() : EnemySkill("Flurry of Blows", "A rapid series of attacks meant to overwhelm the player.", 
    "The enemy strikes multiple times in quick succession!",5, 40, 0, 3, 'w') {}
void FlurryOfBlows::enemyUseSkill(Player& player, Enemy* enemy)  {
	if(enemy->getEnemyMana() >= getSkillMcost() && enemy->getEnemyStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		enemy->setEnemyMana(enemy->getEnemyMana() - getSkillMcost());
		enemy->setEnemyStamina(enemy->getEnemyStamina() - getSkillStmCost());
		//Multiple attacks
		for(int i = 0; i < 2; i++){
		enemyDamageCalc(player,enemy,enemy->getEnemyAtk() + getSkillDmg());}
	}
	else{cout << "Your opponent tries to do something but is to tired to do it!" << '\n';}
}
	 

SecondWind::SecondWind() : EnemySkill("Second Wind", "When dropping below 30% HP, the enemy heals and increases damage for a short time.", 
    "The enemy roars, refusing to fall just yet!", 0, 0, 0, 4, 'w') {}
void SecondWind::enemyUseSkill(Player& player, Enemy* enemy)  {
	int passiveheal;
	if(enemy->getEnemyMana() >= getSkillMcost() && enemy->getEnemyStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		enemy->setEnemyMana(enemy->getEnemyMana() - getSkillMcost());
		enemy->setEnemyStamina(enemy->getEnemyStamina() - getSkillStmCost());
		//Buff
		enemy->setEnemyBuff(true);
		enemy->setEnemyBuffCounter(2);
		passiveheal = (enemy->getEnemyMaxHp()/enemy->getEnemyHp())*0,2;
		cout << "Your opponent heals himself!" << '\n';
		enemy->setEnemyHp(enemy->getEnemyHp() + passiveheal);
	}
	else{cout << "Your opponent tries to do something but is to tired to do it!" << '\n';}
}
	 


BleedingSlash::BleedingSlash() : EnemySkill("Bleeding Slash", "A brutal cut that makes the player bleed.", 
    "A deep wound opens, and blood begins to flow.", 10, 45, 0, 5, 'w') {}
void BleedingSlash::enemyUseSkill(Player& player, Enemy* enemy)  {
		if(enemy->getEnemyMana() >= getSkillMcost() && enemy->getEnemyStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		enemy->setEnemyMana(enemy->getEnemyMana() - getSkillMcost());
		enemy->setEnemyStamina(enemy->getEnemyStamina() - getSkillStmCost());
		enemyDamageCalc(player,enemy,enemy->getEnemyAtk() + getSkillDmg());
		//Bleeding
		player.setPlayerBleeding(true);
		player.setPlayerBleedCounter(4);
	}
		else{cout << "Your opponent tries to do something but is to tired to do it!" << '\n';}
}
//Hunter
LurkingStrike::LurkingStrike() : EnemySkill("Lurking Strike", "a Normal attack", "The enemy waits patiently for the perfect moment... then strikes!", 15, 25, 0, 1, 'h') {}
void LurkingStrike::enemyUseSkill(Player& player, Enemy* enemy)  {
	if(enemy->getEnemyMana() >= getSkillMcost() && enemy->getEnemyStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		enemy->setEnemyMana(enemy->getEnemyMana() - getSkillMcost());
		enemy->setEnemyStamina(enemy->getEnemyStamina() - getSkillStmCost());
		enemyDamageCalc(player,enemy,enemy->getEnemyAtk() + getSkillDmg());
	}
	else{cout << "Your opponent tries to do something but is to tired to do it!" << '\n';}
}
	 

BarbedShot::BarbedShot() : EnemySkill("Barbed Shot", "A piercing arrow or dagger strike that causes the player to bleed.", 
    "The enemy's attack digs deep, drawing blood.", 5, 30, 0, 2, 'h') {}
void BarbedShot::enemyUseSkill(Player& player, Enemy* enemy)  {
		if(enemy->getEnemyMana() >= getSkillMcost() && enemy->getEnemyStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		enemy->setEnemyMana(enemy->getEnemyMana() - getSkillMcost());
		enemy->setEnemyStamina(enemy->getEnemyStamina() - getSkillStmCost());
		enemyDamageCalc(player,enemy,enemy->getEnemyAtk() + getSkillDmg());
		//Bleeding
		player.setPlayerBleeding(true);
		player.setPlayerBleedCounter(2);
	}
		else{cout << "Your opponent tries to do something but is to tired to do it!" << '\n';}
}

EvasiveManeuver::EvasiveManeuver() : EnemySkill("Evasive Maneuver", "After attacking, the enemy becomes harder to hit.", 
    "The enemy moves with inhuman speed, making the next hit harder to land.", 0, 25, 0, 3, 'h') {}
void EvasiveManeuver::enemyUseSkill(Player& player, Enemy* enemy)  {
	if(enemy->getEnemyMana() >= getSkillMcost() && enemy->getEnemyStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		enemy->setEnemyMana(enemy->getEnemyMana() - getSkillMcost());
		enemy->setEnemyStamina(enemy->getEnemyStamina() - getSkillStmCost());
		//Buff
		player.setPlayerDebuff(true);
		player.setPlayerDebuffCounter(2);}
	else{cout << "Your opponent tires to do something but is to tired to do it!" << '\n';}
	}

SmokeBomb::SmokeBomb() : EnemySkill("Smoke Bomb", "The enemy throws a smoke bomb, momentarily stunning you.", 
    "A thick cloud of smoke engulfs you. By the time it clears, the enemy is already upon you!", 0, 30, 0, 4, 'h') {}
void SmokeBomb::enemyUseSkill(Player& player, Enemy* enemy)  {
	if(enemy->getEnemyMana() >= getSkillMcost() && enemy->getEnemyStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		enemy->setEnemyMana(enemy->getEnemyMana() - getSkillMcost());
		enemy->setEnemyStamina(enemy->getEnemyStamina() - getSkillStmCost());
		//Stun
		if(enemy->checkhit()){
		player.setPlayerStun(true);
		cout << "Hit!" << '\n';
		}
		else{cout << "Your opponent misses!" << '\n';}
	}
	else{cout << "Your opponent tries to do something but is to tired to do it!" << '\n';}
}
	 


PiercingStrike::PiercingStrike() : EnemySkill("Piercing Strike", "A perfectly aimed attack meant to deal maximum damage.", 
    "The enemy delivers a precise and deadly strike to you weakest point.", 25, 45, 0, 5, 'h') {}
void PiercingStrike::enemyUseSkill(Player& player, Enemy* enemy)  {
	if(enemy->getEnemyMana() >= getSkillMcost() && enemy->getEnemyStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		enemy->setEnemyMana(enemy->getEnemyMana() - getSkillMcost());
		enemy->setEnemyStamina(enemy->getEnemyStamina() - getSkillStmCost());
		enemyDamageCalc(player,enemy,enemy->getEnemyAtk() + getSkillDmg());
	}
	else{cout << "Your opponent tries to do something but is to tired to do it!" << '\n';}
}
	 

//Mage
Drain::Drain() : EnemySkill("Drain", "Saps the player's energy, reducing both mana and stamina.", 
    "You see your enemy doing a weird hand motion and you feel your strength and magic fading at the same time.", 0, 0, 20, 1, 'm') {}
void Drain::enemyUseSkill(Player& player, Enemy* enemy)  {
	if(enemy->getEnemyMana() >= getSkillMcost() && enemy->getEnemyStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		enemy->setEnemyMana(enemy->getEnemyMana() - getSkillMcost());
		enemy->setEnemyStamina(enemy->getEnemyStamina() - getSkillStmCost());
		player.setPlayerStamina(player.getPlayerStamina() - 20);
		player.setPlayerMana(player.getPlayerMana() - 20);
	}
	else{cout << "Your opponent tries to do something but is to tired to do it!" << '\n';}
}


ArcaneTrap::ArcaneTrap() : EnemySkill("Arcane Trap", "Summons a magical glyph that activates when the player moves, stunning them.", 
    "A strange glyph appears at your feet… which inmovilizes you.", 0, 0, 25, 2, 'm') {}
void ArcaneTrap::enemyUseSkill(Player& player, Enemy* enemy)  {
	if(enemy->getEnemyMana() >= getSkillMcost() && enemy->getEnemyStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		enemy->setEnemyMana(enemy->getEnemyMana() - getSkillMcost());
		enemy->setEnemyStamina(enemy->getEnemyStamina() - getSkillStmCost());
		//Stun
		if(enemy->checkhit()){
		player.setPlayerStun(true);
		cout << "Hit!" << '\n';
		}
		else{ cout << "Your opponent misses!" << '\n';}
	}
	else{cout << "Your opponent tries to do something but is to tired to do it!" << '\n';}
}
	 

ArcaneBurst::ArcaneBurst() : EnemySkill("Arcane Burst", "A sudden explosion of pure magic energy.", 
    "The enemy raises their hands, and a blast of energy surges toward you!", 15, 0, 45, 3, 'm') {}
void ArcaneBurst::enemyUseSkill(Player& player, Enemy* enemy)  {
	if(enemy->getEnemyMana() >= getSkillMcost() && enemy->getEnemyStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		enemy->setEnemyMana(enemy->getEnemyMana() - getSkillMcost());
		enemy->setEnemyStamina(enemy->getEnemyStamina() - getSkillStmCost());
		enemyDamageCalc(player,enemy,enemy->getEnemyAtk() + getSkillDmg());
	}
	else{cout << "Your opponent tries to do something but is to tired to do it!" << '\n';}
}
	 
WarpingCurse::WarpingCurse() : EnemySkill("Warping Curse", "The enemy distorts your perception, making your attacks less effective.", 
    "Reality twists around you… your strength feels strangely weaker.", 0, 0, 20, 4, 'm') {}

void WarpingCurse::enemyUseSkill(Player& player, Enemy* enemy)  {
	if(enemy->getEnemyMana() >= getSkillMcost() && enemy->getEnemyStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		enemy->setEnemyMana(enemy->getEnemyMana() - getSkillMcost());
		enemy->setEnemyStamina(enemy->getEnemyStamina() - getSkillStmCost());
		//Debuff
		player.setPlayerDebuff(true);
		player.setPlayerDebuffCounter(1);
		enemyDamageCalc(player,enemy,getSkillDmg());
	}
	else{cout << "Your opponent tries to do something but is to tired to do it!" << '\n';}
}

ArcaneDetonation::ArcaneDetonation() : EnemySkill("Arcane Detonation", "The enemy overloads their magic, dealing massive damage to both themselves and the player.", 
    "The enemy body glows with unstable magic… then erupts in a blinding explosion!", 25, 0, 0, 5, 'm') {}
void ArcaneDetonation::enemyUseSkill(Player& player, Enemy* enemy)  {
	if(enemy->getEnemyMana() >= getSkillMcost() && enemy->getEnemyStamina() >= getSkillStmCost()){
		cout << getSkillCastDesc() << '\n';
		enemy->setEnemyMana(enemy->getEnemyMana() - getSkillMcost());
		enemy->setEnemyStamina(enemy->getEnemyStamina() - getSkillStmCost());
		enemyDamageCalc(player,enemy,enemy->getEnemyAtk() + getSkillDmg());
		enemy->setEnemyHp(enemy->getEnemyHp() - 60);
		cout << "The battlefield clears, and your foe stands weakened, the cost of their reckless power evident." << '\n';
		
	}
	else{cout << "Your opponent tries to do something but is to tired to do it!" << '\n';}
}



