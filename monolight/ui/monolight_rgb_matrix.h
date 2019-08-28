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

#include <cairo.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <led-matrix-c.h>

#define MONOLIGHT_TYPE_RGB_MATRIX                (monolight_rgb_matrix_get_type())
#define MONOLIGHT_RGB_MATRIX(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOLIGHT_TYPE_RGB_MATRIX, MonolightRGBMatrix))
#define MONOLIGHT_RGB_MATRIX_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOLIGHT_TYPE_RGB_MATRIX, MonolightRGBMatrixClass))
#define MONOLIGHT_IS_RGB_MATRIX(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOLIGHT_TYPE_RGB_MATRIX))
#define MONOLIGHT_IS_RGB_MATRIX_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOLIGHT_TYPE_RGB_MATRIX))
#define MONOLIGHT_RGB_MATRIX_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOLIGHT_TYPE_RGB_MATRIX, MonolightRGBMatrixClass))

#define MONOLIGHT_RGB_MATRIX_DEFAULT_AUDIO_CHANNELS (2)  

#define MONOLIGHT_RGB_MATRIX_DEFAULT_ROWS (32)
#define MONOLIGHT_RGB_MATRIX_DEFAULT_COLS (32)

#define MONOLIGHT_RGB_MATRIX_DEFAULT_CHAIN_LENGTH (9)
#define MONOLIGHT_RGB_MATRIX_DEFAULT_PARALLEL (1)

#define MONOLIGHT_RGB_MATRIX_DEFAULT_WIDTH (3 * MONOLIGHT_RGB_MATRIX_DEFAULT_COLS)
#define MONOLIGHT_RGB_MATRIX_DEFAULT_HEIGHT (3 * MONOLIGHT_RGB_MATRIX_DEFAULT_ROWS)

#define MONOLIGHT_RGB_MATRIX_DEFAULT_SCALE_FACTOR (1)

#define MONOLIGHT_RGB_MATRIX_TIME_LAPSE_DEFAULT_LENGTH (32)

typedef struct _MonolightRGBMatrix MonolightRGBMatrix;
typedef struct _MonolightRGBMatrixClass MonolightRGBMatrixClass;

typedef enum{
  MONOLIGHT_RGB_MATRIX_RUNNING                = 1,
  MONOLIGHT_RGB_MATRIX_DRAW_MAGNITUDE_BUFFER  = 1 <<  1,
}MonolightRGBMatrixFlags;

struct _MonolightRGBMatrix
{
  GObject gobject;

  guint flags;

  guint audio_channels;
  
  guint samplerate;
  guint buffer_size;

  gdouble **magnitude_buffer;

  int rows;
  int cols;
  
  int chain_length;
  int parallel;

  gint width;
  gint height;

  int argc;
  char **argv;
  
  struct RGBLedMatrixOptions options;
  struct RGBLedMatrix *led_matrix;
  
  struct LedCanvas *offscreen_canvas;

  AgsOscClient *osc_client;

  cairo_t *cr;

  gchar **program;
  guint program_count;
  
  gint position;
  gint current_period;
  
  guint time_lapse_length;
  
  gint *time_lapse_period;
  gint *time_lapse_program;
  
  gboolean inverse_angle;
  
  gdouble *time_lapse_start_angle;
  gdouble *time_lapse_end_angle;

  gdouble *time_lapse_scale;

  gint color_position;
  gint time_lapse_color_length;

  guint *time_lapse_red;
  guint *time_lapse_green;
  guint *time_lapse_blue;
  guint *time_lapse_alpha;
};

struct _MonolightRGBMatrixClass
{
  GObjectClass gobject;
};

GType monolight_rgb_matrix_get_type(void);

void monolight_rgb_matrix_start(MonolightRGBMatrix *rgb_matrix);
void monolight_rgb_matrix_stop(MonolightRGBMatrix *rgb_matrix);

uint8_t monolight_rgb_matrix_get_brightness(MonolightRGBMatrix *rgb_matrix);
void monolight_rgb_matrix_set_brightness(MonolightRGBMatrix *rgb_matrix,
					 uint8_t brightness);

void monolight_rgb_matrix_render_magnitude(MonolightRGBMatrix *rgb_matrix,
					   guint audio_channel,
					   guint samplerate,
					   guint buffer_size,
					   gdouble *magnitude_buffer);

gboolean monolight_rgb_matrix_magnitude_buffer_queue_draw_timeout(GObject *gobject);

MonolightRGBMatrix* monolight_rgb_matrix_new();

#endif /*__MONOLIGHT_RGB_MATRIX_H__*/
