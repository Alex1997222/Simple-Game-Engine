
#pragma once
#include "EGraphics.h"

namespace E3D
{
	
    // Drawing interface 
	class ECanvas
	{
	public:
		virtual ~ECanvas(){}

		// Only responsible for drawing the model here
		virtual void onPaint() = 0;
	
		// update the interface
		virtual void update() = 0;
	};
}