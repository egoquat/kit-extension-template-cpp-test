@echo off
call "%~dp0repo" build %*
call "%~dp0repo" build %* --debug
