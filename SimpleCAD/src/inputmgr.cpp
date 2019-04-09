#include "inputmgr.h"
#include <SDL/SDL.h>

std::map<Uint8, InputMgr::KeyState> InputMgr::m_keymap;
std::map<Uint8, InputMgr::KeyState> InputMgr::m_btnmap;
float InputMgr::m_mouse_x;
float InputMgr::m_mouse_y;
int InputMgr::m_size_x;
int InputMgr::m_size_y;

void InputMgr::Update(SDL_Window* window)
{
	for (auto& key : m_keymap)
	{
		if (key.second == kDown)
			key.second = kPressed;
		else if (key.second == kUp)
			key.second = kReleased;
	}

	for (auto& btn : m_btnmap)
	{
		if (btn.second == kDown)
			btn.second = kPressed;
		else if (btn.second == kUp)
			btn.second = kReleased;
	}

	SDL_Event Event;
	while (SDL_PollEvent(&Event))
	{
		switch (Event.type)
		{
		case SDL_KEYDOWN:
			m_keymap[Event.key.keysym.sym] = kDown;
			break;
		case SDL_KEYUP:
			m_keymap[Event.key.keysym.sym] = kUp;
			break;

		case SDL_MOUSEBUTTONDOWN:
			m_btnmap[Event.button.button] = kDown;
			break;
		case SDL_MOUSEBUTTONUP:
			m_btnmap[Event.button.button] = kUp;
			break;

		default:
			break;
		}
	}

	SDL_GetWindowSize(window, &m_size_x, &m_size_y);

	int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);
	m_mouse_x = 2.0f * (float)mouse_x / m_size_x - 1.0f;
	m_mouse_y = -2.0f * (float)mouse_y / m_size_y + 1.0f;
}

InputMgr::KeyState InputMgr::GetKey(Uint8 key)
{
	auto found = m_keymap.find(key);
	if (found != m_keymap.end())
	{
		return found->second;
	}
	return kReleased;
}

bool InputMgr::IsKeyUp(Uint8 key)
{
	KeyState state = GetKey(key);
	return state == kUp;
}

bool InputMgr::IsKeyDown(Uint8 key)
{
	KeyState state = GetKey(key);
	return state == kDown;
}

bool InputMgr::IsKeyReleased(Uint8 key)
{
	KeyState state = GetKey(key);
	return state == kUp || state == kReleased;
}

bool InputMgr::IsKeyPressed(Uint8 key)
{
	KeyState state = GetKey(key);
	return state == kDown || state == kPressed;
}

InputMgr::KeyState InputMgr::GetBtn(Uint8 btn)
{
	auto found = m_btnmap.find(btn);
	if (found != m_btnmap.end())
	{
		return found->second;
	}
	return kReleased;
}

bool InputMgr::IsBtnUp(Uint8 btn)
{
	KeyState state = GetBtn(btn);
	return state == kUp;
}

bool InputMgr::IsBtnDown(Uint8 btn)
{
	KeyState state = GetBtn(btn);
	return state == kDown;
}

bool InputMgr::IsBtnReleased(Uint8 btn)
{
	KeyState state = GetBtn(btn);
	return state == kUp || state == kReleased;
}

bool InputMgr::IsBtnPressed(Uint8 btn)
{
	KeyState state = GetBtn(btn);
	return state == kDown || state == kPressed;
}

float InputMgr::GetMouseX()
{
	return m_mouse_x;
}

float InputMgr::GetMouseY()
{
	return m_mouse_y;
}

int InputMgr::GetSizeX()
{
	return m_size_x;
}

int InputMgr::GetSizeY()
{
	return m_size_y;
}
