/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

        .text
        .global switch_page_directory
        .global enable_paging

enable_paging:
        pushl %ebp
        movl  %esp, %ebp
        movl  %cr0, %eax
        orl   $0x80000000, %eax
        movl  %eax, %cr0
        movl  %ebp, %esp
        popl  %ebp
        ret

switch_page_directory:
        pushl %ebp
        movl  %esp, %ebp
        movl  8(%esp), %eax
        movl  %eax, %cr3
        movl  %ebp, %esp
        popl  %ebp
        ret
