// WindowsProject1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WindowsProject1.h"


#define MAX_LOADSTRING 100



// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.


    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_CROSS);
	wcex.hbrBackground	= static_cast<HBRUSH>(CreateSolidBrush(RGB(0, 153, 51)));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)Freq);

   //start the timer
   SetTimer(hWnd,			// handle to main window 
	   0,					// timer identifier 
	   1000 / 30,			// 33 ~ 30fps 
	   (TIMERPROC)NULL);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
int pw = 60;
int pn = 60;
int counter=0;
int state = 0;
std::vector<int> northCars = {  };		// Decrease to drive
std::vector<int> eastCars = {  };		// Increase to drive
std::vector<int> southCars = {  };		// Increase to drive
std::vector<int> westCars = {  };		// Decrease to drive
std::vector<int> carArr[4] = { northCars, eastCars, southCars, westCars };
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HDC memHdc;
	HBITMAP memBM;

    switch (message)
    {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		addCar(EAST_CAR);
		break;					
	case WM_TIMER:
		counter++;
		//State of the junction
		junctionState();
		if (!(counter % 60)) {
			if (rand() % 101 < pw) {
				if (rand() % 2)
					addCar(EAST_CAR);
				else
					addCar(WEST_CAR);
			}
			if (rand() % 101 < pn) {
				if (rand() % 2)
					addCar(SOUTH_CAR);
				else
					addCar(NORTH_CAR);
			}
		}
		//Move the cars
		DriveCars(carArr, 5);
		InvalidateRect(hWnd, NULL, FALSE);
		UpdateWindow(hWnd);
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		
		RECT dim;
		GetClientRect(hWnd, &dim);

		memHdc = CreateCompatibleDC(hdc);
		memBM = CreateCompatibleBitmap(hdc, dim.right-dim.left, dim.bottom-dim.top);
		SelectObject(memHdc, memBM);


		//1280 x 720
		double scaleX = (dim.right - dim.left) / 1280.0;
		double scaleY = (dim.bottom - dim.top) / 720.0;
		//Grass
		DrawGrass(memHdc, dim);
		//Road 
		DrawRoad(int(500*scaleX), 0, int(200 * scaleX), dim.bottom, memHdc);
		DrawRoad(0, int(300 * scaleY), dim.right, int(200 * scaleY), memHdc);
		
		//North/South
		DrawTrafficLight(int(420 * scaleX), int(70 * scaleY), int(60 * scaleX), state, memHdc);
		DrawTrafficLight(int(720 * scaleX), int(510 * scaleY), int(60 * scaleX), state, memHdc);
		//West/East
		DrawTrafficLight(int(720 * scaleX), int(70 * scaleY), int(60 * scaleX), state+2, memHdc);
		DrawTrafficLight(int(420 * scaleX), int(510 * scaleY), int(60 * scaleX), state+2, memHdc);
		
		//Draw the cars
		DrawCar(memHdc, carArr);

		//Double buffering - switch visible buffer
		BitBlt(hdc, 0, 0, dim.right, dim.bottom, memHdc, 0, 0, SRCCOPY);
		
		EndPaint(hWnd, &ps);
		DeleteDC(memHdc);
		DeleteObject(memBM);
		break;
	}
	case WM_RBUTTONDOWN:
		addCar(SOUTH_CAR);
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_RIGHT:
			pw += 10;
			if (pw > 100)
				pw = 100;
			break;
		case VK_LEFT:
			pw -= 10;
			if (pw < 0)
				pw = 0;
			break;
		case VK_UP:
			pn += 10;
			if (pn > 100)
				pn = 100;
			break;
		case VK_DOWN:
			pn -= 10;
			if (pn < 0)
				pn = 0;
			break;
		default:
			break;
		}
		break;
	break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


LRESULT CALLBACK Freq(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	WORD id = LOWORD(wParam);
	switch (message) {

	case WM_INITDIALOG:     // Initializing
		return TRUE;

	case WM_COMMAND:        // Handling command events
		if (id == IDOK || id == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));

			WCHAR gBuff[5];
			GetWindowText(GetDlgItem(hDlg, IDC_EDIT1), gBuff, 5);
			wchar_t * pEnd;
			pw = wcstol(gBuff, &pEnd, 10);
			GetWindowText(GetDlgItem(hDlg, IDC_EDIT2), gBuff, 5);
			pn = wcstol(gBuff, &pEnd, 10);
			return TRUE;
		}
		break;
	}
	return FALSE;
}


void DrawRoad(int startX, int startY, int width, int length, HDC hdc) {
	HBRUSH hBrush;
	HGDIOBJ hOrg;
	int startRoadX = startX;
	int startRoadY = startY;
	int roadWidth = width;
	int roadLength = length;
	hBrush = CreateSolidBrush(RGB(70, 70, 70));
	hOrg = SelectObject(hdc, hBrush);
	Rectangle(hdc, startRoadX, startRoadY,
		startRoadX + roadWidth, startRoadY + roadLength);
	SelectObject(hdc, hOrg);
	DeleteObject(hBrush);
}

void DrawTrafficLight(int startX, int startY, int size, int state, HDC hdc) {
	
	int startTrafficlightX = startX;
	int startTrafficlightY = startY;
	int width = size;
	state %= 4;
	HBRUSH hBrush;
	HGDIOBJ hOrg;

	// Black rectangle
	// Select black brush
	hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
	// Make rectangle
	hOrg = SelectObject(hdc, hBrush);
	Rectangle(hdc, startTrafficlightX + 0, startTrafficlightY + 0,
		startTrafficlightX + width, startTrafficlightY + width+ 2 * int(width*(7.0 / 8.0)));

	COLORREF  oldCol;
	// Red circle
	if (state < 2) {
		hBrush = (HBRUSH)GetStockObject(DC_BRUSH);
		oldCol = SetDCBrushColor(hdc, RGB(255, 0, 0));
	}
	else {
		hBrush = (HBRUSH)GetStockObject(DC_BRUSH);
		oldCol = SetDCBrushColor(hdc, RGB(128, 128, 128));
	}
	SelectObject(hdc, hBrush);
	Ellipse(hdc, startTrafficlightX + int(width*(1.0 / 8.0)), startTrafficlightY + int(width*(1.0 / 8.0)),
		startTrafficlightX + int(width*(7.0/8.0)), startTrafficlightY + int(width*(7.0 / 8.0)));
	SetDCBrushColor(hdc, oldCol);

	// Yellow circle
	if (state % 2) {
		hBrush = (HBRUSH)GetStockObject(DC_BRUSH);
		oldCol = SetDCBrushColor(hdc, RGB(255, 255, 0));
	}
	else {
		hBrush = (HBRUSH)GetStockObject(DC_BRUSH);
		oldCol = SetDCBrushColor(hdc, RGB(128, 128, 128));
	}
	SelectObject(hdc, hBrush);
	Ellipse(hdc, startTrafficlightX + int(width*(1.0 / 8.0)), startTrafficlightY + width,
		startTrafficlightX + int(width*(7.0 / 8.0)), startTrafficlightY + 2*int(width*(7.0 / 8.0)));
	SetDCBrushColor(hdc, oldCol);

	// Green circle
	if (state == 2) {
		hBrush = (HBRUSH)GetStockObject(DC_BRUSH);
		oldCol = SetDCBrushColor(hdc, RGB(0, 255, 0));
	}
	else {
		hBrush = (HBRUSH)GetStockObject(DC_BRUSH);
		oldCol = SetDCBrushColor(hdc, RGB(128, 128, 128));
	}
	SelectObject(hdc, hBrush);
	Ellipse(hdc, startTrafficlightX + int(width*(1.0 / 8.0)), startTrafficlightY + width
		+ int(width*(7.0 / 8.0)),startTrafficlightX + int(width*(7.0 / 8.0)), startTrafficlightY
		+ 3*int(width*(7.0 / 8.0)));
	SetDCBrushColor(hdc, oldCol);
	//Reset
	SelectObject(hdc, hOrg);
}


void DrawGrass(HDC hdc, RECT &dim) {
	HBRUSH hBrush;
	HGDIOBJ hOrg;
	COLORREF oldCol;
	hBrush = (HBRUSH)GetStockObject(DC_BRUSH);
	hOrg = SelectObject(hdc, hBrush);
	oldCol = SetDCBrushColor(hdc, RGB(0.486*255, 0.988*255, 0));
	Rectangle(hdc, dim.left, dim.top, dim.right, dim.bottom);
	SetDCBrushColor(hdc, oldCol);
	SelectObject(hdc, hOrg);
}


void DrawCar(HDC hdc, std::vector<int> carArr[4]) {

	int width  = 140;
	int height = 140;

	HBITMAP carsBM = LoadBitmap(hInst, (LPCWSTR)IDB_BITMAP4);
	HDC cars = CreateCompatibleDC(hdc);
	SelectObject(cars, carsBM);

	HDC temp = CreateCompatibleDC(hdc);
	HBITMAP tempBM = CreateCompatibleBitmap(hdc, width, height);
	SelectObject(temp, tempBM);

	HDC rotate = CreateCompatibleDC(hdc);
	HBITMAP rotateBM = CreateCompatibleBitmap(hdc, height, width);
	SelectObject(rotate, rotateBM);
	
	POINT point[3];
	point[0].x = 0;			point[0].y = width;
	point[1].x = 0;			point[1].y = 0;
	point[2].x = height;	point[2].y = width;

	for (unsigned int i = 0; i < carArr[3].size(); i++) {
		//West, Y = 130, X = move by decrease
		GdiTransparentBlt(hdc, carArr[3][i], 130, width, height, cars, 0, 0, width, height, RGB(255, 255, 255));
	}
	for (unsigned int i = 0; i < carArr[1].size(); i++) {
		//East, Y = 190, X = move by increase 
		StretchBlt(temp, 0, 0, width, height, cars, width-1, 0, -width, height, SRCCOPY);
		GdiTransparentBlt(hdc, carArr[1][i], 190, width, height, temp, 0, 0, width, height, RGB(255, 255, 255));
	}
	for (unsigned int i = 0; i < carArr[0].size(); i++) {
		//North, X = 400, Y = move by decrease
		StretchBlt(temp, 0, 0, width, height, cars,width - 1, 0, -width, height, SRCCOPY);
		PlgBlt(rotate, point, temp, 0, 0, width, height, NULL, NULL, NULL);
		GdiTransparentBlt(hdc, 400, carArr[0][i], height, width, rotate, 0, 0, height, width, RGB(255, 255, 255));
	}
	for (unsigned int i = 0; i < carArr[2].size(); i++) {
		//South, X = 330, Y = move by increase
		PlgBlt(rotate, point, cars, 0, 0, width, height, NULL, NULL, NULL);
		GdiTransparentBlt(hdc, 330, carArr[2][i], height, width, rotate, 0, 0, height, width, RGB(255, 255, 255));
	}


	DeleteObject(carsBM);
	DeleteDC(cars);
	DeleteDC(temp);
	DeleteObject(tempBM);
	DeleteDC(rotate);
	DeleteObject(rotateBM);
}

void junctionState() {
	if (counter > 30 * 5)
		state = 1;
	if (counter > 30 * 7)
		state = 2;
	if (counter > 30 * 12)
		state = 3;
	if (counter > 30 * 14) {
		state = 0;
		counter = 0;
	}
}

const int westDivider = 505;
const int eastDivider = 210;
const int northDivider = 300;
const int southDivider = 30;
const int spacing = 140 + 10;
void DriveCars(std::vector<int> carArr[4], int speed) {
	
		int stoppedAtLight = 0;
	for (unsigned int j = 0; j < carArr[3].size(); j++) {
		//If west/east may not drive
		if (state!=2) {
			//If west driving car has reached junction
			if (!(carArr[3][j] < westDivider+(stoppedAtLight*spacing)+10 &&
				carArr[3][j] > westDivider+(stoppedAtLight * spacing) - 10)) {
				//stop if at junction
				carArr[3][j] -= speed;
			}
			else {
				stoppedAtLight++;
			}
		}
		else
		{
			carArr[3][j] -= speed;
		}
		if (carArr[3][j] < -140) {
			carArr[3].erase(carArr[3].begin() + j);
		}
	}
	stoppedAtLight = 0;
	for (unsigned int j = 0; j < carArr[1].size(); j++) {
		//If west/east may not drive
		if (state != 2) {
			//If east driving car has reached junction
			if (!(carArr[1][j] < eastDivider - (stoppedAtLight * spacing) + 10 &&
				carArr[1][j] > eastDivider - (stoppedAtLight * spacing) - 10)) {
				//stop if at junction
				carArr[1][j] += speed;
			}
			else {
				stoppedAtLight++;
			}
		}
		else
		{
			carArr[1][j] += speed;
		}
		if (carArr[1][j] > 1000) {
			carArr[1].erase(carArr[1].begin() + j);
		}
	}
	stoppedAtLight = 0;
	for (unsigned int j = 0; j < carArr[0].size(); j++) {
		//If North/South may not drive
		if (state != 0) {
			//If east driving car has reached junction
			if (!(carArr[0][j] < northDivider + (stoppedAtLight * spacing) + 10 &&
				carArr[0][j] > northDivider + (stoppedAtLight * spacing) - 10)) {
				//stop if at junction
				carArr[0][j] -= speed;
			}
			else {
				stoppedAtLight++;
			}
		}
		else
		{
			carArr[0][j] -= speed;
		}
		if (carArr[0][j] < -140) {
			carArr[0].erase(carArr[0].begin() + j);
		}
	}
	stoppedAtLight = 0;
	for (unsigned int j = 0; j < carArr[2].size(); j++) {
		//If North/South may not drive
		if (state != 0) {
			//If east driving car has reached junction
			if (!(carArr[2][j] < southDivider - (stoppedAtLight * spacing) + 10 &&
				carArr[2][j] > southDivider - (stoppedAtLight * spacing) - 10)) {
				//stop if at junction
				carArr[2][j] += speed;
			}
			else {
				stoppedAtLight++;
			}
		}
		else
		{
			carArr[2][j] += speed;
		}
		if (carArr[2][j] > 1000) {
			carArr[2].erase(carArr[2].begin()+j);
		}
	}
}


void addCar(int direction) {
	int start = 0;
	if (direction == NORTH_CAR) {
		if (carArr[NORTH_CAR].size() > 0 && carArr[NORTH_CAR].back()>1000-spacing) {
			start = carArr[NORTH_CAR].back()+spacing;
		}else {
			start = 1000;
		}
	}
	if (direction == EAST_CAR) {
		if (carArr[EAST_CAR].size() > 0 && carArr[EAST_CAR].back() < 0) {
			start = carArr[EAST_CAR].back() - spacing;
		}
		else {
			start = -spacing;
		}
	}
	if (direction == SOUTH_CAR) {
		if (carArr[SOUTH_CAR].size() > 0 && carArr[SOUTH_CAR].back() < 0) {
			start = carArr[SOUTH_CAR].back() - spacing;
		}
		else {
			start = -spacing;
		}
	}
	if (direction == WEST_CAR) {
		if (carArr[WEST_CAR].size() > 0 && carArr[WEST_CAR].back() > 1000 - spacing) {
			start = carArr[WEST_CAR].back() + spacing;
		}
		else {
			start = 1000;
		}
	}
		carArr[direction].push_back(start);
}



