//
//  Menu.cpp
//  SimpleGameFramework
//
//  Created by Edward Rudd on 5/15/15.
//
//

#include "Menu.h"

#include "engine/Engine.h"

#include <SDL_log.h>

Menu::Menu(Renderer& rend)
		: Scene( rend ), m_font( BakedFont::Null ), m_current_item( 0 ) {}

void Menu::update(const InputManager& input, float delta)
{
	auto yAxis = input[InputManager::Force_Y_AXIS];

	auto prev_item = m_current_item;

	if (yAxis.is_set() && !yAxis.was_set()) {
		int16_t direction = 1;
		direction = (yAxis.value < 0) ? -1 : 1;
		m_current_item += direction;
		if (m_current_item >= m_menu_items.size()) {
			m_current_item = direction < 0 ? m_menu_items.size() - 1 : 0;
		}

		auto start = m_current_item;

		while (m_menu_items.at(m_current_item)->disabled()) {
			m_current_item += direction;
			if (m_current_item >= m_menu_items.size()) {
				m_current_item = direction < 0 ? m_menu_items.size() - 1 : 0;
			}
			if (start == m_current_item) break;
		}
	}

	auto& item = m_menu_items.at( m_current_item );

	if (prev_item != m_current_item && !m_menu_items.empty()) {
		auto& prev = m_menu_items.at( prev_item );
		prev->exit( *this );

		item->enter( *this );
	}

	item->update( *this, delta );

	auto xAxis = input[InputManager::Force_X_AXIS];
	if (xAxis.is_set() && !xAxis.was_set()) {
		if (xAxis.value < 0) {
			item->action_left( *this );
		} else {
			item->action_right( *this );
		}
	}

	auto backBtn = input[InputManager::Force_BACK];
	auto startBtn = input[InputManager::Force_START];

	if (backBtn.is_set() && !backBtn.was_set()) {
		handleBack();
	} else if (startBtn.is_set() && !startBtn.was_set()) {
		if (!item->action_select( *this )) {
			SDL_Log( "Start button pressed for %d", m_current_item );
		}
	}
}

const Color inactiveColor( 255, 255, 255 );
const Color currentColor( 0, 255, 255 );
const Color disabledColor( 128, 128, 128 );

void Menu::render()
{
	Vector2i display_size( 400, m_renderer.logical_size().y - 40 );
	Recti menu_area = Recti( display_size ).center_in( m_renderer.logical_size());

	m_renderer.draw_rect_fill( menu_area, Color( 100, 50, 20 ));

	Vector2i pos( menu_area.x + menu_area.w / 2, menu_area.y + m_font->height() / 2 );
	uint16_t i = 0;
	for (auto& m_menu_item : m_menu_items) {
		auto color = i == m_current_item ? currentColor : inactiveColor;
		if (m_menu_item->disabled()) {
			color = disabledColor;
		}
		m_renderer.draw_text( m_font, pos, m_menu_item->label(), Font::ALIGN_CENTER, color );
		pos.y += m_font->height();
		++i;
	}
}

void Menu::onShow(const std::string& previous_name, const SceneRef previous)
{
	auto prev_menu = dynamic_cast<Menu*>(previous);
	if (prev_menu) {
		m_previous_scene = previous_name;
	}
}

void Menu::handleBack()
{
	if (!m_previous_scene.empty()) {
		activateScene( m_previous_scene );
	}
}