#ifndef DRAW_H
#define DRAW_H
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

class Card {
public:
    string name;
    string type;
    int stars;
    int atk;
    int def;
    string effect;
    int status;

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
    void summon(string, int, int , int);     
    void equipSpell();          
    void declareAttack(Card *); 
    void battleCalculation(int);
    string getName() {return name;}     
};
Card draw(vector<Card> &deck) {
    int x = rand() % deck.size(); 
    Card mons = deck[x];
    deck.erase(deck.begin() + x);
    return mons;
}

#endif
