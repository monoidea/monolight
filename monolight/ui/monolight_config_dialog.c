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

#include <monolight/ui/monolight_config_dialog.h>

#include <monolight/i18n.h>

void monolight_config_dialog_class_init(MonolightConfigDialogClass *config_dialog);
void monolight_config_dialog_init(MonolightConfigDialog *config_dialog);
void monolight_config_dialog_finalize(GObject *gobject);

gboolean monolight_config_dialog_delete_event_callback(MonolightConfigDialog *config_dialog,
						       GdkEvent *event,
						       gpointer user_data);

/**
 * SECTION:monolight_config_dialog
 * @short_description: The config_dialog object.
 * @title: MonolightConfigDialog
 * @section_id:
 * @include: monolight/ui/monolight_config_dialog.h
 *
 * #MonolightConfigDialog is a composite toplevel widget.
 */

static gpointer monolight_config_dialog_parent_class = NULL;


GType
monolight_config_dialog_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monolight_type_config_dialog = 0;

    static const GTypeInfo monolight_config_dialog_info = {
      sizeof (MonolightConfigDialogClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monolight_config_dialog_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonolightConfigDialog),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monolight_config_dialog_init,
    };

    monolight_type_config_dialog = g_type_register_static(GTK_TYPE_WINDOW,
							  "MonolightConfigDialog", &monolight_config_dialog_info,
							  0);

    g_once_init_leave(&g_define_type_id__volatile, monolight_type_config_dialog);
  }

  return g_define_type_id__volatile;
}

void
monolight_config_dialog_class_init(MonolightConfigDialogClass *config_dialog)
{
  GObjectClass *gobject;

  monolight_config_dialog_parent_class = g_type_class_peek_parent(config_dialog);

  /* GObjectClass */
  gobject = (GObjectClass *) config_dialog;

  gobject->finalize = monolight_config_dialog_finalize;
}

void
monolight_config_dialog_init(MonolightConfigDialog *config_dialog)
{
  config_dialog->main_window = NULL;

  config_dialog->notebook = gtk_notebook_new();
  gtk_container_add((GtkContainer *) config_dialog,
		    (GtkWidget *) config_dialog->notebook);

  config_dialog->server_config = monolight_server_config_new();
  gtk_notebook_append_page(config_dialog->notebook,
			   config_dialog->server_config,
			   gtk_label_new(i18n("OSC server")));

  g_signal_connect(config_dialog, "delete-event",
		   G_CALLBACK(monolight_config_dialog_delete_event_callback), NULL);
}

void
monolight_config_dialog_finalize(GObject *gobject)
{
  MonolightConfigDialog *config_dialog;

  config_dialog = (MonolightConfigDialog *) gobject;

  /* call parent */
  G_OBJECT_CLASS(monolight_config_dialog_parent_class)->finalize(gobject);
}

gboolean
monolight_config_dialog_delete_event_callback(MonolightConfigDialog *config_dialog,
					      GdkEvent *event,
					      gpointer user_data)
{
  gtk_widget_hide(config_dialog);

  return(TRUE);
}

/**
 * monolight_config_dialog_new:
 *
 * Creates an #MonolightConfigDialog
 *
 * Returns: a new #MonolightConfigDialog
 *
 * Since: 1.0.0
 */
MonolightConfigDialog*
monolight_config_dialog_new()
{
  MonolightConfigDialog *config_dialog;

  config_dialog = (MonolightConfigDialog *) g_object_new(MONOLIGHT_TYPE_CONFIG_DIALOG,
							 NULL);
  
  return(config_dialog);
}
