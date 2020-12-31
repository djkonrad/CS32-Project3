//
//  Game.cpp
//  MiniRogue
//
//  Created by Dom's Mac on 5/14/20.
//  Copyright © 2020 Dom's Mac. All rights reserved.
//

#include <iostream>
#include <vector>
using namespace std;

#include "Game.h"
#include "Dungeon.h"
#include "Player.h"
#include "Object.h"
#include "utilities.h"

Game:: Game(int depth)
{
    //start at level 0
    m_level = 0;
    //assign Goblin smell depth
    m_depth = depth;
    //create a dungeon
    m_dungeon = nullptr;
    std::vector<Object*> inventory;
}

Game:: ~Game()
{
    delete m_dungeon;
}

void Game:: newLevel(int rows, int cols, int level, vector<Object*> inventory, Object* currentWeapon, int health, int armor, int strength, int dexterity, int healthCap)
{
    
    //Update the game's level
    m_level = level;
    
    //making a new Dungeon to play in
    m_dungeon = new Dungeon(rows, cols, level, inventory, currentWeapon, health, armor, strength, dexterity, healthCap);
    
    // Add player
      int rPlayer;
      int cPlayer;
      do
      {
          rPlayer = randInt(1, rows);
          cPlayer = randInt(1, cols);
      } while (m_dungeon->getCellStatus(rPlayer, cPlayer) != EMPTY);
      m_dungeon->addPlayer(rPlayer, cPlayer);
    
    if(level < 4)
    {
        // Add stairCase
          int rStaircase;
          int cStaircase;
          do
          {
              rStaircase = randInt(1, rows);
              cStaircase = randInt(1, cols);
          } while (m_dungeon->getCellStatus(rStaircase, cStaircase) != EMPTY);
          m_dungeon->addStaircase(rStaircase, cStaircase);
    }
    
    if(level == 4)
    {
        // Add idol
        int rIdol;
        int cIdol;
        do
        {
            rIdol = randInt(1, rows);
            cIdol = randInt(1, cols);
        } while (m_dungeon->getCellStatus(rIdol, cIdol) != EMPTY);
        m_dungeon->addIdol(rIdol, cIdol);
        
    }
    
    //add only one goblin
    /*
    int rMonster;
    int cMonster;
    do
    {
        rMonster = randInt(1, rows);
        cMonster = randInt(1, cols);
    } while (m_dungeon->getCellStatus(rMonster, cMonster) != EMPTY);
    m_dungeon->addGoblin(rMonster, cMonster);
    */
    
    
    //Add monsters
    int M = randInt(2, 5*(level+1)+1);
    
    //if on levels 0-2
    if(level < 3)
    {
        for(int i = 0; i < M; i++)
        {
            int rMonster;
            int cMonster;
            do
            {
                rMonster = randInt(1, rows);
                cMonster = randInt(1, cols);
            } while (m_dungeon->getCellStatus(rMonster, cMonster) != EMPTY);
            
            int rando = randInt(1, 2);
            switch (rando) {
                case 1:
                    m_dungeon->addGoblin(rMonster, cMonster);
                    break;
                case 2:
                    m_dungeon->addSnakewoman(rMonster, cMonster);
                    break;
                default:
                    break;
            }
        }
        
    }
    
    //if on level 3 or higher
    else if(level >= 3)
    {
        for(int i = 0; i < M; i++)
        {
            int rMonster;
            int cMonster;
            do
            {
                rMonster = randInt(1, rows);
                cMonster = randInt(1, cols);
            } while (m_dungeon->getCellStatus(rMonster, cMonster) != EMPTY);
            
            int rando = randInt(1, 4);
            switch (rando) {
                case 1:
                    m_dungeon->addGoblin(rMonster, cMonster);
                    break;
                case 2:
                    m_dungeon->addSnakewoman(rMonster, cMonster);
                    break;
                case 3:
                    m_dungeon->addDragon(rMonster, cMonster);
                    break;
                case 4:
                    m_dungeon->addBogeyman(rMonster, cMonster);
                    break;
                default:
                    break;
            }
        }
        
    }
    
    
    //Add objects
    
    for(int i = 0; i < randInt(2, 3); i++)
    {
        int rObject;
        int cObject;
        do
        {
            rObject = randInt(1, rows);
            cObject = randInt(1, cols);
        } while (m_dungeon->getCellStatus(rObject, cObject) != EMPTY);
        
        int rando = randInt(1, 7);
        switch (rando) {
            case 1:
                m_dungeon->addObject(rObject, cObject, MACE);
                break;
            case 2:
                m_dungeon->addObject(rObject, cObject, SHORTSWORD);
                break;
            case 3:
                m_dungeon->addObject(rObject, cObject, LONGSWORD);
                break;
            case 4:
                m_dungeon->addObject(rObject, cObject, ARMOR);
                break;
            case 5:
                m_dungeon->addObject(rObject, cObject, STRENGTH);
                break;
            case 6:
                m_dungeon->addObject(rObject, cObject, HEALTH);
                break;
            case 7:
                m_dungeon->addObject(rObject, cObject, DEXTERITY);
                break;
            default:
                break;
        }
    }
}
string Game:: takePlayerTurn()
{
    for(;;)
    {
        Player* player = m_dungeon->player();
        char playerMove = getCharacter();
        
        switch (playerMove) {
            case 'c':
                player->setHealth(50);
                player->setStrength(9);
                return "";
                break;
            case 'g':
                return player->addToInventory();
                break;
            case 'i':
                player->printInventory();
                getCharacter();
                return "";
                break;
            case 'w':
                return player->weaponSelector();
                break;
            case 'r':
                return player->scrollSelector();
                break;
            case '>':
                return player->staircase();
                break;
            case ARROW_UP:
            case ARROW_RIGHT:
            case ARROW_DOWN:
            case ARROW_LEFT:
                return player->move(playerMove);
                break;
            default:
                break;
        }
        return "";
    }
}

void Game:: play()
{
    vector<Object*> boi;
    newLevel(18, 70, 0, boi, nullptr, 20, 2, 2, 2, 20);
    m_dungeon->display("");
    Player* player = m_dungeon->player();
    if (player == nullptr)
        return;
    while (!player->isDead())
    {
        string msg = takePlayerTurn();
        if(msg == "next level")
        {
            newLevel(18, 70, m_level+1, player->getInventory(), player->getCurrentWeapon(), player->getHealth(), player->getArmor(), player->getStrength(), player->getDexterity(), player->getHealthCap());
            m_dungeon->display("");
            player = m_dungeon->player();
        }
        m_dungeon->moveMonsters();
        m_dungeon->player()->updateStats();
        m_dungeon->display(msg);
        if (player->isDead())
            break;
    }
    cout<<"Press q to quit"<<endl;
    cout<<endl;
    
    char input = getCharacter();
    if(input == 'q')
        exit(0);
}
