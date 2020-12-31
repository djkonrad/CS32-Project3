//
//  Player.h
//  MiniRogue
//
//  Created by Dom's Mac on 5/13/20.
//  Copyright © 2020 Dom's Mac. All rights reserved.
//

#ifndef Player_h
#define Player_h

#include <string>
#include <vector>
using namespace std;

class Dungeon;
class Object;

class Player
{
  public:
    // Constructor
    Player(Dungeon* ap, int r, int c, vector<Object*> inventory, Object* currentweapon, int health, int armor, int strength, int dexterity, int healthCap);

      // Accessors
    vector <Object*> getInventory() const;
    Object* getCurrentWeapon() const;
    int row() const;
    int col() const;
    int getHealth() const;
    int getArmor() const;
    int getStrength() const;
    int getDexterity() const;
    int getHealthCap() const;
    int rowAttacked() const;
    int colAttacked() const;
    int damagePoints() const;
    int attackerPoints() const;
    int defenderPoints() const;
    bool isDead() const;

    // Mutators
    string move(char dir);
    string addToInventory();
    string printInventory();
    string weaponSelector();
    string scrollSelector();
    string staircase();
    void updateStats();
    void setHealth(int health);
    void setStrength(int strength);
    void setDead();

  private:
    Dungeon* m_dungeon;
    Object* m_currentWeapon;
    string m_actionString;
    vector<Object*> m_inventory;
    int m_row;
    int m_col;
    int m_health;
    int m_armor;
    int m_strength;
    int m_dexterity;
    int m_healthCap;
    int m_rowAttacked;
    int m_colAttacked;
    int m_weaponDamage;
    int m_damagePoints;
    int m_attackerPoints;
    int m_defenderPoints;
    bool m_dead;
    
    //helper function
    bool attemptMove(const Dungeon& a, char dir, int& r, int& c);
    //used in printInventory()
    char letters[26] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
};


#endif /* Player_h */
