#ifndef CLASS_H
#define CLASS_H


#include <iostream>
#include <string>
#include <algorithm>
#include <random>
using namespace std;

class Card {
public:
    string name;
    string type;
    int stars;
    int atk;
    int def;
    string effect;

    Card() {} 

    Card(string n, string t, int s, int a, int d, string e)
        : name(n), type(t), stars(s), atk(a), def(d), effect(e) {}

    void show() {
        if(type == "Monster") cout << "\033[38;5;94m";
        else if(type == "Spell") cout << "\033[0;32m";
        else if(type == "Trap") cout << "\033[0;35m";
        else ;

        cout << "\n====================\n";
        cout << "Name  : " << name << endl;
        cout << "Type  : " << type << endl;
        cout << "Stars : " << stars << endl;
        cout << "ATK   : " << atk << endl;
        cout << "DEF   : " << def << endl;
        cout << "Effect: " << effect << endl;
        cout << "====================\n";

        cout << "\033[0m";
    }
};

Card All_Card[10] ={
    Card("Beaver Warrior", "Monster", 4, 1200, 1500, "A beaver that is skilled in combat."),
    Card("Celtic Guardian", "Monster", 4, 1400, 1200, "A warrior with a Celtic sword and shield."),
    Card("Curse of Dragon", "Monster", 5, 2000, 1500, "A dragon that is said to be cursed."),
    Card("Dark Magician", "Monster", 7, 2500, 2100, "The ultimate wizard."),
    Card("Feral Imp", "Monster", 4, 1300, 1400, "A mischievous imp."),
    Card("Gaia The Fierce Knight", "Monster", 7, 2300, 2100, "A fierce knight."),
    Card("Giant Soldier of Stone", "Monster", 3, 1300, 2000, "A stone giant."),
    Card("Griffore", "Monster", 4, 1600, 1000, "A swift griffin."),
    Card("Pot of Greed", "Spell", 0, 0, 0, "Draw 2 cards."),
    Card("Mirror Force", "Trap", 0, 0, 0, "Destroy all attack position monsters your opponent controls.")
   
};
#endif