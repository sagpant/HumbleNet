CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

include(AssetUtilities)

add_subdirectory(lib/lodepng lodepng)

if(EMSCRIPTEN)
	EmscriptenCreatePackage(sdl2_demo DATA_FILE
		PRELOAD ${CMAKE_CURRENT_SOURCE_DIR}/assets@assets
	)
endif()

CreateProgram(sdl2_demo
DIRS
	engine
	game
	.
EXCLUDE
	engine/Font_STB.*
DEFINES
	HUMBLENET_SERVER_URL=\"${HUMBLENET_SERVER_URL}\"
FEATURES
	cxx_auto_type cxx_range_for
PRE_JS
	${DATA_FILE}
ASM_FLAGS
	FORCE_FILESYSTEM=1
INCLUDES
	PRIVATE
		.
LINK
	SDL2
	humblenet
	lodepng
PROPERTIES
	FOLDER HumbleNet/Samples
	MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/macOSInfo.plist.in
)
if(NOT EMSCRIPTEN)
	CopyDependentLibs(sdl2_demo)
	CreateFolderMarkerFile(sdl2_demo asset_path.txt ${CMAKE_CURRENT_SOURCE_DIR}/assets)
endif()