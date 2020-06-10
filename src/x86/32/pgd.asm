; SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
; SPDX-License-Identifier: GPL-3.0-or-later

global enable_paging
global switch_page_directory

enable_paging:
        push ebp
        mov  ebp, esp
        mov  eax, cr0
        or   eax, 0x80000000
        mov  cr0, eax
        mov  esp, ebp
        pop  ebp
        ret

switch_page_directory:
        push ebp
        mov  ebp, esp
        mov  eax, [esp+8]
        mov  cr3, eax
        mov  esp, ebp
        pop  ebp
        ret
