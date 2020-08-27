; SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
; SPDX-License-Identifier: GPL-3.0-or-later

%define KERNEL_CODE_SEGMENT 0x08
%define KERNEL_DATA_SEGMENT 0x10
%define TASK_STATE_SEGMENT  0x28

global load_gdt
global flush_tss

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

flush_tss:
	mov ax, TASK_STATE_SEGMENT
	ltr ax
	ret

reload:
        ret
