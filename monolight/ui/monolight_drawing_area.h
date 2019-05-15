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

#define MONOLIGHT_DRAWING_AREA_DEFAULT_AUDIO_CHANNELS (2)

#define MONOLIGHT_DRAWING_AREA_DEFAULT_SCALE_FACTOR (8)

#define MONOLIGHT_DRAWING_AREA_DEFAULT_WIDTH (96)
#define MONOLIGHT_DRAWING_AREA_DEFAULT_HEIGHT (96)

#define MONOLIGHT_DRAWING_AREA_TIME_LAPSE_DEFAULT_LENGTH (32)

typedef struct _MonolightDrawingArea MonolightDrawingArea;
typedef struct _MonolightDrawingAreaClass MonolightDrawingAreaClass;

struct _MonolightDrawingArea
{
  GtkWidget widget;

  guint audio_channels;
  
  guint samplerate;
  guint buffer_size;

  gchar **program;
  guint program_count;
  
  gint position;
  gint current_period;
  
  guint time_lapse_length;
  
  gint *time_lapse_period;
  gint *time_lapse_program;
  
  gdouble *time_lapse_start_angle;
  gdouble *time_lapse_end_angle;

  gdouble *time_lapse_scale;

  guint *time_lapse_red;
  guint *time_lapse_green;
  guint *time_lapse_blue;
  guint *time_lapse_alpha;
};

struct _MonolightDrawingAreaClass
{
  GtkWidgetClass widget;
};

GType monolight_drawing_area_get_type(void);

void monolight_drawing_area_render_magnitude(MonolightDrawingArea *drawing_area,
					     guint audio_channel,
					     guint samplerate,
					     guint buffer_size,
					     gdouble *magnitude_buffer);

MonolightDrawingArea* monolight_drawing_area_new();

#endif /*__MONOLIGHT_DRAWING_AREA_H__*/
