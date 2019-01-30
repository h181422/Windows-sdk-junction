#pragma once

#include "resource.h"
#include <time.h>
#include <vector>
#include <iostream> 
#include <sstream> 

const int NORTH_CAR = 0;
const int EAST_CAR  = 1;
const int SOUTH_CAR = 2;
const int WEST_CAR  = 3;
	
void DrawRoad(int startX, int startY, int width, int length, HDC hdc);
void DrawTrafficLight(int startX, int startY, int size, int state, HDC hdc);
void DrawGrass(HDC hdc, RECT &dim);
void DrawCar(HDC hdc, std::vector<int> carArr[4]);
void junctionState();
void DriveCars(std::vector<int> carArr[4], int speed);
void addCar(int direction);
LRESULT CALLBACK Freq(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);