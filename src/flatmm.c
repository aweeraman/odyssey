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

#include "flatmm.h"
#include "libk.h"
#include "memory.h"

static flat_mem_page_t *root_frame = (flat_mem_page_t *) MEM_START_ADDR;

void* get_free_frame(size_t size)
{
  void* ptr = NULL;

  if (size > MEM_FRAME_SIZE) {
    return NULL;
  } else if (root_frame->flags &= FRAME_AVAILABLE) {
    root_frame->flags ^= FRAME_AVAILABLE;
    ptr = root_frame->frame;
  } else {
    return NULL;
  }

  return ptr;
}

void create_root_frame()
{
  root_frame->flags |= FRAME_ROOT | FRAME_AVAILABLE;
  root_frame->next = (size_t) root_frame+1;
  root_frame->prev = 0;
  memset(root_frame->frame, '\0', MEM_FRAME_SIZE);
}

void init_flatmm()
{
  printf("Initializing flat memory manager at 0x%x\n", MEM_START_ADDR);
  create_root_frame();
}
