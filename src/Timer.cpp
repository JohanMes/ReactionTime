#include <stdio.h>
#include "Timer.h"
#include "resource.h"

Timer::Timer(HWND hwnd) {
	this->hwnd = hwnd;
	
	// Set clock
	clock = new Clock(false);
	
	// Set default colors
	brush1 = CreateSolidBrush(RGB(255,127,0));
	brush2 = CreateSolidBrush(RGB(255,0,0));
	brush3 = CreateSolidBrush(RGB(0,255,0));
	brush4 = CreateSolidBrush(RGB(127,0,255));
	brush5 = CreateSolidBrush(RGB(0,0,255));
	
	// Set default font
	font = CreateFont(64,0,0,0,0,0,0,0,0,0,0,0,0,"Segoe UI");
	
	// Set initial state
	SetState(tsBeforeStart,"init");
}

Timer::~Timer() {
	DeleteObject(brush1);
	DeleteObject(brush2);
	DeleteObject(brush3);
	DeleteObject(brush4);
	DeleteObject(brush5);
	DeleteObject(font);
	delete clock;
}

void Timer::SetState(TimerState state,const char* changetype) {
	if(this->state != state) {
		char buffer[1024];
		this->state = state;
		switch(state) {
			case tsBeforeStart: {
				snprintf(buffer,1024,"Use left mouse button or space bar to click.\r\n\r\nClick to start");
				break;
			}
			case tsPause: {
				snprintf(buffer,1024,"Wait for it...");
				
				// Increment state after random time
				int randompause = (int)RandomRange(600,1500);
				SetTimer(hwnd,0,randompause,NULL); // timer ID = 0
				break;
			}
			case tsTestStart: {
				snprintf(buffer,1024,"Click!");
				
				// Kill randomized timer
				KillTimer(hwnd,0);
				
				// Record time
				clock->Start();
				break;
			}
			case tsTestInvalid: { // fail, used memory, start all over
				snprintf(buffer,1024,"Clicked too early, removing history! Click to try again");
				
				// Remove history
				scores.clear();
				
				// Kill randomized timer
				KillTimer(hwnd,0);
				break;
			}
			case tsTestEnd: {
				// Get score
				Score score;
				score.time = clock->GetTimeSec() * 1000; // ms
				score.type = changetype;
				scores.insert(scores.begin(),score);
				
				// Gooi alles na de vijfde weg
				if(scores.size() > HISTORY_LENGTH) {
					scores.erase(scores.begin() + HISTORY_LENGTH,scores.end());
				}
				
				// Get average
				double sum = 0;
				for(int i = 0;i < (int)scores.size();i++) {
					sum += scores[i].time;
				}
				double average = sum/scores.size();
				
				// Report average
				if(scores.size() == 1) {
					snprintf(buffer,1024,"Last test: %gms, click to try again",score.time);
				} else {
					snprintf(buffer,1024,"Last test: %gms, average: %gms, click to try again\r\n\r\nHistory:\r\n%s",score.time,average,GetScoreHistory().c_str());
				}
				break;
			}
		}
		
		// Assign buffer to final string
		text.assign(buffer);
		
		// Redraw
		InvalidateRgn(hwnd,NULL,false);
	}
}

string Timer::GetScoreHistory() {
	string result;
	char buffer[256];
	for(int i = 0;i < (int)scores.size();i++) {
		snprintf(buffer,256,"%gms using %s\r\n",scores[i].time,scores[i].type.c_str());
		result.append(buffer);
	}
	return result;
}

void Timer::OnClick(const char* changetype) {
	if(state == tsBeforeStart) {
		SetState(tsPause,changetype);
	} else if(state == tsPause) {
		SetState(tsTestInvalid,changetype);	
	} else if(state == tsTestStart) {
		SetState(tsTestEnd,changetype);
	} else if(state == tsTestInvalid) {
		SetState(tsPause,changetype);
	} else if(state == tsTestEnd) {
		SetState(tsPause,changetype);
	}
}

int Timer::OnMessage(UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		
		// Update timing on mouse down
		case WM_LBUTTONDOWN: {
			OnClick("mouse");
			break;
		}
		
		// ... or on spacebar
		case WM_KEYDOWN: {
			switch(wParam) {
				case VK_SPACE: {
					OnClick("spacebar");
					break;
				}
			}
			break;
		}
		
		// Update background based on state
		case WM_PAINT: {
			
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd,&ps);
			
			RECT rect;
			GetClientRect(hwnd,&rect);
			
			// Draw bg
			switch(state) {
				case tsBeforeStart: {
					FillRect(hdc,&rect,brush1);
					break;
				}
				case tsPause: {
					FillRect(hdc,&rect,brush2);
					break;
				}
				case tsTestStart: {
					FillRect(hdc,&rect,brush3);
					break;
				}
				case tsTestInvalid: {
					FillRect(hdc,&rect,brush4);
					break;
				}
				case tsTestEnd: {
					FillRect(hdc,&rect,brush5);
					break;
				}
			}
			
			// Draw transporent text using custom font and custom color
			SetBkMode(hdc,TRANSPARENT);
			SetTextColor(hdc,RGB(255,255,255));
			SelectObject(hdc,font);
			DrawText(hdc,text.c_str(),-1,&rect,DT_CENTER);
		
			EndPaint(hwnd,&ps);
			
			// Return 0 means we have processed it
			return 0;
		}
		
		// Start response test
		case WM_TIMER: {
			SetState(tsTestStart,"timer");
			break;
		}
		default: {
			return -1; // unprocessed
		}
	}
	return 0; // processed
}
