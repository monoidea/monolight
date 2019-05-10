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

#include <monolight/ui/monolight_menu_bar.h>

#include <monolight/ui/monolight_window.h>

#include <monolight/i18n.h>

void monolight_menu_bar_class_init(MonolightMenuBarClass *menu_bar);
void monolight_menu_bar_init(MonolightMenuBar *menu_bar);
void monolight_menu_bar_finalize(GObject *gobject);

void monolight_menu_bar_preferences_callback(GtkWidget *widget, MonolightMenuBar *menu_bar);
void monolight_menu_bar_connect_callback(GtkWidget *widget, MonolightMenuBar *menu_bar);
void monolight_menu_bar_disconnect_callback(GtkWidget *widget, MonolightMenuBar *menu_bar);

/**
 * SECTION:monolight_menu_bar
 * @short_description: The menu_bar object.
 * @title: MonolightMenuBar
 * @section_id:
 * @include: monolight/ui/monolight_menu_bar.h
 *
 * #MonolightMenuBar is a composite toplevel widget.
 */

static gpointer monolight_menu_bar_parent_class = NULL;


GType
monolight_menu_bar_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monolight_type_menu_bar = 0;

    static const GTypeInfo monolight_menu_bar_info = {
      sizeof (MonolightMenuBarClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monolight_menu_bar_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonolightMenuBar),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monolight_menu_bar_init,
    };

    monolight_type_menu_bar = g_type_register_static(GTK_TYPE_MENU_BAR,
						     "MonolightMenuBar", &monolight_menu_bar_info,
						     0);

    g_once_init_leave(&g_define_type_id__volatile, monolight_type_menu_bar);
  }

  return g_define_type_id__volatile;
}

void
monolight_menu_bar_class_init(MonolightMenuBarClass *menu_bar)
{
  GObjectClass *gobject;

  monolight_menu_bar_parent_class = g_type_class_peek_parent(menu_bar);

  /* GObjectClass */
  gobject = (GObjectClass *) menu_bar;

  gobject->finalize = monolight_menu_bar_finalize;
}

void
monolight_menu_bar_init(MonolightMenuBar *menu_bar)
{
  GtkMenuItem *menu_item;

  menu_item = gtk_image_menu_item_new_from_stock(GTK_STOCK_FILE,
						 NULL);
  gtk_menu_shell_append(menu_bar,
			menu_item);
  
  menu_bar->file = gtk_menu_new();
  gtk_menu_item_set_submenu(menu_item, (GtkWidget*) menu_bar->file);

  /* preferences */
  menu_item = gtk_image_menu_item_new_from_stock(GTK_STOCK_PREFERENCES,
						 NULL);
  gtk_menu_shell_append(menu_bar->file,
			menu_item);

  g_signal_connect(menu_item, "activate",
		   G_CALLBACK(monolight_menu_bar_preferences_callback), menu_bar);
  
  /* connect */
  menu_item = gtk_image_menu_item_new_from_stock(GTK_STOCK_CONNECT,
						 NULL);
  gtk_menu_shell_append(menu_bar->file,
			menu_item);

  g_signal_connect(menu_item, "activate",
		   G_CALLBACK(monolight_menu_bar_connect_callback), menu_bar);
  
  /* disconnect */
  menu_item = gtk_image_menu_item_new_from_stock(GTK_STOCK_DISCONNECT,
						 NULL);
  gtk_menu_shell_append(menu_bar->file,
			menu_item);

  g_signal_connect(menu_item, "activate",
		   G_CALLBACK(monolight_menu_bar_disconnect_callback), menu_bar);
}

void
monolight_menu_bar_finalize(GObject *gobject)
{
  MonolightMenuBar *menu_bar;

  menu_bar = (MonolightMenuBar *) gobject;

  /* call parent */
  G_OBJECT_CLASS(monolight_menu_bar_parent_class)->finalize(gobject);
}

void
monolight_menu_bar_preferences_callback(GtkWidget *widget, MonolightMenuBar *menu_bar)
{
  MonolightWindow *window;

  window = gtk_widget_get_toplevel(menu_bar);

  gtk_widget_show_all(window->config_dialog);
}

void
monolight_menu_bar_connect_callback(GtkWidget *widget, MonolightMenuBar *menu_bar)
{
  MonolightWindow *window;

  window = gtk_widget_get_toplevel(menu_bar);

  //TODO:JK: implement me
}

void
monolight_menu_bar_disconnect_callback(GtkWidget *widget, MonolightMenuBar *menu_bar)
{
  MonolightWindow *window;

  window = gtk_widget_get_toplevel(menu_bar);

  //TODO:JK: implement me
}

/**
 * monolight_menu_bar_new:
 *
 * Creates an #MonolightMenuBar
 *
 * Returns: a new #MonolightMenuBar
 *
 * Since: 1.0.0
 */
MonolightMenuBar*
monolight_menu_bar_new()
{
  MonolightMenuBar *menu_bar;

  menu_bar = (MonolightMenuBar *) g_object_new(MONOLIGHT_TYPE_MENU_BAR,
					       NULL);
  
  return(menu_bar);
}
