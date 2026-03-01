#ifndef ATTACK_H
#define ATTACK_H
#include "Draw.h"
//#include "System.h"
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

inline void Card::showCardInfo() {
    cout << "[" << name << "]\t" 
         << " | ATK: " << atk << " | DEF: " << def << endl;
}

// inline void Card::summon(string n, int t, int atk_input, int def_input) {
//     name = n; atk = atk_input; def = def_input; status = t; 
//     power = (status == 1) ? atk : def;
//     string statusStr = (status == 1) ? "Attack" : "Defend";

//     cout << ">> " player.name <<  " summon '" << name << "' in " << statusStr << " Position!!!\n";
//     showCardInfo();
// }

inline void Card::summon(Player &player, int t) {
    status = t; 
    power = (status == 1) ? atk : def;
    string statusStr = (status == 1) ? "Attack" : "Defend";

    cout << "--------------------------------------------------\n";
    cout << ">> " << player.name << " summons '" << name << "' in " << statusStr << " Position!!!\n";
    showCardInfo();
    cout << "--------------------------------------------------\n";
}

inline void Card::equipSpell() {
    cout << ">> I activate an Equip Spell Card on '" << name << "'!\n";
    cout << ">> ATK and DEF increased by 500 points!\n";
    atk += 500;
    def += 500;

    if(power == atk - 500) power = atk;
    else if(power == def - 500) power = def;
    showCardInfo();
}

// void Card::declareAttack(Card *target) {
//     cout << "\n>> Battle! '" << name << "' attacks '" << target->getName() << "'\n";
//     target->battleCalculation(power);
// }

inline void Card::declareAttack(Card *target, Player &attacker, Player &defender) {
    cout << "\n>> Battle! " << attacker.name << "'s '" << name << "' attacks " << defender.name << "'s '" << target->getName() << "'!\n";
    target->battleCalculation(this, attacker, defender); 
}

inline void Card::battleCalculation(Card *attacker_card, Player &attacker, Player &defender) {
    int damage = attacker_card->power - this->power; 

    //(Attack Position)
    if (this->status == 1) { 
        if (damage > 0) {
            cout << ">> '" << this->name << "' is destroyed!\n";
            defender.lp -= damage; // หัก LP ของผู้ตั้งรับ
            cout << ">> " << defender.name << " takes " << damage << " damage! (LP Left: " << defender.lp << ")\n";
        } 
        else if (damage < 0) {
            cout << ">> '" << attacker_card->name << "' is destroyed!\n";
            attacker.lp -= (-damage); // หัก LP ของผู้โจมตี
            cout << ">> " << attacker.name << " takes " << -damage << " damage! (LP Left: " << attacker.lp << ")\n";
        } 
        else {
            cout << ">> Both monsters are destroyed!\n";
        }
    } 
    //(Defense Position)
    else { 
        if (damage > 0) {
            cout << ">> '" << this->name << "' is destroyed! No Battle Damage is taken.\n";
        } 
        else if (damage < 0) {
            attacker.lp -= (-damage); 
            cout << ">> Attack repelled! " << attacker.name << " takes " << -damage << " damage! (LP Left: " << attacker.lp << ")\n";
        } 
        else {
            cout << ">> No monsters are destroyed.\n";
        }
    }
}

inline void Action(Card a, Card b) {
    // สร้างผู้เล่น 2 คน
    Player yugi("Yugi");
    Player kaiba("Kaiba");

    cout << "\n--- DUEL START! " << yugi.name << " (LP: " << yugi.lp << ") VS " << kaiba.name << " (LP: " << kaiba.lp << ") ---\n\n";

    a.summon(yugi,1); 

    b.summon(kaiba,1); 

    a.declareAttack(&b, yugi, kaiba);

    cout << "\n--- END OF BATTLE ---\n";
}
#endif 