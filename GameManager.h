#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Draw.h" 
#include <vector>
#include <iostream>
#include <algorithm> 
#include <random>    
using namespace std;

class GameManager {
private:
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

private:
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

//ยังไม่ได้ล็อคให้อัญเชิญได้เทิร์นละ 1 ใบเดี๋ยวอัพเดตให้ 

    void playerMainPhase() {
        cout << "\n[ Phase: MAIN PHASE 1 ]\n";
        bool endPhase = false;
        
        while (!endPhase) {
            showPlayerHand(); // แสดงไพ่บนมือแบบย่อ
            
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
                if (selectedCard.type == "Monster") {
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

    void playerBattlePhase() { /* รอรับคำสั่งโจมตีจากผู้เล่น */ }

    void botMainPhase() { /* โลจิกสมองบอทในการเลือกลงการ์ด */ }
    void botBattlePhase() { /* โลจิกสมองบอทในการประเมินความเสี่ยงและสั่งโจมตี */ }

    void endPhase(int playerIndex) { /* จบเทิร์น */ }

    void announceWinner() {
        if (LP[0] <= 0) cout << "\nYOU LOSE!!!\n";
        else if (LP[1] <= 0) cout << "\nYOU WIN!!!\n";
    }
};

#endif