:: This script builds entire ION system and forms 'bar' directory.
:: bcc32 and ilink32 are required.
:: Copyright (c) 2014 Mark Karpov

@echo off

:: setting some parameters

setlocal enableextensions

:: edit these variables if needed, also take a look at the section
:: 'copying of resources' if you have version of C++ Builder different from XE5

set rad=c:\program files\embarcadero\rad studio\11.0\include
set bin=c:\program files\embarcadero\rad studio\11.0\bin

:: creation of 'bar'

if not exist bar\bin mkdir bar\bin

:: compilation of prl.dll

call :build_link_dll prl
copy .\src\libs\prl\win32\release\prl.dll .\bar\bin

:: compilation of wav.dll

call :build_link_dll wav
copy .\src\libs\wav\win32\release\wav.dll .\bar\bin

:: compilation of ion.dll

call :build_link_dll ion .\src\libs\prl\win32\release\prl.lib .\src\libs\wav\win32\release\wav.lib
copy .\src\libs\ion\win32\release\ion.dll .\bar\bin

:: compilation of cie.bpl

call :build_module cie cie
call :build_module cie ciearg
call :build_module cie cieexc
call :build_module cie cieslc
call :build_module cie ciecnf
call :build_module cie cieabt
call :build_module cie ciechs
call :build_module cie ciefnc
call :build_module cie cieprg
call :link_cie_package
copy .\src\cie\win32\release\cie.bpl .\bar\bin

:: compilation of ion.exe

call :build_app ion ion
call :build_app ion main
call :link_app ion
copy .\src\ion\win32\release\ion.exe .\bar\bin

:: compilation of prl.exe

call :build_app prl prl
call :build_app prl main
call :link_app prl
copy .\src\prl\win32\release\prl.exe .\bar\bin

:: compilation of wm.exe

call :build_app wm wm
call :build_app wm main
call :link_app wm
copy .\src\wm\win32\release\wm.exe .\bar\bin

:: compilation of scr.exe

call :build_app scr scr
call :build_app scr main
call :link_app scr
copy .\src\scr\win32\release\scr.exe .\bar\bin

:: compilation of snr.exe

call :build_app snr snr
call :build_app snr main
call :link_app snr
copy .\src\snr\win32\release\snr.exe .\bar\bin

:: compilation of gnr.exe

call :build_app gnr gnr
call :build_app gnr main
call :build_app gnr wizard
call :build_app gnr egenerator
call :link_app gnr .\src\gnr\win32\release\wizard.obj .\src\gnr\win32\release\egenerator.obj
copy .\src\gnr\win32\release\gnr.exe .\bar\bin

:: copying of resources

copy LICENSE.md                 bar
copy NEWS.md                    bar
copy README.md                  bar
copy resources\icons\prldoc.ico bar
copy resources\icons\scrdoc.ico bar
copy resources\icons\snrdoc.ico bar
copy resources\icons\gnrdoc.ico bar
copy resources\prl\ion.prl      bar
xcopy resources\scores          bar\scores\     /s /e
xcopy resources\sonorities      bar\sonorities\ /s /e
xcopy resources\generators      bar\generators\ /s /e
copy "%bin%\borlndmm.dll"       bar\bin
copy "%bin%\cc32130.dll"        bar\bin
copy "%bin%\cc32130mt.dll"      bar\bin
copy "%bin%\rtl180.bpl"         bar\bin
copy "%bin%\vcl180.bpl"         bar\bin
copy "%bin%\vclactnband180.bpl" bar\bin
copy "%bin%\vclx180.bpl"        bar\bin

goto :eof

:: procedures

:build_link_dll

if not exist .\src\libs\%1\win32\release mkdir .\src\libs\%1\win32\release

bcc32 -DNDEBUG -D_RTLDLL;USEPACKAGES -n.\src\libs\%1\win32\release ^
-I"%rad%\dinkumware";".\src\libs\include"; ^
-Q -c -tD -tM -C8 -o.\src\libs\%1\win32\release\%1.obj ^
-w-par -O2 -v- -vi -H=.\src\libs\%1\win32\release\%1.pch ^
-H .\src\libs\%1\%1.cpp

ilink32 -L.\src\libs\%1\win32\release -l.\src\libs\%1\win32\release ^
-Gi -H:0x00400000 -V5.0 -G8 -Tpd c0d32 rtl.bpi memmgr.lib sysinit.obj ^
.\src\libs\%1\win32\release\%1.obj , ^
.\src\libs\%1\win32\release\%1.dll , ^
.\src\libs\%1\win32\release\%1.map , ^
vcle.lib %2 %3 import32.lib cp32mti.lib , , .\src\libs\%1\%1.res

goto :eof

:build_module

if not exist .\src\%1\win32\release mkdir .\src\%1\win32\release

bcc32 -DNDEBUG -D_RTLDLL;USEPACKAGES -n.\src\%1\win32\release ^
-I"%rad%\dinkumware";".\src\libs\include";".\src\cie" ^
-Q -c -tD -tM -C8 -o.\src\%1\win32\release\%2.obj ^
-w-par -O2 -v- -vi -H=.\src\%1\win32\release\%2.pch -H .\src\%1\%2.cpp

goto :eof

:link_cie_package

copy .\src\cie\*.dfm .\src\cie\win32\release

ilink32 -L.\src\cie\win32\release -l.\src\cie\win32\release ^
-Gi -aa -V5.0 -Gpr -GBcie -G8 -Tpp c0pkg32 rtl.bpi vcl.bpi vclactnband.bpi ^
vclx.bpi memmgr.lib sysinit.obj ^
.\src\cie\win32\release\cie.obj ^
.\src\cie\win32\release\cieabt.obj ^
.\src\cie\win32\release\ciearg.obj ^
.\src\cie\win32\release\ciechs.obj ^
.\src\cie\win32\release\ciecnf.obj ^
.\src\cie\win32\release\cieexc.obj ^
.\src\cie\win32\release\ciefnc.obj ^
.\src\cie\win32\release\cieprg.obj ^
.\src\cie\win32\release\cieslc.obj , ^
.\src\cie\win32\release\cie.bpl , ^
.\src\cie\win32\release\cie.map , ^
.\src\libs\prl\win32\release\prl.lib import32.lib cp32mti.lib , , .\src\cie\cie.res

goto :eof

:build_app

if not exist .\src\%1\win32\release mkdir .\src\%1\win32\release

bcc32 -DNDEBUG -D_RTLDLL;USEPACKAGES -n.\src\%1\win32\release ^
-I"%rad%\dinkumware";".\src\libs\include";".\src\cie" ^
-Q -c -tM -tU -tW -C8 -o.\src\%1\win32\release\%2.obj -w-par -O2 ^
-v- -vi -H=.\src\%1\win32\release\ion.pch -H .\src\%1\%2.cpp

goto :eof

:link_app

copy .\src\%1\*.dfm .\src\%1\win32\release

ilink32.exe -L.\src\%1\win32\release -l.\src\%1\win32\release ^
-Gi -aa -V5.0 -G8 -Tpe  c0w32w rtl.bpi vcl.bpi vclactnband.bpi ^
vclx.bpi .\src\cie\win32\release\cie.bpi memmgr.lib sysinit.obj ^
.\src\%1\win32\release\%1.obj .\src\%1\win32\release\main.obj , ^
.\src\%1\win32\release\%1.exe , ^
.\src\%1\win32\release\%1.map , ^
import32.lib .\src\libs\prl\win32\release\prl.lib ^
.\src\libs\wav\win32\release\wav.lib cp32mti.lib ^
.\src\libs\ion\win32\release\ion.lib %2 %3 , , .\src\%1\%1.res 

goto :eof
