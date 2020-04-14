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

#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>

#define STRINGIFY2(ver) #ver
#define STRINGIFY(ver) STRINGIFY2(ver)
#define CONFIG_VERSION "v" STRINGIFY(CONFIG_VERSION_MAJOR) "." \
  STRINGIFY(CONFIG_VERSION_MINOR)

#endif
