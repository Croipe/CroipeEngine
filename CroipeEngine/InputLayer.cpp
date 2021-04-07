#ifndef INPUT_LAYER
#define INPUT_LAYER
/* Contains Functions To Get Player Input */
#include <Windows.h>

namespace Croipe::InputLayer {
    static int lastKeyPressed;

    bool KeyPressed(int keycode) {
        if (GetAsyncKeyState(keycode) && keycode != lastKeyPressed) {
            lastKeyPressed = keycode;
            return true;
        }

        return false;
    }

    bool KeyHeld(int keycode) {
        if (GetAsyncKeyState(keycode)) {
            return true;
        }

        return false;
    }
}

#endif