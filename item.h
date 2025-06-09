#ifndef ITEM_H
#define ITEM_H
#include <iostream>
#include <string>
using namespace std;
class Item {
	private:
		string itemname;
		string itemdesc;
		char itemcode;
		int value;
		int weight;
	public:
		virtual ~Item();
		Item(string name, string desc, char code, int value,int weight);
		string getItemName();
		string getItemDesc();
		int getItemValue();
		int getItemWeight();
		char getItemCode();
};	
	class Weapon : public Item {
			private:
				int atkbonus;
				int staminabonus;
				int manab;
				char classcode;
			public:
				 //  Copy Constructor
    				Weapon(const Weapon& other); 
				//Constructor
				Weapon(string name, string desc,char code, int value, int weight, int atk,int staminabonus, int mana, char classcode);
				int getAtkBonus();
				void setAtkBonus(int dmg);
				int getStaminaBonus();
				void setStaminaBonus(int stam);
				int getManaB();
				void setManaB(int mana);
				char getClassCode();
					
			};

class Armor : public Item {
			private:
				int defbonus;
				int hpa;
				char classcode;
				string armortype;
			public:
				Armor(const Armor& other);
				Armor(string name, string desc, char code, int value, int weight, int def,int hp, char classcode, string armortype);
				int getDefBonus();
				void setDefBonus(int prot);
			        int getHpa();
				void setHpa(int hp);
				char getClassCode();
				string getArmorType();
				
};
		class Potion : public Item {
			private:
				int hpbonus;
				int manabonus;
				int staminabonus;
			public:
				Potion(const Potion& other);
				Potion(string name, string desc,char code, int value, int hp, int mana, int stamina, int weight);
				int getHpBonus();
				void setHpBonus(int bonus);
				int getManaBonus();
				void setManaBonus(int bonus);
				int getStaminaBonus();
				void setStaminaBonus(int bonus);

		};
extern Weapon* storeweapon[15];  
extern Armor* storearmor[27];  
extern Potion* storepotion[9];
void initializestoreitems();
void cleanupStoreItems();
extern Item* displayitems[7];

#endif // ITEM_H
