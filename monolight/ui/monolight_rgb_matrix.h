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

#ifndef __MONOLIGHT_RGB_MATRIX_H__
#define __MONOLIGHT_RGB_MATRIX_H__

#include <glib.h>
#include <glib-object.h>

#define MONOLIGHT_TYPE_RGB_MATRIX                (monolight_rgb_matrix_get_type())
#define MONOLIGHT_RGB_MATRIX(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOLIGHT_TYPE_RGB_MATRIX, MonolightRGBMatrix))
#define MONOLIGHT_RGB_MATRIX_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOLIGHT_TYPE_RGB_MATRIX, MonolightRGBMatrixClass))
#define MONOLIGHT_IS_RGB_MATRIX(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOLIGHT_TYPE_RGB_MATRIX))
#define MONOLIGHT_IS_RGB_MATRIX_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOLIGHT_TYPE_RGB_MATRIX))
#define MONOLIGHT_RGB_MATRIX_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOLIGHT_TYPE_RGB_MATRIX, MonolightRGBMatrixClass))

#define MONOLIGHT_RGB_MATRIX_DEFAULT_PARALLEL_COUNT (3)
#define MONOLIGHT_RGB_MATRIX_DEFAULT_DAISY_CHAINED_COUNT (3)

#define MONOLIGHT_RGB_MATRIX_DEFAULT_WIDTH (32)
#define MONOLIGHT_RGB_MATRIX_DEFAULT_HEIGHT (32)

typedef struct _MonolightRGBMatrix MonolightRGBMatrix;
typedef struct _MonolightRGBMatrixClass MonolightRGBMatrixClass;

typedef enum{
  MONOLIGHT_RGB_MATRIX_RUNNING   = 1,
}MonolightRGBMatrixFlags;

typedef enum{
  MONOLIGHT_RGB_MATRIX_PIXEL_FORMAT_RGB24,
  MONOLIGHT_RGB_MATRIX_PIXEL_FORMAT_RGBA32,
}MonolightRGBMatrixPixelFormat;

struct _MonolightRGBMatrix
{
  GObject gobject;

  guint flags;

  guint audio_channels;
  
  guint samplerate;
  guint buffer_size;
  guint format;
  
  guint parallel_count;
  guint daisy_chained_count;
  
  guint width;
  guint height;

  guint pixel_format;
  void *pixel_buffer;
};

struct _MonolightRGBMatrixClass
{
  GObjectClass gobject;
};

GType monolight_rgb_matrix_get_type(void);

void monolight_rgb_matrix_render_magnitude(MonolightRGBMatrix *rgb_matrix,
					   guint audio_channel,
					   guint samplerate,
					   guint buffer_size,
					   gdouble *magnitude_buffer);

MonolightRGBMatrix* monolight_rgb_matrix_new();

#endif /*__MONOLIGHT_RGB_MATRIX_H__*/
