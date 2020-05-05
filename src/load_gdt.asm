;**
; SPDX-License-Identifier: GPL-3.0-or-later
;
; Copyright 2020, Anuradha Weeraman
;

%define KERNEL_CODE_SEGMENT 0x08
%define KERNEL_DATA_SEGMENT 0x10

global load_gdt

load_gdt:
        mov eax, [esp + 4]
        lgdt [eax]

        mov ax, KERNEL_DATA_SEGMENT
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax

        jmp KERNEL_CODE_SEGMENT:reload
        ret

reload:
        ret
