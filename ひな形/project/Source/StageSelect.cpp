#include <iostream>
#include <conio.h>  // _getch()
#include <thread>
#include <chrono>
#include <windows.h>
using namespace std;

int main() {
    int select = 1; // 現在の選択位置（1?3）
    bool running = true;

    while (running) {
        system("cls");

        cout << "=== レベル選択 ===\n\n";

        cout << (select == 1 ? "＞ ステージ1" : "   ステージ1") << endl << endl;
        cout << (select == 2 ? "＞ ステージ2" : "　 ステージ2") << endl << endl;
       // cout << (select == 3 ? "＞ ステージ3" : "　 ステージ3") << endl << endl;

        cout << "↑↓キーで選択、Enterで決定\n";

        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72) {        // ↑キー
                if (select > 1) select--;
            }
            else if (key == 80) {   // ↓キー
                if (select < 3) select++;
            }
        }
        else if (key == 13) { // Enter
            running = false;
        }
    }

    system("cls");
    cout << "レベル " << select << " が選択されました！\n";
    // std::this_thread::sleep_for(std::chrono::seconds(3));
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { 0, 3 }; // 表示位置（行・列）

    for (int i = 3; i > 0; i--) {
        SetConsoleCursorPosition(hConsole, pos);
        cout << "   " << i << "   ";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
