#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "inputmanager.h"

static InputManager s_InputManager;
InputManager* InputManager::instance = &s_InputManager;

InputManager::InputManager()
{
	memset(m_keys, 0, sizeof(m_keys));
	m_cursorPos = glm::vec2(0.0f);
	m_deltaCursorPos = glm::vec2(0.0f);
	m_lastCurPos = glm::vec2(0.0f);
	m_firstMouse = true;
	m_bCursorPosCallbackState = false;
	m_lastCodePoint = 0;
}

InputManager::~InputManager()
{
}

void InputManager::update()
{
	m_lastCodePoint = 0;
}

void InputManager::OnKeyboardAction(uint32_t keyId, bool state)
{
	if (keyId >= 420)
		return;

	m_keys[keyId] = state;
}

void InputManager::OnMousePosAction(float x, float y)
{
	setCursorPosCallback(true);
	if (m_firstMouse)
	{
		m_lastCurPos.x = x;
		m_lastCurPos.y = y;
		m_firstMouse = false;
	}

	m_deltaCursorPos.x = x - m_lastCurPos.x;
	m_deltaCursorPos.y = m_lastCurPos.y - y;

	m_lastCurPos.x = x;
	m_lastCurPos.y = y;

	m_cursorPos = m_lastCurPos;
}

void InputManager::OnCharacterAction(uint32_t codepoint)
{
	m_lastCodePoint = codepoint;
}

bool InputManager::GetKey(SDL_Scancode key)
{
	if ((uint32_t)key >= 420)
		return false;

	return m_keys[(uint32_t)key];
}

bool InputManager::GetKeyWithReset(SDL_Scancode key)
{
	if ((uint32_t)key >= 420)
		return false;

	if (m_keys[(uint32_t)key])
	{
		m_keys[(uint32_t)key] = false;
		return true;
	}

	return false;
}

glm::vec2 InputManager::GetDeltaCursorPos()
{
	return m_deltaCursorPos;
}

