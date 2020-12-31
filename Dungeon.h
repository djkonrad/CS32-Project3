//
//  Dungeon.h
//  MiniRogue
//
//  Created by Dom's Mac on 5/12/20.
//  Copyright © 2020 Dom's Mac. All rights reserved.
//

#ifndef Dungeon_h
#define Dungeon_h

#include "globals.h"
#include <vector>
using namespace std;

class Player;
class Monster;
class Object;

class Dungeon
{
public:
    // Constructor/destructor
    Dungeon(int nRows, int nCols, int level, const vector<Object*> & inventory, Object* currentweapon, int health, int armor, int strength, int dexterity, int healthCap);
    ~Dungeon();
    
    // Accessors
    Player* player() const;
    vector<Object*> objects() const;
    vector<Monster*> monsters() const;
    string monsterTypeAtCoord(int r, int c) const;
    int rows() const;
    int cols() const;
    int getCellStatus(int r, int c) const;
    int monsterDefenderPointsAtCoord(int r, int c) const;
    int monsterHealthPointsAtCoord(int r, int c) const;
    int totalDamage() const;
    int staircaseRow() const;
    int staircaseCol() const;
    int idolRow() const;
    int idolCol() const;
    
    // Mutators
    void setCellStatus(int r, int c, int status);
    bool addPlayer(int r, int c);
    bool addBogeyman(int r, int c);
    bool addSnakewoman(int r, int c);
    bool addDragon(int r, int c);
    bool addGoblin(int r, int c);
    bool addStaircase(int r, int c);
    bool addIdol(int r, int c);
    bool addObject(int r, int c, int type);
    void removeObject(int i);
    void moveMonsters();
    
    void makeThreeRooms();
    void makeFourRooms();
    void makeFiveRooms();
    void display(string msg);
    
    
private:
    Player* m_player;
    vector<Monster*> m_monsters;
    vector<Object*> m_objects;
    int m_grid[MAXROWS][MAXCOLS];
    int m_rows;
    int m_cols;
    int m_StaircaseRow;
    int m_StaircaseCol;
    int m_IdolRow;
    int m_IdolCol;
    int m_level;
    
    vector<Object*> m_inventory;
    Object* m_currentWeapon;
    int m_playerHealth;
    int m_playerArmor;
    int m_playerStrength;
    int m_playerDexterity;
    int m_playerHealthCap;
    
    int m_totalDamage;
    vector<string> m_MonsterMessage;
    
    //Helper functions
    bool isPosInBounds(int r, int c) const;
    void checkPos(int r, int c, std::string functionName) const;
};



#endif /* Dungeon_h */
