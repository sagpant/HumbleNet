#include "humblenet_p2p.h"
#include "humblenet_events.h"
#include "humblenet_lobbies.h"

#include <SDL.h>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE

#endif

#include <locale>
#include <iostream>
#include <iterator>
#include <string>

#include "engine/AssetManager.h"
#include "engine/FileSystem.h"
#include "engine/Renderer_SDL2.h"
#include "engine/Engine.h"

#include "game/MainMenu.h"
#include "game/HostGame.h"
#include "game/Game.h"

#define TIMER_BEGIN(x) { auto timer_start = SDL_GetTicks(); auto timer_msg = #x;
#define TIMER_END() auto timer_end = SDL_GetTicks(); SDL_Log("Time for %s: %f", timer_msg, (timer_end - timer_start)/1000.0f); }

struct State {
	bool quit = false;
	PeerId myPeer = 0;
	bool connected = false;
	Uint32 last_time = 0;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
};

static State state;

const uint8_t CHANNEL = 52;

void process_humblenet()
{
	humblenet_p2p_wait( 0 );

	// check any pending HumbleNet events

	HumbleNet_Event event;
	while (humblenet_event_poll( &event )) {
		switch (event.type) {
			case HUMBLENET_EVENT_P2P_CONNECTED:
				std::cout << "P2P connected" << std::endl;
				break;
			case HUMBLENET_EVENT_P2P_ASSIGN_PEER:
				std::cout << "P2P Peer assigned: " << event.peer.peer_id << std::endl;
				state.myPeer = event.peer.peer_id;
				state.connected = true;
				break;
//			case HUMBLENET_EVENT_PEER_CONNECTED:
//			case HUMBLENET_EVENT_PEER_DISCONNECTED:
			case HUMBLENET_EVENT_PEER_REJECTED:
				std::cout << "Peer rejected: " << event.peer.peer_id << std::endl;
				break;
			case HUMBLENET_EVENT_PEER_NOT_FOUND:
				std::cout << "Peer not found: " << event.peer.peer_id << std::endl;
				break;
			case HUMBLENET_EVENT_LOBBY_CREATE_SUCCESS:
				std::cout << "Created lobby " << event.lobby.lobby_id << std::endl;
				break;
			case HUMBLENET_EVENT_LOBBY_JOIN:
				std::cout << "Joined lobby " << event.lobby.lobby_id << std::endl;
				break;
			case HUMBLENET_EVENT_LOBBY_LEAVE:
				std::cout << "Left lobby " << event.lobby.lobby_id << std::endl;
				break;
			case HUMBLENET_EVENT_LOBBY_MEMBER_JOIN:
				std::cout << "Member joined lobby " << event.lobby.lobby_id
						  << " :: " << event.lobby.peer_id << std::endl;
				break;
			case HUMBLENET_EVENT_LOBBY_MEMBER_LEAVE:
				std::cout << "Member left lobby " << event.lobby.lobby_id
						  << " :: " << event.lobby.peer_id << std::endl;
				break;
			default:
				std::cout << "Unhandled event: 0x" << std::hex << event.type << std::endl;
				break;
		}
	}

	// Poll if connected
	if (state.connected) {
		bool done = false;
		do {
			char buff[1024];
			PeerId otherPeer = 0;
			int ret = humblenet_p2p_recvfrom( buff, sizeof( buff ), &otherPeer, CHANNEL );
			if (ret < 0) {
				if (otherPeer == 0) {
					done = true;
				}
			} else if (ret > 0) {
				// process message
			} else {
				done = true;
			}
		} while (!done);
	}
}

void handleEvents(Engine* engine)
{
	SDL_Event event;
	while (SDL_PollEvent( &event )) {
		switch (event.type) {
			case SDL_QUIT:
				state.quit = true;
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_LEFT:
					case SDL_SCANCODE_RIGHT: {
						const Uint8* keys = SDL_GetKeyboardState( nullptr );
						float xPos = 0;
						if (keys[SDL_SCANCODE_LEFT] && keys[SDL_SCANCODE_RIGHT]) {
							xPos = 0;
						} else if (keys[SDL_SCANCODE_LEFT]) {
							xPos = -1.0f;
						} else if (keys[SDL_SCANCODE_RIGHT]) {
							xPos = 1.0f;
						}

						engine->input().apply( InputManager::Force_X_AXIS, xPos );
					}
						break;
					case SDL_SCANCODE_UP:
					case SDL_SCANCODE_DOWN: {
						const Uint8* keys = SDL_GetKeyboardState( nullptr );
						float yPos = 0;

						if (keys[SDL_SCANCODE_UP] && keys[SDL_SCANCODE_DOWN]) {
							yPos = 0;
						} else if (keys[SDL_SCANCODE_DOWN]) {
							yPos = 1.0f;
						} else if (keys[SDL_SCANCODE_UP]) {
							yPos = -1.0f;
						}

						engine->input().apply( InputManager::Force_Y_AXIS, yPos );
					}
						break;
					case SDL_SCANCODE_SPACE:
						engine->input().apply( InputManager::Force_SHOOT,
											   event.key.state == SDL_PRESSED ? 1.0f : 0.0f );
						break;
					case SDL_SCANCODE_ESCAPE:
						engine->input().apply( InputManager::Force_BACK, event.key.state == SDL_PRESSED ? 1.0f : 0.0f );
						break;
					case SDL_SCANCODE_RETURN:
						engine->input().apply( InputManager::Force_START,
											   event.key.state == SDL_PRESSED ? 1.0f : 0.0f );
						break;
					default:
						break;
				}
				break;
			case SDL_TEXTINPUT:
				engine->input().addText(event.text.text);
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						SDL_Log( "Window Size: %d x %d", event.window.data1, event.window.data2 );
						engine->renderer().update_window_size();
						break;
					default:
						break;
				}
				break;

		}
	}
}

void setup_window()
{
	auto ret = SDL_CreateWindowAndRenderer( 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI, &state.window,
											&state.renderer );
	if (ret == -1) {
		SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, "Error", "Failed to create window", nullptr );
		state.quit = true;
	}
}

void main_loop(Engine* engine)
{
	engine->input().cycle();
	process_humblenet();
	handleEvents( engine );

	Uint32 cur_time = SDL_GetTicks();
	float delta = float( cur_time - state.last_time ) / 1000.0f;
	state.last_time = cur_time;

	engine->update( delta );
	engine->render();
	if (engine->shouldQuit()) {
		state.quit = true;
	}
}

int main(int argc, char* argv[])
{
	std::string peerServer = HUMBLENET_SERVER_URL;
	if (argc == 2) {
		peerServer = argv[1];
	}
	humblenet_p2p_init( peerServer.c_str(), "client_token", "client_secret", nullptr );

	SDL_SetHint( SDL_HINT_RENDER_DRIVER, "opengl" );

	SDL_Init( SDL_INIT_VIDEO );
	setup_window();

	char* base = SDL_GetBasePath();
	std::string base_path( base );
	SDL_free( base );

	SDL_Log("Base Path: %s", base_path.c_str());

	std::string asset_path_marker = FileSystem::join_path( base_path, "asset_path.txt" );
	if (FileSystem::exists( asset_path_marker )) {
		FILE* fp = FileSystem::open( asset_path_marker );
		if (fp) {
			char buff[1024];
			fgets( buff, sizeof( buff ), fp );
			base_path = FileSystem::join_path( base_path, buff );
			fclose( fp );
		}
	}

	auto* asset_manager = new AssetManager();
#ifdef EMSCRIPTEN
	asset_manager->add_path( "/assets" );
#else
	asset_manager->add_path( base_path );
#endif
	AssetManager::setDefaultManager( asset_manager );

	auto engine = Engine::create<Renderer_SDL2>( state.window, state.renderer );

#ifndef EMSCRIPTEN
	engine->renderer().set_swap_interval( 1 );
#endif
	engine->renderer().set_logical_size( Vector2i( 640, 400 ));

	engine->registerScene<MainMenu>( "main_menu" );
	engine->registerScene<HostGame>( "host_game" );
	engine->registerScene<Game>( "game" );

	engine->activateScene( "main_menu" );

	state.last_time = SDL_GetTicks();

#ifdef EMSCRIPTEN
	emscripten_set_main_loop_arg( (em_arg_callback_func)main_loop, engine, 0, 1 );
#else
	while (!state.quit) {
		main_loop( engine );
	}

	humblenet_shutdown();

	delete engine;

	SDL_DestroyRenderer( state.renderer );
	SDL_DestroyWindow( state.window );

	SDL_Quit();
#endif
	return 0;
}
