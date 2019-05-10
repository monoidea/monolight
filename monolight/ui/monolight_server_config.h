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

#ifndef __MONOLIGHT_SERVER_CONFIG_H__
#define __MONOLIGHT_SERVER_CONFIG_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#define MONOLIGHT_TYPE_SERVER_CONFIG                (monolight_server_config_get_type())
#define MONOLIGHT_SERVER_CONFIG(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOLIGHT_TYPE_SERVER_CONFIG, MonolightServerConfig))
#define MONOLIGHT_SERVER_CONFIG_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOLIGHT_TYPE_SERVER_CONFIG, MonolightServerConfigClass))
#define MONOLIGHT_IS_SERVER_CONFIG(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOLIGHT_TYPE_SERVER_CONFIG))
#define MONOLIGHT_IS_SERVER_CONFIG_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOLIGHT_TYPE_SERVER_CONFIG))
#define MONOLIGHT_SERVER_CONFIG_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOLIGHT_TYPE_SERVER_CONFIG, MonolightServerConfigClass))

typedef struct _MonolightServerConfig MonolightServerConfig;
typedef struct _MonolightServerConfigClass MonolightServerConfigClass;

struct _MonolightServerConfig
{
  GtkVBox vbox;

  GtkEntry *osc_server_ip4;
  GtkEntry *osc_server_ip6;

  GtkEntry *osc_server_domain;
  GtkEntry *osc_server_port;
};

struct _MonolightServerConfigClass
{
  GtkVBoxClass vbox;
};

GType monolight_server_config_get_type(void);

MonolightServerConfig* monolight_server_config_new();

#endif /*__MONOLIGHT_SERVER_CONFIG_H__*/
