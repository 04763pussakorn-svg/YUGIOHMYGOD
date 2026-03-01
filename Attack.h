#ifndef ATTACK_H
#define ATTACK_H
#include "Draw.h"
// class Card {
//     string name;
//     int atk;     
//     int def;
//     int power;     
//     void showCardInfo();

// public:
//     void summon(string, int, int , int);     
//     void equipSpell();          
//     void declareAttack(Card *); 
//     void battleCalculation(int);
//     string getName() {return name;}     
// };

void Card::showCardInfo() {
    cout << "[" << name << "]\t" 
         << " | ATK: " << atk << " | DEF: " << def << endl;
}

void Card::summon(string n, int t, int atk_input, int def_input) {
    string status;
    name = n;
    atk = atk_input;
    def = def_input;

    if (t == 1) power = atk;
    else power = def;

    if (t == 1)status = "Attack";
    else status = "Defend";

    cout << "--------------------------------------------------\n";
    cout << ">> I summon '" << name << "' in " << status << " Position!!!\n";
    showCardInfo();
    cout << "--------------------------------------------------\n";
}

void Card::equipSpell() {
    cout << ">> I activate an Equip Spell Card on '" << name << "'!\n";
    cout << ">> ATK and DEF increased by 500 points!\n";
    atk += 500;
    def += 500;

    if(power == atk - 500) power = atk;
    else if(power == def - 500) power = def;
    showCardInfo();
}

void Card::declareAttack(Card *target) {
    cout << "\n>> Battle! '" << name << "' attacks '" << target->getName() << "'\n";
    target->battleCalculation(power);
}

void Card::battleCalculation(int atker_power) {
    int damage = atker_power - power; // power ของ target

    if(damage > 0)  cout << ">> '" << name << "' is destroyed and sent to the Graveyard!\n";// name of target
}

void Action(Card a,Card b) {

    a.summon(a.name, 1, a.atk , a.def);
    b.summon(b.name, 1, b.atk , b.def);
    a.equipSpell(); 
    a.equipSpell();
    b.equipSpell();
    a.declareAttack(&b);

}
#endif 