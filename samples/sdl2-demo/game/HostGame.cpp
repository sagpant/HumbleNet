//
//  MainMenu.cpp
//  SimpleGameFramework
//
//  Created by Edward Rudd on 5/15/15.
//
//

#include "HostGame.h"
#include "engine/Renderer.h"
#include "MenuItems.h"

HostGame::HostGame(Renderer& rend)
		: Menu( rend )
{
	m_font = m_renderer.load_baked_font( "DejaVuSans-30" );

	add<MenuItems::Input>( "Lobby", m_lobbyName, 16 );
	add<MenuItems::Input>( "Player", m_playerName, 16 );
	add<MenuItems::Value<int16_t>>( "Max players", m_maxPlayers, 2, 8 );
	m_startItem = add<MenuItems::SceneChange>( "Start", "game" );
}

void HostGame::onShow(const std::string& previous_name, SceneRef previous)
{
	Menu::onShow( previous_name, previous );
}

void HostGame::update(const InputManager& input, float delta)
{
	Menu::update( input, delta );

	bool disable = m_lobbyName.empty() || m_playerName.empty();
	auto ptr = item<BaseItem>( m_startItem );
	if (ptr) {
		ptr->setDisabled(disable);
	}
}
