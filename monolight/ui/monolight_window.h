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

#ifndef __MONOLIGHT_WINDOW_H__
#define __MONOLIGHT_WINDOW_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monolight/ui/monolight_menu_bar.h>
#include <monolight/ui/monolight_drawing_area.h>
#include <monolight/ui/monolight_config_dialog.h>

#define MONOLIGHT_TYPE_WINDOW                (monolight_window_get_type())
#define MONOLIGHT_WINDOW(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOLIGHT_TYPE_WINDOW, MonolightWindow))
#define MONOLIGHT_WINDOW_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOLIGHT_TYPE_WINDOW, MonolightWindowClass))
#define MONOLIGHT_IS_WINDOW(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOLIGHT_TYPE_WINDOW))
#define MONOLIGHT_IS_WINDOW_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOLIGHT_TYPE_WINDOW))
#define MONOLIGHT_WINDOW_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOLIGHT_TYPE_WINDOW, MonolightWindowClass))

typedef struct _MonolightWindow MonolightWindow;
typedef struct _MonolightWindowClass MonolightWindowClass;

struct _MonolightWindow
{
  GtkWindow window;

  AgsOscClient *osc_client;
  
  MonolightMenuBar *menu_bar;

  MonolightDrawingArea *drawing_area;

  MonolightConfigDialog *config_dialog;
};

struct _MonolightWindowClass
{
  GtkWindowClass window;
};

GType monolight_window_get_type(void);

MonolightWindow* monolight_window_new();

#endif /*__MONOLIGHT_WINDOW_H__*/
