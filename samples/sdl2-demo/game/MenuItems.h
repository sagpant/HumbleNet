//
// Created by Edward Rudd on 5/3/20.
//

#pragma once

#include "Menu.h"

namespace MenuItems {
	struct SceneChange : Menu::BaseItem {
		std::string m_scene;

		SceneChange(std::string label, std::string scene) : BaseItem( std::move( label )),
															m_scene( std::move( scene )) {}

		bool action_select(Scene& scene) override;
	};

	struct Quit : Menu::BaseItem {
		explicit Quit(std::string label) : BaseItem( std::move( label )) {}

		bool action_select(Scene& scene) override;
	};

	struct Input : Menu::BaseItem {
		Input(std::string label, std::string& ref, uint16_t maximum)
				: BaseItem( std::move( label )), m_ref( ref ), m_maxLength( maximum ) {}

		std::string label() override;

		bool action_select(Scene& scene) override;

		void enter(Scene &scene) override;

		void exit(Scene &scene) override;

		void update(Scene &scene, float delta) override;
	protected:
		std::string& m_ref;
		uint16_t m_maxLength;
	};

	// region Value
	template<typename IntType>
	struct Value : Menu::BaseItem {
		Value(std::string label, IntType& ref, IntType minimum = 0, IntType maximum = 1, IntType increment = 1)
				: BaseItem( std::move( label )), m_ref( ref ), m_min( minimum ), m_max( maximum ), m_inc( increment ) {}

		std::string label() override;

		bool action_left(Scene& scene) override;

		bool action_right(Scene& scene) override;

	protected:
		IntType& m_ref;
		IntType m_min;
		IntType m_max;
		IntType m_inc;
	};

	template<typename IntType>
	std::string Value<IntType>::label()
	{
		return m_label + ": " + std::to_string( m_ref );
	}

	template<typename IntType>
	bool Value<IntType>::action_left(Scene& scene)
	{
		if ((m_ref - m_inc) < m_min) {
			m_ref = m_min;
		} else {
			m_ref -= m_inc;
		}
		return true;
	}

	template<typename IntType>
	bool Value<IntType>::action_right(Scene& scene)
	{
		if ((m_ref + m_inc) > m_max) {
			m_ref = m_max;
		} else {
			m_ref += m_inc;
		}
		return true;
	}
	// endregion
};


