//
//  Font.h
//  SimpleGameFramework
//
//  Created by Edward Rudd on 4/13/15.
//
//

#pragma once

namespace Font {

	enum Flags {
		ALIGN_H_LEFT = 0x000u,
		ALIGN_H_CENTER = 0x001u,
		ALIGN_H_RIGHT = 0x002u,
		ALIGN_V_TOP = 0x000u,
		ALIGN_V_CENTER = 0x004u,
		ALIGN_V_BOTTOM = 0x008u,
		WRAP = 0x010u,
		ALIGN_CENTER = ALIGN_H_CENTER | ALIGN_V_CENTER,
		ALIGN_LEFT_TOP = ALIGN_H_LEFT | ALIGN_V_TOP,

		// MASKS

		ALIGN_H_MASK = 0x003u,
		ALIGN_V_MASK = 0x00cu,

		DEFAULT_FLAGS = ALIGN_CENTER | WRAP,
	};
};


