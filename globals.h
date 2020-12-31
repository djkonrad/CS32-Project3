//
//  globals.h
//  MiniRogue
//
//  Created by Dom's Mac on 5/12/20.
//  Copyright © 2020 Dom's Mac. All rights reserved.
//

#ifndef globals_h
#define globals_h

class Dungeon;

const int MAXROWS = 18;  // max number of rows in the arena
const int MAXCOLS = 70;  // max number of columns in the arena

const int EMPTY = 0;
const int WALL = 1;
const int PLAYER = 2;
const int MONSTER = 3;
const int OBJECT = 4;

const int MACE = 5;
const int SHORTSWORD = 6;
const int LONGSWORD = 7;
const int MAGICAXE = 8;
const int MAGICFANGSOFSLEEP = 9;

const int TELEPORTATION = 10;
const int ARMOR = 11;
const int STRENGTH = 12;
const int HEALTH = 13;
const int DEXTERITY = 14;

#endif /* globals_h */
