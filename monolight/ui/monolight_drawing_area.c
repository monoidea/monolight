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

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>
#include <math.h>

#include <monolight/ui/monolight_animation_util.h>

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

void monolight_drawing_area_delegate_render(MonolightDrawingArea *drawing_area,
					    cairo_t *cr,
					    gchar *program,
					    guint samplerate,
					    guint buffer_size,
					    gdouble *magnitude_buffer,
					    guint buffer_start, guint buffer_end,
					    guint x0, guint x1,
					    guint width, guint height,
					    gdouble angle, gdouble scale,
					    guint r, guint g, guint b, guint a);

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
  guint i;
  
  g_object_set(G_OBJECT(drawing_area),
	       "app-paintable", TRUE,
	       "double-buffered", TRUE,
	       NULL);

  drawing_area->audio_channels = MONOLIGHT_DRAWING_AREA_DEFAULT_AUDIO_CHANNELS;
  
  drawing_area->samplerate = AGS_SOUNDCARD_DEFAULT_SAMPLERATE;
  drawing_area->buffer_size = AGS_SOUNDCARD_DEFAULT_BUFFER_SIZE;

  drawing_area->program = (gchar **) malloc(5 * sizeof(gchar *));

  drawing_area->program[0] = g_strdup("cross");
  drawing_area->program[1] = g_strdup("square");
  drawing_area->program[2] = g_strdup("wave-pulse");
  drawing_area->program[3] = g_strdup("block-pulse");
  drawing_area->program[4] =  NULL;

  drawing_area->program_count =  4;

  drawing_area->position = 0;
  drawing_area->current_period = 0;
  
  drawing_area->time_lapse_length = MONOLIGHT_DRAWING_AREA_TIME_LAPSE_DEFAULT_LENGTH;

  /* period and program */
  drawing_area->time_lapse_period = (gint *) malloc(drawing_area->time_lapse_length * sizeof(gint));

  for(i = 0; i < drawing_area->time_lapse_length; i++){
    drawing_area->time_lapse_period[i] = 60;
  }

  drawing_area->time_lapse_program = (gint *) malloc(drawing_area->time_lapse_length * sizeof(gint));
  
  for(i = 0; i < drawing_area->time_lapse_length; i++){
    drawing_area->time_lapse_program[i] = 0xf;
  }

  /* angle and scale */
  drawing_area->time_lapse_start_angle = (gdouble *) malloc(drawing_area->time_lapse_length * sizeof(gdouble));
  
  for(i = 0; i < drawing_area->time_lapse_length; i++){
    drawing_area->time_lapse_start_angle[i] = 0.0;
  }
  
  drawing_area->time_lapse_end_angle = (gdouble *) malloc(drawing_area->time_lapse_length * sizeof(gdouble));
  
  for(i = 0; i < drawing_area->time_lapse_length; i++){
    drawing_area->time_lapse_end_angle[i] = 2 * M_PI;
  }

  drawing_area->time_lapse_scale = (gdouble *) malloc(drawing_area->time_lapse_length * sizeof(gdouble));
  
  for(i = 0; i < drawing_area->time_lapse_length; i++){
    drawing_area->time_lapse_scale[i] = 1.0;
  }

  /* colors */
  drawing_area->time_lapse_red = (guint *) malloc(drawing_area->time_lapse_length * sizeof(guint));
  
  for(i = 0; i < drawing_area->time_lapse_length; i++){
    if(i < 1.0 * drawing_area->time_lapse_length / 3.0){
      drawing_area->time_lapse_red[i] = 255;
    }else if(i < 2.0 * drawing_area->time_lapse_length / 3.0){
      drawing_area->time_lapse_red[i] = 255 - (((gdouble) i * (drawing_area->time_lapse_length / 6.0)) * 255.0);
    }else{
      drawing_area->time_lapse_red[i] = 0;
    }
  }

  drawing_area->time_lapse_green = (guint *) malloc(drawing_area->time_lapse_length * sizeof(guint));
  
  for(i = 0; i < drawing_area->time_lapse_length; i++){
    if(i < 1.0 * drawing_area->time_lapse_length / 3.0){
      drawing_area->time_lapse_green[i] = 0;
    }else if(i < 2.0 * drawing_area->time_lapse_length / 3.0){
      drawing_area->time_lapse_green[i] = 255;
    }else{
      drawing_area->time_lapse_green[i] = 255 - (((gdouble) i * (drawing_area->time_lapse_length / 9.0)) * 255.0);
    }
  }

  drawing_area->time_lapse_blue = (guint *) malloc(drawing_area->time_lapse_length * sizeof(guint));
  
  for(i = 0; i < drawing_area->time_lapse_length; i++){
    if(i < 1.0 * drawing_area->time_lapse_length / 3.0){
      drawing_area->time_lapse_blue[i] = 255 - (((gdouble) i * (drawing_area->time_lapse_length / 3.0)) * 255.0);
    }else if(i < 2.0 * drawing_area->time_lapse_length / 3.0){
      drawing_area->time_lapse_blue[i] = 0;
    }else{
      drawing_area->time_lapse_blue[i] = 255;
    }
  }

  drawing_area->time_lapse_alpha = (guint *) malloc(drawing_area->time_lapse_length * sizeof(guint));
  
  for(i = 0; i < drawing_area->time_lapse_length; i++){
    drawing_area->time_lapse_alpha[i] = 255;
  }
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
monolight_drawing_area_delegate_render(MonolightDrawingArea *drawing_area,
				       cairo_t *cr,
				       gchar *program,
				       guint samplerate,
				       guint buffer_size,
				       gdouble *magnitude_buffer,
				       guint buffer_start, guint buffer_end,
				       guint x0, guint x1,
				       guint width, guint height,
				       gdouble angle, gdouble scale,
				       guint r, guint g, guint b, guint a)
{
  if(!MONOLIGHT_IS_DRAWING_AREA(drawing_area)){
    return;
  }
  
  width *= MONOLIGHT_DRAWING_AREA_DEFAULT_SCALE_FACTOR;
  height *= MONOLIGHT_DRAWING_AREA_DEFAULT_SCALE_FACTOR;
  
  if(!g_ascii_strncasecmp(program,
			  "cross",
			  6)){
    monolight_animation_util_render_cross(cr,
					  samplerate,
					  buffer_size,
					  magnitude_buffer,
					  buffer_start, buffer_end,
					  x0, x1,
					  width, height,
					  angle, scale,
					  r, g, b, a);
  }else if(!g_ascii_strncasecmp(program,
				"square",
				7)){
    monolight_animation_util_render_square(cr,
					   samplerate,
					   buffer_size,
					   magnitude_buffer,
					   buffer_start, buffer_end,
					   x0, x1,
					   width, height,
					   angle, scale,
					   r, g, b, a);
  }else if(!g_ascii_strncasecmp(program,
				"wave-pulse",
				11)){
    monolight_animation_util_render_wave_pulse(cr,
					       samplerate,
					       buffer_size,
					       magnitude_buffer,
					       buffer_start, buffer_end,
					       x0, x1,
					       width, height,
					       angle, scale,
					       r, g, b, a);
  }else if(!g_ascii_strncasecmp(program,
				"block-pulse",
				12)){
    monolight_animation_util_render_block_pulse(cr,
						samplerate,
						buffer_size,
						magnitude_buffer,
						buffer_start, buffer_end,
						x0, x1,
						width, height,
						angle, scale,
						r, g, b, a);
  }
}

void
monolight_drawing_area_render_magnitude(MonolightDrawingArea *drawing_area,
					guint audio_channel,
					guint samplerate,
					guint buffer_size,
					gdouble *magnitude_buffer)
{
  cairo_t *cr;

  guint i;

  if(!MONOLIGHT_IS_DRAWING_AREA(drawing_area)){
    return;
  }

  cr = gdk_cairo_create(GTK_WIDGET(drawing_area)->window);
  
  if(cr == NULL){
    return;
  }
  
  cairo_push_group(cr);

  /* clear with background color */
  cairo_set_source_rgba(cr,
			0.0,
			0.0,
			0.0,
			0.25);

  cairo_rectangle(cr,
		  0.0, 0.0,
		  MONOLIGHT_DRAWING_AREA_DEFAULT_SCALE_FACTOR * MONOLIGHT_DRAWING_AREA_DEFAULT_WIDTH, MONOLIGHT_DRAWING_AREA_DEFAULT_SCALE_FACTOR * MONOLIGHT_DRAWING_AREA_DEFAULT_HEIGHT);
  
  cairo_fill(cr);

  for(i = 0; i < drawing_area->program_count; i++){  
    if(((0x1 << i) & drawing_area->time_lapse_program[drawing_area->position]) != 0){
      guint buffer_start, buffer_end;
      gdouble angle;
      guint red, green, blue, alpha;

      buffer_start = i * (buffer_size / (2 * drawing_area->program_count));
      buffer_end = (i + 1) * (buffer_size / (2 * drawing_area->program_count));
      
      angle = drawing_area->time_lapse_start_angle[drawing_area->position] +
	((drawing_area->current_period / drawing_area->time_lapse_period[drawing_area->position]) *
	 (drawing_area->time_lapse_end_angle[drawing_area->position] - drawing_area->time_lapse_start_angle[drawing_area->position]));

      red = drawing_area->time_lapse_red[drawing_area->position];
      green = drawing_area->time_lapse_green[drawing_area->position];
      blue = drawing_area->time_lapse_blue[drawing_area->position];
      alpha = drawing_area->time_lapse_alpha[drawing_area->position];
      
      monolight_drawing_area_delegate_render(drawing_area,
					     cr,
					     drawing_area->program[i],
					     samplerate,
					     buffer_size,
					     magnitude_buffer,
					     buffer_start, buffer_end,
					     0, 0,
					     MONOLIGHT_DRAWING_AREA_DEFAULT_WIDTH, MONOLIGHT_DRAWING_AREA_DEFAULT_HEIGHT,
					     angle, drawing_area->time_lapse_scale[drawing_area->position],
					     red, green, blue, alpha);
    }
  }

  /* increment current period and position */
  drawing_area->current_period += 1;
  
  if(drawing_area->current_period >= drawing_area->time_lapse_period[drawing_area->position]){
    drawing_area->position += 1;
    
    drawing_area->current_period = 0;
  }
  
  if(drawing_area->position >= drawing_area->time_lapse_length){
    drawing_area->position = 0;
  }

  cairo_pop_group_to_source(cr);
  cairo_paint(cr);

  cairo_surface_mark_dirty(cairo_get_target(cr));
  cairo_destroy(cr);
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
