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

#include <monolight/ui/monolight_drawing_area.h>

#include <monolight/i18n.h>

void monolight_drawing_area_class_init(MonolightDrawingAreaClass *drawing_area);
void monolight_drawing_area_init(MonolightDrawingArea *drawing_area);
void monolight_drawing_area_finalize(GObject *gobject);

/**
 * SECTION:monolight_drawing_area
 * @short_description: The drawing_area object.
 * @title: MonolightDrawingArea
 * @section_id:
 * @include: monolight/ui/monolight_drawing_area.h
 *
 * #MonolightDrawingArea is a drawing widget to render FFTW3
 * data.
 */

static gpointer monolight_drawing_area_parent_class = NULL;


GType
monolight_drawing_area_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monolight_type_drawing_area = 0;

    static const GTypeInfo monolight_drawing_area_info = {
      sizeof (MonolightDrawingAreaClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monolight_drawing_area_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonolightDrawingArea),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monolight_drawing_area_init,
    };

    monolight_type_drawing_area = g_type_register_static(GTK_TYPE_WIDGET,
							 "MonolightDrawingArea", &monolight_drawing_area_info,
							 0);

    g_once_init_leave(&g_define_type_id__volatile, monolight_type_drawing_area);
  }

  return g_define_type_id__volatile;
}

void
monolight_drawing_area_class_init(MonolightDrawingAreaClass *drawing_area)
{
  GObjectClass *gobject;

  monolight_drawing_area_parent_class = g_type_class_peek_parent(drawing_area);

  /* GObjectClass */
  gobject = (GObjectClass *) drawing_area;

  gobject->finalize = monolight_drawing_area_finalize;
}

void
monolight_drawing_area_init(MonolightDrawingArea *drawing_area)
{
  //TODO:JK: implement me
}

void
monolight_drawing_area_finalize(GObject *gobject)
{
  MonolightDrawingArea *drawing_area;

  drawing_area = (MonolightDrawingArea *) gobject;

  /* call parent */
  G_OBJECT_CLASS(monolight_drawing_area_parent_class)->finalize(gobject);
}

/**
 * monolight_drawing_area_new:
 *
 * Creates an #MonolightDrawingArea
 *
 * Returns: a new #MonolightDrawingArea
 *
 * Since: 1.0.0
 */
MonolightDrawingArea*
monolight_drawing_area_new()
{
  MonolightDrawingArea *drawing_area;

  drawing_area = (MonolightDrawingArea *) g_object_new(MONOLIGHT_TYPE_DRAWING_AREA,
						       NULL);
  
  return(drawing_area);
}
