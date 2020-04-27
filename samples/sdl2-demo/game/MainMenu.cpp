//
//  MainMenu.cpp
//  SimpleGameFramework
//
//  Created by Edward Rudd on 5/15/15.
//
//

#include "MainMenu.h"
#include "engine/Renderer.h"
#include "MenuItems.h"

MainMenu::MainMenu(Renderer& rend)
		: Menu( rend )
{
	m_font = m_renderer.load_baked_font( "DejaVuSans-30" );

	add<MenuItems::SceneChange>( "Play", "game" );
	add<MenuItems::SceneChange>( "Host game", "host_game" );
	add<MenuItems::SceneChange>( "Find game", "lobbies" );
	add<MenuItems::Quit>( "Quit" );
}
