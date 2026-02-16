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



    Card C[10];

   Card C[0] = Card("Beaver Warrior", "Monster", 4, 1200, 1500, "A beaver that is skilled in combat.");
   Card C[1] = Card("Celtic Guardian", "Monster", 4, 1400, 1200, "A warrior with a Celtic sword and shield.");
   Card C[2] = Card("Curse of Dragon", "Monster", 5, 2000, 1500, "A dragon that is said to be cursed.");
   Card C[3] = Card("Dark Magician", "Monster", 7, 2500, 2100, "The ultimate wizard.");
   Card C[4] = Card("Feral Imp", "Monster", 4, 1300, 1400, "A mischievous imp.");
   Card C[5] = Card("Gaia The Fierce Knight", "Monster", 7, 2300, 2100, "A fierce knight.");
   Card C[6] = Card("Giant Soldier of Stone", "Monster", 3, 1300, 2000, "A stone giant.");
   Card C[7] = Card("Griffore", "Monster", 4, 1600, 1000, "A swift griffin.");
   Card C[8] = Card("Pot of Greed", "Spell", 0, 0, 0, "Draw 2 cards.");
   Card C[9] = Card("Mirror Force", "Trap", 0, 0, 0, "Destroy all attack position monsters your opponent controls.");
   

