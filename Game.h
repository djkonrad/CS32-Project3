//
//  Game.h
//  MiniRogue
//
//  Created by Dom's Mac on 5/14/20.
//  Copyright © 2020 Dom's Mac. All rights reserved.
//

#ifndef Game_h
#define Game_h

#include <string>
#include <vector>
using namespace std;

class Dungeon;
class Object;

class Game
{
  public:
    // Constructor/destructor
    Game(int depth);
    ~Game();
    
    int smellDepth();

    // Mutators
    void play();

  private:
    Dungeon* m_dungeon;
    int m_level;
    int m_depth;
    //int dir;

    // Helper functions
    std::vector<Object*> inventory;
    void newLevel(int rows, int cols, int level, const std::vector<Object*> inventory, Object* currentWeapon, int health, int armor, int strength, int dexterity, int healthCap);
    bool decodeDirection(char ch, int& dir);
    std::string takePlayerTurn();
};


#endif /* Game_h */
