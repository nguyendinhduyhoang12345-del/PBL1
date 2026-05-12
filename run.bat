@echo off
title Chay Do An PBL1
color 0F

echo ====================================================
echo      DANG BIEN DICH CHUONG TRINH PBL1...
echo ====================================================

:: Tat chuong trinh cu neu no dang chay ngam
taskkill /F /IM PBL1.exe >nul 2>&1

:: Bien dich tat ca cac file .c trong project thong qua gcc
gcc main.c app/services/*.c app/controllers/*.c app/ui/*.c app/utils/*.c -o PBL1.exe

:: Kiem tra xem qua trinh bien dich co bi loi khong
if %ERRORLEVEL% NEQ 0 (
    color 0C
    echo.
    echo [!] CO LOI XAY RA TRONG QUAN TRINH BIEN DICH.
    echo [!] Vui long cuon len tren de xem chi tiet loi o file nao.
    echo.
    pause
    exit /b
)

echo.
echo [OK] Bien dich thanh cong! 
echo [OK] Dang khoi dong chuong trinh...
echo.

:: Xoa man hinh console cho sach se va chay file thuc thi
cls
PBL1.exe
