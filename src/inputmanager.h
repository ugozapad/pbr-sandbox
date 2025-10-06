#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SDL.h>
#include <glm/glm.hpp>

class InputManager
{
public:
	InputManager();
	~InputManager();

	void update();

	// ACTIONS
	void OnKeyboardAction(uint32_t keyId, bool state);
	void OnMousePosAction(float x, float y);
	bool getCursorPosCallback() { return m_bCursorPosCallbackState; }
	bool setCursorPosCallback(bool state) { return m_bCursorPosCallbackState = state; }
	void OnCharacterAction(uint32_t codepoint);

	// GETTERS
	bool GetKey(SDL_Scancode key);
	glm::vec2 getCursorPos() { return m_cursorPos; }
	uint32_t getLastCharacter() { return m_lastCodePoint; }

	bool GetKeyWithReset(SDL_Scancode key);

	glm::vec2 GetDeltaCursorPos();

private:
	bool m_keys[SDL_NUM_SCANCODES];
	bool m_bCursorPosCallbackState;

	glm::vec2 m_cursorPos;
	glm::vec2 m_lastCurPos;
	glm::vec2 m_deltaCursorPos;
	bool m_firstMouse;

	uint32_t m_lastCodePoint;

	//glm::vec2 m_accumulatedDeltaCursorPos;
};

extern InputManager* g_inputManager;

#endif // !INPUTMANAGER_H
