/***************************************************************************
 *  C++ source
 *
 *  ION Lanuncher
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
#include "main.h"

USEFORM("main.cpp", MainForm);

const wchar_t *aMutex = L"ION32-107d34c0-73c5-4e94-88fa-c0fdefd43ca3";

WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    try
    {
        HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, 0, aMutex);
        if (!hMutex) hMutex = CreateMutex(0, 0, aMutex);
        else
        {
            HWND hWnd = FindWindow(L"TMainForm", L"ION Launcher");
            SetForegroundWindow(hWnd);
            if (IsIconic(hWnd)) ShowWindow(hWnd, SW_RESTORE);
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
