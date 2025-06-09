#include <iostream>
#include <string>
#include "item.h"
#include "player.h"
using namespace std;
	Item::~Item() {
		//cout << "DEBUG: ITEM DESTRUCTOR ENTERED" << '\n';
	}
	Item::Item(string name, string desc, char code, int value,int weight)
		: itemname(name), itemdesc(desc),itemcode(code), value(value), weight(weight){}
	string Item::getItemName(){
			if(itemname.empty()){ return "no item";}
			else{return itemname;}}
	string Item::getItemDesc(){
			return itemdesc;}
	int Item::getItemValue(){
			return value;}
	int Item::getItemWeight(){
			return weight;}
	char Item::getItemCode(){
			return itemcode;}	
	 //  Copy Constructor
	Weapon::Weapon(const Weapon& other) 
        : Item(other), atkbonus(other.atkbonus), staminabonus(other.staminabonus), manab(other.manab), classcode(other.classcode) {}

	Weapon::Weapon(string name, string desc,char code, int value, int weight, int atk,int staminabonus, int mana, char classcode)
	: Item(name,desc,code,value,weight), atkbonus(atk), staminabonus(staminabonus), manab(mana), classcode(classcode){}

	int Weapon::getAtkBonus(){
		return atkbonus;}
	void Weapon::setAtkBonus(int dmg){
		atkbonus = dmg;}
	int Weapon::getStaminaBonus(){
		return staminabonus;}
	void Weapon::setStaminaBonus(int stam){
		staminabonus = stam;}
	int Weapon::getManaB(){
		return manab;}
	void Weapon::setManaB(int mana){
		manab = mana;}
	char Weapon::getClassCode(){
		return classcode;}				

Armor::Armor(const Armor& other)
: Item(other), defbonus(other.defbonus), hpa(other.hpa), classcode(other.classcode), armortype(other.armortype) {}

Armor::Armor(string name, string desc, char code, int value, int weight, int def,int hp, char classcode, string armortype)
: Item(name,desc,code,value, weight), defbonus(def), hpa(hp), classcode(classcode), armortype(armortype) {}
int Armor::getDefBonus(){
	return defbonus;}
void Armor::setDefBonus(int prot){
	defbonus = prot;}
int Armor::getHpa(){
	return hpa;}
void Armor::setHpa(int hp){
	hpa = hp;}
char Armor::getClassCode(){
	return classcode;}
string Armor::getArmorType(){
	return armortype;}	

Potion::Potion(const Potion& other)
 : Item(other), hpbonus(other.hpbonus), manabonus(other.manabonus), staminabonus(other.staminabonus) {}
Potion::Potion(string name, string desc,char code, int value, int weight, int hp, int mana, int stamina)
: Item(name,desc,code,value,weight), hpbonus(hp), manabonus(mana), staminabonus(stamina){}
int Potion::getHpBonus(){
	return hpbonus;}
void Potion::setHpBonus(int bonus){
	hpbonus = bonus;}
int Potion::getManaBonus(){
	return manabonus;}
void Potion::setManaBonus(int bonus){
	manabonus = bonus;}
int Potion::getStaminaBonus(){
	return staminabonus;}
void Potion::setStaminaBonus(int bonus){
	staminabonus = bonus;}


Item* displayitems[7] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
Weapon* storeweapon[15];  
Armor* storearmor[27];  
Potion* storepotion[9];


void initializestoreitems(){
    for (int i = 0; i < 15; i++) storeweapon[i] = nullptr;
    for (int i = 0; i < 27; i++) storearmor[i] = nullptr;
    for (int i = 0; i < 9; i++) storepotion[i] = nullptr;	
   storeweapon[0] = new Weapon("recruit blade", "A dull and worn-out sword, barely fit for battle.", 'w', 26, 6, 8, 4, 0, 'w');
   storeweapon[1] = new Weapon("silver sword", "A finely crafted sword with a silver edge.", 'w', 70, 8, 16, 16, 0, 'w');
   storeweapon[2] = new Weapon("iron axe", "A heavy axe with a brutal edge but tiring to wield.", 'w', 100, 14, 22, -8, 0, 'w');
   storeweapon[3] =  new Weapon("iron sword", "A well-balanced sword, reliable in any fight.", 'w', 65, 8, 18, 4, 0, 'w');
   storeweapon[4] = new Weapon("orkish sword", "A savage, jagged blade that takes a toll on stamina.", 'w', 120, 10, 28, -16, 0, 'w');
    // 🏹 Hunter Weapons
   storeweapon[5] =  new Weapon("wooden bow", "A weak bow barely holding together, fit for a beginner.", 'w', 25, 4, 8, 4, 0, 'h');
   storeweapon[6] =   new Weapon("longbow", "A powerful bow with excellent range but tiring to use.", 'w', 135, 10, 26, -8, 0, 'h');
   storeweapon[7] =  new Weapon("crossbow", "A sturdy, balanced weapon requiring steady aim.", 'w', 95, 8, 18, 0, 0, 'h');
   storeweapon[8] =    new Weapon("iron dagger", "A sharp dagger, quick and light, boosting stamina.", 'w', 80, 4, 22, 12, 0, 'h');
   storeweapon[9] =   new Weapon("goblin dagger", "A small, crude blade, easy to wield and enhances stamina.", 'w', 95, 3, 18, 16, 0, 'h');
    // 🔮 Mage Weapons
   storeweapon[10] =  new Weapon("wooden staff", "A simple wooden staff, best suited for beginners.", 'w', 20, 8, 12, 0, 10, 'm');
   storeweapon[11] =   new Weapon("obsidian staff", "A dark, powerful staff that enhances both magic and offense.", 'w', 70, 10, 26, 0, 20, 'm');
   storeweapon[12] =   new Weapon("book of elemental magic", "A tome filled with ancient spells, boosting magical power.", 'w', 90, 7, 20, 0, 30, 'm');
   storeweapon[13] =   new Weapon("elder scroll", "A scroll imbued with immense power, yet it requires skill.", 'w', 140, 6, 32, 0, 4, 'm');
   storeweapon[14] =  new Weapon("warstaff", "A battle-worn staff that grants great power but drains mana.", 'w', 100, 11, 30, 0, -12, 'm');
    // 🛡️ Warrior Armor
   storearmor[0] = new Armor("iron helm", "A sturdy iron helmet.", 'a', 40, 3, 3, 2, 'w', "head");  
   storearmor[1] = new Armor("iron vest", "Basic iron chest armor.", 'a', 60, 8, 5, 4, 'w', "vest");  
   storearmor[2] = new Armor("iron greaves", "Heavy iron leg guards.", 'a', 50, 5, 4, 3, 'w', "leg");  
   storearmor[3] = new Armor("plate helm", "A full metal helmet.", 'a', 80, 5, 8, 6, 'w', "head"); 
   storearmor[4] = new Armor("plate armor", "Thick plate armor.", 'a', 120, 12, 16, 8, 'w', "vest");  
   storearmor[5] = new Armor("plate greaves", "Heavy-duty greaves.", 'a', 90, 6, 10, 6, 'w', "leg"); 
   storearmor[6] = new Armor("titan helm", "A battle-worn helmet.", 'a', 200, 7, 11, 7, 'w', "head");  
   storearmor[7] = new Armor("titan plate", "Nearly unbreakable armor.", 'a', 350, 15, 24, 10, 'w', "vest");  
   storearmor[8] = new Armor("titan greaves", "Ultra-dense leg protection.", 'a', 250, 9, 14, 8, 'w', "leg");  
    // 🏹 Hunter Armor
  storearmor[9] = new Armor("leather hood", "A light leather hood.", 'a', 30, 2, 1, 1, 'h', "head");
  storearmor[10] = new Armor("leather vest", "Reinforced leather armor.", 'a', 50, 5, 4, 2, 'h', "vest");
  storearmor[11] = new Armor("leather boots", "Flexible boots for agility.", 'a', 35, 3, 2, 2, 'h', "leg");
  storearmor[12] = new Armor("hunter hood", "A hood reinforced with iron.", 'a', 60, 3, 6, 4, 'h', "head");
  storearmor[13] = new Armor("hunter vest", "Sturdy leather with plating.", 'a', 90, 6, 11, 6, 'h', "vest");
  storearmor[14] = new Armor("hunter boots", "Thick boots for endurance.", 'a', 70, 4, 8, 5, 'h', "leg");
  storearmor[15] = new Armor("shadowstalker hood", "A hood that blends with darkness.", 'a', 150, 3, 9, 6, 'h', "head");
  storearmor[16] = new Armor("shadowstalker vest", "Reinforced leather with dark runes.", 'a', 250, 8, 15, 8, 'h', "vest");
  storearmor[17] = new Armor("shadowstalker boots", "Silent boots for unmatched speed.", 'a', 200, 5, 11, 7, 'h', "leg");
    // 🔮 Mage Armor
   storearmor[18] =   new Armor("cloth hat", "A simple cloth hat.", 'a', 20, 1, 1, 1, 'm', "head");
   storearmor[19] =  new Armor("cloth robe", "Basic mage robe.", 'a', 40, 3, 3, 2, 'm', "vest");
   storearmor[20] =   new Armor("cloth sandals", "Light sandals for movement.", 'a', 25, 2, 2, 1, 'm', "leg");
   storearmor[21] =  new Armor("enchanted hat", "Cloth imbued with magic.", 'a', 50, 2, 4, 2, 'm', "head");
   storearmor[22] =   new Armor("enchanted robe", "A robe that boosts mana.", 'a', 80, 4, 6, 4, 'm', "vest");
   storearmor[23] =  new Armor("enchanted sandals", "Floating sandals.", 'a', 55, 3, 4, 2, 'm', "leg");
   storearmor[24] =  new Armor("archmage hood", "Woven with powerful arcane threads.", 'a', 180, 2, 6, 3, 'm', "head");
   storearmor[25] =   new Armor("archmage robe", "Infused with runes for maximum mana.", 'a', 250, 5, 10, 5, 'm', "vest");
   storearmor[26] =   new Armor("archmage sandals", "Hovering sandals that increase speed.", 'a', 200, 3, 7, 4, 'm', "leg");
   storepotion[0] = new Potion("small hp potion", "Restores 20 HP.", 'p', 10, 1,  20, 0, 0);
   storepotion[1] = new Potion("medium hp potion", "Restores 50 HP.", 'p', 25, 2, 50, 0, 0);
   storepotion[2] = new Potion("big hp potion", "Restores 100 HP.", 'p', 50, 3,  100, 0, 0);
   storepotion[3] =  new Potion("small mana potion", "Restores 20 Mana.", 'p', 10, 1, 0, 20, 0);
   storepotion[4] = new Potion("medium mana potion", "Restores 50 Mana.", 'p', 25, 2,  0, 50, 0);
   storepotion[5] = new Potion("big mana potion", "Restores 100 Mana.", 'p', 50, 3 , 0, 100, 0);
   storepotion[6] = new Potion("small stamina potion", "Restores 20 Stamina.", 'p', 10, 1, 0, 0,20);
   storepotion[7] = new Potion("medium stamina potion", "Restores 50 Stamina.", 'p', 25,2, 0,0,50);
   storepotion[8] = new Potion("big stamina potion", "Restores 100 Stamina.", 'p', 50, 3, 0,0,100);
   
 
}
void cleanupStoreItems() {
    for (int i = 0; i < 15; i++) {
	if(storeweapon[i] != nullptr){
        delete storeweapon[i];
        storeweapon[i] = nullptr;
			}
    }
    for (int i = 0; i < 27; i++) {
	if(storearmor[i] != nullptr){
        delete storearmor[i];
        storearmor[i] = nullptr;
	}
    }
    for (int i = 0; i < 8; i++) {
	if(storepotion[i] != nullptr){
        delete storepotion[i];
        storepotion[i] = nullptr;
		}
    }
}


