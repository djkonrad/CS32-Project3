//
//  Monster.h
//  MiniRogue
//
//  Created by Dom's Mac on 5/14/20.
//  Copyright © 2020 Dom's Mac. All rights reserved.
//

#ifndef Monster_h
#define Monster_h

#include <queue>
#include <stack>

class Dungeon;

class Monster
{
public:
    // Constructor/Destructor
    Monster(Dungeon* ap, int r, int c, int health, int armor, int strength, int dexterity, char gridChar);
    virtual ~Monster();

    // Accessors
    int row() const;
    int col() const;
    int getHealth() const;
    int getArmor() const;
    int getStrength() const;
    int getWeaponDamage() const;
    int getDexterity() const;
    string getType() const;
    string getActionString() const;
    int getGridChar() const;
    bool isPlayerNearby() const;
    int attackerPoints() const;
    int defenderPoints() const;
    bool isDead() const;

    // Mutators
    virtual void move() = 0;
    void updateMonsterStats();
    
    //helper function
    bool attemptMove(const Dungeon& a, char dir, int& r, int& c);
    
protected:
    Dungeon* m_dungeon;
    int m_row;
    int m_col;
    int m_health;
    int m_armor;
    int m_strength;
    int m_dexterity;
    int m_weaponDamage;
    char m_gridChar;
    string m_type;
    string m_actionString;
    int m_damagePoints;
    int m_attackerPoints;
    int m_defenderPoints;
};

class Bogeyman: public Monster
{
public:
    
    //Constructor
    Bogeyman(Dungeon* ap, int r, int c);
    
    //Mutators
    virtual void move();
};

class Snakewoman: public Monster
{
public:
    
    //Constructor
    Snakewoman(Dungeon* ap, int r, int c);
    
    //Mutators
    virtual void move();
};

class Dragon: public Monster
{
public:
    //Constructor
    Dragon(Dungeon* ap, int r, int c);
    
    //Mutators
    virtual void move();
};

class Goblin: public Monster
{
    struct Coord;
public:
    //Constructor
    Goblin(Dungeon* ap, int r, int c);
    
    //Mutators
    virtual void move();
    bool canMove(int depth, int& endRow, int& endCol);
    bool canWalkOnSpot(int r, int c);
    void markEncountered();
private:
    struct Coord
    {
        Coord (int r, int c) : row(r) , col(c){}
        int row;
        int col;
    };
    
    queue<Coord> coordQueue;
    vector<Coord> spotsEncountered;
};



#endif /* Monster_h */
