#pragma once
#include "pch.h"
#include <stdio.h>
#include <windows.h>
#include <Windows.h>
#include <stdlib.h>


#include <map>
#include <fstream>
#include <sstream>

#include "imgui.h"
#include "imgui_impl_opengl2.h"
#include "imgui_impl_win32.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include <GL/gl.h>
#include <GL/glu.h>

#include "MinHook.h"
#include <thread>


bool ui_open;
static bool clickspam = false;
static bool isspamclick = false;
static bool rightclickspam = false;
static bool isspamrightclick = false;
static bool bhoping = false;
static int clickdelay = 70;
std::hash<std::string> hash_str;

template <typename T>
inline MH_STATUS MH_CreateHookApiEx1(LPCWSTR pszModule, LPCSTR pszProcName, LPVOID pDetour, T** ppOriginal)
{
	return MH_CreateHookApi(pszModule, pszProcName, pDetour, reinterpret_cast<LPVOID*>(ppOriginal));
}

using namespace std;


BOOL WINAPI BYPASS()
{
	DWORD nOldProtect;
	if (!VirtualProtect(abort, 1, PAGE_EXECUTE_READWRITE, &nOldProtect))
		return FALSE;
	*(BYTE*)(abort) = 0xC3;
	if (!VirtualProtect(abort, 1, nOldProtect, &nOldProtect))
		return FALSE;
	if (!VirtualProtect(FreeConsole, 1, PAGE_EXECUTE_READWRITE, &nOldProtect))
		return FALSE;
	*(BYTE*)(FreeConsole) = 0xC3;
	if (!VirtualProtect(FreeConsole, 1, nOldProtect, &nOldProtect))
		return FALSE;
	AllocConsole();
}

void StartConsole() {
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	SetWindowPos(ConsoleHandle, HWND_TOPMOST, 50, 20, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	ShowWindow(ConsoleHandle, 1);
	SetConsoleTitle(TEXT("HaxonMC ~ by Google Chrome#6242"));
	printf("Welcome to HaxonMC, a cheat for Minecraft.\n");
	printf("For the moment its shit ok\n");
	printf("thank u KowalskiFX for that ImGui hook <3 \n");
	Sleep(3000);
}

void startSpamClick() {
	while (true) {
		if (clickspam) {
			while (isspamclick) {
				if (!ui_open) {
					__try {
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
						Sleep(clickdelay);
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					}
					__except (EXCEPTION_CONTINUE_EXECUTION) {}
				}
			}
		}
	}
}

void startBHOP() {
	while (true) {
		while(bhoping) {
			if (!ui_open) {
				__try {
					printf("should jump rn \n");
					keybd_event(VK_SPACE, 0, 0, 0);
					Sleep(550);
				}
				__except (EXCEPTION_CONTINUE_EXECUTION) {}
			}
		}
	}
}

void startRSpamClick() {
	while (true) {
		if (rightclickspam) {
			while (isspamrightclick) {
				if (!ui_open) {
					__try {
						mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
						Sleep(clickdelay);
						mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
					}
					__except (EXCEPTION_CONTINUE_EXECUTION) {}
				}
			}
		}
	}
}

void draw_imGUI(RECT rec)
{
	auto& io = ImGui::GetIO();
	io.DisplaySize.x = rec.right - rec.left;
	io.DisplaySize.y = rec.bottom - rec.top;
	/*-----------------------------------------------*/
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplWin32_NewFrame();
	/*-----------------------------------------------*/
	ImGui::NewFrame();

	/*-----------------------------------------------*/
	{

		ImGui::Begin("HaxonMC", &ui_open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);
		ImGui::Text("By Google Chrome#6242");
		ImGui::Checkbox("Hold to spam left click", &clickspam);
		ImGui::Checkbox("Hold to spam right click", &rightclickspam);
		ImGui::SliderInt("Delay within each clicks (ms)", &clickdelay, 1, 120);
		ImGui::Checkbox("Bunny Hop", &bhoping);
		ImGui::End();
	}
	/*-----------------------------------------------*/

	ImGui::Render();

	glClear(256);
	glMatrixMode(5889);
	glOrtho(0.0f, (int)io.DisplaySize.x, (int)io.DisplaySize.y, 0.0f, 0.0f, 100.0f);
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);

	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

}


HGLRC h_last_rc = NULL;
std::map<int, HGLRC> rc_pixelformat_lut;

void(*o_SwapBuffers)(HDC dc) = nullptr;
static void h_SwapBuffers(HDC dc)
{
	if (!dc)
		return o_SwapBuffers(dc);

	if (ui_open)
	{
		HWND const wnd = WindowFromDC(dc);
		RECT wnd_rect;
		GetClientRect(wnd, &wnd_rect);

		draw_imGUI(wnd_rect);
	}

	return o_SwapBuffers(dc);
}


typedef LRESULT(CALLBACK* pWindowProc) (
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
	);

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
pWindowProc oWndProc = nullptr;
static LRESULT WINAPI hWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{

	DWORD oof;
	auto& io = ImGui::GetIO();
	if (ui_open)
		if (ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam))
			return TRUE;

	switch (Msg)
	{
		case WM_CAPTURECHANGED:
		case WM_KEYUP:
		{
			if (wParam == VK_INSERT)
			{
				ui_open = !ui_open;
				return 0;
			}
			else {
				if (ui_open) {
					return 0;
				}
			}
			break;
		}
		case WM_LBUTTONDOWN: {
			if (!isspamclick) {
				if (clickspam) {
					isspamclick = true;
					printf("holding\n");
				}
			}
			break;
		}
		case WM_LBUTTONUP: {
			if (isspamclick) {
				isspamclick = false;
				printf("released\n");
			}
			break;
		}
		case WM_RBUTTONDOWN: {
			if (!isspamrightclick) {
				if (rightclickspam) {
					isspamrightclick = true;
					printf("holding\n");
				}
			}
			break;
		}
		case WM_RBUTTONUP: {
			if (isspamrightclick) {
				isspamrightclick = false;
				printf("released\n");
			}
			break;
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_CHAR:
		case WM_SETCURSOR:
		{
			if (ui_open)
			{
				break;
			}
		}
	}
ret:
	return CallWindowProc(oWndProc, hWnd, Msg, wParam, lParam);
}



void main()
{
	BYPASS();
	std::thread jew(StartConsole);
	ImGui::CreateContext();
	auto hMinecraft = FindWindow(TEXT("JLEWG"), NULL);
	ImGui_ImplWin32_Init(hMinecraft);
	ImGui_ImplOpenGL2_Init();
	oWndProc = (pWindowProc)SetWindowLongPtr(hMinecraft, GWLP_WNDPROC, (LONG_PTR)hWndProc);
	auto& style = ImGui::GetStyle();
	style.WindowTitleAlign = { 0.5, 0.5 };
	style.WindowRounding = 0;
	style.ScrollbarRounding = 0;
	MH_Initialize();
	if (MH_CreateHookApiEx1(L"Gdi32.dll", "SwapBuffers", &h_SwapBuffers, &o_SwapBuffers) != MH_OK)
	{
		MessageBox(NULL, TEXT("oof"), TEXT("oof"), MB_OK);
	}
	MH_EnableHook(MH_ALL_HOOKS);
	std::thread pp(startSpamClick);
	std::thread pp1(startRSpamClick);
	std::thread jew1(startBHOP);
}

BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, void* Reserved)
{
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(Module);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
		break;
	case DLL_PROCESS_DETACH:
		break;
	default: break;
	}

	return TRUE;
}

