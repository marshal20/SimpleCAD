#pragma once
#include <map>
#include <SDL/SDL.h>
#include "math.h"

// A key/button can have 4 states (2 main, 2 transition):
// - Pressed: the key is held.
// - Released: the key is not held.
// - Up: the transition from pressed to released.
// - Down: the transition from released to pressed.

class InputMgr
{
public:
	enum KeyState { kUp = 1, kDown, kReleased, kPressed };

	static void Update(SDL_Window* window);

	static KeyState GetKey(Uint8 key);
	static bool IsKeyUp(Uint8 key);
	static bool IsKeyDown(Uint8 key);
	static bool IsKeyReleased(Uint8 key);
	static bool IsKeyPressed(Uint8 key);

	static KeyState GetBtn(Uint8 btn);
	static bool IsBtnUp(Uint8 btn);
	static bool IsBtnDown(Uint8 btn);
	static bool IsBtnReleased(Uint8 btn);
	static bool IsBtnPressed(Uint8 btn);

	static Point GetMouse();
	static float GetMouseX();
	static float GetMouseY();
	static int GetSizeX();
	static int GetSizeY();

private:
	static std::map<Uint8, KeyState> m_keymap;
	static std::map<Uint8, KeyState> m_btnmap;
	static float m_mouse_x;
	static float m_mouse_y;
	static int m_size_x;
	static int m_size_y;

};
