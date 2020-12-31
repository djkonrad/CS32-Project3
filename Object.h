//
//  Object.h
//  MiniRogue
//
//  Created by Dom's Mac on 5/17/20.
//  Copyright © 2020 Dom's Mac. All rights reserved.
//

#ifndef Object_h
#define Object_h

#include <string>
using namespace std;

class Dungeon;

class Object
{
public:
    //Constructor
    Object (Dungeon* ap, int r, int c, string type, char gridChar);
    
    //Accessors
    int row() const;
    int col() const;
    string getType() const;
    int getGridChar() const;
    
protected:
    Dungeon* m_dungeon;
    int m_row;
    int m_col;
    string m_type;
    char m_gridChar;
    
};

class Mace: public Object
{
public:
    Mace(Dungeon* ap, int r, int c);
private:
};

class ShortSword: public Object
{
public:
    ShortSword(Dungeon* ap, int r, int c);
private:
};

class LongSword: public Object
{
public:
    LongSword(Dungeon* ap, int r, int c);
private:
};

class MagicAxe: public Object
{
public:
    MagicAxe(Dungeon* ap, int r, int c);
private:
};

class MagicFangsOfSleep: public Object
{
public:
    MagicFangsOfSleep(Dungeon* ap, int r, int c);
private:
};

class teleportationScroll: public Object
{
public:
    teleportationScroll(Dungeon* ap, int r, int c);
};

class armorScroll: public Object
{
public:
    armorScroll(Dungeon* ap, int r, int c);
private:
};

class strengthScroll: public Object
{
public:
    strengthScroll(Dungeon* ap, int r, int c);
private:
};

class healthScroll: public Object
{
public:
    healthScroll(Dungeon* ap, int r, int c);
private:
};

class dexterityScroll: public Object
{
public:
    dexterityScroll(Dungeon* ap, int r, int c);
private:
};

#endif /* Object_h */
