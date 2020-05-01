/**
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2020, Anuradha Weeraman
 */

#include "gdt.h"
#include "libk.h"

gdt_descriptor_t gdt;
gdt_entry_t      gdt_entries[GDT_ENTRIES];

void gdt_init()
{
  printf("GDT [%d]\n", GDT_ENTRIES);

  gdt_entry(0, 0, 0, 0, 0);
  gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xC); /* kernel code segment */
  gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xC); /* kernel data segment */

  gdt.size   = sizeof(gdt_entries);
  gdt.offset = gdt_entries;

  load_gdt((uint32_t) (&gdt));
}

void gdt_entry(int entry, int32_t base, int32_t limit,
               uint8_t access, uint8_t flags)
{

  gdt_entries[entry].limit_lo = limit & 0xFFFF;
  gdt_entries[entry].base_lo  = base  & 0xFFFF;
  gdt_entries[entry].base_mid = (base >> 16)  & 0xFF;
  gdt_entries[entry].access   = access;
  gdt_entries[entry].limit_hi = (limit >> 16) & 0xF;
  gdt_entries[entry].flags    = flags & 0xF;
  gdt_entries[entry].base_hi  = (base >> 24)  & 0xFF;

  printf("  %d: base=%x limit=%x access=%b flags=%b\n",
               entry, base, limit, access, flags);
}
