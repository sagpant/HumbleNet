//
// Created by Edward Rudd on 5/3/20.
//

#include "MenuItems.h"

#include "engine/Scene.h"
#include "engine/Engine.h"

namespace MenuItems {
	bool SceneChange::action_select(Scene& scene)
	{
		if (disabled()) {
			return false;
		} else {
			return scene.activateScene( this->m_scene );
		}
	}

	bool Quit::action_select(Scene& scene)
	{
		if (disabled()) {
			return false;
		} else {
			scene.engine()->setShouldQuit();
			return true;
		}
	}

	std::string Input::label()
	{
		return m_label + ": " + m_ref;
	}

	bool Input::action_select(Scene& scene)
	{
		if (disabled()) {
			return false;
		} else {
			m_ref = "";
			return true;
		}
	}

	void Input::enter(Scene& scene)
	{
		scene.engine()->input().setAcceptTextInput( true );
	}

	void Input::exit(Scene& scene)
	{
		scene.engine()->input().setAcceptTextInput( false );
	}

	void Input::update(Scene& scene, float delta)
	{
		if (disabled()) {
			return;
		} else {
			auto text = scene.engine()->input().text();
			if ((m_ref.size() + text.size()) <= m_maxLength) {
				m_ref += text;
			}
		}
	}
}