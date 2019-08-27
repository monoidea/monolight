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

#include <monolight/ui/monolight_window.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monolight/i18n.h>

void monolight_window_class_init(MonolightWindowClass *window);
void monolight_window_init(MonolightWindow *window);
void monolight_window_finalize(GObject *gobject);

gboolean monolight_window_delete_event_callback(MonolightWindow *window,
						GdkEvent *event,
						gpointer user_data);

/**
 * SECTION:monolight_window
 * @short_description: The window object.
 * @title: MonolightWindow
 * @section_id:
 * @include: monolight/ui/monolight_window.h
 *
 * #MonolightWindow is a composite toplevel widget.
 */

static gpointer monolight_window_parent_class = NULL;

GType
monolight_window_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monolight_type_window = 0;

    static const GTypeInfo monolight_window_info = {
      sizeof (MonolightWindowClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monolight_window_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonolightWindow),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monolight_window_init,
    };

    monolight_type_window = g_type_register_static(GTK_TYPE_WINDOW,
						  "MonolightWindow", &monolight_window_info,
						  0);

    g_once_init_leave(&g_define_type_id__volatile, monolight_type_window);
  }

  return g_define_type_id__volatile;
}

void
monolight_window_class_init(MonolightWindowClass *window)
{
  GObjectClass *gobject;

  monolight_window_parent_class = g_type_class_peek_parent(window);

  /* GObjectClass */
  gobject = (GObjectClass *) window;

  gobject->finalize = monolight_window_finalize;
}

void
monolight_window_init(MonolightWindow *window)
{
  GtkVBox *vbox;

  guint i;
  
  window->flags = 0;

  window->osc_client = ags_osc_client_new();
  ags_osc_client_set_flags(window->osc_client,
                           (AGS_OSC_CLIENT_INET4 |
                            AGS_OSC_CLIENT_TCP));

  window->audio_channels = MONOLIGHT_WINDOW_DEFAULT_AUDIO_CHANNELS;

  window->samplerate = 44100;
  window->buffer_size = AGS_SOUNDCARD_DEFAULT_BUFFER_SIZE;

  window->magnitude_buffer = (gdouble **) malloc(window->audio_channels * sizeof(gdouble *));
  
  for(i = 0; i < window->audio_channels; i++){
    window->magnitude_buffer[i] = (gdouble *) malloc(window->buffer_size * sizeof(gdouble));

    ags_audio_buffer_util_clear_double(window->magnitude_buffer[i], 1,
				       window->buffer_size);
  }
  
  /* UI */
  vbox = gtk_vbox_new(FALSE,
		      0);
  gtk_container_add((GtkContainer *) window,
		    vbox);

  /* menu bar */
  window->menu_bar = monolight_menu_bar_new();
  gtk_box_pack_start((GtkBox *) vbox,
		     window->menu_bar,
		     FALSE, FALSE,
		     0);

  /* drawing area */
  window->drawing_area = monolight_drawing_area_new();
  gtk_box_pack_start((GtkBox *) vbox,
		     window->drawing_area,
		     FALSE, FALSE,
		     0);

  /* config dialog */
  window->config_dialog = monolight_config_dialog_new();
  window->config_dialog->main_window = window;

  g_signal_connect(window, "delete-event",
		   G_CALLBACK(monolight_window_delete_event_callback), NULL);

  /* magnitude buffer queue draw */
  window->flags |= MONOLIGHT_WINDOW_DRAW_MAGNITUDE_BUFFER;

  g_timeout_add(1000 / 30, (GSourceFunc) monolight_window_magnitude_buffer_queue_draw_timeout, (gpointer) window);
}

void
monolight_window_finalize(GObject *gobject)
{
  MonolightWindow *window;

  window = (MonolightWindow *) gobject;

  if(window->osc_client != NULL){
    g_object_unref(window->osc_client);
  }
  
  /* call parent */
  G_OBJECT_CLASS(monolight_window_parent_class)->finalize(gobject);
}

gboolean
monolight_window_delete_event_callback(MonolightWindow *window,
				       GdkEvent *event,
				       gpointer user_data)
{
  gtk_main_quit();

  return(FALSE);
}

gboolean
monolight_window_magnitude_buffer_queue_draw_timeout(GtkWidget *widget)
{
  MonolightWindow *window;

  window = (MonolightWindow *) widget;
  
  if((MONOLIGHT_WINDOW_DRAW_MAGNITUDE_BUFFER & (window->flags)) != 0){
    int ip4_fd, ip6_fd;

    pthread_mutex_t *osc_client_mutex;
    
    pthread_mutex_lock(ags_osc_client_get_class_mutex());
    
    osc_client_mutex = window->osc_client->obj_mutex;
    
    pthread_mutex_unlock(ags_osc_client_get_class_mutex());

    /*  */
    pthread_mutex_lock(osc_client_mutex);
    
    ip4_fd = window->osc_client->ip4_fd;
    ip6_fd = window->osc_client->ip6_fd;

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
	current_data = ags_osc_client_read_bytes(window->osc_client,
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
	    
	    for(i = 0; i < window->buffer_size / 2 && offset < packet_length; i++){
	      ags_osc_buffer_util_get_double(current_packet + offset,
					     &(window->magnitude_buffer[audio_channel][i]));
      
	      offset += sizeof(gdouble);
	    }
	    
	    n_audio_channels++;
	  }

	  if(current_packet != NULL){
	    free(current_packet);

	    current_packet = NULL;
	  }
	}

	if(n_audio_channels >= MONOLIGHT_WINDOW_DEFAULT_AUDIO_CHANNELS){
	  success = TRUE;
	}
      }
      
      for(i = 0; i < window->audio_channels; i++){
	monolight_drawing_area_render_magnitude(window->drawing_area,
						i,
						window->samplerate,
						window->buffer_size,
						window->magnitude_buffer[i]);
      }
    }
    
    return(TRUE);
  }else{
    return(FALSE);
  }
}

/**
 * monolight_window_new:
 *
 * Creates an #MonolightWindow
 *
 * Returns: a new #MonolightWindow
 *
 * Since: 1.0.0
 */
MonolightWindow*
monolight_window_new()
{
  MonolightWindow *window;

  window = (MonolightWindow *) g_object_new(MONOLIGHT_TYPE_WINDOW,
					    NULL);
  
  return(window);
}
