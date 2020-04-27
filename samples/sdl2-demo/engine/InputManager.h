//
//  InputManager.h
//  SimpleGameFramework
//
//  Created by Edward Rudd on 5/11/15.
//
//

#pragma once

#include "Math.h"

#include <string>

class InputManager {
public:
	enum InputForce {
		Force_X_AXIS,
		Force_Y_AXIS,
		Force_SHOOT,
		Force_START,
		Force_BACK,
		Force_MAX,
	};

	struct ForceTracker {
		float value;
		float last_value;
		float dead_zone;

		ForceTracker() : value( 0.0f ), last_value( 0.0f ), dead_zone( 0.1 ) {}

		void reset()
		{
			value = 0.0f;
			last_value = 0.0f;
		}

		void cycle()
		{
			last_value = value;
		}

		bool was_set() const
		{
			return (Math::abs( last_value ) > dead_zone);
		}

		bool is_set() const
		{
			return (Math::abs( value ) > dead_zone);
		}
	};

private:
	bool m_textEnabled = false;
	std::string m_textInput;
	ForceTracker m_values[Force_MAX];
public:
	InputManager();

	void apply(InputForce force, float value);

	void addText(const std::string& text);

	void reset();

	void cycle();

	ForceTracker operator[](InputForce force) const;

	float value(InputForce force) const;

	float last_value(InputForce force) const;

	std::string text() const;

	void setAcceptTextInput(bool enabled);
};
