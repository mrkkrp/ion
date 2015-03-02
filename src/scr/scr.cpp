/***************************************************************************
 *  C++ source
 *
 *  ION Score Editor
 *
 *  Version 4.2.0
 *
 *  Copyright (c) 2014 Mark Karpov
 *
 *  This program is free software: you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 **************************************************************************/

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

USEFORM("main.cpp", MainForm);

const wchar_t *aMutex = L"SCR32-6143a35a-2821-4ef2-95bc-8041957bcc8d";

static const UINT UWM_AREYOUME = ::RegisterWindowMessage(aMutex);

HWND hWnd;
BOOL CALLBACK LittleCallback(HWND, LPARAM);

WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    try
    {
        HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, 0, aMutex);
        if (!hMutex) hMutex = CreateMutex(0, 0, aMutex);
        else // so, it's second copy of application
        {
            EnumWindows(LittleCallback, 0); // get handle of window
            SetForegroundWindow(hWnd);
            if (IsIconic(hWnd)) ShowWindow(hWnd, SW_RESTORE);
            if (ParamCount()) // if command line is not empty
            {
                wchar_t aCMDLine[256];
                wcscpy(aCMDLine, ParamStr(1).c_str());
                COPYDATASTRUCT aCDS;
                aCDS.cbData = (wcslen(aCMDLine) + 1) * 2;
                aCDS.lpData = aCMDLine;
                SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM)&aCDS);
            }
                        return EXIT_SUCCESS;
        }
        Application->Initialize();
        Application->MainFormOnTaskBar = true;
        Application->CreateForm(__classid(TMainForm), &MainForm);
        Application->Run();
        ReleaseMutex(hMutex);
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    catch (...)
    {
        try
        {
            throw Exception("");
        }
        catch (Exception &exception)
        {
            Application->ShowException(&exception);
        }
    }
    return EXIT_SUCCESS;
}

BOOL CALLBACK LittleCallback(HWND hWndArg, LPARAM lParamArg)
{
    if (SendMessage(hWndArg, UWM_AREYOUME, 0, 0))
    {
        hWnd = hWndArg;
        return false;
    }
    else return true;
}
