﻿/* error.c -- perror() + exit() for Windows.
*
* Copyright Ⓒ 2018-2019  Thomas Girard <thomas.g.girard@free.fr>
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
*  * Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*  * Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
*  * Neither the name of the author nor the names of its contributors
*    may be used to endorse or promote products derived from this software
*    without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*/

#include <sdkddkver.h>
#include <tchar.h>
#include <Windows.h>
#include <strsafe.h>

/*
 * This function is based on:
 *  https://msdn.microsoft.com/en-us/library/windows/desktop/ms680582(v=vs.85).aspx
 */
void ErrorExit(LPTSTR lpszFunction)
{
	LPVOID lpError;
	LPVOID lpDisplayBuf;
	DWORD dwLastError = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, dwLastError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpError, 0, NULL);

	lpDisplayBuf = (LPVOID) LocalAlloc(
		LMEM_ZEROINIT,
		(_tcslen((LPTSTR) lpError) + _tcslen(lpszFunction) + 14) * sizeof(TCHAR));
	StringCchPrintf(
		(LPTSTR) lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		_T("%s failed: %s"),
		lpszFunction, lpError);

	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, _T("connect-or-cut"), MB_OK | MB_ICONERROR);

	LocalFree(lpError);
	LocalFree(lpDisplayBuf);
	ExitProcess(dwLastError);
}
