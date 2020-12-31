//
//  Player.cpp
//  MiniRogue
//
//  Created by Dom's Mac on 5/13/20.
//  Copyright © 2020 Dom's Mac. All rights reserved.
//

#include "Player.h"
#include "Dungeon.h"
#include "Object.h"
#include "utilities.h"

#include <iostream>
#include <cstdlib>

using namespace std;

Player::Player(Dungeon* ap, int r, int c, vector<Object*> inventory, Object* currentWeapon, int health, int armor, int strength, int dexterity, int healthCap)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
    m_dungeon = ap;
    m_row = r;
    m_col = c;
    
    m_health = health;
    m_armor = armor;
    m_strength = strength;
    m_dexterity = dexterity;
    m_healthCap = healthCap;
    
    m_rowAttacked = 0;
    m_colAttacked = 0;
    
    if(inventory.empty())
    {
        m_inventory.push_back(new ShortSword(ap, 0, 0));
    }
    else
        m_inventory = inventory;

    if(currentWeapon == nullptr)
        m_currentWeapon = m_inventory[0];
    else
    {
        if(currentWeapon->getType() == "mace")
        {
            m_weaponDamage = 2;
            m_actionString = "swings mace at";
        }
        else if(currentWeapon->getType() == "short sword")
        {
            m_weaponDamage = 2;
            m_actionString = "slashes short sword at";
        }
        else if(currentWeapon->getType() == "long sword")
        {
            m_weaponDamage = 4;
            m_actionString = "swings long sword at";
        }
        else if(currentWeapon->getType() == "magic axe")
        {
            m_weaponDamage = 5;
            m_actionString = "swings magic axe at";
        }
        else if(currentWeapon->getType() == "magic fangs of sleep")
        {
            m_weaponDamage = 2;
            m_actionString = "strikes magic fangs of sleep at";
        }
    }
    
    m_weaponDamage = 2;
    m_actionString = "slashes short sword at";
    m_attackerPoints = m_dexterity;
    m_defenderPoints = m_dexterity + m_armor;
    m_dead = false;
}

vector<Object*> Player:: getInventory() const
{
    return m_inventory;
}

Object* Player:: getCurrentWeapon() const
{
    return m_currentWeapon;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

int Player:: getHealth() const
{
    return m_health;
}

int Player:: getArmor() const
{
    return m_armor;
}

int Player:: getStrength() const
{
    return m_strength;
}

int Player:: getDexterity() const
{
    return m_dexterity;
}

int Player:: getHealthCap() const
{
    return m_healthCap;
}

int Player:: rowAttacked() const
{
    return m_rowAttacked;
}

int Player:: colAttacked() const
{
    return m_colAttacked;
}

int Player :: damagePoints() const
{
    return m_damagePoints;
}

int Player:: attackerPoints() const
{
    return m_attackerPoints;
}

int Player:: defenderPoints() const
{
    return m_defenderPoints;
}

bool Player::isDead() const
{
    return m_dead;
}

string Player::move(char dir)
{
    if (attemptMove(*m_dungeon, dir, m_row, m_col))
        {
            switch (dir)
            {
                case ARROW_UP:
                    m_dungeon->setCellStatus(m_row+1, m_col, EMPTY);
                    m_dungeon->setCellStatus(m_row, m_col, PLAYER);
                    m_rowAttacked = 0;
                    m_colAttacked = 0;
                    break;
                case ARROW_RIGHT:
                    m_dungeon->setCellStatus(m_row, m_col-1, EMPTY);
                    m_dungeon->setCellStatus(m_row, m_col, MONSTER);
                    m_rowAttacked = 0;
                    m_colAttacked = 0;
                    break;
                case ARROW_DOWN:
                    m_dungeon->setCellStatus(m_row-1, m_col, EMPTY);
                    m_dungeon->setCellStatus(m_row, m_col, MONSTER);
                    m_rowAttacked = 0;
                    m_colAttacked = 0;
                    break;
                case ARROW_LEFT:
                    m_dungeon->setCellStatus(m_row, m_col+1, EMPTY);
                    m_dungeon->setCellStatus(m_row, m_col, MONSTER);
                    m_rowAttacked = 0;
                    m_colAttacked = 0;
                    break;
            }
        }
        else if(!attemptMove(*m_dungeon, dir, m_row, m_col))
        {
            switch (dir)
            {
                case ARROW_UP:
                    if(m_dungeon->getCellStatus(m_row-1, m_col) == MONSTER)
                    {
                        string mon = m_dungeon->monsterTypeAtCoord(m_row-1, m_col); //gets name of monster at the coordinate player is attacking
                        m_damagePoints = randInt(0, (m_strength + m_weaponDamage) - 1);
                        int attack = randInt(1, m_attackerPoints);
                        int defend = randInt(1, m_dungeon->monsterDefenderPointsAtCoord(m_row-1, m_col));
                        if (attack >= defend)
                        {
                            //attacker has hit defender
                            m_rowAttacked = m_row-1;
                            m_colAttacked = m_col;
                            if(m_dungeon->monsterHealthPointsAtCoord(m_row-1, m_col) <= m_damagePoints)
                            {
                                return "Player " + m_actionString + " " + mon + " dealing a final blow.";
                            }
                            else
                                return "Player " + m_actionString + " " + mon + " and hits.";
                            
                        }
                        else
                        {
                            //attacker misses defender
                            m_rowAttacked = 0;
                            m_colAttacked = 0;
                            return "Player " + m_actionString + " " + mon + " and misses.";
                        }
                    }
                    else
                    {
                        m_rowAttacked = 0;
                        m_colAttacked = 0;
                    }
                    break;
                case ARROW_RIGHT:
                    if(m_dungeon->getCellStatus(m_row, m_col+1) == MONSTER)
                    {
                        string mon = m_dungeon->monsterTypeAtCoord(m_row, m_col+1); //gets name of monster at the coordinate player is attacking
                        m_damagePoints = randInt(0, (m_strength + m_weaponDamage) - 1);
                        int attack = randInt(1, m_attackerPoints);
                        int defend = randInt(1, m_dungeon->monsterDefenderPointsAtCoord(m_row, m_col+1));
                        if (attack >= defend)
                        {
                            //attacker has hit defender
                            m_rowAttacked = m_row;
                            m_colAttacked = m_col+1;
                            if(m_dungeon->monsterHealthPointsAtCoord(m_row, m_col+1) <= m_damagePoints)
                            {
                                return "Player " + m_actionString + " " + mon + " dealing a final blow.";
                            }
                            else
                                return "Player " + m_actionString + " " + mon + " and hits.";
                            
                        }
                        else
                        {
                            //attacker misses defender
                            m_rowAttacked = 0;
                            m_colAttacked = 0;
                            return "Player " + m_actionString + " " + mon + " and misses.";
                        }
                    }
                    else
                    {
                        m_rowAttacked = 0;
                        m_colAttacked = 0;
                    }
                    break;
                case ARROW_DOWN:
                    if(m_dungeon->getCellStatus(m_row+1, m_col) == MONSTER)
                    {
                        string mon = m_dungeon->monsterTypeAtCoord(m_row+1, m_col); //gets name of monster at the coordinate player is attacking
                        m_damagePoints = randInt(0, (m_strength + m_weaponDamage) - 1);
                        int attack = randInt(1, m_attackerPoints);
                        int defend = randInt(1, m_dungeon->monsterDefenderPointsAtCoord(m_row+1, m_col));
                        if (attack >= defend)
                        {
                            //attacker has hit defender
                            m_rowAttacked = m_row+1;
                            m_colAttacked = m_col;
                            if(m_dungeon->monsterHealthPointsAtCoord(m_row+1, m_col) <= m_damagePoints)
                            {
                                return "Player " + m_actionString + " " + mon + " dealing a final blow.";
                            }
                            else
                                return "Player " + m_actionString + " " + mon + " and hits.";
                            
                        }
                        else
                        {
                            //attacker misses defender
                            m_rowAttacked = 0;
                            m_colAttacked = 0;
                            return "Player " + m_actionString + " " + mon + " and misses.";
                        }
                    }
                    else
                    {
                        m_rowAttacked = 0;
                        m_colAttacked = 0;
                    }
                    break;
                case ARROW_LEFT:
                    if(m_dungeon->getCellStatus(m_row, m_col-1) == MONSTER)
                    {
                        string mon = m_dungeon->monsterTypeAtCoord(m_row, m_col-1); //gets name of monster at the coordinate player is attacking
                        m_damagePoints = randInt(0, (m_strength + m_weaponDamage) - 1);
                        int attack = randInt(1, m_attackerPoints);
                        int defend = randInt(1, m_dungeon->monsterDefenderPointsAtCoord(m_row, m_col-1));
                        if (attack >= defend)
                        {
                            //attacker has hit defender
                            m_rowAttacked = m_row;
                            m_colAttacked = m_col-1;
                            if(m_dungeon->monsterHealthPointsAtCoord(m_row, m_col-1) <= m_damagePoints)
                            {
                                return "Player " + m_actionString + " " + mon + " dealing a final blow.";
                            }
                            else
                                return "Player " + m_actionString + " " + mon + " and hits.";
                            
                        }
                        else
                        {
                            //attacker misses defender
                            m_rowAttacked = 0;
                            m_colAttacked = 0;
                            return "Player " + m_actionString + " " + mon + " and misses.";
                        }
                    }
                    else
                    {
                        //player does not attempt to move towards a monster, hits a wall
                        m_rowAttacked = 0;
                        m_colAttacked = 0;
                    }
                    break;
            }
        }
    return "";
}

void Player::updateStats()
{
    m_health = m_health - m_dungeon->totalDamage();
    if(m_health <= 0)
        m_dead = true;
}

string Player:: addToInventory()
{
    if(m_dungeon->idolRow() == m_row && m_dungeon->idolCol() == m_col)
    {
        return "Congratulations, you won!";
    }
    vector<Object*> Objects = m_dungeon->objects();

    for (int i = 0; i < Objects.size(); i++)
    {
        if(Objects[i]->row() == m_row && Objects[i]->col() == m_col)
        {
            if(m_inventory.size() <= 25)
            {
                Object* newObject = Objects[i];
                m_inventory.push_back(newObject);
                m_dungeon->removeObject(i);
                if((newObject->getType() == "scroll of teleportation" || newObject->getType() == "scroll of improve armor" || newObject->getType() == "scroll of raise strength"|| newObject->getType() == "scroll of enhance health"|| newObject->getType() == "scroll of enhance dexterity"))
                {
                    return "You pick up a scroll called " + newObject->getType();
                }
                
                else if((newObject->getType() == "mace" || newObject->getType() == "short sword" || newObject->getType() == "long sword"|| newObject->getType() == "magic axe"|| newObject->getType() == "magic fangs of sleep"))
                {
                    return "You pick up " + newObject->getType();
                }
            }
            
            if(m_inventory.size() > 25)
            {
                return "Your knapsack is full; you can't pick that up.";
            }
        }
    }
    return "";
}

string Player:: printInventory()
{
    clearScreen();
    cout<<"Inventory: "<<endl<<endl;
    int i = 0;
    
    while (i < m_inventory.size())
    {
        cout<<letters[i] <<". " <<m_inventory[i]->getType()<<endl;
        i++;
    }
    
    return "";
}

string Player:: weaponSelector()
{
    clearScreen();
    cout<<"Inventory: "<<endl<<endl;
    int i = 0;
    
    while (i < m_inventory.size())
    {
        cout<<letters[i] <<". " <<m_inventory[i]->getType()<<endl;
        i++;
    }
    
    char input = getCharacter();
    int k = 0;
    while (k <= 25)
    {
        if(input == letters[k])
            break;
        k++;
    }
    
    if((k+1) > m_inventory.size())
    {
        //cout<<"bigger than inventory size"<<endl;
        return "bigger than inventory size";
    }
    
    else if((m_inventory[k]->getType() == "scroll of teleportation" || m_inventory[k]->getType() == "scroll of improve armor" || m_inventory[k]->getType() == "scroll of raise strength"|| m_inventory[k]->getType() == "scroll of enhance health"|| m_inventory[k]->getType() == "scroll of enhance dexterity"))
    {
        return "You can't wield a " + m_inventory[k]->getType();
    }
    
    else if(m_inventory[k]->getType() == "mace")
    {
        m_currentWeapon = m_inventory[k];
        m_actionString = "swings mace at";
        m_weaponDamage = 2;
        m_attackerPoints = m_dexterity; //mace provides no dexterity bonus
        return "You are wiedling mace";
    }
    
    else if(m_inventory[k]->getType() == "short sword")
    {
        m_currentWeapon = m_inventory[k];
        m_actionString = "slashes short sword at";
        m_weaponDamage = 2;
        m_attackerPoints = m_dexterity; //short sword provides no dexterity bonus
        return "You are wiedling short sword";
    }
    
    else if(m_inventory[k]->getType() == "long sword")
    {
        m_currentWeapon = m_inventory[k];
        m_actionString = "swings long sword at";
        m_weaponDamage = 4;
        m_attackerPoints = m_dexterity + 2;
        return "You are wiedling long sword";
    }
    
    else if(m_inventory[k]->getType() == "magic axe")
    {
        m_currentWeapon = m_inventory[k];
        m_actionString = "swings magic axe at";
        m_weaponDamage = 5;
        m_attackerPoints = m_dexterity + 5;
        return "You are wiedling magic axe";
        
    }
    
    else if(m_inventory[k]->getType() == "magic fangs of sleep")
    {
        m_currentWeapon = m_inventory[k];
        m_actionString = "strikes magic fangs of sleep at";
        m_weaponDamage = 2;
        m_attackerPoints = m_dexterity + 3;
        return "You are wiedling magic fangs of sleep";
    }
    
    return "boi";
    
}

string Player::scrollSelector()
{
    clearScreen();
    cout<<"Inventory: "<<endl<<endl;
    int i = 0;
    
    while (i < m_inventory.size())
    {
        cout<<letters[i] <<". " <<m_inventory[i]->getType()<<endl;
        i++;
    }
    
    char input = getCharacter();
    int k = 0;
    while (k <= 25)
    {
        if(input == letters[k])
            break;
        k++;
    }
    
    if((k+1) > m_inventory.size())
    {
        return "bigger than inventory size";
    }
    
    else if((m_inventory[k]->getType() == "mace" || m_inventory[k]->getType() == "short sword" || m_inventory[k]->getType() == "long sword"|| m_inventory[k]->getType() == "magic axe" || m_inventory[k]->getType() == "magic fangs of sleep"))
    {
        return"You can't read a " + m_inventory[k]->getType();
    }
    
    else if(m_inventory[k]->getType() == "scroll of teleportation")
    {
        //delete scroll once it is read
        delete m_inventory[k];
        m_inventory.erase(m_inventory.begin() + k);
        
        return "You read the scroll called scroll of teleportation";
    }
    
    else if(m_inventory[k]->getType() == "scroll of improve armor")
    {
        int armorIncrease = randInt(1, 3);
        m_armor+=armorIncrease;
        m_defenderPoints = m_dexterity + m_armor;
        
        //delete scroll once it is read
        delete m_inventory[k];
        m_inventory.erase(m_inventory.begin() + k);
        
        return "You read the scroll called scroll of improve armor";
    }
    
    else if(m_inventory[k]->getType() == "scroll of raise strength")
    {
        int strengthIncrease = randInt(1, 3);
        m_strength+=strengthIncrease;
        
        //delete scroll once it is read
        delete m_inventory[k];
        m_inventory.erase(m_inventory.begin() + k);
        
        return "You read the scroll called scroll of raise strength";
    }
    
    else if(m_inventory[k]->getType() == "scroll of enhance health")
    {
        int healthCapIncrease = randInt(3, 8);
        m_healthCap+=healthCapIncrease;
        
        //delete scroll once it is read
        delete m_inventory[k];
        m_inventory.erase(m_inventory.begin() + k);
        
        return "You read the scroll called scroll of enhance health";
    }
    
    else if(m_inventory[k]->getType() == "scroll of enhance dexterity")
    {
        int dexterityIncrease = 1;
        m_dexterity+=dexterityIncrease;
        m_defenderPoints = m_dexterity + m_armor;
        
        //delete scroll once it is read
        delete m_inventory[k];
        m_inventory.erase(m_inventory.begin() + k);
        
        return "You read the scroll called scroll of enhance dexterity";
    }
    return "";
}

string Player:: staircase()
{
    if(m_row == m_dungeon->staircaseRow() && m_col == m_dungeon->staircaseCol())
    {
        return "next level";
    }
    else
        return "";
}

void Player::setHealth(int health)
{
    m_health = health;
}

void Player::setStrength(int strength)

{
    m_strength = strength;
}

void Player::setDead()
{
    m_dead = true;
}

bool Player:: attemptMove(const Dungeon& a, char dir, int& r, int& c)
{
    int rnew = r;
    int cnew = c;
    switch (dir)
    {
        case ARROW_UP:
            if (r <= 1 || a.getCellStatus(r-1, c) == WALL || a.getCellStatus(r-1, c) == MONSTER)
                return false;
            else
                rnew--;
            break;
        case ARROW_RIGHT:
            if (c >= a.cols() || a.getCellStatus(r, c+1) == WALL || a.getCellStatus(r, c+1) == MONSTER)
                return false;
            else
                cnew++;
            break;
        case ARROW_DOWN:
            if (r >= a.rows() || a.getCellStatus(r+1, c) == WALL || a.getCellStatus(r+1, c) == MONSTER)
                return false;
            else rnew++;
            break;
        case ARROW_LEFT:
            if (c <= 1 || a.getCellStatus(r, c-1) == WALL || a.getCellStatus(r, c-1) == MONSTER)
                return false;
            else
                cnew--;
            break;
    }
    r = rnew;
    c = cnew;
    return true;
}

