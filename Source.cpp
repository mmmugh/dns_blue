#ifndef UNICODE
#define UNICODE
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <MinHook.h>
#include "exploit.h"

#if defined _M_X64
#pragma comment(lib, "minhook.x64.lib")
#elif defined _M_IX86
#pragma comment(lib, "minhook.x86.lib")
#endif

using namespace std;

typedef int (WINAPI *MESSAGEBOXW)(HWND, LPCWSTR, LPCWSTR, UINT);

// Pointer for calling original MessageBoxW.
MESSAGEBOXW fpMessageBoxW = NULL;

// Detour function which overrides MessageBoxW.
int WINAPI DetourMessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
	return fpMessageBoxW(hWnd, L"Hooked!", lpCaption, uType);
	return 1;
}


int test_hook_win_api()
{
	// Initialize MinHook.
	if (MH_Initialize() != MH_OK)
	{
		return 1;
	}

	// Create a hook for MessageBoxW, in disabled state.
	if (MH_CreateHook(&MessageBoxW, &DetourMessageBoxW,
		reinterpret_cast<LPVOID*>(&fpMessageBoxW)) != MH_OK)
	{
		return 1;
	}

	// or you can use the new helper funtion like this.
	//if (MH_CreateHookApiEx(
	//    L"user32", "MessageBoxW", &DetourMessageBoxW, &fpMessageBoxW) != MH_OK)
	//{
	//    return 1;
	//}

	// Enable the hook for MessageBoxW.
	if (MH_EnableHook(&MessageBoxW) != MH_OK)
	{
		return 1;
	}

	// Expected to tell "Hooked!".
	MessageBoxW(NULL, L"Not hooked...", L"MinHook Sample", MB_OK);

	// Disable the hook for MessageBoxW.
	if (MH_DisableHook(&MessageBoxW) != MH_OK)
	{
		return 1;
	}

	// Expected to tell "Not hooked...".
	MessageBoxW(NULL, L"Not hooked...", L"MinHook Sample", MB_OK);

	// Uninitialize MinHook.
	if (MH_Uninitialize() != MH_OK)
	{
		return 1;
	}

	return 0;

}

void Function()
{
	cout << "real function" << endl;
}

typedef void(*FUNCTION)();
FUNCTION fpFunction = NULL;

void DetourFunction()
{
	cout << "fake function" << endl;
}

int test_hook_common_function()
{

	if (MH_Initialize() != MH_OK)
	{
		return 1;
	}

	// Create a hook for MessageBoxW, in disabled state.
	if (MH_CreateHook(&Function, &DetourFunction,
		reinterpret_cast<LPVOID*>(&fpFunction)) != MH_OK)
	{
		return 1;
	}

	// Enable the hook for MessageBoxW.
	if (MH_EnableHook(&Function) != MH_OK)
	{
		return 1;
	}

	// Expected to tell "Hooked!".
	Function();

	// Disable the hook for MessageBoxW.
	if (MH_DisableHook(&Function) != MH_OK)
	{
		return 1;
	}

	// Expected to tell "Not hooked...".
	Function();

	// Uninitialize MinHook.
	if (MH_Uninitialize() != MH_OK)
	{
		return 1;
	}

	return 0;
}

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
int main()
{
	
	cout << "=========================================" << endl;
	cout << "1:test hook win api" << endl;
	cout << "2:test hook common function" << endl;
	cout << "=========================================" << endl;

	cout << "input:" << endl;
	int k;
	while (cin >> k)
	{
		switch (k)
		{
		case 1:
		{
			test_hook_win_api();
			break;
		}
		case 2:
		{
			test_hook_common_function();
		}
		}

		cout << "input:" << endl;
	}

	system("pause");


	return 0;
}

