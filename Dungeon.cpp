//
//  Dungeon.cpp
//  MiniRogue
//
//  Created by Dom's Mac on 5/12/20.
//  Copyright © 2020 Dom's Mac. All rights reserved.
//

#include <iostream>
#include <random>
using namespace std;

#include "utilities.h"
#include "Dungeon.h"
#include "Player.h"
#include "Monster.h"
#include "Object.h"

//constructor

Dungeon :: Dungeon(int nRows, int nCols, int level, const vector<Object*> & inventory, Object* currentWeapon, int health, int armor, int strength, int dexterity, int healthCap)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
             << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    
    
    m_player = nullptr;
    m_inventory = inventory;
    m_currentWeapon = currentWeapon;
    m_playerHealth = health;
    m_playerArmor = armor;
    m_playerStrength = strength;
    m_playerDexterity = dexterity;
    m_playerHealthCap = healthCap;
    
    m_level = level;
    m_totalDamage = 0;
    
    
    int rando = randInt(3, 5);
        switch (rando)
        {
            case 3:
                makeThreeRooms();
                break;
            case 4:
                makeFourRooms();
                break;
            case 5:
                makeFiveRooms();
                break;
            default:
                break;
        }
    /*
    //mini-game
    
    for(int i = 1; i <= m_rows; i++)
    {
        for(int j = 1; j <= m_cols; j++)
        {
            m_grid[i][j] = WALL;
        }
    }
    
    for(int i = 0; i <= 2; i++)
    {
        m_grid[i][0] = EMPTY;
    }
    */
}

//destructor
Dungeon :: ~Dungeon()
{
    delete m_player;
}

//Accessors

Player* Dungeon::player() const
{
    return m_player;
}

vector<Object*> Dungeon:: objects() const
{
    return m_objects;
}

vector<Monster*> Dungeon:: monsters() const
{
    return m_monsters;
}

string Dungeon:: monsterTypeAtCoord(int r, int c) const
{
    for (int i = 0; i < m_monsters.size(); i++)
    {
        if(m_monsters[i]->row() == r && m_monsters[i]->col() == c)
        {
            return m_monsters[i]->getType();
        }
    }
    return "boi";
}

int Dungeon::rows() const
{
    return m_rows;
}

int Dungeon::cols() const
{
    return m_cols;
}

int Dungeon::getCellStatus(int r, int c) const
{
    checkPos(r, c, "Dungeon::getCellStatus");
    return m_grid[r-1][c-1];
}

int Dungeon:: monsterDefenderPointsAtCoord(int r, int c) const
{
    for (int i = 0; i < m_monsters.size(); i++)
    {
        if(m_monsters[i]->row() == r && m_monsters[i]->col() == c)
        {
            return m_monsters[i]->defenderPoints();
        }
    }
    return 0;
}

int Dungeon:: monsterHealthPointsAtCoord(int r, int c) const
{
    for (int i = 0; i < m_monsters.size(); i++)
    {
        if(m_monsters[i]->row() == r && m_monsters[i]->col() == c)
        {
            return m_monsters[i]->getHealth();
        }
    }
    return 0;
}

int Dungeon::totalDamage() const
{
    return m_totalDamage;
}

int Dungeon:: staircaseRow() const
{
    return m_StaircaseRow;
}

int Dungeon:: staircaseCol() const
{
    return m_StaircaseCol;
}

int Dungeon:: idolRow() const
{
    return m_IdolRow;
}

int Dungeon:: idolCol() const
{
    return m_IdolCol;
}

//Mutators
void Dungeon::setCellStatus(int r, int c, int status)
{
    checkPos(r, c, "Dungeon::setCellStatus");
    m_grid[r-1][c-1] = status;
}

bool Dungeon::addPlayer(int r, int c)
{
    if (!isPosInBounds(r, c))
        return false;

    // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;
    
    //indicate where player is spawned on the grid
    this->setCellStatus(r, c, PLAYER);

    m_player = new Player(this, r, c, m_inventory, m_currentWeapon, m_playerHealth, m_playerArmor, m_playerStrength, m_playerDexterity, m_playerHealthCap);
    return true;
}

bool Dungeon::addBogeyman(int r, int c)
{
    if (!isPosInBounds(r, c))
        return false;

    // Don't add a vampire on a spot with a player
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
        return false;
    
    //indicate where monster is spawned on the grid
    this->setCellStatus(r, c, MONSTER);
    
    m_monsters.push_back(new Bogeyman(this, r, c));
    return true;
}

bool Dungeon::addSnakewoman(int r, int c)
{
    if (!isPosInBounds(r, c))
        return false;

    // Don't add a monster on a spot with a player
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
        return false;
    
    //indicate where monster is spawned on the grid
    this->setCellStatus(r, c, MONSTER);
    
    //add monster to vector of monster pointers
    m_monsters.push_back(new Snakewoman(this, r, c));
    return true;
}

bool Dungeon::addGoblin(int r, int c)
{
    if (!isPosInBounds(r, c))
        return false;

    // Don't add a monster on a spot with a player
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
        return false;
    
    //indicate where monster is spawned on the grid
    this->setCellStatus(r, c, MONSTER);
    
    //add monster to vector of monster pointers
    m_monsters.push_back(new Goblin(this, r, c));
    return true;
}

bool Dungeon::addDragon(int r, int c)
{
    if (!isPosInBounds(r, c))
        return false;

    // Don't add a monster on a spot with a player
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
        return false;
    
    //indicate where monster is spawned on the grid
    this->setCellStatus(r, c, MONSTER);
    
    //add monster to vector of monster pointers
    m_monsters.push_back(new Dragon(this, r, c));
    return true;
    
}

bool Dungeon:: addStaircase(int r, int c)
{
    if (!isPosInBounds(r, c))
        return false;
    
    //indicate where staircase is spawned on the grid
    this->setCellStatus(r, c, OBJECT);
    m_StaircaseRow = r;
    m_StaircaseCol = c;
    return true;
}

bool Dungeon:: addIdol(int r, int c)
{
    if (!isPosInBounds(r, c))
        return false;
    
    //indicate where staircase is spawned on the grid
    this->setCellStatus(r, c, OBJECT);
    m_IdolRow = r;
    m_IdolCol = c;
    return true;
    
}

bool Dungeon::addObject(int r, int c, int type)
{
    if (!isPosInBounds(r, c))
        return false;
    
    //indicate where object is spawned on the grid
    this->setCellStatus(r, c, OBJECT);
    
    //add object to vector of object pointers depending on object type
    switch (type)
    {
        case MACE:
            m_objects.push_back(new Mace(this,r,c));
            break;
        case SHORTSWORD:
            m_objects.push_back(new ShortSword(this,r,c));
            break;
        case LONGSWORD:
            m_objects.push_back(new LongSword(this,r,c));
            break;
        case MAGICAXE:
            m_objects.push_back(new MagicAxe(this,r,c));
            break;
        case MAGICFANGSOFSLEEP:
            m_objects.push_back(new MagicFangsOfSleep(this,r,c));
            break;
        case TELEPORTATION:
            m_objects.push_back(new teleportationScroll(this, r, c));
            break;
        case ARMOR:
            m_objects.push_back(new armorScroll(this, r, c));
            break;
        case STRENGTH:
            m_objects.push_back(new strengthScroll(this, r, c));
            break;
        case HEALTH:
            m_objects.push_back(new healthScroll(this, r, c));
            break;
        case DEXTERITY:
            m_objects.push_back(new dexterityScroll(this, r, c));
            break;
        default:
            break;
    }
    return true;
}

void Dungeon::removeObject(int i)
{
    m_objects.erase(m_objects.begin() + i);
}
void Dungeon:: moveMonsters()
{
    //set total damage done to Player to zero at the beginning of each monster turn
    m_totalDamage = 0;
    
    //clear all monster messages
    m_MonsterMessage.clear();
    
    // Move all monsters
    for(vector<Monster*>:: iterator it = m_monsters.begin(); it != m_monsters.end();  )
    {
        //update Monster's health if player attacked the monster during the player's turn
        (*it)->updateMonsterStats();
        
        //if monster dies, it might drop an item    =====INCOMPLETE=====
        if((*it)->isDead())
        {
            //if monster is a snakewoman
            if((*it)->getGridChar() == 'S')
            {
                int rando = randInt(1, 3);
                if(rando == 1)
                    addObject((*it)->row(), (*it)->col(), MAGICFANGSOFSLEEP);
                else
                    setCellStatus((*it)->row(), (*it)->col(), EMPTY);
            }

            //if monster is a goblin
            else if((*it)->getGridChar() == 'G')
            {
                int rando = randInt(1, 3);
                if(rando == 1)
                {
                    int rand = randInt(1, 2);
                    if(rand == 1)
                        addObject((*it)->row(), (*it)->col(), MAGICFANGSOFSLEEP);
                    else
                        addObject((*it)->row(), (*it)->col(), MAGICAXE);
                }
                else
                    setCellStatus((*it)->row(), (*it)->col(), EMPTY);
            }
            
            else if((*it)->getGridChar() == 'B')
            {
                int rando = randInt(1, 10);
                if(rando == 1)
                {
                    addObject((*it)->row(), (*it)->col(), MAGICAXE);
                }
                else
                    setCellStatus((*it)->row(), (*it)->col(), EMPTY);
            }
            
            else if((*it)->getGridChar() == 'B')
            {
                int rando = randInt(1, 10);
                if(rando == 1)
                {
                    addObject((*it)->row(), (*it)->col(), MAGICAXE);
                }
                else
                    setCellStatus((*it)->row(), (*it)->col(), EMPTY);
            }
            
            else if((*it)->getGridChar() == 'D')
            {
                int rando = randInt(1, 10);
                if(rando == 1)
                {
                    addObject((*it)->row(), (*it)->col(), TELEPORTATION);
                }
                
                if(rando == 2)
                {
                    addObject((*it)->row(), (*it)->col(), ARMOR);
                }
                
                if(rando == 3)
                {
                    addObject((*it)->row(), (*it)->col(), STRENGTH);
                }
                
                if(rando == 4)
                {
                    addObject((*it)->row(), (*it)->col(), HEALTH);
                }
                
                if(rando == 5)
                {
                    addObject((*it)->row(), (*it)->col(), DEXTERITY);
                }
                
                else
                    setCellStatus((*it)->row(), (*it)->col(), EMPTY);
            }
            
            delete *it; //delete the iterator pointing to the dead monster
            it = m_monsters.erase(it);  //iterator points at next monster to move
        }
        
        //if monster not dead, check if a player is nearby
        else if((*it)->isPlayerNearby())
        {
            //if player nearby, attempt to attack the player
            int damagePoints = randInt(0, ((*it)->getStrength() + (*it)->getWeaponDamage()));
            int attack = randInt(1, (*it)->attackerPoints());
            int defend = randInt(1, m_player->defenderPoints());
            if (attack >= defend)
            {
                //attacker has hit defender
                if(m_player->getHealth() <= damagePoints)
                {
                    m_MonsterMessage.push_back("the " + (*it)->getType() + " " + (*it)->getActionString() + " Player dealing a final blow.");
                    m_totalDamage += damagePoints;
                    it++;
                }
                else
                {
                    m_MonsterMessage.push_back("the " + (*it)->getType() + " " + (*it)->getActionString() + " Player and hits.");
                    m_totalDamage += damagePoints;
                    it++;
                }
            }
            else
            {
                //attacker misses defender
                m_MonsterMessage.push_back("the " + (*it)->getType() + " " + (*it)->getActionString() + " Player and misses.");
                it++;
            }
        }
        
        //if monster not dead and player not nearby, move the monster
        else if(!(*it)->isPlayerNearby())
        {
            m_MonsterMessage.push_back("");
            (*it)->move();
            it++;
        }
    }
}

void Dungeon::display(string msg)
{
    clearScreen();
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    
    // Fill displayGrid with empty spaces and walls
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r-1][c-1] = (getCellStatus(r,c) == EMPTY ? ' ' : '#');
    
    //Fill display grid with staricase
    if(m_level < 4)
        displayGrid[m_StaircaseRow-1][m_StaircaseCol-1] = '>';
    
    //Fill display grid with idol
    if(m_level == 4)
        displayGrid[m_IdolRow-1][m_IdolCol-1] = '&';
    
    //Fill displayGrid with objects
    for(int i = 0; i < m_objects.size(); i++)
    {
        const Object* o = m_objects[i];
        displayGrid[o->row()-1][o->col()-1] = o->getGridChar();
    }
    
    // Indicate monster's position
    for(int i = 0; i < m_monsters.size(); i++)
    {
        const Monster* m = m_monsters[i];
        displayGrid[m->row()-1][m->col()-1] = m->getGridChar();
    }
    
    //Indicate player's position
    if (m_player != nullptr)
        displayGrid[m_player->row()-1][m_player->col()-1] = (m_player->isDead() ? 'X' : '@');

    //draw the grid
    clearScreen();
    for(int i = 1; i <= m_rows; i++)
    {
        for(int j = 1; j <= m_cols; j++)
        {
            cout<<displayGrid[i-1][j-1];
        }
        cout<<endl;
    }
    
    //write player info
    cout<<"Dungeon Level: "<<m_level<<", Hit points: "<< m_player->getHealth()<<", Armor: "<<m_player->getArmor()<<", Strength: "<<m_player->getStrength()<<", Dexterity: "<<m_player->getDexterity()<<endl;
    cout<<endl;
    
    // Write message
    if(msg != "")
        cout<<msg<<endl;
    else if(msg != "next level")
    {
        cout<<""<<endl;
    }
    else if(msg == "Congratulations, you won!")
    {
        cout<<msg<<endl;
        exit(1);
    }
    for(int i = 0; i < m_MonsterMessage.size(); i++)
    {
        if(m_MonsterMessage[i] != "")
        {
            cout<<m_MonsterMessage[i]<<endl;
        }
    }
}

bool Dungeon::isPosInBounds(int r, int c) const
{
    return (r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols);
}

void Dungeon::checkPos(int r, int c, string functionName) const
{
    if (!isPosInBounds(r, c))
    {
        cout << "***** " << "Invalid arena position (" << r << ","
             << c << ") in call to " << functionName << endl;
        exit(1);
    }
}


//==========CODE FOR GENERATING ROOMS AND CONNECTING CORRIDORS============

void Dungeon::makeThreeRooms()
{
    //fills entire dungeon with impenetrable walls '#'
    
    for(int i = 0; i < m_rows; i++)
    {
        for(int j = 0; j < m_cols; j++)
        {
            m_grid[i][j] = WALL;
        }
    }
    
    //making arrays to store coordinates of each room
    
    int arrTopLeftY[3]; //rows
    int arrTopLeftX[3]; //columns
    int widths[3];
    int lengths[3];
    
    //makes rooms
    
    int counter = 0;
    while(counter != 3)
    {
        int topLeftY = randInt(1, 10);
        int topLeftX = randInt(1,54);
        int width = randInt(4, 7);
        int length = randInt(7, 15);
        bool isRoomValid = true;
        
        
        //checks if rooms overlap
        
        for(int i = topLeftY; i < topLeftY + width; i++)
        {
            for(int j = topLeftX; j < topLeftX + length; j++)
            {
                if(m_grid[i][j] == EMPTY || m_grid[i+1][j] == EMPTY || m_grid[i-1][j] == EMPTY || m_grid[i][j+1] == EMPTY || m_grid[i][j-1] == EMPTY)
                {
                    isRoomValid = false;
                }
            }
        }
        
        //if valid make a new room
        
        if(isRoomValid)
        {
            for(int i = topLeftY; i < topLeftY + width; i++)
            {
                for(int j = topLeftX; j < topLeftX + length; j++ )
                {
                    m_grid[i][j] = EMPTY;
                }
            }
            
            arrTopLeftY[counter] = topLeftY;
            arrTopLeftX[counter] = topLeftX;
            widths[counter] = width;
            lengths[counter] = length;
            
            counter++;
        }
    }
    
    int midpointY0 = arrTopLeftY[0] + widths[0]/2;
    int midpointY1 = arrTopLeftY[1] + widths[1]/2;
    int midpointY2 = arrTopLeftY[2] + widths[2]/2;
    
    int midpointX0 = arrTopLeftX[0] + lengths[0]/2;
    int midpointX1 = arrTopLeftX[1] + lengths[1]/2;
    int midpointX2 = arrTopLeftX[2] + lengths[2]/2;
    
    //connecting room 0 with room 1
    
        if(midpointY1 > midpointY0)
        {
            for(int i = midpointY0; i < midpointY1+1; i++)
            {
                m_grid[i][midpointX0] = EMPTY;
            }
        }
        else
        {
            for(int i = midpointY1; i < midpointY0+1; i++)
            {
                m_grid[i][midpointX0] = EMPTY;
            }
        }
        //drawing corrdior in X direction
        if(midpointX1 > midpointX0)
        {
            for(int j = midpointX0; j < midpointX1+1; j++)
            {
                m_grid[midpointY1][j] = EMPTY;
            }
        }
        else
        {
            for(int j = midpointX1; j < midpointX0+1; j++)
            {
                m_grid[midpointY1][j] = EMPTY;
            }
        }
        
        //connecting room 1 with room 2
        
        //drawing corridor in Y direction
        if(midpointY2 > midpointY1)
        {
            for(int i = midpointY1; i < midpointY2+1; i++)
            {
                m_grid[i][midpointX1] = EMPTY;
            }
        }
        else
        {
            for(int i = midpointY2; i < midpointY1+1; i++)
            {
                m_grid[i][midpointX1] = EMPTY;
            }
        }
        
        //drawing corrdior in X direction
        if(midpointX2 > midpointX1)
        {
            for(int j = midpointX1; j < midpointX2+1; j++)
            {
                m_grid[midpointY2][j] = EMPTY;
            }
        }
        else
        {
            for(int j = midpointX2; j < midpointX1+1; j++)
            {
                m_grid[midpointY2][j] = EMPTY;
            }
        }
        
        //connecting room 0 with room 2
        
        //drawing corridor in Y direction
        if(midpointY2 > midpointY0)
        {
            for(int i = midpointY0; i < midpointY2+1; i++)
            {
                m_grid[i][midpointX0] = EMPTY;
            }
        }
        else
        {
            for(int i = midpointY2; i < midpointY0+1; i++)
            {
                m_grid[i][midpointX0] = EMPTY;
            }
        }
        //drawing corrdior in X direction
        if(midpointX2 > midpointX0)
        {
            for(int j = midpointX0; j < midpointX2+1; j++)
            {
                m_grid[midpointY2][j] = EMPTY;
            }
        }
        else
        {
            for(int j = midpointX2; j < midpointX0+1; j++)
            {
                m_grid[midpointY2][j] = EMPTY;
            }
        }
}

void Dungeon::makeFourRooms()
{
    //fills entire dungeon with impenetrable walls '#'
    
    for(int i = 0; i < m_rows; i++)
    {
        for(int j = 0; j < m_cols; j++)
        {
            m_grid[i][j] = WALL;
        }
    }
    
    //making arrays to store coordinates of each room
    
    int arrTopLeftY[4]; //rows
    int arrTopLeftX[4]; //columns
    int widths[4];
    int lengths[4];
    
    //makes rooms
    
    int counter = 0;
    while(counter != 4)
    {
        int topLeftY = randInt(1, 10);
        int topLeftX = randInt(1,54);
        int width = randInt(4, 7);
        int length = randInt(7, 15);
        bool isRoomValid = true;
        
        
        //checks if rooms overlap
        
        for(int i = topLeftY; i < topLeftY + width; i++)
        {
            for(int j = topLeftX; j < topLeftX + length; j++)
            {
                if(m_grid[i][j] == EMPTY || m_grid[i+1][j] == EMPTY || m_grid[i-1][j] == EMPTY || m_grid[i][j+1] == EMPTY || m_grid[i][j-1] == EMPTY)
                {
                    isRoomValid = false;
                }
            }
        }
        
        //if valid make a new room
        
        if(isRoomValid)
        {
            for(int i = topLeftY; i < topLeftY + width; i++)
            {
                for(int j = topLeftX; j < topLeftX + length; j++ )
                {
                    m_grid[i][j] = EMPTY;
                }
            }
            
            arrTopLeftY[counter] = topLeftY;
            arrTopLeftX[counter] = topLeftX;
            widths[counter] = width;
            lengths[counter] = length;
            
            counter++;
        }
    }
    
    int midpointY0 = arrTopLeftY[0] + widths[0]/2;
    int midpointY1 = arrTopLeftY[1] + widths[1]/2;
    int midpointY2 = arrTopLeftY[2] + widths[2]/2;
    int midpointY3 = arrTopLeftY[3] + widths[3]/2;
    
    int midpointX0 = arrTopLeftX[0] + lengths[0]/2;
    int midpointX1 = arrTopLeftX[1] + lengths[1]/2;
    int midpointX2 = arrTopLeftX[2] + lengths[2]/2;
    int midpointX3 = arrTopLeftX[3] + lengths[3]/2;
    
    //connecting room 0 with room 1
    
        if(midpointY1 > midpointY0)
        {
            for(int i = midpointY0; i < midpointY1+1; i++)
            {
                m_grid[i][midpointX0] = EMPTY;
            }
        }
        else
        {
            for(int i = midpointY1; i < midpointY0+1; i++)
            {
                m_grid[i][midpointX0] = EMPTY;
            }
        }
        //drawing corrdior in X direction
        if(midpointX1 > midpointX0)
        {
            for(int j = midpointX0; j < midpointX1+1; j++)
            {
                m_grid[midpointY1][j] = EMPTY;
            }
        }
        else
        {
            for(int j = midpointX1; j < midpointX0+1; j++)
            {
                m_grid[midpointY1][j] = EMPTY;
            }
        }
        
        //connecting room 1 with room 2
        
        //drawing corridor in Y direction
        if(midpointY2 > midpointY1)
        {
            for(int i = midpointY1; i < midpointY2+1; i++)
            {
                m_grid[i][midpointX1] = EMPTY;
            }
        }
        else
        {
            for(int i = midpointY2; i < midpointY1+1; i++)
            {
                m_grid[i][midpointX1] = EMPTY;
            }
        }
        
        //drawing corrdior in X direction
        if(midpointX2 > midpointX1)
        {
            for(int j = midpointX1; j < midpointX2+1; j++)
            {
                m_grid[midpointY2][j] = EMPTY;
            }
        }
        else
        {
            for(int j = midpointX2; j < midpointX1+1; j++)
            {
                m_grid[midpointY2][j] = EMPTY;
            }
        }
        
        //connecting room 0 with room 2
        
        //drawing corridor in Y direction
        if(midpointY2 > midpointY0)
        {
            for(int i = midpointY0; i < midpointY2+1; i++)
            {
                m_grid[i][midpointX0] = EMPTY;
            }
        }
        else
        {
            for(int i = midpointY2; i < midpointY0+1; i++)
            {
                m_grid[i][midpointX0] = EMPTY;
            }
        }
        //drawing corrdior in X direction
        if(midpointX2 > midpointX0)
        {
            for(int j = midpointX0; j < midpointX2+1; j++)
            {
                m_grid[midpointY2][j] = EMPTY;
            }
        }
        else
        {
            for(int j = midpointX2; j < midpointX0+1; j++)
            {
                m_grid[midpointY2][j] = EMPTY;
            }
        }
    
        //connecting room 0 with room 3
        
        //drawing corridor in Y direction
        if(midpointY3 > midpointY0)
        {
            for(int i = midpointY0; i < midpointY3+1; i++)
            {
                m_grid[i][midpointX0] = EMPTY;
            }
        }
        else
        {
            for(int i = midpointY3; i < midpointY0+1; i++)
            {
                m_grid[i][midpointX0] = EMPTY;
            }
        }
        //drawing corrdior in X direction
        if(midpointX3 > midpointX0)
        {
            for(int j = midpointX0; j < midpointX3+1; j++)
            {
                m_grid[midpointY3][j] = EMPTY;
            }
        }
        else
        {
            for(int j = midpointX3; j < midpointX0+1; j++)
            {
                m_grid[midpointY3][j] = EMPTY;
            }
        }
    
        //connecting room 1 with room 3
        
        //drawing corridor in Y direction
        if(midpointY3 > midpointY1)
        {
            for(int i = midpointY1; i < midpointY3+1; i++)
            {
                m_grid[i][midpointX1] = EMPTY;
            }
        }
        else
        {
            for(int i = midpointY3; i < midpointY1+1; i++)
            {
                m_grid[i][midpointX1] = EMPTY;
            }
        }
        
        //drawing corrdior in X direction
        if(midpointX3 > midpointX1)
        {
            for(int j = midpointX1; j < midpointX3+1; j++)
            {
                m_grid[midpointY3][j] = EMPTY;
            }
        }
        else
        {
            for(int j = midpointX3; j < midpointX1+1; j++)
            {
                m_grid[midpointY3][j] = EMPTY;
            }
        }
    
        //connecting room 2 with room 3
        
        //drawing corridor in Y direction
        if(midpointY3 > midpointY2)
        {
            for(int i = midpointY2; i < midpointY3+1; i++)
            {
                m_grid[i][midpointX2] = EMPTY;
            }
        }
        else
        {
            for(int i = midpointY3; i < midpointY2+1; i++)
            {
                m_grid[i][midpointX2] = EMPTY;
            }
        }
        
        //drawing corrdior in X direction
        if(midpointX3 > midpointX2)
        {
            for(int j = midpointX2; j < midpointX3+1; j++)
            {
                m_grid[midpointY3][j] = EMPTY;
            }
        }
        else
        {
            for(int j = midpointX3; j < midpointX2+1; j++)
            {
                m_grid[midpointY3][j] = EMPTY;
            }
        }
    
    
}

void Dungeon::makeFiveRooms()
{
    //fills entire dungeon with impenetrable walls '#'
    
    for(int i = 0; i < m_rows; i++)
    {
        for(int j = 0; j < m_cols; j++)
        {
            m_grid[i][j] = WALL;
        }
    }
    
    //making arrays to store coordinates of each room
    
    int arrTopLeftY[5]; //rows
    int arrTopLeftX[5]; //columns
    int widths[5];
    int lengths[5];
    
    //makes rooms
    
    int counter = 0;
    while(counter != 5)
    {
        int topLeftY = randInt(1, 10);
        int topLeftX = randInt(1,54);
        int width = randInt(4, 7);
        int length = randInt(7, 15);
        bool isRoomValid = true;
        
        
        //checks if rooms overlap
        
        for(int i = topLeftY; i < topLeftY + width; i++)
        {
            for(int j = topLeftX; j < topLeftX + length; j++)
            {
                if(m_grid[i][j] == EMPTY || m_grid[i+1][j] == EMPTY || m_grid[i-1][j] == EMPTY || m_grid[i][j+1] == EMPTY || m_grid[i][j-1] == EMPTY)
                {
                    isRoomValid = false;
                }
            }
        }
        
        //if valid make a new room
        
        if(isRoomValid)
        {
            for(int i = topLeftY; i < topLeftY + width; i++)
            {
                for(int j = topLeftX; j < topLeftX + length; j++ )
                {
                    m_grid[i][j] = EMPTY;
                }
            }
            
            arrTopLeftY[counter] = topLeftY;
            arrTopLeftX[counter] = topLeftX;
            widths[counter] = width;
            lengths[counter] = length;
            
            counter++;
        }
    }
    
    int midpointY0 = arrTopLeftY[0] + widths[0]/2;
    int midpointY1 = arrTopLeftY[1] + widths[1]/2;
    int midpointY2 = arrTopLeftY[2] + widths[2]/2;
    int midpointY3 = arrTopLeftY[3] + widths[3]/2;
    int midpointY4 = arrTopLeftY[4] + widths[4]/2;
    
    int midpointX0 = arrTopLeftX[0] + lengths[0]/2;
    int midpointX1 = arrTopLeftX[1] + lengths[1]/2;
    int midpointX2 = arrTopLeftX[2] + lengths[2]/2;
    int midpointX3 = arrTopLeftX[3] + lengths[3]/2;
    int midpointX4 = arrTopLeftX[4] + lengths[4]/2;
    
    //connecting room 0 with room 1
    
        if(midpointY1 > midpointY0)
        {
            for(int i = midpointY0; i < midpointY1+1; i++)
            {
                m_grid[i][midpointX0] = EMPTY;
            }
        }
        else
        {
            for(int i = midpointY1; i < midpointY0+1; i++)
            {
                m_grid[i][midpointX0] = EMPTY;
            }
        }
        //drawing corrdior in X direction
        if(midpointX1 > midpointX0)
        {
            for(int j = midpointX0; j < midpointX1+1; j++)
            {
                m_grid[midpointY1][j] = EMPTY;
            }
        }
        else
        {
            for(int j = midpointX1; j < midpointX0+1; j++)
            {
                m_grid[midpointY1][j] = EMPTY;
            }
        }
        
        //connecting room 1 with room 2
        
        //drawing corridor in Y direction
        if(midpointY2 > midpointY1)
        {
            for(int i = midpointY1; i < midpointY2+1; i++)
            {
                m_grid[i][midpointX1] = EMPTY;
            }
        }
        else
        {
            for(int i = midpointY2; i < midpointY1+1; i++)
            {
                m_grid[i][midpointX1] = EMPTY;
            }
        }
        
        //drawing corrdior in X direction
        if(midpointX2 > midpointX1)
        {
            for(int j = midpointX1; j < midpointX2+1; j++)
            {
                m_grid[midpointY2][j] = EMPTY;
            }
        }
        else
        {
            for(int j = midpointX2; j < midpointX1+1; j++)
            {
                m_grid[midpointY2][j] = EMPTY;
            }
        }
        
        //connecting room 0 with room 2
        
        //drawing corridor in Y direction
        if(midpointY2 > midpointY0)
        {
            for(int i = midpointY0; i < midpointY2+1; i++)
            {
                m_grid[i][midpointX0] = EMPTY;
            }
        }
        else
        {
            for(int i = midpointY2; i < midpointY0+1; i++)
            {
                m_grid[i][midpointX0] = EMPTY;
            }
        }
        //drawing corrdior in X direction
        if(midpointX2 > midpointX0)
        {
            for(int j = midpointX0; j < midpointX2+1; j++)
            {
                m_grid[midpointY2][j] = EMPTY;
            }
        }
        else
        {
            for(int j = midpointX2; j < midpointX0+1; j++)
            {
                m_grid[midpointY2][j] = EMPTY;
            }
        }
    
        //connecting room 0 with room 3
        
        //drawing corridor in Y direction
        if(midpointY3 > midpointY0)
        {
            for(int i = midpointY0; i < midpointY3+1; i++)
            {
                m_grid[i][midpointX0] = EMPTY;
            }
        }
        else
        {
            for(int i = midpointY3; i < midpointY0+1; i++)
            {
                m_grid[i][midpointX0] = EMPTY;
            }
        }
        //drawing corrdior in X direction
        if(midpointX3 > midpointX0)
        {
            for(int j = midpointX0; j < midpointX3+1; j++)
            {
                m_grid[midpointY3][j] = EMPTY;
            }
        }
        else
        {
            for(int j = midpointX3; j < midpointX0+1; j++)
            {
                m_grid[midpointY3][j] = EMPTY;
            }
        }
    
        //connecting room 1 with room 3
        
        //drawing corridor in Y direction
        if(midpointY3 > midpointY1)
        {
            for(int i = midpointY1; i < midpointY3+1; i++)
            {
                m_grid[i][midpointX1] = EMPTY;
            }
        }
        else
        {
            for(int i = midpointY3; i < midpointY1+1; i++)
            {
                m_grid[i][midpointX1] = EMPTY;
            }
        }
        
        //drawing corrdior in X direction
        if(midpointX3 > midpointX1)
        {
            for(int j = midpointX1; j < midpointX3+1; j++)
            {
                m_grid[midpointY3][j] = EMPTY;
            }
        }
        else
        {
            for(int j = midpointX3; j < midpointX1+1; j++)
            {
                m_grid[midpointY3][j] = EMPTY;
            }
        }
    
        //connecting room 2 with room 3
        
        //drawing corridor in Y direction
        if(midpointY3 > midpointY2)
        {
            for(int i = midpointY2; i < midpointY3+1; i++)
            {
                m_grid[i][midpointX2] = EMPTY;
            }
        }
        else
        {
            for(int i = midpointY3; i < midpointY2+1; i++)
            {
                m_grid[i][midpointX2] = EMPTY;
            }
        }
        
        //drawing corrdior in X direction
        if(midpointX3 > midpointX2)
        {
            for(int j = midpointX2; j < midpointX3+1; j++)
            {
                m_grid[midpointY3][j] = EMPTY;
            }
        }
        else
        {
            for(int j = midpointX3; j < midpointX2+1; j++)
            {
                m_grid[midpointY3][j] = EMPTY;
            }
        }
    
    //connecting room 0 with room 4
    
        if(midpointY4 > midpointY0)
        {
            for(int i = midpointY0; i < midpointY4+1; i++)
            {
                m_grid[i][midpointX0] = EMPTY;
            }
        }
        else
        {
            for(int i = midpointY4; i < midpointY0+1; i++)
            {
                m_grid[i][midpointX0] = EMPTY;
            }
        }
        //drawing corrdior in X direction
        if(midpointX4 > midpointX0)
        {
            for(int j = midpointX0; j < midpointX4+1; j++)
            {
                m_grid[midpointY4][j] = EMPTY;
            }
        }
        else
        {
            for(int j = midpointX4; j < midpointX0+1; j++)
            {
                m_grid[midpointY4][j] = EMPTY;
            }
        }
        
        //connecting room 1 with room 4
        
        //drawing corridor in Y direction
        if(midpointY4 > midpointY1)
        {
            for(int i = midpointY1; i < midpointY4+1; i++)
            {
                m_grid[i][midpointX1] = EMPTY;
            }
        }
        else
        {
            for(int i = midpointY4; i < midpointY1+1; i++)
            {
                m_grid[i][midpointX1] = EMPTY;
            }
        }
        
        //drawing corrdior in X direction
        if(midpointX4 > midpointX1)
        {
            for(int j = midpointX1; j < midpointX4+1; j++)
            {
                m_grid[midpointY4][j] = EMPTY;
            }
        }
        else
        {
            for(int j = midpointX4; j < midpointX1+1; j++)
            {
                m_grid[midpointY4][j] = EMPTY;
            }
        }
    
        //connecting room 2 with room 4
        
        //drawing corridor in Y direction
        if(midpointY4 > midpointY2)
        {
            for(int i = midpointY2; i < midpointY4+1; i++)
            {
                m_grid[i][midpointX2] = EMPTY;
            }
        }
        else
        {
            for(int i = midpointY4; i < midpointY2+1; i++)
            {
                m_grid[i][midpointX2] = EMPTY;
            }
        }
        
        //drawing corrdior in X direction
        if(midpointX4 > midpointX2)
        {
            for(int j = midpointX2; j < midpointX4+1; j++)
            {
                m_grid[midpointY4][j] = EMPTY;
            }
        }
        else
        {
            for(int j = midpointX4; j < midpointX2+1; j++)
            {
                m_grid[midpointY4][j] = EMPTY;
            }
        }
    
    //connecting room 3 with room 4
    
    //drawing corridor in Y direction
    if(midpointY3 > midpointY4)
    {
        for(int i = midpointY4; i < midpointY3+1; i++)
        {
            m_grid[i][midpointX4] = EMPTY;
        }
    }
    else
    {
        for(int i = midpointY3; i < midpointY4+1; i++)
        {
            m_grid[i][midpointX4] = EMPTY;
        }
    }
    
    //drawing corrdior in X direction
    if(midpointX3 > midpointX4)
    {
        for(int j = midpointX4; j < midpointX3+1; j++)
        {
            m_grid[midpointY3][j] = EMPTY;
        }
    }
    else
    {
        for(int j = midpointX3; j < midpointX4+1; j++)
        {
            m_grid[midpointY3][j] = EMPTY;
        }
    }
}


