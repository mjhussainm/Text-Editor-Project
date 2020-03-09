
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <stdio.h>

// Global variables
#define OPEN_FILE_BUTTON 1
#define SAVE_FILE_BUTTON 2

// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Text Editor");

HINSTANCE hInst;
HWND hMainWindow, hEdit;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void AddControls(HWND);
void open_file(HWND);
void display_file(wchar_t*);
void save_file(HWND);
void write_file(wchar_t* path);


int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    WNDCLASSW wcex = {0};

    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = szWindowClass;

    if (!RegisterClassW(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    // Store instance handle in our global variable
    hInst = hInstance;

    hMainWindow = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 430, 400, NULL, NULL, NULL, NULL);

    if (!hMainWindow)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        switch (wParam)
        {
        case OPEN_FILE_BUTTON:
            open_file(hWnd);
            break;
        case SAVE_FILE_BUTTON:
            save_file(hWnd);
            break;
        }
        break;
    case WM_CREATE:
        AddControls(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

void AddControls(HWND hWnd)
{
    CreateWindow(L"Button", L"Open File", WS_VISIBLE | WS_CHILD, 10, 10, 150, 36, hWnd, (HMENU)OPEN_FILE_BUTTON, NULL, NULL);
    CreateWindow(L"Button", L"Save File", WS_VISIBLE | WS_CHILD, 170, 10, 150, 36, hWnd, (HMENU)SAVE_FILE_BUTTON, NULL, NULL);
    hEdit = CreateWindow(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER | WS_VSCROLL | WS_HSCROLL, 10, 50, 400, 300, hWnd, NULL, NULL, NULL);
}

void open_file(HWND hWnd)
{
    OPENFILENAME ofn;
    wchar_t file_name[200];

    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = file_name;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 200;
    ofn.lpstrFilter = L"All files\0*.*\0Source Files\0*.CPP\0Text Files\0*.TXT\0";
    ofn.nFilterIndex = 1;

    GetOpenFileName(&ofn);

    display_file(ofn.lpstrFile);
}

void display_file(wchar_t* path)
{
    HANDLE hFile = CreateFile(path,GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    constexpr size_t BUFSIZE = 10000;

    char buffer[BUFSIZE];
    memset(buffer, 0,BUFSIZE); //clear garbage i's

    DWORD dwBytesToRead = BUFSIZE - 1;
    DWORD dwBytesRead = 0;

    BOOL bRlt = ReadFile(hFile, (void*)buffer, dwBytesToRead, &dwBytesRead, NULL);

    SetWindowTextA(hEdit, buffer);

    CloseHandle(hFile);
}

void write_file(wchar_t* path)
{
    HANDLE hFile = CreateFile(path, GENERIC_WRITE, 0 , NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

    constexpr size_t BUFSIZE = 10000;

    char buffer[BUFSIZE];
    memset(buffer, 0, BUFSIZE);

    GetWindowTextA(hEdit, buffer, BUFSIZE);

    DWORD dwBytesToWrite = BUFSIZE - 1;
    DWORD dwBytesWrote = 0;

    WriteFile(hFile, (void*)buffer, dwBytesToWrite, &dwBytesWrote, NULL);

    CloseHandle(hFile);
}


void save_file(HWND hWnd)
{
    OPENFILENAME ofn;
    wchar_t file_name[200];

    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = file_name;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 200;
    ofn.lpstrFilter = L"All files\0*.*\0Source Files\0*.CPP\0Text Files\0*.TXT\0";
    ofn.nFilterIndex = 1;

    GetSaveFileName(&ofn);

    write_file(ofn.lpstrFile);
}
