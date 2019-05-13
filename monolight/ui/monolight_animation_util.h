/* Monolight - monoidea's monolight
 * Copyright (C) 2019 Joël Krähemann
 *
 * This file is part of Monolight.
 *
 * Monolight is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Monolight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Monolight.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __MONOLIGHT_ANIMATION_UTIL_H__
#define __MONOLIGHT_ANIMATION_UTIL_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

void monolight_animation_util_render_magnitude(cairo_t *cr,
					       guint width, guint height,
					       gdouble *magnitude_buffer,
					       guint buffer_size);

#endif /*__MONOLIGHT_ANIMATION_UTIL_H__*/
