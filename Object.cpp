//
//  Object.cpp
//  MiniRogue
//
//  Created by Dom's Mac on 5/17/20.
//  Copyright © 2020 Dom's Mac. All rights reserved.
//

#include "Object.h"
#include "globals.h"
#include "utilities.h"

Object::Object(Dungeon* ap, int r, int c, string type, char gridChar)
{
    m_row = r;
    m_col = c;
    m_dungeon = ap;
    m_gridChar = gridChar;
    m_type = type;
}

int Object::row() const
{
    return m_row;
}

int Object::col() const
{
    return m_col;
}

string Object:: getType() const
{
    return m_type;
}

int Object::getGridChar() const
{
    return m_gridChar;
}

Mace::Mace(Dungeon* ap, int r, int c):Object(ap, r, c, "mace", ')')
{
}

ShortSword::ShortSword(Dungeon* ap, int r, int c):Object(ap, r, c, "short sword", ')')
{
}

LongSword::LongSword(Dungeon* ap, int r, int c):Object(ap, r, c, "long sword", ')')
{
}

MagicAxe::MagicAxe(Dungeon* ap, int r, int c):Object(ap, r, c, "magic axe", ')')
{
}

MagicFangsOfSleep::MagicFangsOfSleep(Dungeon* ap, int r, int c):Object(ap, r, c, "magic fangs of sleep", '?')
{
}

teleportationScroll::teleportationScroll(Dungeon* ap, int r, int c):Object(ap, r, c, "scroll of teleportation", '?')
{
}

armorScroll::armorScroll(Dungeon* ap, int r, int c):Object(ap, r, c, "scroll of improve armor", '?')
{
}

strengthScroll::strengthScroll(Dungeon* ap, int r, int c):Object(ap, r, c, "scroll of raise strength", '?')
{
}

healthScroll::healthScroll(Dungeon* ap, int r, int c):Object(ap, r, c, "scroll of enhance health", '?')
{
}

dexterityScroll::dexterityScroll(Dungeon* ap, int r, int c):Object(ap, r, c, "scroll of enhance dexterity", '?')
{
}


