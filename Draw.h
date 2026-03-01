#ifndef DRAW_H
#define DRAW_H
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

class Player {
public:
    string name;
    int lp;
    Player(string n){name = n; lp = 4000;}
};

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

    Card(string n, string t, int s, int a, int d, string e) 
        : name(n), type(t), stars(s), atk(a), def(d), effect(e), status(0) {}

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
    void showCardInfo();
    void summon(string, int, int , int);     
    void equipSpell();          
    void declareAttack(Card *target, Player &attacker, Player &defender); 
    void battleCalculation(Card *attacker_card, Player &attacker, Player &defender);
    string getName() {return name;}     
    
};
inline Card draw(vector<Card> &deck) {
    int x = rand() % deck.size(); 
    Card mons = deck[x];
    deck.erase(deck.begin() + x);
    return mons;
}
//last
#endif
