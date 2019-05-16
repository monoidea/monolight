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

#include <monolight/ui/monolight_animation_util.h>

#include <math.h>

void
monolight_animation_util_render_block_pulse(cairo_t *cr,
					    guint samplerate,
					    guint buffer_size,
					    gdouble *magnitude_buffer,
					    guint buffer_start, guint buffer_end,
					    guint x0, guint x1,
					    guint width, guint height,
					    gdouble angle, gdouble scale,
					    guint r, guint g, guint b, guint a)
{
  //TODO:JK: implement me
}

void
monolight_animation_util_render_wave_pulse(cairo_t *cr,
					   guint samplerate,
					   guint buffer_size,
					   gdouble *magnitude_buffer,
					   guint buffer_start, guint buffer_end,
					   guint x0, guint x1,
					   guint width, guint height,
					   gdouble angle, gdouble scale,
					   guint r, guint g, guint b, guint a)
{
  //TODO:JK: implement me
}

void
monolight_animation_util_render_square(cairo_t *cr,
				       guint samplerate,
				       guint buffer_size,
				       gdouble *magnitude_buffer,
				       guint buffer_start, guint buffer_end,
				       guint x0, guint x1,
				       guint width, guint height,
				       gdouble angle, gdouble scale,
				       guint r, guint g, guint b, guint a)
{
  //TODO:JK: implement me
}

void
monolight_animation_util_render_cross(cairo_t *cr,
				      guint samplerate,
				      guint buffer_size,
				      gdouble *magnitude_buffer,
				      guint buffer_start, guint buffer_end,
				      guint x0, guint x1,
				      guint width, guint height,
				      gdouble angle, gdouble scale,
				      guint r, guint g, guint b, guint a)
{
  gdouble line_width[5];
  
  gdouble a_pos_x0, a_pos_y0;
  gdouble a_pos_x1, a_pos_y1;
  gdouble b_pos_x0, b_pos_y0;
  gdouble b_pos_x1, b_pos_y1;
  gdouble max_line_width;
  guint i;
  
  max_line_width = 3.0;

  for(i = 0; i < 5; i++){
    line_width[i] = max_line_width;
  }
  
  for(i = 0; i < buffer_end - buffer_start; i++){
    gdouble value;

    value = magnitude_buffer[buffer_start + i];  

    line_width[(guint) floor((double) i / (double) (buffer_end - buffer_start) * 5.0)] *= value;
  }

  cairo_set_source_rgba(cr,
			r, g, b, a);

  for(i = 0; i < 5; i++){
    /* cross */
    if((angle >= 0.0 &&
	angle < 0.25 * (2.0 * M_PI)) ||
       (angle >= 0.5 * (2.0 * M_PI) &&
	angle < 0.75 * (2.0 * M_PI))){
      /* line a */
      a_pos_x0 = ((gdouble) width / 2.0 * (5 / (5 - i))) + ((gdouble) height / 2.0) * tan(angle + ((i / 5) * 0.25 * (2.0 * M_PI)));
      a_pos_y0 = (gdouble) height - (gdouble) height * (5 / (5 - i));

      a_pos_x1 = ((gdouble) width / 2.0 * (5 / (5 - i))) - ((gdouble) height / 2.0) * tan(angle);
      a_pos_y1 = (gdouble) height * (5 / (5 - i));

      /* line b */
      b_pos_x0 = (gdouble) width - (gdouble) width * (5 / (5 - i));
      b_pos_y0 = ((gdouble) height / 2.0 * (5 / (5 - i))) + ((gdouble) width / 2.0) * tan(angle + 0.25 * (2.0 * M_PI));

      b_pos_x1 = (gdouble) width * (5 / (5 - i));
      b_pos_y1 = ((gdouble) height / 2.0 * (5 / (5 - i))) - ((gdouble) width / 2.0) * tan(angle + 0.25 * (2.0 * M_PI));
    }else{
      /* line a */
      a_pos_x0 = (gdouble) width - (gdouble) width * (5 / (5 - i));
      a_pos_y0 = ((gdouble) height / 2.0 * (5 / (5 - i))) + ((gdouble) width / 2.0) * tan(angle);

      a_pos_x1 = (gdouble) width * (5 / (5 - i));
      a_pos_y1 = ((gdouble) height / 2.0 * (5 / (5 - i))) - ((gdouble) width / 2.0) * tan(angle);

      /* line b */
      b_pos_x0 = ((gdouble) width / 2.0 * (5 / (5 - i))) + ((gdouble) height / 2.0) * tan(angle + 0.25 * (2.0 * M_PI));
      b_pos_y0 = (gdouble) height - (gdouble) height * (5 / (5 - i));

      b_pos_x1 = ((gdouble) width / 2.0 * (5 / (5 - i))) - ((gdouble) height / 2.0) * tan(angle + 0.25 * (2.0 * M_PI));
      b_pos_y1 = (gdouble) height * (5 / (5 - i));
    }

    /* line width */
    cairo_set_line_width(cr,
			 line_width[i]);
    
    /* line a */
    cairo_move_to(cr,
		  a_pos_x0, a_pos_y0);

    cairo_line_to(cr,
		  a_pos_x1, a_pos_y1);

    cairo_stroke(cr);
    
    /* line b */
    cairo_move_to(cr,
		  b_pos_x0, b_pos_y0);

    cairo_line_to(cr,
		  b_pos_x1, b_pos_y1);

    cairo_stroke(cr);
  }
}
