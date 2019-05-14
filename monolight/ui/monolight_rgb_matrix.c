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

#include <monolight/ui/monolight_rgb_matrix.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monolight/i18n.h>

void monolight_rgb_matrix_class_init(MonolightRGBMatrixClass *rgb_matrix);
void monolight_rgb_matrix_init(MonolightRGBMatrix *rgb_matrix);
void monolight_rgb_matrix_finalize(GObject *gobject);

/**
 * SECTION:monolight_rgb_matrix
 * @short_description: The RGB matrix object.
 * @title: MonolightRGBMatrix
 * @section_id:
 * @include: monolight/ui/monolight_rgb_matrix.h
 *
 * #MonolightRGBMatrix is an object dealing with RGB matrices.
 */

static gpointer monolight_rgb_matrix_parent_class = NULL;

GType
monolight_rgb_matrix_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monolight_type_rgb_matrix = 0;

    static const GTypeInfo monolight_rgb_matrix_info = {
      sizeof (MonolightRGBMatrixClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monolight_rgb_matrix_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonolightRGBMatrix),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monolight_rgb_matrix_init,
    };

    monolight_type_rgb_matrix = g_type_register_static(G_TYPE_OBJECT,
						       "MonolightRGBMatrix", &monolight_rgb_matrix_info,
						       0);

    g_once_init_leave(&g_define_type_id__volatile, monolight_type_rgb_matrix);
  }

  return g_define_type_id__volatile;
}

void
monolight_rgb_matrix_class_init(MonolightRGBMatrixClass *rgb_matrix)
{
  GObjectClass *gobject;

  monolight_rgb_matrix_parent_class = g_type_class_peek_parent(rgb_matrix);

  /* GObjectClass */
  gobject = (GObjectClass *) rgb_matrix;

  gobject->finalize = monolight_rgb_matrix_finalize;
}

void
monolight_rgb_matrix_init(MonolightRGBMatrix *rgb_matrix)
{
  rgb_matrix->flags = 0;
  
  rgb_matrix->audio_channels = AGS_SOUNDCARD_DEFAULT_PCM_CHANNELS;
  
  rgb_matrix->samplerate = AGS_SOUNDCARD_DEFAULT_SAMPLERATE;
  rgb_matrix->buffer_size = AGS_SOUNDCARD_DEFAULT_BUFFER_SIZE;
  rgb_matrix->format = AGS_SOUNDCARD_DOUBLE;

  rgb_matrix->parallel_count = MONOLIGHT_RGB_MATRIX_DEFAULT_PARALLEL_COUNT;
  rgb_matrix->daisy_chained_count = MONOLIGHT_RGB_MATRIX_DEFAULT_DAISY_CHAINED_COUNT;

  rgb_matrix->width = MONOLIGHT_RGB_MATRIX_DEFAULT_WIDTH;
  rgb_matrix->height = MONOLIGHT_RGB_MATRIX_DEFAULT_HEIGHT;

  rgb_matrix->pixel_format = MONOLIGHT_RGB_MATRIX_PIXEL_FORMAT_RGB24;
  rgb_matrix->pixel_buffer = (guint32 *) malloc(rgb_matrix->parallel_count *
						rgb_matrix->daisy_chained_count *
						rgb_matrix->width *
						rgb_matrix->height *
						sizeof(guint32));
}

void
monolight_rgb_matrix_finalize(GObject *gobject)
{
  MonolightRGBMatrix *rgb_matrix;

  rgb_matrix = (MonolightRGBMatrix *) gobject;

  if(rgb_matrix->pixel_buffer != NULL){
    free(rgb_matrix->pixel_buffer);
  }
  
  /* call parent */
  G_OBJECT_CLASS(monolight_rgb_matrix_parent_class)->finalize(gobject);
}

void
monolight_rgb_matrix_render_magnitude(MonolightRGBMatrix *rgb_matrix,
				      guint audio_channel,
				      guint samplerate,
				      guint buffer_size,
				      gdouble *magnitude_buffer)
{
  //TODO:JK: implement me
}

/**
 * monolight_rgb_matrix_new:
 *
 * Creates an #MonolightRGBMatrix
 *
 * Returns: a new #MonolightRGBMatrix
 *
 * Since: 1.0.0
 */
MonolightRGBMatrix*
monolight_rgb_matrix_new()
{
  MonolightRGBMatrix *rgb_matrix;

  rgb_matrix = (MonolightRGBMatrix *) g_object_new(MONOLIGHT_TYPE_RGB_MATRIX,
						   NULL);
  
  return(rgb_matrix);
}
