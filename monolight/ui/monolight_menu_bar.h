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

#ifndef __MONOLIGHT_MENU_BAR_H__
#define __MONOLIGHT_MENU_BAR_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#define MONOLIGHT_TYPE_MENU_BAR                (monolight_menu_bar_get_type())
#define MONOLIGHT_MENU_BAR(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOLIGHT_TYPE_MENU_BAR, MonolightMenuBar))
#define MONOLIGHT_MENU_BAR_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOLIGHT_TYPE_MENU_BAR, MonolightMenuBarClass))
#define MONOLIGHT_IS_MENU_BAR(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOLIGHT_TYPE_MENU_BAR))
#define MONOLIGHT_IS_MENU_BAR_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOLIGHT_TYPE_MENU_BAR))
#define MONOLIGHT_MENU_BAR_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOLIGHT_TYPE_MENU_BAR, MonolightMenuBarClass))

typedef struct _MonolightMenuBar MonolightMenuBar;
typedef struct _MonolightMenuBarClass MonolightMenuBarClass;

struct _MonolightMenuBar
{
  GtkMenuBar menu_bar;

  GtkMenu *file;
};

struct _MonolightMenuBarClass
{
  GtkMenuBarClass menu_bar;
};

GType monolight_menu_bar_get_type(void);

MonolightMenuBar* monolight_menu_bar_new();

#endif /*__MONOLIGHT_MENU_BAR_H__*/
