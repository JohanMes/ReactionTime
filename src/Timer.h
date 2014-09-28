#ifndef TIMETESTER_H
#define TIMETESTER_H

#include <windows.h>
#include <vector>
using std::vector;
#include <string>
using std::string;

#include "Clock.h"

#define HISTORY_LENGTH 8

struct Score {
	double time; // ms
	string type;
};

enum TimerState {
	tsBeforeStart,
	tsPause,
	tsTestStart,
	tsTestInvalid,
	tsTestEnd,
};

class Timer {
	TimerState state;
	HWND hwnd;
	HBRUSH brush1; // follows enum IDs
	HBRUSH brush2;
	HBRUSH brush3;
	HBRUSH brush4;
	HBRUSH brush5;
	vector<Score> scores;
	Clock* clock;
	string text; // colored area text
	HFONT font;
	public:
		Timer(HWND hwnd);
		~Timer();
		
		int OnMessage(UINT Message, WPARAM wParam, LPARAM lParam);
		void OnClick(const char* changetype);
		void SetState(TimerState state,const char* changetype);
		string GetScoreHistory();
};

#endif
