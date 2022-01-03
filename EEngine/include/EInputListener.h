
#pragma once

namespace E3D
{
	// Mouse keyboard listener
	class EInputListener
	{
	public:
		virtual EBool keyPress(EInt key)=0;
		virtual EBool keyRlease(EInt key) = 0;

		virtual EBool mouseButtonDown(EInt mouseButton) = 0;
		virtual EBool mouseButtonRelease(EInt mouseButton) = 0;
		virtual EBool mouseMove(EInt x, EInt y) = 0;
		virtual EBool mouseWheel(EInt delta) = 0;

		virtual ~EInputListener(){}
	};
}