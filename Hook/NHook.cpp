#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <Shlobj.h>
#include <shellapi.h>
#include <shlwapi.h>

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Shlwapi.lib")

#define IDT_MOUSETRAP 101

KBDLLHOOKSTRUCT kbdStruct;
HWND  			hwndMain;
HHOOK 			mouseHook;
HHOOK 			hKeyBoardHook;
HANDLE 			hThread = NULL;
HINSTANCE 		hInst;
UINT 			uResult;
TCHAR 			OutStr[225];
POINT 			ptOld;
int 			iSecond;

DWORD WINAPI CursorThread(LPVOID);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void GetMouseCoordinat();

LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		switch(wParam)
        {
			case WM_SYSKEYDOWN:
			case WM_SYSCOMMAND:
			case WM_KEYDOWN:
				if(uResult != 0)
					KillTimer(hwndMain,IDT_MOUSETRAP);
					
				if(hThread){
					SuspendThread(hThread);
					TerminateThread(hThread, 0);
					hThread = NULL;
				}
				if(!hThread)
					hThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CursorThread, (LPVOID)hwndMain, 0, NULL);
				
			break;
		}
	}
	return CallNextHookEx(hKeyBoardHook, nCode, wParam, lParam);
}

LRESULT __stdcall MouseHookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        switch(wParam)
        {
			//case WM_MOUSEMOVE:
				//GetMouseCoordinat();
			//break;
			case WM_MOUSEWHEEL:
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
				if(uResult != 0)
					KillTimer(hwndMain,IDT_MOUSETRAP);
				
				if(hThread){
					SuspendThread(hThread);
					TerminateThread(hThread, 0);
					hThread = NULL;
				}
				if(!hThread)
						hThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CursorThread, (LPVOID)hwndMain, 0, NULL);
			break;
        }
    }
    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

void SetHook()
{
    if (!(mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookCallback, NULL, 0)))
    {
         MessageBox(NULL,TEXT("FAILED TO INSTALL HOOK"),TEXT("HIT"),0);
    }
	
	if (!(hKeyBoardHook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0)))
	{
		MessageBox(NULL, L"Failed to install hook!", L"Error", MB_ICONERROR);
	}
}

DWORD WINAPI CursorThread(LPVOID lpvoid)
{
	uResult = SetTimer(hwndMain, IDT_MOUSETRAP, iSecond, (TIMERPROC) NULL);
	return 0;
}

int WINAPI wWinMain( /*_In_ */ HINSTANCE hInstance, /* _In_opt_ */ HINSTANCE, /*_In_*/ PWSTR pCmdLine, /*_In_*/ int nCmdShow)
{
    const wchar_t CLASS_NAME[]  = L"Hk";
    
    WNDCLASSEX wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	
    RegisterClassEx(&wc);
	hInst = hInstance;

    HWND hWnd = CreateWindowEx(0, CLASS_NAME, L"Go & hack the world", WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,     
        NULL,     
        hInstance,
        NULL      
        );

    if (hWnd == NULL)
    {
        return 0;
    }
	
	LPWSTR *szArglist = NULL;
	int nArgs;
	
	szArglist = CommandLineToArgvW(GetCommandLine(), &nArgs);
	
	if(nArgs != 1)
	{
		StrToIntEx(szArglist[1], STIF_DEFAULT, &iSecond);
		//MessageBox(NULL,szArglist[1], L"hit", 0);
	}
	else
	{
		iSecond = 45000;
	}
	
	
    //ShowWindow(hWnd, SW_HIDE);
    ShowWindow(hWnd, nCmdShow);

	SetHook();
	
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
    switch (uMsg)
    {
		case WM_CREATE:
			/* uResult = SetTimer(hWnd, IDT_MOUSETRAP, 10000, (TIMERPROC) NULL);
			if (uResult == 0) 
			{ 
				MessageBox(NULL,L"No timer is available.",L"Time",0);
			} */
			hwndMain = hWnd;
			
			if(!hThread)
					hThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CursorThread, (LPVOID)hwndMain, 0, NULL);
		break;
		
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			TextOut(hdc, 30, 30,OutStr,wcslen(OutStr));
			EndPaint(hWnd,&ps);
		break;
		
		case WM_TIMER:
			//SetCursorPos(400, 400);
			INPUT input;
			input.type=INPUT_MOUSE;
			input.mi.dx=ptOld.x;
			input.mi.dy=ptOld.y;
			input.mi.dwFlags=(MOUSEEVENTF_MOVE|MOUSEEVENTF_RIGHTDOWN|MOUSEEVENTF_RIGHTUP);
			input.mi.mouseData=1;
			input.mi.dwExtraInfo=NULL;
			input.mi.time=0;
			SendInput(1,&input,sizeof(INPUT));
		break;
		
		case WM_KEYDOWN:
		break;
			
		case WM_DESTROY:
			PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

VOID CALLBACK MyTimerProc( HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)  
{ 
    RECT rc; 
    //POINT pt;
	GetCursorPos(&ptOld); 
   
	
        //GetWindowRect(hwnd, &rc); 
        SetCursorPos(ptOld.x, ptOld.y); 
       // ptOld.x = pt.x; 
        //ptOld.y = pt.y;
  
} 

void GetMouseCoordinat()
{
	POINT ptOld;
	//int xPos, yPos;
	//xPos = GET_X_LPARAM(lParam); 
	//yPos = GET_Y_LPARAM(lParam); 
	GetCursorPos(&ptOld);
	wsprintf(OutStr,L"X = %d,  y = %d",ptOld.x, ptOld.y);
	 InvalidateRect(hwndMain, NULL, TRUE); 
}
//signtool.exe sign /a /t http://timestamp.verisign.com/scripts/timstamp.dll /v D:\hook\HookHack.exe
//makecert -sk HooKey -n "NitinRocks" NRocks.cer  