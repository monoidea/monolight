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

void monolight_rgb_matrix_create(MonolightRGBMatrix *rgb_matrix);
void monolight_rgb_matrix_delete(MonolightRGBMatrix *rgb_matrix);

void monolight_rgb_matrix_delegate_render(MonolightRGBMatrix *rgb_matrix,
					  cairo_t *cr,
					  gchar *program,
					  guint samplerate,
					  guint buffer_size,
					  gdouble *magnitude_buffer,
					  guint buffer_start, guint buffer_end,
					  guint x0, guint x1,
					  guint width, guint height,
					  gdouble angle, gdouble scale,
					  guint r, guint g, guint b, guint a);

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
  guint i;
  
  rgb_matrix->flags = 0;
  
  rgb_matrix->audio_channels = AGS_SOUNDCARD_DEFAULT_PCM_CHANNELS;
  
  rgb_matrix->samplerate = AGS_SOUNDCARD_DEFAULT_SAMPLERATE;
  rgb_matrix->buffer_size = AGS_SOUNDCARD_DEFAULT_BUFFER_SIZE;

  rgb_matrix->magnitude_buffer = (gdouble **) malloc(rgb_matrix->audio_channels * sizeof(gdouble *));
  
  for(i = 0; i < rgb_matrix->audio_channels; i++){
    rgb_matrix->magnitude_buffer[i] = (gdouble *) malloc(rgb_matrix->buffer_size * sizeof(gdouble));

    ags_audio_buffer_util_clear_double(rgb_matrix->magnitude_buffer[i], 1,
				       rgb_matrix->buffer_size);
  }

  rgb_matrix->rows = MONOLIGHT_RGB_MATRIX_DEFAULT_ROWS;
  rgb_matrix->cols = MONOLIGHT_RGB_MATRIX_DEFAULT_COLS;
  
  rgb_matrix->chain_length = MONOLIGHT_RGB_MATRIX_DEFAULT_CHAIN_LENGTH;
  rgb_matrix->parallel = MONOLIGHT_RGB_MATRIX_DEFAULT_PARALLEL;
  
  rgb_matrix->width = MONOLIGHT_RGB_MATRIX_DEFAULT_WIDTH;
  rgb_matrix->height = MONOLIGHT_RGB_MATRIX_DEFAULT_HEIGHT;

  rgb_matrix->argc = 0;
  rgb_matrix->argv = NULL;
  
  memset(&(rgb_matrix->options), 0, sizeof(rgb_matrix->options));
  rgb_matrix->led_matrix = NULL;

  rgb_matrix->offscreen_canvas = NULL;

  rgb_matrix->osc_client = ags_osc_client_new();
  ags_osc_client_set_flags(rgb_matrix->osc_client,
                           (AGS_OSC_CLIENT_INET4 |
                            AGS_OSC_CLIENT_TCP));

  rgb_matrix->cr = cairo_image_surface_create(CAIRO_FORMAT_RGB24,
					      rgb_matrix->width,
					      rgb_matrix->height);

  
  rgb_matrix->program = (gchar **) malloc(5 * sizeof(gchar *));

  rgb_matrix->program[0] = g_strdup("cross");
  rgb_matrix->program[1] = g_strdup("square");
  rgb_matrix->program[2] = g_strdup("wave-pulse");
  rgb_matrix->program[3] = g_strdup("block-pulse");
  rgb_matrix->program[4] =  NULL;

  rgb_matrix->program_count =  4;

  rgb_matrix->position = 0;
  rgb_matrix->current_period = 0;
  
  rgb_matrix->time_lapse_length = MONOLIGHT_RGB_MATRIX_TIME_LAPSE_DEFAULT_LENGTH;

  /* period and program */
  rgb_matrix->time_lapse_period = (gint *) malloc(rgb_matrix->time_lapse_length * sizeof(gint));

  for(i = 0; i < rgb_matrix->time_lapse_length; i++){
    rgb_matrix->time_lapse_period[i] = 60;
  }

  rgb_matrix->time_lapse_program = (gint *) malloc(rgb_matrix->time_lapse_length * sizeof(gint));
  
  for(i = 0; i < rgb_matrix->time_lapse_length; i++){
    rgb_matrix->time_lapse_program[i] = 0xf;
  }

  /* angle and scale */
  rgb_matrix->time_lapse_start_angle = (gdouble *) malloc(rgb_matrix->time_lapse_length * sizeof(gdouble));
  
  for(i = 0; i < rgb_matrix->time_lapse_length; i++){
    rgb_matrix->time_lapse_start_angle[i] = 0.0;
  }
  
  rgb_matrix->time_lapse_end_angle = (gdouble *) malloc(rgb_matrix->time_lapse_length * sizeof(gdouble));
  
  for(i = 0; i < rgb_matrix->time_lapse_length; i++){
    rgb_matrix->time_lapse_end_angle[i] = 2 * M_PI;
  }

  rgb_matrix->time_lapse_scale = (gdouble *) malloc(rgb_matrix->time_lapse_length * sizeof(gdouble));
  
  for(i = 0; i < rgb_matrix->time_lapse_length; i++){
    rgb_matrix->time_lapse_scale[i] = 1.0;
  }

  /* colors */
  rgb_matrix->time_lapse_red = (guint *) malloc(rgb_matrix->time_lapse_length * sizeof(guint));
  
  for(i = 0; i < rgb_matrix->time_lapse_length; i++){
    if(i < 1.0 * rgb_matrix->time_lapse_length / 3.0){
      rgb_matrix->time_lapse_red[i] = 255;
    }else if(i < 2.0 * rgb_matrix->time_lapse_length / 3.0){
      rgb_matrix->time_lapse_red[i] = 255 - (((gdouble) i * (rgb_matrix->time_lapse_length / 6.0)) * 255.0);
    }else{
      rgb_matrix->time_lapse_red[i] = 0;
    }
  }

  rgb_matrix->time_lapse_green = (guint *) malloc(rgb_matrix->time_lapse_length * sizeof(guint));
  
  for(i = 0; i < rgb_matrix->time_lapse_length; i++){
    if(i < 1.0 * rgb_matrix->time_lapse_length / 3.0){
      rgb_matrix->time_lapse_green[i] = 0;
    }else if(i < 2.0 * rgb_matrix->time_lapse_length / 3.0){
      rgb_matrix->time_lapse_green[i] = 255;
    }else{
      rgb_matrix->time_lapse_green[i] = 255 - (((gdouble) i * (rgb_matrix->time_lapse_length / 9.0)) * 255.0);
    }
  }

  rgb_matrix->time_lapse_blue = (guint *) malloc(rgb_matrix->time_lapse_length * sizeof(guint));
  
  for(i = 0; i < rgb_matrix->time_lapse_length; i++){
    if(i < 1.0 * rgb_matrix->time_lapse_length / 3.0){
      rgb_matrix->time_lapse_blue[i] = 255 - (((gdouble) i * (rgb_matrix->time_lapse_length / 3.0)) * 255.0);
    }else if(i < 2.0 * rgb_matrix->time_lapse_length / 3.0){
      rgb_matrix->time_lapse_blue[i] = 0;
    }else{
      rgb_matrix->time_lapse_blue[i] = 255;
    }
  }

  rgb_matrix->time_lapse_alpha = (guint *) malloc(rgb_matrix->time_lapse_length * sizeof(guint));
  
  for(i = 0; i < rgb_matrix->time_lapse_length; i++){
    rgb_matrix->time_lapse_alpha[i] = 255;
  }

  /* magnitude buffer queue draw */
  rgb_matrix->flags |= MONOLIGHT_RGB_MATRIX_DRAW_MAGNITUDE_BUFFER;

  g_timeout_add(1000 / 30, (GSourceFunc) monolight_rgb_matrix_magnitude_buffer_queue_draw_timeout, (gpointer) rgb_matrix);
}

void
monolight_rgb_matrix_finalize(GObject *gobject)
{
  MonolightRGBMatrix *rgb_matrix;

  guint i;
  
  rgb_matrix = (MonolightRGBMatrix *) gobject;

  for(i = 0; i < rgb_matrix->audio_channels; i++){
    free(rgb_matrix->magnitude_buffer[i]);
  }

  free(rgb_matrix->magnitude_buffer);

  if(rgb_matrix->osc_client != NULL){
    g_object_unref(rgb_matrix->osc_client);
  }

  cairo_surface_destroy(rgb_matrix->cr);
  
  /* call parent */
  G_OBJECT_CLASS(monolight_rgb_matrix_parent_class)->finalize(gobject);
}

void
monolight_rgb_matrix_create(MonolightRGBMatrix *rgb_matrix)
{
  if(!MONOLIGHT_IS_RGB_MATRIX(rgb_matrix)){
    return;
  }
  
  memset(&(rgb_matrix->options), 0, sizeof(rgb_matrix->options));

  rgb_matrix->options.rows = rgb_matrix->rows;
  rgb_matrix->options.cols = rgb_matrix->cols;

  rgb_matrix->options.chain_length = rgb_matrix->chain_length;

  /* This supports all the led commandline options. Try --led-help */
  rgb_matrix->led_matrix = led_matrix_create_from_options(&(rgb_matrix->options), &(rgb_matrix->argc), &(rgb_matrix->argv));

  if(rgb_matrix->led_matrix == NULL){
    g_critical("failed to create LED matrix");
    
    return;
  }

  rgb_matrix->offscreen_canvas = led_matrix_create_offscreen_canvas(rgb_matrix->led_matrix);

  g_message("Size: %dx%d. Hardware gpio mapping: %s",
	    rgb_matrix->width, rgb_matrix->height,
	    rgb_matrix->options.hardware_mapping);
}

void
monolight_rgb_matrix_delete(MonolightRGBMatrix *rgb_matrix)
{
  if(!MONOLIGHT_IS_RGB_MATRIX(rgb_matrix)){
    return;
  }
  
  led_matrix_delete(rgb_matrix->led_matrix);
}

uint8_t
monolight_rgb_matrix_get_brightness(MonolightRGBMatrix *rgb_matrix)
{
  if(!MONOLIGHT_IS_RGB_MATRIX(rgb_matrix)){
    return;
  }

  return(led_matrix_get_brightness(rgb_matrix->led_matrix));
}

void
monolight_rgb_matrix_start(MonolightRGBMatrix *rgb_matrix)
{
  if(!MONOLIGHT_IS_RGB_MATRIX(rgb_matrix)){
    return;
  }

  //TODO:JK: implement me
}

void
monolight_rgb_matrix_stop(MonolightRGBMatrix *rgb_matrix)
{
  if(!MONOLIGHT_IS_RGB_MATRIX(rgb_matrix)){
    return;
  }

  //TODO:JK: implement me
}

void
monolight_rgb_matrix_set_brightness(MonolightRGBMatrix *rgb_matrix,
				    uint8_t brightness)
{
  if(!MONOLIGHT_IS_RGB_MATRIX(rgb_matrix)){
    return;
  }
  
  led_matrix_set_brightness(rgb_matrix->led_matrix,
			    brightness);
}

void
monolight_rgb_matrix_delegate_render(MonolightRGBMatrix *rgb_matrix,
				     cairo_t *cr,
				     gchar *program,
				     guint samplerate,
				     guint buffer_size,
				     gdouble *magnitude_buffer,
				     guint buffer_start, guint buffer_end,
				     guint x0, guint x1,
				     guint width, guint height,
				     gdouble angle, gdouble scale,
				     guint r, guint g, guint b, guint a)
{
  if(!MONOLIGHT_IS_RGB_MATRIX(rgb_matrix)){
    return;
  }
  
  width *= MONOLIGHT_RGB_MATRIX_DEFAULT_SCALE_FACTOR;
  height *= MONOLIGHT_RGB_MATRIX_DEFAULT_SCALE_FACTOR;
  
  if(!g_ascii_strncasecmp(program,
			  "cross",
			  6)){
    monolight_animation_util_render_cross(cr,
					  samplerate,
					  buffer_size,
					  magnitude_buffer,
					  buffer_start, buffer_end,
					  x0, x1,
					  width, height,
					  angle, scale,
					  r, g, b, a);
  }else if(!g_ascii_strncasecmp(program,
				"square",
				7)){
    monolight_animation_util_render_square(cr,
					   samplerate,
					   buffer_size,
					   magnitude_buffer,
					   buffer_start, buffer_end,
					   x0, x1,
					   width, height,
					   angle, scale,
					   r, g, b, a);
  }else if(!g_ascii_strncasecmp(program,
				"wave-pulse",
				11)){
    monolight_animation_util_render_wave_pulse(cr,
					       samplerate,
					       buffer_size,
					       magnitude_buffer,
					       buffer_start, buffer_end,
					       x0, x1,
					       width, height,
					       angle, scale,
					       r, g, b, a);
  }else if(!g_ascii_strncasecmp(program,
				"block-pulse",
				12)){
    monolight_animation_util_render_block_pulse(cr,
						samplerate,
						buffer_size,
						magnitude_buffer,
						buffer_start, buffer_end,
						x0, x1,
						width, height,
						angle, scale,
						r, g, b, a);
  }
}

void
monolight_rgb_matrix_render_magnitude(MonolightRGBMatrix *rgb_matrix,
				      guint audio_channel,
				      guint samplerate,
				      guint buffer_size,
				      gdouble *magnitude_buffer)
{
  cairo_t *cr;

  guint i;

  if(!MONOLIGHT_IS_RGB_MATRIX(rgb_matrix)){
    return;
  }

  cr = rgb_matrix->cr;
  
  if(cr == NULL){
    return;
  }
  
  cairo_push_group(cr);

  /* clear with background color */
  cairo_set_source_rgba(cr,
			0.0,
			0.0,
			0.0,
			0.25);

  cairo_rectangle(cr,
		  0.0, 0.0,
		  MONOLIGHT_RGB_MATRIX_DEFAULT_SCALE_FACTOR * MONOLIGHT_RGB_MATRIX_DEFAULT_WIDTH, MONOLIGHT_RGB_MATRIX_DEFAULT_SCALE_FACTOR * MONOLIGHT_RGB_MATRIX_DEFAULT_HEIGHT);
  
  cairo_fill(cr);

  for(i = 0; i < rgb_matrix->program_count; i++){  
    if(((0x1 << i) & rgb_matrix->time_lapse_program[rgb_matrix->position]) != 0){
      guint buffer_start, buffer_end;
      gdouble angle;
      guint red, green, blue, alpha;

      buffer_start = i * (buffer_size / (2 * rgb_matrix->program_count));
      buffer_end = (i + 1) * (buffer_size / (2 * rgb_matrix->program_count));
      
      angle = rgb_matrix->time_lapse_start_angle[rgb_matrix->position] +
	((rgb_matrix->current_period / rgb_matrix->time_lapse_period[rgb_matrix->position]) *
	 (rgb_matrix->time_lapse_end_angle[rgb_matrix->position] - rgb_matrix->time_lapse_start_angle[rgb_matrix->position]));

      red = rgb_matrix->time_lapse_red[rgb_matrix->position];
      green = rgb_matrix->time_lapse_green[rgb_matrix->position];
      blue = rgb_matrix->time_lapse_blue[rgb_matrix->position];
      alpha = rgb_matrix->time_lapse_alpha[rgb_matrix->position];
      
      monolight_rgb_matrix_delegate_render(rgb_matrix,
					     cr,
					     rgb_matrix->program[i],
					     samplerate,
					     buffer_size,
					     magnitude_buffer,
					     buffer_start, buffer_end,
					     0, 0,
					     MONOLIGHT_RGB_MATRIX_DEFAULT_WIDTH, MONOLIGHT_RGB_MATRIX_DEFAULT_HEIGHT,
					     angle, rgb_matrix->time_lapse_scale[rgb_matrix->position],
					     red, green, blue, alpha);
    }
  }

  /* increment current period and position */
  rgb_matrix->current_period += 1;
  
  if(rgb_matrix->current_period >= rgb_matrix->time_lapse_period[rgb_matrix->position]){
    rgb_matrix->position += 1;
    
    rgb_matrix->current_period = 0;
  }
  
  if(rgb_matrix->position >= rgb_matrix->time_lapse_length){
    rgb_matrix->position = 0;
  }

  cairo_pop_group_to_source(cr);
  cairo_paint(cr);
}

gboolean
monolight_rgb_matrix_magnitude_buffer_queue_draw_timeout(GObject *gobject)
{
  MonolightRGBMatrix *rgb_matrix;

  rgb_matrix = (MonolightRGBMatrix *) gobject;
  
  if((MONOLIGHT_RGB_MATRIX_DRAW_MAGNITUDE_BUFFER & (rgb_matrix->flags)) != 0){
    if((MONOLIGHT_RGB_MATRIX_RUNNING & (rgb_matrix->flags)) != 0){
      int ip4_fd, ip6_fd;

      pthread_mutex_t *osc_client_mutex;

      osc_client_mutex = AGS_OSC_CLIENT_GET_OBJ_MUTEX(rgb_matrix->osc_client);

      /*  */
      pthread_mutex_lock(osc_client_mutex);
    
      ip4_fd = rgb_matrix->osc_client->ip4_fd;
      ip6_fd = rgb_matrix->osc_client->ip6_fd;

      pthread_mutex_unlock(osc_client_mutex);
    
      if(ip4_fd != -1 ||
	 ip6_fd != -1){      
	unsigned char *current_data;
	unsigned char *current_packet;
	gchar *address_pattern;
	gchar *type_tag;
	gchar *path;
      
	guint offset;
	guint n_audio_channels;
	guint data_length;
	guint packet_length;
	guint type_tag_length;
	guint path_length;
	guint audio_channel;
	guint i;
	gboolean success;

	n_audio_channels = 0;
      
	success = FALSE;
      
	while(!success){
	  current_data = ags_osc_client_read_bytes(rgb_matrix->osc_client,
						   &data_length);

	  if(current_data != NULL &&
	     data_length >= 8){
	    current_packet = ags_osc_util_slip_decode(current_data,
						      data_length,
						      &packet_length);

	    offset = 4;
	    ags_osc_buffer_util_get_message(current_packet + offset,
					    &address_pattern, &type_tag);

	    type_tag_length = strlen(type_tag);

	    if(!g_strcmp0(address_pattern,
			  "/meter")){
	      offset += 8;
	      offset += (guint) 4 * ceil(((gdouble) type_tag_length + 1.0) / 4.0);
	    
	      ags_osc_buffer_util_get_string(current_packet + offset,
					     &path, &path_length);

	      /* /AgsSoundProvider/AgsAudio[?] */
	      audio_channel = 0;
	      sscanf(path + 29,
		     "/AgsInput[%u]/AgsAnalyseChannel[0]/AgsPort[\"./magnitude-buffer[0]\"]:value",
		     &audio_channel);
	    
	      offset += (guint) 4 * ceil(((gdouble) path_length + 1.0) / 4.0);
	    
	      for(i = 0; i < rgb_matrix->buffer_size / 2 && offset < packet_length; i++){
		ags_osc_buffer_util_get_double(current_packet + offset,
					       &(rgb_matrix->magnitude_buffer[audio_channel][i]));
      
		offset += sizeof(gdouble);
	      }
	    
	      n_audio_channels++;
	    }

	    if(current_packet != NULL){
	      free(current_packet);

	      current_packet = NULL;
	    }
	  }

	  if(n_audio_channels >= MONOLIGHT_RGB_MATRIX_DEFAULT_AUDIO_CHANNELS){
	    success = TRUE;
	  }
	}
      
	for(i = 0; i < rgb_matrix->audio_channels; i++){
	  monolight_rgb_matrix_render_magnitude(rgb_matrix,
						i,
						rgb_matrix->samplerate,
						rgb_matrix->buffer_size,
						rgb_matrix->magnitude_buffer[i]);
	}
      }
    }
    
    return(TRUE);
  }else{
    return(FALSE);
  }
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
