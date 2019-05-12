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

  window->osc_client = ags_osc_client_new();
  ags_osc_client_set_flags(window->osc_client,
                           (AGS_OSC_CLIENT_INET4 |
                            AGS_OSC_CLIENT_TCP));
  
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
}

void
monolight_window_finalize(GObject *gobject)
{
  MonolightWindow *window;

  window = (MonolightWindow *) gobject;

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
