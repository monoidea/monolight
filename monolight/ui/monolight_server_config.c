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

#include <monolight/ui/monolight_server_config.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monolight/i18n.h>

void monolight_server_config_class_init(MonolightServerConfigClass *server_config);
void monolight_server_config_init(MonolightServerConfig *server_config);
void monolight_server_config_finalize(GObject *gobject);

/**
 * SECTION:monolight_server_config
 * @short_description: The server_config object.
 * @title: MonolightServerConfig
 * @section_id:
 * @include: monolight/ui/monolight_server_config.h
 *
 * #MonolightServerConfig is a composite widget to configure
 * OSC client.
 */

static gpointer monolight_server_config_parent_class = NULL;


GType
monolight_server_config_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monolight_type_server_config = 0;

    static const GTypeInfo monolight_server_config_info = {
      sizeof (MonolightServerConfigClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monolight_server_config_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonolightServerConfig),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monolight_server_config_init,
    };

    monolight_type_server_config = g_type_register_static(GTK_TYPE_VBOX,
							  "MonolightServerConfig", &monolight_server_config_info,
							  0);

    g_once_init_leave(&g_define_type_id__volatile, monolight_type_server_config);
  }

  return g_define_type_id__volatile;
}

void
monolight_server_config_class_init(MonolightServerConfigClass *server_config)
{
  GObjectClass *gobject;

  monolight_server_config_parent_class = g_type_class_peek_parent(server_config);

  /* GObjectClass */
  gobject = (GObjectClass *) server_config;

  gobject->finalize = monolight_server_config_finalize;
}

void
monolight_server_config_init(MonolightServerConfig *server_config)
{
  GtkTable *table;
  GtkLabel *label;

  gchar *str;
  
  table = gtk_table_new(4,
			2,
			TRUE);
  gtk_box_pack_start((GtkBox *) server_config,
		     (GtkWidget *) table,
		     FALSE, FALSE,
		     0);
  
  /* IPv4 */
  label = gtk_label_new(i18n("OSC server IPv4"));
  gtk_table_attach(table,
		   label,
		   0, 1,
		   0, 1,
		   GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND,
		   0, 0);
  
  server_config->osc_server_ip4 = gtk_entry_new();
  gtk_entry_set_text(server_config->osc_server_ip4,
		     AGS_OSC_CLIENT_DEFAULT_INET4_ADDRESS);
  gtk_table_attach(table,
		   server_config->osc_server_ip4,
		   1, 2,
		   0, 1,
		   GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND,
		   0, 0);
  
  /* IPv6 */
  label = gtk_label_new(i18n("OSC server IPv6"));
  gtk_table_attach(table,
		   label,
		   0, 1,
		   1, 2,
		   GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND,
		   0, 0);
  
  server_config->osc_server_ip6 = gtk_entry_new();
  gtk_entry_set_text(server_config->osc_server_ip6,
		     AGS_OSC_CLIENT_DEFAULT_INET6_ADDRESS);
  gtk_table_attach(table,
		   server_config->osc_server_ip6,
		   1, 2,
		   1, 2,
		   GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND,
		   0, 0);
  
  /* domain */
  label = gtk_label_new(i18n("OSC server domain"));
  gtk_table_attach(table,
		   label,
		   0, 1,
		   2, 3,
		   GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND,
		   0, 0);
  
  server_config->osc_server_domain = gtk_entry_new();
  gtk_entry_set_text(server_config->osc_server_domain,
		     AGS_OSC_CLIENT_DEFAULT_DOMAIN);
  gtk_table_attach(table,
		   server_config->osc_server_domain,
		   1, 2,
		   2, 3,
		   GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND,
		   0, 0);
  
  /* port */
  label = gtk_label_new(i18n("OSC server port"));
  gtk_table_attach(table,
		   label,
		   0, 1,
		   3, 4,
		   GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND,
		   0, 0);

  str = g_strdup_printf("%d", AGS_OSC_CLIENT_DEFAULT_SERVER_PORT);
  
  server_config->osc_server_port = gtk_entry_new();
  gtk_entry_set_text(server_config->osc_server_port,
		     str);
  gtk_table_attach(table,
		   server_config->osc_server_port,
		   1, 2,
		   3, 4,
		   GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND,
		   0, 0);

  g_free(str);
}

void
monolight_server_config_finalize(GObject *gobject)
{
  MonolightServerConfig *server_config;

  server_config = (MonolightServerConfig *) gobject;

  /* call parent */
  G_OBJECT_CLASS(monolight_server_config_parent_class)->finalize(gobject);
}

/**
 * monolight_server_config_new:
 *
 * Creates an #MonolightServerConfig
 *
 * Returns: a new #MonolightServerConfig
 *
 * Since: 1.0.0
 */
MonolightServerConfig*
monolight_server_config_new()
{
  MonolightServerConfig *server_config;

  server_config = (MonolightServerConfig *) g_object_new(MONOLIGHT_TYPE_SERVER_CONFIG,
							 NULL);
  
  return(server_config);
}
