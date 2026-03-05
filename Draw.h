#ifndef DRAW_H //+ class + function 
#define DRAW_H
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <functional>
using namespace std;

// class Player {
// public:
//     string name;
//     int lp;
//     Player(string n){name = n; lp = 4000;}
// };

class GameManager;
class Card{
public:
    int id;
    string name;
    string type;
    int stars;
    int atk;
    int def;
    string effect;
    int status;
    int power;

    std::function<void(GameManager*, int)> spellEffect;

    Card(int id, string n, string t, int s, int a, int d, string e) 
        : id(id), name(n), type(t), stars(s), atk(a), def(d), effect(e), status(0), power(0) {
        spellEffect = [](GameManager* gm, int playerIndex) {}; // ค่าเริ่มต้นกัน Error
    }

    Card(string n, string t, int s, int a, int d, string e) 
        : id(0), name(n), type(t), stars(s), atk(a), def(d), effect(e), status(0), power(0) {
        spellEffect = [](GameManager* gm, int playerIndex) {};
    }

    void show() {
        if(type == "Monster") cout << "\033[38;5;94m";
        else if(type == "Spell") cout << "\033[0;32m";
        else if(type == "Trap") cout << "\033[0;35m";

        cout << "\n====================\n";
        cout << "ID    : " << id << endl;
        cout << "Name  : " << name << endl;
        cout << "Type  : " << type << endl;
        cout << "Stars : " << stars << endl;
        cout << "ATK   : " << atk << endl;
        cout << "DEF   : " << def << endl;
        cout << "Effect: " << effect << endl;
        cout << "====================\n";
        cout << "\033[0m";
}   
    string getName() {return name;}
    
    // void showCardInfo();
    // void summon(Player &player, int);     
    // void equipSpell();          
    // void declareAttack(Card *target, Player &attacker, Player &defender); 
    // void battleCalculation(Card *attacker_card, Player &attacker, Player &defender);
    // bool isDes();     
    
};

inline Card draw(vector<Card> &deck) {
    if (deck.empty()) return Card(0, "Empty", "None", 0, 0, 0, "");
    int x = rand() % deck.size(); 
    Card mons = deck[x];
    deck.erase(deck.begin() + x);
    return mons;
}

#endif
