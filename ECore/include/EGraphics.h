
#pragma once

#include "ECommon.h"
#include <Windows.h>
namespace E3D
{
	extern HINSTANCE GHInstance;
	class EBitmap
	{
	public:
		EInt width, height;

		EBitmap(const EString &filename);
		~EBitmap();

		inline EString getName() const { return name; }
		inline bool isValid() const { return valid;}
		EColor getPixel(EInt x, EInt y);
		inline EInt getHeight() const { return height;}
		inline EInt getWidth() const { return width;}

	private:
		EString					name;


		HBITMAP					hBitmap;
		BITMAP					bitmap;
		HDC						bitmapHDC;

	public:
		EColor					*pixels;		

		EInt					pitch;

		EBool					valid;
	};


	class EGraphics
	{
	public:

		
		// Initialize the drawing system
		static bool initGraphics(HINSTANCE hinstance);
		// Close the drawing system
		static void shutdownGraphics();

		// Detect the z value, return true to indicate pass, you can call setPixel
		static EBool checkZ(EInt x, EInt y, EFloat z);
		static void setPixel(EInt x, EInt y, /*EFloat z, */const EColor &c);
		static EColor getPixel(EInt x, EInt y);
		// Draw a line in the current buffer
		static void drawLine(EInt x0, EInt y0, EInt x1, EInt y1, const EColor &c);
		static void drawString(const EString &str, EInt x, EInt y, const EColor &c = EColor(255, 255, 255));
		static void fillTriangle(EInt x0, EInt y0, EInt x1, EInt y1, EInt x2, EInt y2, 
							const EColor &c = EColor(255, 255, 255));

		static void enableSmoothingMode(EBool enable);

		static void clearBuffer(const EColor &c = EColor());
		static void fillBuffer(HDC hdc);

		static EInt getScreenWidth() { return SCREEN_WIDTH; }
		static EInt getScreenHeight() { return SCREEN_HEIGHT; }

	private:

		// save the variable
		static HBITMAP							GBufferedHandle;
		static HDC								GBufferedHDC;
		static HBRUSH							GBgBrush;
		static HPEN								GPen;
		static HINSTANCE						GInstance;

		static BYTE								*GDatas;
		static EInt								GPitch;

		static DIBSECTION						GDIBSection;
	
		static RECT								GBufferSize;

		static EFloat							*GZBuffer;
	};
}
