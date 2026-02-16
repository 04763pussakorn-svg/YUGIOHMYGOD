#include <iostream>
#include <string>
using namespace std;

class DuelMonster {
    string name;
    int atk;     
    int def;
    int power;     
    
    void showCardInfo();

public:
    void summon(string, int);     
    void equipSpell();          
    void declareAttack(DuelMonster *); 
    void battleCalculation(int);       
};

void DuelMonster::showCardInfo() {
    cout << "[" << name << "]\t" 
         << " | ATK: " << atk << " | DEF: " << def << endl;
}

void DuelMonster::summon(string n, int t) {
    name = n;
    atk = 2500;   
    def = 2000; 
    if (t == 1) power = atk;
    else power = def;

    
    cout << "--------------------------------------------------\n";
    cout << ">> I summon '" << name << "' in Attack Position!!!\n";
    showCardInfo();
    cout << "--------------------------------------------------\n";
}

void DuelMonster::equipSpell() {
    cout << ">> I activate an Equip Spell Card on '" << name << "'!\n";
    cout << ">> ATK and DEF increased by 500 points!\n";
    atk += 500;
    def += 500;
    showCardInfo();
}

void DuelMonster::declareAttack(DuelMonster *target) {
    cout << "\n>> Battle! '" << name << "' attacks with " << name << " Burst Stream!!\n";
    target->battleCalculation(power);
}

void DuelMonster::battleCalculation(int opp_power) {
    int damage = opp_power - power;

    if(damage < 0)  cout << ">> '" << name << "' is destroyed and sent to the Graveyard!\n";//here
}

int main() {
    DuelMonster *cards = new DuelMonster[2];

    cards[0].summon("Blue-Eyes White Dragon", 1); 
    cards[1].summon("Dark Magician", 1);
    cards[0].equipSpell(); 
    cards[0].equipSpell();
    cards[1].equipSpell(); 
    cards[0].declareAttack(&cards[1]);

    delete [] cards;
    return 0;
}