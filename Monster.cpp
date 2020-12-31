//
//  Monster.cpp
//  MiniRogue
//
//  Created by Dom's Mac on 5/14/20.
//  Copyright © 2020 Dom's Mac. All rights reserved.
//
#include <iostream>
#include <cstdlib>
using namespace std;

#include "Monster.h"
#include "Dungeon.h"
#include "Player.h"
#include "utilities.h"


Monster::Monster(Dungeon* ap, int r, int c, int health, int armor, int strength, int dexterity, char gridChar)
{
    if (ap == nullptr)
    {
        cout << "***** A monster must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Monster created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    m_dungeon = ap;
    m_row = r;
    m_col = c;
    m_health = health;
    m_armor = armor;
    m_strength = strength;
    m_dexterity = dexterity;
    m_gridChar = gridChar;
    m_type = "Monster";
    m_actionString = "attacks";
    m_weaponDamage = 0;
    m_damagePoints = 0;
    m_attackerPoints = 0;
    m_defenderPoints = 0;
}

Monster::~Monster()
{
    
}

int Monster::row() const
{
    return m_row;
}

int Monster::col() const
{
    return m_col;
}

int Monster::getGridChar() const
{
    return m_gridChar;
}

string Monster:: getType() const
{
    return m_type;
}

string Monster:: getActionString() const
{
    return m_actionString;
}

int Monster:: getHealth() const
{
    return m_health;
}

int Monster:: getStrength() const
{
    return m_strength;
}

bool Monster::isPlayerNearby() const
{
    if((m_dungeon->player()->row() == m_row-1) && (m_dungeon->player()->col() == m_col))
    {
        return true;
    }
    
    else if((m_dungeon->player()->row() == m_row+1) && (m_dungeon->player()->col() == m_col))
    {
        return true;
    }
    
    else if((m_dungeon->player()->row()== m_row) && (m_dungeon->player()->col() == m_col-1))
    {
        return true;
    }
    
    else if((m_dungeon->player()->row()== m_row) && (m_dungeon->player()->col() == m_col+1))
    {
        return true;
    }
    else
        return false;
}

int Monster:: getWeaponDamage() const
{
    return m_weaponDamage;
}

/*int Monster:: damagePoints() const
{
    return m_damagePoints;
}*/

int Monster:: attackerPoints() const
{
    return m_attackerPoints;
}

int Monster:: defenderPoints() const
{
    return m_defenderPoints;
}

bool Monster::isDead() const
{
    if(m_health <= 0)
        return true;
    else
        return false;
}

void Monster::updateMonsterStats() //========INCOMPLETE==========
{
    //if player attacks (row,col) which monster is on 
    if(m_dungeon->player()->rowAttacked() == m_row && m_dungeon->player()->colAttacked() == m_col)
    {
        m_health = m_health - (m_dungeon->player()->damagePoints());
    }
}

bool Monster:: attemptMove(const Dungeon& a, char dir, int& r, int& c)
{
    int rnew = r;
    int cnew = c;
    switch (dir)
    {
        case ARROW_UP:
            if (r <= 1 || a.getCellStatus(r-1, c) == WALL || a.getCellStatus(r-1, c) == MONSTER || a.getCellStatus(r-1, c) == PLAYER)
                return false;
            else
            {
                rnew--;
            }
                
            break;
        case ARROW_RIGHT:
            if (c >= a.cols() || a.getCellStatus(r, c+1) == WALL || a.getCellStatus(r, c+1) == MONSTER || a.getCellStatus(r, c+1) == PLAYER)
                return false;
            else
            {
                cnew++;
            }
            break;
        case ARROW_DOWN:
            if (r >= a.rows() || a.getCellStatus(r+1, c) == WALL || a.getCellStatus(r+1, c) == MONSTER || a.getCellStatus(r+1, c) == PLAYER)
                return false;
            else
            {
                rnew++;
            }
            break;
        case ARROW_LEFT:
                if (c <= 1 || a.getCellStatus(r, c-1) == WALL || a.getCellStatus(r, c-1) == MONSTER || a.getCellStatus(r, c-1) == PLAYER)
                return false;
            else
            {
                cnew--;
            }
            break;
    }
    r = rnew;
    c = cnew;
    return true;
}

Bogeyman::Bogeyman(Dungeon* ap, int r, int c): Monster(ap, r, c, randInt(5, 10), 2, randInt(2, 3), randInt(2, 3), 'B')
{
    m_type = "Bogeyman";
    m_actionString = "slashes short sword at";
    m_weaponDamage = 2;
    m_damagePoints = randInt(0, (m_strength + 2) - 1);
    m_attackerPoints = m_dexterity; //short swords provide no dexterity bonus
    m_defenderPoints = m_dexterity + m_armor;
}

void Bogeyman::move()
{
    if(isPlayerNearby())
    {
        //attack
        return;
    }
    
    Player* player = m_dungeon->player();
    
    int rowDistance = abs(m_row-player->row());
    int colDistance = abs(m_col-player->col());
    
    if(rowDistance <= 5 && colDistance <= 5)
    {
        
        if((player->row() - m_row < 0) && (attemptMove(*m_dungeon, ARROW_UP, m_row, m_col) == true))
        {
            m_dungeon->setCellStatus(m_row+1, m_col, EMPTY);
            m_dungeon->setCellStatus(m_row, m_col, MONSTER);
        }
        
        else if ((player->col() - m_col > 0) && (attemptMove(*m_dungeon, ARROW_RIGHT, m_row, m_col) == true))
        {
            m_dungeon->setCellStatus(m_row, m_col-1, EMPTY);
            m_dungeon->setCellStatus(m_row, m_col, MONSTER);
        }
        
        else if ((player->row() - m_row > 0 ) && (attemptMove(*m_dungeon, ARROW_DOWN, m_row, m_col) == true))
        {
            m_dungeon->setCellStatus(m_row-1, m_col, EMPTY);
            m_dungeon->setCellStatus(m_row, m_col, MONSTER);
        }
        
        else if((player->col() - m_col < 0) && (attemptMove(*m_dungeon, ARROW_LEFT, m_row, m_col) == true))
        {
            m_dungeon->setCellStatus(m_row, m_col+1, EMPTY);
            m_dungeon->setCellStatus(m_row, m_col, MONSTER);
        }
    }
}

Snakewoman::Snakewoman(Dungeon* ap, int r, int c): Monster(ap, r, c, randInt(3, 6), 3, 2, 3, 'S')
{
    m_type = "Snakewoman";
    m_actionString = "strikes magic fangs of sleep at";
    m_weaponDamage = 2;
    m_damagePoints = randInt(0, (m_strength + 2) - 1);
    m_attackerPoints = m_dexterity + 3;
    m_defenderPoints = m_dexterity + m_armor;
}
void Snakewoman::move()
{
    if(isPlayerNearby())
    {
        //attack
        return;
    }
    
    Player* player = m_dungeon->player();
    
    int rowDistance = abs(m_row-player->row());
    int colDistance = abs(m_col-player->col());
    
    if(rowDistance <= 3 && colDistance <= 3)
    {
        
        if((player->row() - m_row < 0) && (attemptMove(*m_dungeon, ARROW_UP, m_row, m_col) == true))
        {
            m_dungeon->setCellStatus(m_row+1, m_col, EMPTY);
            m_dungeon->setCellStatus(m_row, m_col, MONSTER);
        }
        
        else if ((player->col() - m_col > 0) && (attemptMove(*m_dungeon, ARROW_RIGHT, m_row, m_col) == true))
        {
            m_dungeon->setCellStatus(m_row, m_col-1, EMPTY);
            m_dungeon->setCellStatus(m_row, m_col, MONSTER);
        }
        
        else if ((player->row() - m_row > 0 ) && (attemptMove(*m_dungeon, ARROW_DOWN, m_row, m_col) == true))
        {
            m_dungeon->setCellStatus(m_row-1, m_col, EMPTY);
            m_dungeon->setCellStatus(m_row, m_col, MONSTER);
        }
        
        else if((player->col() - m_col < 0) && (attemptMove(*m_dungeon, ARROW_LEFT, m_row, m_col) == true))
        {
            m_dungeon->setCellStatus(m_row, m_col+1, EMPTY);
            m_dungeon->setCellStatus(m_row, m_col, MONSTER);
        }
    }
}

Dragon::Dragon(Dungeon* ap, int r, int c): Monster(ap, r, c, randInt(20, 25), 4, 4, 4, 'D')
{
    m_type = "Dragon";
    m_actionString = "swings long sword at";
    m_weaponDamage = 4;
    m_damagePoints = randInt(0, (m_strength + 4) - 1);
    m_attackerPoints = m_dexterity + 2;
    m_defenderPoints = m_dexterity + m_armor;
}

void Dragon::move()
{
    if(isPlayerNearby())
    {
        //attack
        return;
    }
}


Goblin::Goblin(Dungeon* ap, int r, int c): Monster(ap, r, c, randInt(15, 20), 1, 3, 1, 'G')
{
    m_type = "Goblin";
    m_actionString = "slashes short sword at";
    m_weaponDamage = 2;
    m_damagePoints = randInt(0, (m_strength + 2) - 1);
    m_attackerPoints = m_dexterity; //short swords provide no dexterity bonus
    m_defenderPoints = m_dexterity + m_armor;
}

bool Goblin:: canWalkOnSpot(int r, int c)
{
    if(m_dungeon->getCellStatus(r, c) == WALL)
    {
        return false;
    }
    
    if(abs(r-m_row) > 15 || abs(c-m_col) > 15)
    {
        return false;
    }
    
    for(int i = 0; i < m_dungeon->monsters().size(); i++)
    {
        if(r == m_dungeon->monsters()[i]->row() && c == m_dungeon->monsters()[i]->col())
        {
            return false;
        }
    }
    
    for(int i = 0; i < spotsEncountered.size(); i++)
    {
        if(spotsEncountered[i].row == r && spotsEncountered[i].col == c)
        {
            return false;
        }
    }
    
    return true;
}

void Goblin::markEncountered()
{
    for(int i = 0; i < spotsEncountered.size(); i++)
    {
        m_dungeon->setCellStatus(spotsEncountered[i].row, spotsEncountered[i].col, WALL);
    }
}

bool Goblin::canMove(int depth, int &endRow, int &endCol)
{
    if (coordQueue.empty())
    {
        return false;
    }
    
    Coord pl = coordQueue.front();
    
    int playerRow = pl.row;
    int playerCol = pl.col;
    
    int goblinRow = m_row;
    int goblinCol = m_col;
    
    if( depth <= 0)
    {
        //didn't solve maze in less than 15 steps
        return false;
    }
    
    if (coordQueue.empty())
    {
        return false;
    }
    
    if((abs(goblinCol-playerCol) == 1 && (goblinRow == playerRow)) || ((abs(goblinRow-playerRow) == 1) && goblinCol == playerCol))
    {
        m_dungeon->setCellStatus(goblinRow, goblinCol, EMPTY);
        endRow = playerRow;
        endCol = playerCol;
        return true;
    }
    
    else
    {
        coordQueue.pop();
        depth--;
        
        //SOUTH
        if(canWalkOnSpot(playerRow+1, playerCol))
        {
            coordQueue.push(Coord(playerRow+1, playerCol));
            spotsEncountered.push_back(Coord(playerRow+1, playerCol));
        }
        
        //WEST
        if(canWalkOnSpot(playerRow, playerCol-1))
        {
            coordQueue.push(Coord(playerRow, playerCol-1));
            spotsEncountered.push_back(Coord(playerRow, playerCol-1));
        }
        
        //NORTH
        if(canWalkOnSpot(playerRow-1, playerCol))
        {
            coordQueue.push(Coord(playerRow-1, playerCol));
            spotsEncountered.push_back(Coord(playerRow-1, playerCol));
        }
        
        //EAST
        if(canWalkOnSpot(playerRow, playerCol+1))
        {
            coordQueue.push(Coord(playerRow, playerCol-1));
            spotsEncountered.push_back(Coord(playerRow, playerCol+1));
        }
        
        else    //we cannot move anywhere
        {}
        
        return canMove(depth, endRow, endCol);
    }
}

void Goblin::move()
{
    if(isPlayerNearby())
    {
        //attack
        return;
    }
    
    while(!coordQueue.empty())
    {
        coordQueue.pop();
    }
    
    while(!spotsEncountered.empty())
    {
        spotsEncountered.pop_back();
    }
    
    Coord player(m_dungeon->player()->row(), m_dungeon->player()->col());
    
    int depth = 15;
    coordQueue.push(player);
    int playerRow = m_dungeon->player()->row();
    int playerCol = m_dungeon->player()->col();
    if(canMove(depth, playerRow, playerCol))
    {
        while(!spotsEncountered.empty())
        {
            spotsEncountered.pop_back();
        }
        
        if(canWalkOnSpot(playerRow, playerCol))
        {
            m_row = playerRow;
            m_col = playerCol;
            m_dungeon->setCellStatus(m_row, m_col, MONSTER);
        }
    }
    //markEncountered();
}
