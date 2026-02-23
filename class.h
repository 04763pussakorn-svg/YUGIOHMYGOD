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
    int status; // -1 = in deck zone, 0 = in hand, 1 = on field stand up, 2 = on field horizontal, 3 = in graveyard

    Card() {} 

    Card(string n, string t, int s, int a, int d, string e)
        : name(n), type(t), stars(s), atk(a), def(d), effect(e), status(0) {}

    void show() {
        if(type == "Monster") cout << "\033[38;5;94m";
        else if(type == "Spell") cout << "\033[0;32m";
        else if(type == "Trap") cout << "\033[0;35m";

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

Card Class_Card(int a){

    Card C[10];

    C[0] = Card("Beaver Warrior", "Monster", 4, 1200, 1500, "\"A beaver that is skilled in combat.\"");
    C[1] = Card("Celtic Guardian", "Monster", 4, 1400, 1200, "\"An elf who learned to wield a sword, he baffles enemies with lightning-swift attacks.\"");
    C[2] = Card("Curse of Dragon", "Monster", 5, 2000, 1500, "\"A wicked dragon that taps into dark forces to execute a powerful attack.\"");
    C[3] = Card("Dark Magician", "Monster", 7, 2500, 2100, "\"The ultimate wizard in terms of attack and defense.\"");
    C[4] = Card("Feral Imp", "Monster", 4, 1300, 1400, "\"A playful little fiend that lurks in the dark, waiting to attack an unwary enemy.\"");
    C[5] = Card("Gaia The Fierce Knight", "Monster", 7, 2300, 2100, "\"A knight whose horse travels faster than wind. His battle-charge is a force to reckoned with\"");
    C[6] = Card("Giant Soldier of Stone", "Monster", 3, 1300, 2000, "\"A giant warrior made of stone. A punch from this creature has earth-shaking results.\"");
    C[7] = Card("Griffore", "Monster", 4, 1600, 1000, "\"This monster's tough hide deflects almost any attack.\"");
    C[8] = Card("Pot of Greed", "Spell", 0, 0, 0, "Draw 2 cards.");
    C[9] = Card("Mirror Force", "Trap", 0, 0, 0, "Destroy all attack position monsters your opponent controls.");
   
    return C[a];
}
