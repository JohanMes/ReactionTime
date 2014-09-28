#include <windows.h>
#include "Timer.h"
#include "resource.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	// Send message to timer if it has been created
	if(timer) {
		int timerresult = timer->OnMessage(Message,wParam,lParam);
		if(timerresult != -1) { // processed
			return timerresult;
		}
	}
	switch(Message) {
		case WM_DESTROY: {
			delete timer;
			PostQuitMessage(0);
			break;
		}
		case WM_SIZE: {
			// Redraw all
			InvalidateRgn(hwnd,NULL,false);
			break;
		}
		case WM_CREATE: {
			timer = new Timer(hwnd);
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* insert window procedure function here */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, "A"); /* use "A" as icon name when you want to use the project icon */
	wc.hIconSm		 = LoadIcon(NULL, "A"); /* as above */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindow("WindowClass","ReactionTime",WS_VISIBLE|WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,600,600,NULL,NULL,hInstance,NULL);
	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	
	// maximze
	ShowWindow(hwnd,SW_MAXIMIZE);

	while(GetMessage(&Msg, NULL, 0, 0) > 0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
