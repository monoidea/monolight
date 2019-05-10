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

#ifndef __MONOLIGHT_CONFIG_DIALOG_H__
#define __MONOLIGHT_CONFIG_DIALOG_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <monolight/ui/monolight_server_config.h>

#define MONOLIGHT_TYPE_CONFIG_DIALOG                (monolight_config_dialog_get_type())
#define MONOLIGHT_CONFIG_DIALOG(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOLIGHT_TYPE_CONFIG_DIALOG, MonolightConfigDialog))
#define MONOLIGHT_CONFIG_DIALOG_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOLIGHT_TYPE_CONFIG_DIALOG, MonolightConfigDialogClass))
#define MONOLIGHT_IS_CONFIG_DIALOG(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOLIGHT_TYPE_CONFIG_DIALOG))
#define MONOLIGHT_IS_CONFIG_DIALOG_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOLIGHT_TYPE_CONFIG_DIALOG))
#define MONOLIGHT_CONFIG_DIALOG_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOLIGHT_TYPE_CONFIG_DIALOG, MonolightConfigDialogClass))

typedef struct _MonolightConfigDialog MonolightConfigDialog;
typedef struct _MonolightConfigDialogClass MonolightConfigDialogClass;

struct _MonolightConfigDialog
{
  GtkWindow window;

  GtkWidget *main_window;

  GtkNotebook *notebook;

  MonolightServerConfig *server_config;
};

struct _MonolightConfigDialogClass
{
  GtkWindowClass window;
};

GType monolight_config_dialog_get_type(void);

MonolightConfigDialog* monolight_config_dialog_new();

#endif /*__MONOLIGHT_CONFIG_DIALOG_H__*/
