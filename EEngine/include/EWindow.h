
#pragma once

#include "ECommon.h"
#include <Windows.h>

namespace E3D
{
	class EInputListener;
	class ECanvas;
	// Game window management class
	class EGameWindow
	{
	public:
		// Global class pointer, used to call back in WinProc
		static EGameWindow *GWindow;
		
		// window loop
		void startLoop();

		EGameWindow(const EString& windowTitle, HINSTANCE hInstance);
		~EGameWindow();

		// Display window
	    void showWindow(EBool show);
		void updateWindow();

		void quiteApplication();

		HWND getHWnd() const { return mHwnd; }
		HINSTANCE getHInstance() const { return mHinstance;}

		// Register/remove IO event listener
		void addInputListener(EInputListener* listener);
		void removeInputListener(EInputListener* listener);

		// Set the drawing listener, and there can only be one drawing Canvas at the same time
		void setCanvasListener(ECanvas *canvasListener);

	protected:

		// Global Win event callback function
		static LRESULT WinProc(HWND hWnd, UINT Msg,WPARAM wParam,LPARAM lParam);

		// Window drawing function
		virtual void onPaint(HDC hdc);
		virtual void onUpdate();
		virtual void onKeyDown(EInt msg);
		virtual void onKeyRelease(EInt msg);
		virtual void onMousePress(bool rightPress);
		virtual void onMouseMove(EInt x, EInt y);
		virtual void onMouseWheel(EInt delta);

	protected:
		// Global Win event callback function
		bool registerWindow();
		// Create one window
		bool createWindow();

	protected:
		
		HWND							mHwnd;
		EString							mWindowTitle;
		HINSTANCE						mHinstance;
		std::vector<EInputListener*>	mInputListeners;
		ECanvas							*mCanvasListener;

		EBool							mQuite;

		typedef std::vector<EInputListener*>::iterator	InputItr;
	};
}