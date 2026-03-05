#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Draw.h" 
#include <vector>
#include <iostream>
#include <algorithm> 
#include <random>    
#include <ctime>
using namespace std;

class GameManager {
public:
    //Index 0 = ผู้เล่น, Index 1 = บอท
    int LP[2]; 
    vector<Card> deck[2];
    vector<Card> hand[2];
    
    // โซนบนกระดาน 
    vector<Card> monsterZone[2];   // บนสนามลงมอนสเตอร์ได้สูงสุด 5 ใบ
    vector<Card> spellTrapZone[2]; // โซนเวท/กับดัก 5 ใบ
    vector<Card> graveyard[2];     // สุสาน
    int turnCount;                 // นับจำนวนเทิร์นทั้งหมด
    int currentPlayer;             // 0 หรือ 1 เพื่อบอกว่าเทิร์นใคร 0 = ผู้เล่น, 1 = บอท

public:
    GameManager() {
        LP[0] = 4000;
        LP[1] = 4000;
        turnCount = 1;
        currentPlayer = 0; 
    }

    void setDecks(vector<Card>& playerDeck, vector<Card>& botDeck) {
        deck[0] = playerDeck;
        deck[1] = botDeck;

        auto rng = default_random_engine(time(0));
        shuffle(deck[0].begin(), deck[0].end(), rng);
        shuffle(deck[1].begin(), deck[1].end(), rng);
    }

    void startDuel() {
        cout << "\n==============================\n";
        cout << "         DUEL START!!!        \n";
        cout << "==============================\n";

        initialDraw();
        while (LP[0] > 0 && LP[1] > 0) {
            playTurn(currentPlayer);
            
            // สลับเทิร์นถ้าเป็น 0 จะกลายเป็น 1, ถ้าเป็น 1 จะกลายเป็น 0)
            currentPlayer = 1 - currentPlayer; 
            turnCount++;
        }

        announceWinner();
    }

public:
    void drawCard(int playerIndex, int amount = 1) {
        for (int i = 0; i < amount; i++) {
            // 1. Deck Out = แพ้ทันทีถ้าไม่มีการ์ดให้จั่ว
            if (deck[playerIndex].empty()) {
                cout << "\n[!] Deck Out! ";
                if (playerIndex == 0) cout << "You have no more cards to draw. You LOSE!\n";
                else cout << "Bot has no more cards to draw. You WIN!\n";
                LP[playerIndex] = 0; // บังคับให้ LP เป็น 0 เพื่อจบเกม
                return;
            }

            // 2. ดึงการ์ดใบบนสุด (ใบท้ายสุดของ vector)
            Card drawnCard = deck[playerIndex].back();
            deck[playerIndex].pop_back(); // ลบออกจากกอง

            // 3. เปลี่ยนสถานะการ์ดเป็น "อยู่บนมือ" (สมมติให้ status 5 = Hand ตามที่คุณคอมเมนต์ไว้)
            drawnCard.status = 5;

            // 4. เอาการ์ดเข้ามือ
            hand[playerIndex].push_back(drawnCard);

            // 5. แสดงข้อความ
            if (playerIndex == 0) {
                cout << "You drew: \033[33m" << drawnCard.name << "\033[0m\n";
            } else {
                // ไม่ต้องบอกว่าบอทจั่วได้อะไร
                cout << "Bot drew a card.\n";
            }
        }
    }

private:

    // คืนค่า: 0 = ไม่มีใครตาย, 1 = เป้าหมาย(Target)ตาย, 2 = คนตี(Attacker)ตาย, 3 = ตายทั้งคู่
    int executeBattle(Card attacker, Card target, int &attackerLP, int &defenderLP, string attackerOwner, string defenderOwner) {
        
        if (target.status == 1) { // ตีเป้าหมายตั้งโจมตี (ATK vs ATK)
            if (attacker.atk > target.atk) {
                int dmg = attacker.atk - target.atk;
                cout << defenderOwner << "'s monster is DESTROYED! \033[31m" << defenderOwner << " takes " << dmg << " damage!\033[0m\n";
                defenderLP -= dmg;
                return 1; // เป้าหมายตาย
            } 
            else if (attacker.atk < target.atk) {
                int dmg = target.atk - attacker.atk;
                cout << attackerOwner << "'s attack failed! Monster is DESTROYED! \033[31m" << attackerOwner << " takes " << dmg << " damage!\033[0m\n";
                attackerLP -= dmg;
                return 2; // คนตีตาย
            } 
            else {
                cout << "Both monsters have equal ATK! Both are DESTROYED!\n";
                return 3; // ตายคู่
            }
        } 
        else { // ตีเป้าหมายตั้งป้องกัน (ATK vs DEF)
            if (attacker.atk > target.def) {
                cout << defenderOwner << "'s defending monster is DESTROYED! (No damage to LP)\n";
                return 1; // เป้าหมายตาย
            } 
            else if (attacker.atk < target.def) {
                int dmg = target.def - attacker.atk;
                cout << attackerOwner << " couldn't break the defense! \033[31m" << attackerOwner << " takes " << dmg << " recoil damage!\033[0m\n";
                attackerLP -= dmg;
                return 0; // รอดคู่
            } 
            else {
                cout << "Attack stopped. No monsters destroyed.\n";
                return 0; // รอดคู่
            }
        }
    }
    // ฟังก์ชันแสดงไพ่บนมือของผู้เล่น พร้อมแยกสีตาม Draw.h
    void showPlayerHand() {
        cout << "\n======================================================\n";
        cout << "                  [ YOUR HAND ]                       \n";
        cout << "======================================================\n";
        
        if (hand[0].empty()) {
            cout << "  (Your hand is empty)\n";
        } else {
            for (int i = 0; i < hand[0].size(); i++) {
                // ดึงข้อมูลการ์ด
                Card c = hand[0][i];
                
                // กำหนดสีตามประเภทการ์ด (อ้างอิงจาก Draw.h)
                string colorCode = "\033[0m"; // ค่าเริ่มต้น (Reset สี)
                if (c.type == "Monster") colorCode = "\033[38;5;94m"; // สีน้ำตาล
                else if (c.type == "Spell") colorCode = "\033[0;32m"; // สีเขียว
                else if (c.type == "Trap") colorCode = "\033[0;35m";  // สีม่วง/ชมพู

                // แสดงเลข Index ให้ผู้เล่นพิมพ์เลือกได้ง่ายๆ
                cout << " [" << i + 1 << "] " << colorCode << c.name << "\033[0m ";
                
                // แสดงข้อมูลเพิ่มเติมตามประเภทการ์ด
                if (c.type == "Monster") {
                    cout << "(Monster) - ATK: " << c.atk << " / DEF: " << c.def << " / LV: " << c.stars;
                } else {
                    cout << "(" << c.type << ")";
                }
                cout << "\n";
            }
        }
        cout << "======================================================\n";
    }

private:
    // --- ระบบ Phase ต่างๆ ---
    void playTurn(int playerIndex) {
        if (playerIndex == 0) cout << "\n--- Player 1's Turn --- (Turn " << turnCount << ")\n";
        else cout << "\n--- Bot's Turn --- (Turn " << turnCount << ")\n";

        drawPhase(playerIndex);
        
        if (playerIndex == 0) {
            // ฟังก์ชันรับคำสั่งจากผู้เล่นพิมพ์คีย์บอร์ด
            playerMainPhase();
            if (turnCount > 1) playerBattlePhase(); // เทิร์นแรกห้ามตี
        } else {
            // ฟังก์ชันที่บอทจะคิดและเล่นเอง
            botMainPhase();
            if (turnCount > 1) botBattlePhase();
        }
        
        endPhase(playerIndex);
    }

    void initialDraw() {
        drawCard(0, 5); // ผู้เล่นจั่ว 5 ใบ
        drawCard(1, 5); // บอทจั่ว 5 ใบ
    }

    void drawPhase(int playerIndex) {
        if (playerIndex == 0) cout << "\n[ Phase: DRAW PHASE ]\n";
        drawCard(playerIndex, 1);
    }

    void playerMainPhase() {
        cout << "\n[ Phase: MAIN PHASE 1 ]\n";
        bool endPhase = false;
        bool hasSummoned = false;
        while (!endPhase) {

            showPlayerHand();
            
            cout << "\nOptions:\n";
            if (!hand[0].empty()) cout << "[1-" << hand[0].size() << "] Select a card to Check Info & Play\n";
            cout << "[0] End Main Phase\n";
            cout << "Choose action: ";
            
            int choice;
            cin >> choice;
            
            if (choice == 0) {
                endPhase = true;
                cout << "\nEnding Main Phase...\n";
            } 
            else if (choice > 0 && choice <= hand[0].size()) {
                int cardIndex = choice - 1;
                Card selectedCard = hand[0][cardIndex];
                selectedCard.show(); 

                // --- กรณีเป็นการ์ดมอนสเตอร์ ---
                if (selectedCard.type == "Monster" && !hasSummoned) {
                    cout << "\nWhat would you like to do with this monster?\n";
                    if (monsterZone[0].size() >= 5) {
                        cout << "\033[31m[!] Your Monster Zone is full! You can only Cancel.\033[0m\n";
                        cout << "[0] Cancel\n";
                    } else {
                        cout << "[1] Normal Summon (Face-up Attack)\n";
                        cout << "[2] Set (Face-down Defense)\n";
                        cout << "[0] Cancel (Keep in hand)\n";
                    }
                    cout << "Choice: ";
                    
                    int posChoice;
                    cin >> posChoice;

                    if ((posChoice == 1 || posChoice == 2) && monsterZone[0].size() < 5) {
                        selectedCard.status = posChoice; 
                        monsterZone[0].push_back(selectedCard); 
                        hand[0].erase(hand[0].begin() + cardIndex); 
                        
                        if (posChoice == 1) {
                            cout << "\n>> \033[38;5;94mYou Summoned '" << selectedCard.name << "' in Attack Position!\033[0m\n";
                        } else {
                            cout << "\n>> \033[38;5;94mYou Set a monster in face-down Defense Position.\033[0m\n";
                        }

                        hasSummoned = true;
                    }

                } 
                // --- กรณีเป็นการ์ดเวทมนตร์หรือกับดัก ---
                else {
                    cout << "\nWhat would you like to do with this card?\n";
                    if (spellTrapZone[0].size() >= 5) {
                        cout << "\033[31m[!] Your Spell/Trap Zone is full! You can only Cancel.\033[0m\n";
                        cout << "[0] Cancel\n";
                    } else {
                        if (selectedCard.type == "Spell") {
                            cout << "[1] Activate Spell (Face-up)\n";
                            cout << "[2] Set Spell (Face-down)\n";
                        } else { // Trap
                            cout << "[2] Set Trap (Face-down)\n";
                        }
                        cout << "[0] Cancel (Keep in hand)\n";
                    }
                    cout << "Choice: ";
                    
                    int posChoice;
                    cin >> posChoice;

                    if (posChoice == 1 && selectedCard.type == "Spell" && spellTrapZone[0].size() < 5) {
                        cout << "\n>> \033[0;32mYou Activated Spell Card: '" << selectedCard.name << "'!\033[0m\n";
                        selectedCard.spellEffect(this, 0);//เพิ่มเติมสำหรับใช้เวทย์
                        spellTrapZone[0].push_back(selectedCard); 
                        hand[0].erase(hand[0].begin() + cardIndex);
                    } 
                    else if (posChoice == 2 && spellTrapZone[0].size() < 5) {
                        selectedCard.status = 2; 
                        spellTrapZone[0].push_back(selectedCard);
                        hand[0].erase(hand[0].begin() + cardIndex);
                        cout << "\n>> You Set a card in your Spell/Trap Zone.\n";
                    }
                }
            } else {
                cout << "\033[31m[!] Invalid choice. Please try again.\033[0m\n";
            }
        }
    }

    void playerBattlePhase() {
        cout << "\n[ Phase: BATTLE PHASE ]\n";
        
        // 1. เช็คก่อนว่ามีมอนสเตอร์หงายหน้าโจมตีไหม
        bool hasAttackableMonster = false;
        for (Card c : monsterZone[0]) {
            if (c.status == 1) hasAttackableMonster = true; // status 1 = Attack Position
        }
        
        if (!hasAttackableMonster) {
            cout << "You have no monsters in Attack Position. Skipping Battle Phase...\n";
            return;
        }

        // สร้าง Array จำว่าตัวไหนตีไปแล้วบ้าง (อ้างอิงตาม index ของ monsterZone[0])
        vector<bool> hasAttacked(monsterZone[0].size(), false);
        bool endPhase = false;

        while (!endPhase) {
            cout << "\n--- BATTLE MENU ---\n";
            int attackableCount = 0;
            
            // แสดงมอนสเตอร์ที่สามารถสั่งโจมตีได้
            for (int i = 0; i < monsterZone[0].size(); i++) {
                if (monsterZone[0][i].status == 1 && !hasAttacked[i]) {
                    cout << "[" << i + 1 << "] Attack with \033[38;5;94m" << monsterZone[0][i].name 
                         << "\033[0m (ATK: " << monsterZone[0][i].atk << ")\n";
                    attackableCount++;
                }
            }

            if (attackableCount == 0) {
                cout << "All your monsters have attacked!\n";
                break; // จบเฟสอัตโนมัติถ้าตีครบทุกตัวแล้ว
            }

            cout << "[0] End Battle Phase\n";
            cout << "Choose an attacker: ";
            
            int attackerChoice;
            cin >> attackerChoice;

            if (attackerChoice == 0) {
                endPhase = true;
                cout << "\nEnding Battle Phase...\n";
            } 
            else if (attackerChoice > 0 && attackerChoice <= monsterZone[0].size() && 
                     monsterZone[0][attackerChoice - 1].status == 1 && !hasAttacked[attackerChoice - 1]) {
                
                int aIndex = attackerChoice - 1;
                Card attacker = monsterZone[0][aIndex];
                
                // 2. เลือกว่าจะตีใคร
                if (monsterZone[1].empty()) {
                    // --- กรณี DIRECT ATTACK ---

                    bool trapActivated = false; 
                        for (int t = 0; t < spellTrapZone[1].size(); t++) {
                            if (spellTrapZone[1][t].type == "Trap") {
                                
                                // สุ่มความฉลาดของบอท (สมมติให้มีโอกาส 70% ที่จะยอมเปิดกับดัก)
                                int botTrapDecision = rand() % 100;
                                
                                if (botTrapDecision < 70) {
                                    cout << "\n\033[0;31m[!] BOT ACTIVATED A TRAP CARD: " << spellTrapZone[1][t].name << "!\033[0m\n";
                                    spellTrapZone[1][t].spellEffect(this, 1); // 1 คือบอทเป็นคนใช้
                                    graveyard[1].push_back(spellTrapZone[1][t]);
                                    spellTrapZone[1].erase(spellTrapZone[1].begin() + t);
                                    trapActivated = true; 
                                    break;
                                } else {
                                    // บอทกั๊กกับดักไว้ ไม่ยอมเปิด (แต่เราจะไม่ cout บอกผู้เล่น ให้มันเป็นความลับ)
                                }
                            }
                        }
                        if (trapActivated) continue; // ข้ามดาเมจ

                    cout << "\n>> \033[38;5;94m" << attacker.name << "\033[0m attacks Bot directly!!!\n";
                    LP[1] -= attacker.atk;
                    cout << "\033[31mBot takes " << attacker.atk << " damage!\033[0m (Bot LP: " << LP[1] << ")\n";
                    hasAttacked[aIndex] = true;
                } 
                else {
                    // --- กรณีตีมอนสเตอร์บนสนาม ---
                    cout << "\nBot's Monsters:\n";
                    for (int j = 0; j < monsterZone[1].size(); j++) {
                        if (monsterZone[1][j].status == 1) {
                            cout << "[" << j + 1 << "] \033[38;5;94m" << monsterZone[1][j].name 
                                 << "\033[0m (ATK: " << monsterZone[1][j].atk << ")\n";
                        } else {
                            cout << "[" << j + 1 << "] Face-down Defense Monster\n";
                        }
                    }
                    cout << "[0] Cancel Attack\n";
                    cout << "Choose target: ";
                    
                    int targetChoice;
                    cin >> targetChoice;

                    if (targetChoice > 0 && targetChoice <= monsterZone[1].size()) {
                        int tIndex = targetChoice - 1;
                        Card target = monsterZone[1][tIndex];
                        hasAttacked[aIndex] = true; // ยืนยันการตี

                        bool trapActivated = false; //แทรกกับดัก
                        for (int t = 0; t < spellTrapZone[1].size(); t++) {
                            if (spellTrapZone[1][t].type == "Trap") {
                                cout << "\n\033[0;31m[!] BOT ACTIVATED A TRAP CARD: " << spellTrapZone[1][t].name << "!\033[0m\n";
                                spellTrapZone[1][t].spellEffect(this, 1); // 1 คือบอทเป็นคนใช้
                                graveyard[1].push_back(spellTrapZone[1][t]);
                                spellTrapZone[1].erase(spellTrapZone[1].begin() + t);
                                trapActivated = true; break;
                            }
                        }
                        if (trapActivated) continue; // ข้ามดาเมจ

                        cout << "\n>> \033[38;5;94m" << attacker.name << "\033[0m attacks ";
                        if (target.status == 1) cout << "\033[38;5;94m" << target.name << "\033[0m!\n";
                        else cout << "the face-down monster! (It flips up: \033[38;5;94m" << target.name << "\033[0m - DEF: " << target.def << ")\n";

                        // --- DAMAGE CALCULATION (การคำนวณความเสียหาย) ---

                        // โยนการ์ดและ LP ของทั้งสองฝั่งให้ฟังก์ชันกลางคำนวณ
                        int battleResult = executeBattle(attacker, target, LP[0], LP[1], "You", "Bot");

                        // เช็คผลลัพธ์ว่าใครตายบ้าง เพื่อลบออกจากกระดาน
                        if (battleResult == 1 || battleResult == 3) { // ถ้าเป้าหมายตาย หรือตายคู่
                            graveyard[1].push_back(target);
                            monsterZone[1].erase(monsterZone[1].begin() + tIndex);
                        }
                        if (battleResult == 2 || battleResult == 3) { // ถ้าคนตีตาย หรือตายคู่
                            graveyard[0].push_back(attacker);
                            monsterZone[0].erase(monsterZone[0].begin() + aIndex);
                            hasAttacked.erase(hasAttacked.begin() + aIndex); // ลบออกจากลิสต์จำด้วย
                        }
                    } else if (targetChoice != 0) {
                        cout << "\033[31m[!] Invalid target.\033[0m\n";
                    }
                }
            } else {
                cout << "\033[31m[!] Invalid choice or monster has already attacked.\033[0m\n";
            }
            
            // อัปเดต LP ให้เห็นเสมอ
            cout << "\n[ CURRENT LP ] You: " << LP[0] << " | Bot: " << LP[1] << "\n";
            
            // ถ้ามีคน LP หมด ให้จบเกมทันที
            if (LP[0] <= 0 || LP[1] <= 0) break; 
        }
    }

    void botMainPhase() {
        cout << "\n[ Phase: BOT MAIN PHASE 1 ]\n";
        cout << "Bot is thinking...\n";
        
        bool hasSummoned = false; // กติกา: ลงมอนสเตอร์ปกติได้เทิร์นละ 1 ตัว

        // วนลูปเช็คไพ่บนมือบอท (วนจากหลังมาหน้า เพื่อให้ใช้คำสั่ง erase ได้ปลอดภัย)
        for (int i = hand[1].size() - 1; i >= 0; i--) {
            Card selectedCard = hand[1][i];

            // --- ตรรกะการลงมอนสเตอร์ ---
            if (selectedCard.type == "Monster" && !hasSummoned && monsterZone[1].size() < 5) {
                
                // สุ่มความลังเล: มีโอกาส 25% ที่บอทจะเก็บมอนสเตอร์ไว้บนมือเฉยๆ ไม่ยอมลง!
                int hesitation = rand() % 100;
                if (hesitation < 25) {
                    // ข้ามไพ่ใบนี้ไปเลย
                    continue; 
                }

                // สุ่มท่าทีการลงสนาม: ถ้า ATK เยอะ (>=1500) โอกาสหงายตีสูง แต่ถ้าป้องกันเยอะ หรือสุ่มได้เลขน้อย จะหมอบ
                int playStyle = rand() % 100;
                
                if (selectedCard.atk >= 1500 && playStyle < 80) {
                    // หงายหน้าโจมตี (80% สำหรับมอนสเตอร์โจมตีสูง)
                    selectedCard.status = 1;
                    monsterZone[1].push_back(selectedCard);
                    cout << ">> \033[38;5;94mBot Summons a Monster in Face-up Attack Position!\033[0m\n";
                    // บอทลงแบบหงายหน้า เราจึงให้โชว์ชื่อการ์ดด้วย
                    cout << "   (It's " << selectedCard.name << " - ATK: " << selectedCard.atk << ")\n";
                } else {
                    // คว่ำหน้าป้องกัน
                    selectedCard.status = 2;
                    monsterZone[1].push_back(selectedCard);
                    // บอทคว่ำไพ่ เราจะไม่ให้โชว์ชื่อไพ่ เพื่อความสมจริง!
                    cout << ">> Bot Sets a Monster in face-down Defense Position.\n";
                }
                
                hand[1].erase(hand[1].begin() + i);
                hasSummoned = true; // บันทึกว่าเทิร์นนี้ลงมอนสเตอร์ไปแล้ว
            }
            
            // --- ตรรกะการเล่นการ์ดเวทมนตร์/กับดัก ---
            else if ((selectedCard.type == "Spell" || selectedCard.type == "Trap") && spellTrapZone[1].size() < 5) {
                
                int r = rand() % 100;
                
                // กับดักต้องหมอบเสมอ หรือ เวทมนตร์มีโอกาสถูกหมอบ 40% เพื่อหลอกผู้เล่น
                if (selectedCard.type == "Trap" || r < 40) {
                    selectedCard.status = 2;
                    spellTrapZone[1].push_back(selectedCard);
                    cout << ">> Bot Sets a card in the Spell/Trap Zone.\n";
                    hand[1].erase(hand[1].begin() + i);
                } 
                // เวทมนตร์ มีโอกาส 60% ที่จะใช้งานเลย
                else if (selectedCard.type == "Spell") {
                    cout << ">> \033[0;32mBot Activates Spell Card: '" << selectedCard.name << "'!\033[0m\n";
                    selectedCard.spellEffect(this, 1);//เพิ่มเติมสำหรับใช้เวทย์
                    spellTrapZone[1].push_back(selectedCard);
                    hand[1].erase(hand[1].begin() + i);
                }
            }
        }
        cout << "\nBot ends its Main Phase.\n";
    }
    void botBattlePhase() {
        cout << "\n[ Phase: BOT BATTLE PHASE ]\n";
        
        // ถ้าบอทไม่มีมอนสเตอร์หงายหน้าโจมตีเลย ก็จบเฟส
        bool canAttack = false;
        for (Card c : monsterZone[1]) {
            if (c.status == 1) canAttack = true; // status 1 คือ หงายหน้าโจมตี
        }
        if (!canAttack) {
            cout << "Bot has no monsters able to attack.\n";
            return;
        }

        // --- ระบบความกลัว (Fear System) ---
        int botCourage = 100; // ความกล้าตั้งต้น 100%
        
        // เช็คไพ่หมอบของฝั่งผู้เล่น (index 0)
        int playerSetCards = spellTrapZone[0].size();
        if (playerSetCards > 0) {
            cout << "Bot is looking closely at your face-down cards...\n";
            // ความกล้าลดลง 30% ต่อการ์ดหมอบ 1 ใบของผู้เล่น
            botCourage -= (playerSetCards * 30); 
        }

        // เริ่มสั่งมอนสเตอร์แต่ละตัวให้โจมตี
        for (int i = 0; i < monsterZone[1].size(); i++) {
            if (monsterZone[1][i].status == 1) { // ถ้ามอนสเตอร์ตัวนี้อยู่ในโหมดโจมตี
                
                // สุ่มทอยเต๋าใจดีสู้เสือ
                int roll = rand() % 100;
                
                if (roll < botCourage) {
                    cout << ">> Bot's \033[38;5;94m" << monsterZone[1][i].name << "\033[0m declares an ATTACK!\n";

                    // 🌟 [ระบบใหม่] ถามผู้เล่นก่อนเปิดกับดัก
                    bool trapActivated = false; 
                    for (int t = 0; t < spellTrapZone[0].size(); t++) {
                        if (spellTrapZone[0][t].type == "Trap") {
                            // เกมหยุดชั่วคราวและถามเรา
                            cout << "\n\033[0;35m[?] Bot is attacking! You have a face-down Trap Card: '" << spellTrapZone[0][t].name << "'\033[0m\n";
                            cout << "Do you want to activate it?\n";
                            cout << "[1] Yes, activate it!\n";
                            cout << "[2] No, keep it face-down.\n";
                            cout << "Choice: ";
                            
                            int trapChoice;
                            cin >> trapChoice;

                            if (trapChoice == 1) {
                                cout << "\n\033[0;35m[!] YOU ACTIVATED A TRAP CARD: " << spellTrapZone[0][t].name << "!\033[0m\n";
                                spellTrapZone[0][t].spellEffect(this, 0); 
                                graveyard[0].push_back(spellTrapZone[0][t]);
                                spellTrapZone[0].erase(spellTrapZone[0].begin() + t);
                                trapActivated = true; 
                                break; // ทำงานใบเดียวพอ
                            } else {
                                cout << ">> You chose to keep the Trap Card hidden.\n";
                                // วนลูปเช็คใบต่อไปเผื่อมีกับดักใบอื่นที่เราอยากเปิดแทน
                            }
                        }
                    }

                    if (trapActivated) continue; // ข้ามดาเมจถ้าติดกับดัก
                    // TODO: ระบบคำนวณดาเมจตรงนี้
                    
                    if (monsterZone[0].empty()) {
                        cout << ">> Bot attacks directly!\n";
                        LP[0] -= attacker.atk;
                        cout << "\033[31mYou take " << attacker.atk << " damage!\033[0m (Your LP: " << LP[0] << ")\n";
                    } else {
                        // บอทตีตัวแรกเสมอ
                        Card target = monsterZone[0][0]; 
                        cout << ">> Bot attacks your " << target.name << "!\n";
                        
                        // 🌟 เรียกใช้ฟังก์ชันกลาง (สลับเอา LP[1] ขึ้นก่อน เพราะบอทเป็นคนตี)
                        int battleResult = executeBattle(attacker, target, LP[1], LP[0], "Bot", "You");

                        // เช็คผลลัพธ์ว่าใครตายบ้าง
                        if (battleResult == 1 || battleResult == 3) { // ถ้าเป้าหมาย (เรา) ตาย
                            graveyard[0].push_back(target);
                            monsterZone[0].erase(monsterZone[0].begin()); // ลบตัวแรกเสมอ (index 0)
                        }
                        if (battleResult == 2 || battleResult == 3) { // ถ้าคนตี (บอท) ตาย
                            graveyard[1].push_back(attacker);
                            monsterZone[1].erase(monsterZone[1].begin() + i); // ลบตัวที่ i ของบอท
                        }
                    }
                    }else {
                    cout << ">> Bot hesitates... \033[38;5;94m" << monsterZone[1][i].name << "\033[0m cancels its attack out of fear!\n";
                }
            }
        }
    }

    void endPhase(int playerIndex) {
        cout << "\n[ Phase: END PHASE ]\n";

        // กฎ: ห้ามมีการ์ดบนมือเกิน 6 ใบในตอนจบเทิร์น
        while (hand[playerIndex].size() > 6) {
            if (playerIndex == 0) {
                // --- กรณีเทิร์นของผู้เล่น ---
                cout << "\033[31m[!] Hand size limit exceeded! You have " << hand[0].size() << " cards.\033[0m\n";
                cout << "You must discard down to 6 cards.\n";
                
                showPlayerHand(); // แสดงไพ่ให้ผู้เล่นเลือกทิ้ง
                
                cout << "Choose a card to DISCARD (1-" << hand[0].size() << "): ";
                int choice;
                cin >> choice;

                if (choice > 0 && choice <= hand[0].size()) {
                    int discardIndex = choice - 1;
                    cout << ">> You discarded \033[31m" << hand[0][discardIndex].name << "\033[0m to the Graveyard.\n";
                    
                    // นำไพ่ลงสุสาน (Graveyard)
                    graveyard[0].push_back(hand[0][discardIndex]);
                    
                    // ลบไพ่ใบนั้นออกจากมือ
                    hand[0].erase(hand[0].begin() + discardIndex);
                } else {
                    cout << "\033[31m[!] Invalid choice. You must pick a card to discard.\033[0m\n";
                }

            } else {
                // --- กรณีเทิร์นของบอท ---
                cout << "Bot's hand exceeds 6 cards. Bot must discard...\n";
                
                // บอทสุ่มทิ้งไพ่ 1 ใบ
                int discardIndex = rand() % hand[1].size();
                cout << ">> Bot discarded a card to the Graveyard.\n";
                
                graveyard[1].push_back(hand[1][discardIndex]);
                hand[1].erase(hand[1].begin() + discardIndex);
            }
        }

        // ประกาศจบเทิร์น
        if (playerIndex == 0) {
            cout << "\n=== END OF PLAYER 1'S TURN ===\n";
        } else {
            cout << "\n=== END OF BOT'S TURN ===\n";
        }
        
        // หน่วงเวลาเล็กน้อย หรือรอให้ผู้เล่นกด Enter ก่อนเริ่มเทิร์นถัดไป เพื่อไม่ให้ข้อความรันเร็วจนอ่านไม่ทัน
        if (playerIndex == 1) {
            cout << "\nPress Enter to start your next turn...";
            cin.ignore(); // ล้างบัฟเฟอร์
            cin.get();    // รอรับค่า Enter
        }
    }

    void announceWinner() {
        if (LP[0] <= 0) cout << "\nYOU LOSE!!!\n";
        else if (LP[1] <= 0) cout << "\nYOU WIN!!!\n";
    }
};

#endif