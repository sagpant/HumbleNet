//
//  InputManager.cpp
//  SimpleGameFramework
//
//  Created by Edward Rudd on 5/11/15.
//
//

#include "InputManager.h"

#include <SDL.h>

InputManager::InputManager()
{
	SDL_StopTextInput();
	reset();
}

void InputManager::apply(InputForce force, float value)
{
	if (force >= 0 && force < Force_MAX) {
		m_values[force].value = Math::clamp( value, -1.0f, 1.0f );
	}
}

void InputManager::reset()
{
	for (auto& m_value : m_values) {
		m_value.reset();
	}
}

void InputManager::cycle()
{
	for (auto& m_value : m_values) {
		m_value.cycle();
	}
	m_textInput = "";
}

InputManager::ForceTracker InputManager::operator[](InputForce force) const
{
	if (force >= 0 && force < Force_MAX) {
		return m_values[force];
	} else {
		return {};
	}
}

float InputManager::value(InputForce force) const
{
	if (force >= 0 && force < Force_MAX) {
		return m_values[force].value;
	} else {
		return 0.0f;
	}
}

float InputManager::last_value(InputForce force) const
{
	if (force >= 0 && force < Force_MAX) {
		return m_values[force].last_value;
	} else {
		return 0.0f;
	}
}

void InputManager::setAcceptTextInput(bool enabled)
{
	if (m_textEnabled != enabled) {
		m_textEnabled = enabled;
		m_textEnabled ? SDL_StartTextInput() : SDL_StopTextInput();
	}
}

void InputManager::addText(const std::string& text)
{
	m_textInput += text;
}

std::string InputManager::text() const
{
	return m_textInput;
}
