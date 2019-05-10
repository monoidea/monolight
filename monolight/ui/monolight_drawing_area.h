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

#ifndef __MONOLIGHT_DRAWING_AREA_H__
#define __MONOLIGHT_DRAWING_AREA_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#define MONOLIGHT_TYPE_DRAWING_AREA                (monolight_drawing_area_get_type())
#define MONOLIGHT_DRAWING_AREA(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOLIGHT_TYPE_DRAWING_AREA, MonolightDrawingArea))
#define MONOLIGHT_DRAWING_AREA_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOLIGHT_TYPE_DRAWING_AREA, MonolightDrawingAreaClass))
#define MONOLIGHT_IS_DRAWING_AREA(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOLIGHT_TYPE_DRAWING_AREA))
#define MONOLIGHT_IS_DRAWING_AREA_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOLIGHT_TYPE_DRAWING_AREA))
#define MONOLIGHT_DRAWING_AREA_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOLIGHT_TYPE_DRAWING_AREA, MonolightDrawingAreaClass))

typedef struct _MonolightDrawingArea MonolightDrawingArea;
typedef struct _MonolightDrawingAreaClass MonolightDrawingAreaClass;

struct _MonolightDrawingArea
{
  GtkWidget widget;
};

struct _MonolightDrawingAreaClass
{
  GtkWidgetClass widget;
};

GType monolight_drawing_area_get_type(void);

MonolightDrawingArea* monolight_drawing_area_new();

#endif /*__MONOLIGHT_DRAWING_AREA_H__*/
