#include <windows.h>
#include <iostream>

int main() {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);

    float keyboard;
    while (true) {
        INPUT_RECORD input;
        DWORD count;

        ReadConsoleInput(hStdin, &input, 1, &count);
        if (count > 0 && input.EventType == KEY_EVENT && input.Event.KeyEvent.bKeyDown) {
            keyboard = static_cast<float>(input.Event.KeyEvent.wVirtualKeyCode);
            std::cout << keyboard << '\n';
        }
    }

    return 0;
}