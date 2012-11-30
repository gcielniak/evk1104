@echo off
setlocal enableextensions enabledelayedexpansion

: This command script programs the released ISP (flash array), the ISP
: configuration word (User page) and the general-purpose fuse bits.

: Copyright (C) 2006-2008, Atmel Corporation All rights reserved.
:
: Redistribution and use in source and binary forms, with or without
: modification, are permitted provided that the following conditions are met:
:
: 1. Redistributions of source code must retain the above copyright notice, this
: list of conditions and the following disclaimer.
:
: 2. Redistributions in binary form must reproduce the above copyright notice,
: this list of conditions and the following disclaimer in the documentation and/
: or other materials provided with the distribution.
:
: 3. The name of ATMEL may not be used to endorse or promote products derived
: from this software without specific prior written permission.
:
: THIS SOFTWARE IS PROVIDED BY ATMEL ``AS IS'' AND ANY EXPRESS OR IMPLIED
: WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
: MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
: SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
: INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
: BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
: DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
: OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
: NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
: EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


echo.
echo Programming MCU memory from `at32uc3a3-isp-1.0.3.hex'.
"c:\Program Files (x86)\Atmel\Atmel Studio 6.0\avrdbg\atprogram.exe" -t avrdragon -i jtag -d at32uc3a3256 program -c --verify --format hex -f at32uc3a3-isp-1.0.3.hex

: 5 second pause
PING 127.0.0.1 -n 6 >NUL

echo.
echo Programming ISP configuration word from `at32uc3a3-isp_cfg-1.0.3.bin'.
"c:\Program Files (x86)\Atmel\Atmel Studio 6.0\avrdbg\atprogram.exe" -t avrdragon -i jtag -d at32uc3a3256 program --format bin -o 0x808001FC -f at32uc3a3-isp_cfg-1.0.3.bin
: 5 second pause
PING 127.0.0.1 -n 6 >NUL

echo.
echo Programming general-purpose fuse bits.
"c:\Program Files (x86)\Atmel\Atmel Studio 6.0\avrdbg\atprogram.exe" -t avrdragon -i jtag -d at32uc3a3256 write -o 0xFFFE1410 -fs --values FFF7FFFF
: 5 second pause
PING 127.0.0.1 -n 6 >NUL

PAUSE