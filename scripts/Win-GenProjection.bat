@echo off
pushd ..\
call vendor\bin\premake\premake5win.exe vs2019
popd

PAUSE