#include <iostream>
#include <string>
using namespace std;

class DuelMonster {
    string name;
    int lp;      // Life Points
    int max_lp;
    int atk;     
    int def;     
    
    void showCardInfo();

public:
    void summon(string);     
    void equipSpell();          
    void declareAttack(DuelMonster *); 
    void battleCalculation(int);       
};

void DuelMonster::showCardInfo() {
    cout << "[" << name << "]\t" 
         << "LP: " << lp << "/" << max_lp 
         << " | ATK: " << atk << " | DEF: " << def << endl;
}

void DuelMonster::summon(string n) {
    name = n;
    lp = 4000;    
    max_lp = 4000;
    atk = 2500;   
    def = 2000;   
    
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
    target->battleCalculation(atk);
}

void DuelMonster::battleCalculation(int opp_atk) {
    // Logic: ความเสียหาย = พลังโจมตีศัตรู - พลังป้องกันเรา
    int damage = opp_atk - def;
    if(damage < 0) damage = 0;

    lp = lp - damage;
    
    cout << ">> '" << name << "' took " << damage << " battle damage!\n";

    if(lp <= 0) {
        lp = 0;
        cout << ">> '" << name << "' is destroyed and sent to the Graveyard!\n";
    } else {
        showCardInfo();
    }
}

int main() {
    // สร้าง Pointer Array สำหรับเก็บมอนสเตอร์ 2 ตัว
    DuelMonster *cards = new DuelMonster[2];

    // Player 1 อัญเชิญ Blue-Eyes White Dragon 
    cards[0].summon("Blue-Eyes White Dragon"); 
    
    // Player 2 อัญเชิญ Dark Magician 
    cards[1].summon("Dark Magician");
    
    // Player 1 เสริมพลังให้ Blue-Eyes
    cards[0].equipSpell(); 
    cards[0].equipSpell(); // เพิ่มพลัง 2 รอบ (ATK น่าจะเป็น 3500 แล้ว)
    
    // การต่อสู้เริ่มขึ้น (Battle Phase)
    // Blue-Eyes โจมตี Dark Magician
    cards[0].declareAttack(&cards[1]);
    
    // สมมติ Dark Magician สวนกลับ (ถ้ายังไม่ตาย)
    cards[1].declareAttack(&cards[0]);
    
    // Blue-Eyes ปิดฉาก
    cards[0].declareAttack(&cards[1]);
    
    // คืนหน่วยความจำ (End Phase)
    delete [] cards;
    return 0;
}