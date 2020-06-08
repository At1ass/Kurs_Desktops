#define WM_ShellNote (WM_APP + 1)
#define id_MyIcon 123

#include <Windows.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>


HWND hWnd;
HINSTANCE hInstance;
NOTIFYICONDATA Icon = {0};
HMENU hMenu;
HDESK original_desktop, hidden_desktop[4], hidden;
int f = 0;
BOOL bRes;
const char Desktops[4][1024] = {"MyDesk1", "Mydesk2", "Mydesk3", "Mydesk4"};
int have_desk[4] = { 0, 0, 0, 0 };

HDESK CreateHiddenDesktop(TCHAR *desktop_name)
{
	HDESK hidden_desktopp = NULL;
	HDESK original_desktop = GetThreadDesktop(GetCurrentThreadId());
	if (!hidden_desktopp)
	{
		hidden_desktopp = CreateDesktop(desktop_name, NULL, NULL, 0, GENERIC_ALL, NULL);
	}

	return hidden_desktopp;
}

void Transfer_Handle(int b)
{
	if (CloseDesktop(hidden_desktop[b]))
		MessageBox(hWnd, "Рабочий стол успешно удален", "VICTORY", MB_OK);
	else
		MessageBox(hWnd, "Рабочий стол не удален", "ERROR", MB_OK);
	have_desk[b] = 0;
}


LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	int lw = LOWORD(wParam);
	switch (msg)
	{
	case WM_SYSCOMMAND:
	{
		if (wParam == SC_MINIMIZE) {
			ShowWindow(hWnd, SW_HIDE);
			return 0;
		}
		if (wParam == SC_CLOSE)
		{
			SendMessage(hWnd, WM_DESTROY, 0, 0);
		}
	}
	return 0;
///////////////////////////////////////////////////////////////////////////////////////
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
			case VK_F1:
			{

				SetThreadDesktop(hidden_desktop[0]);
				//{
					//MessageBox(hWnd, "NO", "ERROR", MB_OK);
				//	return 0;
				//}

				STARTUPINFO si;
				PROCESS_INFORMATION pi;
				ZeroMemory(&si, sizeof(si));
				si.cb = sizeof(si);
				si.lpDesktop = (LPSTR)"MyDesk1";
				ZeroMemory(&pi, sizeof(pi));
				TCHAR procName[1024] = "explorer.exe";
				bRes = CreateProcess(0, procName, 0, 0, 0, 0, 0, 0, &si, &pi);

				SwitchDesktop(hidden_desktop[0]);
			}
			break;
				
			case VK_F2:
			{
				SetThreadDesktop(hidden_desktop[1]);
				STARTUPINFO si;
				PROCESS_INFORMATION pi;
				ZeroMemory(&si, sizeof(si));
				si.cb = sizeof(si);
				si.lpDesktop = (LPSTR)"MyDesk2";
				ZeroMemory(&pi, sizeof(pi));
				TCHAR procName[1024] = "explorer.exe";
				bRes = CreateProcess(0, procName, 0, 0, 0, 0, 0, 0, &si, &pi);

				SwitchDesktop(hidden_desktop[1]);
			}
			break;
			case VK_F3:
			{
				SetThreadDesktop(hidden_desktop[2]);
				STARTUPINFO si;
				PROCESS_INFORMATION pi;
				ZeroMemory(&si, sizeof(si));
				si.cb = sizeof(si);
				si.lpDesktop = (LPSTR)"MyDesk3";
				ZeroMemory(&pi, sizeof(pi));
				TCHAR procName[1024] = "explorer.exe";
				bRes = CreateProcess(0, procName, 0, 0, 0, 0, 0, 0, &si, &pi);
				SwitchDesktop(hidden_desktop[2]);
			}
			break;
			case VK_F4:
			{
				SetThreadDesktop(hidden_desktop[3]);
				STARTUPINFO si;
				PROCESS_INFORMATION pi;
				ZeroMemory(&si, sizeof(si));
				si.cb = sizeof(si);
				si.lpDesktop = (LPSTR)"MyDesk4";
				ZeroMemory(&pi, sizeof(pi));
				TCHAR procName[1024] = "explorer.exe";
				bRes = CreateProcess(0, procName, 0, 0, 0, 0, 0, 0, &si, &pi);
				SwitchDesktop(hidden_desktop[3]);
			}
			break;
			default:
			{
				//MessageBox(hWnd, "Используйте F1 - F4", "NOT", MB_OK);
			}
				
		}
		
	}
/////////////////////////////////////////////////////////////////////////////////////////////
	return 0;

	case WM_CHAR:
	{
		switch (wParam)
		{
			case '1':
			{
				Transfer_Handle(0);
				
			}
			break;
			case '2':
			{
				Transfer_Handle(1);

			}
			break;
			case '3':
			{
				Transfer_Handle(2);

			}
			break;
			case '4':
			{
				Transfer_Handle(3);

			}
			break;
			default:
			{
				MessageBox(hWnd, "Для удаления рабочего стола \n нажмите от 1 до 4 соответственно", "PRIVET", MB_OK);
			}
			break;
			

		}
	}
	return 0;

	case WM_COMMAND:
	{
		switch (lw)
		{
			case 0:
			{
				int count = 0;
				for (int i = 0; i < 4; i++)
				{
					if (have_desk[i] == 0)
					{
						hidden_desktop[i] = CreateDesktop(Desktops[i], NULL, NULL, DF_ALLOWOTHERACCOUNTHOOK, GENERIC_ALL, NULL);
						MessageBox(hWnd, "Рабочий стол создан", "УВЕДОМЛЕНИЕ", MB_OK);
						have_desk[i] = 1;
						break;
					}
					count++;
				}
				if (count == 4)
				{
					MessageBox(hWnd, "Нельзя создать больше 4 столов", "DENIED", MB_OK);
				}
				
			}
			return 0;

			case 1:
			{
				MessageBox(hWnd, "Для удаления рабочего стола \n нажмите от 1 до 4 соответственно", "PRIVET", 0);
			}
			return 0;

			case 2:
			{
				MessageBox(hWnd, "Используйте F1 - F4", "NOT", MB_OK);
			}
			return 0;

			case 3:
				SendMessage(hWnd, WM_DESTROY, 0, 0);
			return 0;

			case 4:
			{
				TCHAR str[8];
				for (int r = 0; r < 7; r++)
				{
					if (r % 2 == 0)
						if (have_desk[r - r / 2] == 1)
							str[r] = '1';
						else
							str[r] = '0';
					else
						str[r] = ' ';
				}
				str[7] = '\0';

				MessageBox(hWnd, str, "НАЛИЧИЕ", MB_OK);
			}
			return 0;

			default:
				return 0;

		}
	}
	return 0;

	case WM_ShellNote:
	{
		switch (lParam) {
		case WM_LBUTTONDBLCLK:
			ShowWindow(hWnd, SW_SHOW);
		break;
		case WM_RBUTTONUP:	
		{	GetCursorPos(&pt);	
			TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
			DestroyMenu(hMenu);
		}
			break;

		default:
			break;
		}

	}

	case WM_CREATE:
	{
		original_desktop = GetThreadDesktop(GetCurrentThreadId());
		hMenu = CreatePopupMenu();
		AppendMenu(hMenu, MF_STRING, 0, "Создать новый рабочий стол");
		AppendMenu(hMenu, MF_STRING, 1, "Удалить рабочий стол");
		AppendMenu(hMenu, MF_STRING, 2, "Переключиться между рабочими столами");
		AppendMenu(hMenu, MF_STRING, 4, "Активные рабочие столы");
		AppendMenu(hMenu, MF_SEPARATOR, 6, 0);
		AppendMenu(hMenu, MF_STRING, 3, "Выход из программы");
		
	}
	return 0;

	case WM_PAINT:
	{

	}
	return 0;

	case WM_DESTROY:
	{
		Shell_NotifyIcon(NIM_DELETE, &Icon);
		PostQuitMessage(0);
	}
	return 0;

	default:

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInsrance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg{};

	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpszClassName = "MyClass";
	wc.lpszMenuName = 0;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = WinProc;

	if (!(RegisterClassEx(&wc)))
		return FALSE;

	if ((hWnd = CreateWindowEx(0, wc.lpszClassName, "Window", (WS_OVERLAPPEDWINDOW&~WS_THICKFRAME) | WS_VISIBLE, 50, 50, 400, 400, nullptr, nullptr, wc.hInstance, nullptr)) == INVALID_HANDLE_VALUE)
		return FALSE;

	Icon.cbSize = sizeof(NOTIFYICONDATA);
	Icon.hWnd = hWnd;
	Icon.uVersion = NOTIFYICON_VERSION;
	Icon.uID = id_MyIcon;
	Icon.uCallbackMessage = WM_ShellNote;
	Icon.hIcon = LoadIcon(NULL, IDI_SHIELD);
	Icon.uFlags = NIF_MESSAGE | NIF_ICON;
	Shell_NotifyIcon(NIM_ADD, &Icon);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	if (RegisterHotKey(NULL, 1, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, 0x45)) {
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			if (msg.message == WM_HOTKEY)
			{
				SwitchDesktop(original_desktop);
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return static_cast<int>(msg.wParam);
}