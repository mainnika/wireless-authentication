#pragma once

#include "Client.h"

class CSampleProvider;

class CCommandWindow
{
public:
    CCommandWindow();
    ~CCommandWindow();
    HRESULT Initialize(__in CSampleProvider *pProvider);
    BOOL GetConnectedStatus();

private:
    HRESULT _MyRegisterClass();
    HRESULT _InitInstance();
    BOOL _ProcessNextMessage();
    
    static DWORD WINAPI _ThreadProc(__in LPVOID lpParameter);
    static LRESULT CALLBACK    _WndProc(__in HWND hWnd, __in UINT message, __in WPARAM wParam, __in LPARAM lParam);
    
    CSampleProvider            *_pProvider;        // Pointer to our owner.
    HWND                        _hWnd;             // Handle to our window.
    HWND                        _hWndButton;       // Handle to our window's button.
    HINSTANCE                   _hInst;            // Current instance
    BOOL                        _fConnected;       // Whether or not we're connected.
};
