#pragma once

#include <string>

#include "CEngine/Graphics/Graphics.h"
#include "CEngine/Drawable/Drawable.h"

class GFXResource {
public:
	virtual ~GFXResource() = default;
	
	virtual void Bind(Graphics& gfx) = 0;
};