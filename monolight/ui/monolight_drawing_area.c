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

void monolight_drawing_area_map(GtkWidget *widget);
void monolight_drawing_area_realize(GtkWidget *widget);
void monolight_drawing_area_size_request(GtkWidget *widget,
					 GtkRequisition   *requisition);
void monolight_drawing_area_size_allocate(GtkWidget *widget,
					  GtkAllocation *allocation);
gboolean monolight_drawing_area_expose(GtkWidget *widget,
				       GdkEventExpose *event);
void monolight_drawing_area_show(GtkWidget *widget);

void monolight_drawing_area_draw(MonolightDrawingArea *drawing_area);

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
  GtkWidgetClass *widget;

  monolight_drawing_area_parent_class = g_type_class_peek_parent(drawing_area);

  /* GObjectClass */
  gobject = (GObjectClass *) drawing_area;

  gobject->finalize = monolight_drawing_area_finalize;

  /* GtkWidgetClass */
  widget = (GtkWidgetClass *) drawing_area;

  //  widget->map = monolight_drawing_area_map;
  widget->realize = monolight_drawing_area_realize;
  widget->expose_event = monolight_drawing_area_expose;
  widget->size_request = monolight_drawing_area_size_request;
  widget->size_allocate = monolight_drawing_area_size_allocate;
  widget->show = monolight_drawing_area_show;
}

void
monolight_drawing_area_init(MonolightDrawingArea *drawing_area)
{
  g_object_set(G_OBJECT(drawing_area),
	       "app-paintable", TRUE,
	       NULL);

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

void
monolight_drawing_area_map(GtkWidget *widget)
{
  if(gtk_widget_get_realized (widget) && !gtk_widget_get_mapped(widget)){
    GTK_WIDGET_CLASS(monolight_drawing_area_parent_class)->map(widget);
    
    gdk_window_show(widget->window);
    monolight_drawing_area_draw((MonolightDrawingArea *) widget);
  }
}

void
monolight_drawing_area_realize(GtkWidget *widget)
{
  MonolightDrawingArea *drawing_area;

  GdkWindowAttr attributes;

  gint attributes_mask;

  g_return_if_fail(widget != NULL);
  g_return_if_fail(MONOLIGHT_IS_DRAWING_AREA(widget));

  drawing_area = MONOLIGHT_DRAWING_AREA(widget);

  gtk_widget_set_realized(widget, TRUE);

  /*  */
  attributes.window_type = GDK_WINDOW_CHILD;
  
  attributes.x = widget->allocation.x;
  attributes.y = widget->allocation.y;
  attributes.width = widget->allocation.width;
  attributes.height = widget->allocation.height;

  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;

  attributes.wclass = GDK_INPUT_OUTPUT;
  attributes.visual = gtk_widget_get_visual (widget);
  attributes.colormap = gtk_widget_get_colormap (widget);
  attributes.event_mask = gtk_widget_get_events (widget);
  attributes.event_mask |= (GDK_EXPOSURE_MASK);

  widget->window = gdk_window_new(gtk_widget_get_parent_window (widget),
				  &attributes, attributes_mask);
  gdk_window_set_user_data(widget->window, drawing_area);

  widget->style = gtk_style_attach(widget->style,
				   widget->window);
  gtk_style_set_background(widget->style,
			   widget->window,
			   GTK_STATE_NORMAL);

  gtk_widget_queue_resize(widget);
}

void
monolight_drawing_area_size_request(GtkWidget *widget,
				    GtkRequisition *requisition)
{
  MonolightDrawingArea *drawing_area;

  drawing_area = MONOLIGHT_DRAWING_AREA(widget);

  requisition->width = MONOLIGHT_DRAWING_AREA_DEFAULT_SCALE_FACTOR * MONOLIGHT_DRAWING_AREA_DEFAULT_WIDTH;
  requisition->height = MONOLIGHT_DRAWING_AREA_DEFAULT_SCALE_FACTOR * MONOLIGHT_DRAWING_AREA_DEFAULT_HEIGHT;
}

void
monolight_drawing_area_size_allocate(GtkWidget *widget,
				     GtkAllocation *allocation)
{
  MonolightDrawingArea *drawing_area;

  drawing_area = MONOLIGHT_DRAWING_AREA(widget);
  
  widget->allocation = *allocation;

  widget->allocation.width = MONOLIGHT_DRAWING_AREA_DEFAULT_SCALE_FACTOR * MONOLIGHT_DRAWING_AREA_DEFAULT_WIDTH;
  widget->allocation.height = MONOLIGHT_DRAWING_AREA_DEFAULT_SCALE_FACTOR * MONOLIGHT_DRAWING_AREA_DEFAULT_HEIGHT;
}

gboolean
monolight_drawing_area_expose(GtkWidget *widget,
			      GdkEventExpose *event)
{
  monolight_drawing_area_draw(MONOLIGHT_DRAWING_AREA(widget));

  return(FALSE);
}

void
monolight_drawing_area_show(GtkWidget *widget)
{
  GTK_WIDGET_CLASS(monolight_drawing_area_parent_class)->show(widget);
}

void
monolight_drawing_area_draw(MonolightDrawingArea *drawing_area)
{
  //TODO:JK: implement me
}

void
monolight_drawing_area_render_magnitude(MonolightDrawingArea *drawing_area,
					guint audio_channel,
					gdouble *magnitude_buffer,
					guint buffer_size)
{
  //TODO:JK: implement me
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
