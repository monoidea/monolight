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

#include <stdlib.h>
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
  gdouble line_width[5];

  gdouble correction;
  gdouble frequency;
  gdouble magnitude;  
  gdouble a_pos_x0, a_pos_y0;
  gdouble a_width, a_height;
  gdouble max_line_width;
  guint nth;
  guint i, k;

  width *= scale;
  height *= scale;
  
  max_line_width = scale * 5.0;

  for(i = 0; i < 5; i++){
    line_width[i] = max_line_width;
  }

  correction = (gdouble) samplerate / (gdouble) buffer_size;
  magnitude = 0.0;
  
  for(nth = buffer_start + 1, i = 0, k = 0; i < buffer_end - buffer_start && nth < buffer_size / 2; i++, nth++){
    frequency = nth * correction;
    
    magnitude += (magnitude_buffer[nth] / 100.0);
    k++;

    if(nth % ((buffer_end - buffer_start) / 5) == 0){
      line_width[(guint) floor((double) i / (double) (buffer_end - buffer_start) * 5.0)] *= magnitude;

      magnitude = 0.0;
      k = 0;
    }
  }

  cairo_set_source_rgba(cr,
			r / 255.0, g / 255.0, b / 255.0, a / 255.0);

  for(i = 0; i < 5; i++){
    gdouble length;
    
    /* line width */
    cairo_set_line_width(cr,
			 line_width[i]);

    length = (gdouble) i * (gdouble) (rand() % (width / 5));
    
    a_pos_x0 = ((gdouble) width / 2.0) - (length / 2.0);
    a_pos_y0 = ((gdouble) height / 2.0) - (length / 2.0);

    a_width = length;
    a_height = length;
    
    cairo_rectangle(cr,
		    a_pos_x0, a_pos_y0,
		    a_width, a_height);
    cairo_stroke(cr);
  }
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
  gdouble line_width[5];

  gdouble correction;
  gdouble frequency;
  gdouble magnitude;  
  gdouble a_pos_x0, a_pos_y0;
  gdouble a_radius;
  gdouble max_line_width;
  guint nth;
  guint i, k;
  
  width *= scale;
  height *= scale;
  
  max_line_width = scale * 10.0;

  for(i = 0; i < 5; i++){
    line_width[i] = max_line_width;
  }

  correction = (gdouble) samplerate / (gdouble) buffer_size;
  magnitude = 0.0;
  
  for(nth = buffer_start + 1, i = 0, k = 0; i < buffer_end - buffer_start && nth < buffer_size / 2; i++, nth++){
    frequency = nth * correction;
    
    magnitude += (magnitude_buffer[nth] / 100.0);
    k++;

    if(nth % ((buffer_end - buffer_start) / 5) == 0){
      line_width[(guint) floor((double) i / (double) (buffer_end - buffer_start) * 5.0)] *= magnitude;

      magnitude = 0.0;
      k = 0;
    }
  }

  cairo_set_source_rgba(cr,
			r / 255.0, g / 255.0, b / 255.0, a / 255.0);

  for(i = 0; i < 5; i++){
    /* line width */
    cairo_set_line_width(cr,
			 line_width[i]);

    a_pos_x0 = (gdouble) width / 2.0;
    a_pos_y0 = (gdouble) height / 2.0;

    a_radius = (gdouble) i * (gdouble) (rand() % (width / 5));
    
    cairo_arc(cr,
	      a_pos_x0, a_pos_y0,
	      a_radius,
	      0.0, 2.0 * M_PI);
    cairo_stroke(cr);
  }
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
  gdouble line_length[5];

  gdouble correction;
  gdouble frequency;
  gdouble magnitude;
  gdouble a_x0, a_y0;
  gdouble a_width, a_height;
  gdouble b_x0, b_y0;
  gdouble b_width, b_height;
  gdouble c_x0, c_y0;
  gdouble c_width, c_height;
  gdouble d_x0, d_y0;
  gdouble d_width, d_height;
  gdouble e_x0, e_y0;
  gdouble e_width, e_height;
  gdouble max_line_length;
  guint nth;
  guint i, k;

  width *= scale;
  height *= scale;
  
  max_line_length = scale * (width / 4.0);

  for(i = 0; i < 5; i++){
    line_length[i] = max_line_length;
  }
  
  correction = (gdouble) samplerate / (gdouble) buffer_size;
  magnitude = 0.0;
  
  for(nth = buffer_start + 1, i = 0, k = 0; i < buffer_end - buffer_start && nth < buffer_size / 2; i++, nth++){
    frequency = nth * correction;
    
    magnitude += (magnitude_buffer[nth] / 100.0);
    k++;

    if(nth % ((buffer_end - buffer_start) / 5) == 0){
      line_length[(guint) floor((double) i / (double) (buffer_end - buffer_start) * 5.0)] *= magnitude;

      magnitude = 0.0;
      k = 0;
    }
  }

  cairo_set_source_rgba(cr,
			r / 255.0, g / 255.0, b / 255.0, a / 255.0);

  cairo_save(cr);

  cairo_translate(cr,
		  width / 2.0, height / 2.0);
  cairo_rotate(cr,
	       angle);
  cairo_translate(cr,
		  -1.0 * width / 2.0, -1.0 * height / 2.0);

  for(i = 0; i < 4; i++){    
    cairo_save(cr);

    switch(i){
    case 0:
    {
      a_x0 = ((gdouble) width * 0.25) - line_length[0];
      a_y0 = ((gdouble) height * 0.25) - line_length[0];

      b_x0 = ((gdouble) width * 0.25) - line_length[1];
      b_y0 = ((gdouble) height * 0.25) + line_length[1];

      c_x0 = ((gdouble) width * 0.25) + line_length[2];
      c_y0 = ((gdouble) height * 0.25) + line_length[2];

      d_x0 = ((gdouble) width * 0.25) + line_length[3];
      d_y0 = ((gdouble) height * 0.25) - line_length[3];

      e_x0 = ((gdouble) width * 0.25) - (line_length[4] / 2.0);
      e_y0 = ((gdouble) height * 0.25) - (line_length[4] / 2.0);
    }
    break;
    case 1:
    {
      a_x0 = ((gdouble) width * 0.25) - line_length[0];
      a_y0 = ((gdouble) height * 0.75) + line_length[0];

      b_x0 = ((gdouble) width * 0.25) - line_length[1];
      b_y0 = ((gdouble) height * 0.75) - line_length[1];

      c_x0 = ((gdouble) width * 0.25) + line_length[2];
      c_y0 = ((gdouble) height * 0.75) - line_length[2];

      d_x0 = ((gdouble) width * 0.25) - line_length[3];
      d_y0 = ((gdouble) height * 0.75) + line_length[3];

      e_x0 = ((gdouble) width * 0.25) - (line_length[4] / 2.0);
      e_y0 = ((gdouble) height * 0.75) - (line_length[4] / 2.0);
    }
    break;
    case 2:
    {
      a_x0 = ((gdouble) width * 0.75) + line_length[0];
      a_y0 = ((gdouble) height * 0.75) + line_length[0];

      b_x0 = ((gdouble) width * 0.75) - line_length[1];
      b_y0 = ((gdouble) height * 0.75) - line_length[1];

      c_x0 = ((gdouble) width * 0.75) - line_length[2];
      c_y0 = ((gdouble) height * 0.75) + line_length[2];

      d_x0 = ((gdouble) width * 0.75) + line_length[3];
      d_y0 = ((gdouble) height * 0.75) - line_length[3];

      e_x0 = ((gdouble) width * 0.75) - (line_length[4] / 2.0);
      e_y0 = ((gdouble) height * 0.75) - (line_length[4] / 2.0);
    }
    break;
    case 3:
    {
      a_x0 = ((gdouble) width * 0.75) + line_length[0];
      a_y0 = ((gdouble) height * 0.25) - line_length[0];

      b_x0 = ((gdouble) width * 0.75) + line_length[1];
      b_y0 = ((gdouble) height * 0.25) + line_length[1];

      c_x0 = ((gdouble) width * 0.75) - line_length[2];
      c_y0 = ((gdouble) height * 0.25) - line_length[2];

      d_x0 = ((gdouble) width * 0.75) - line_length[3];
      d_y0 = ((gdouble) height * 0.25) + line_length[3];

      e_x0 = ((gdouble) width * 0.75) - (line_length[4] / 2.0);
      e_y0 = ((gdouble) height * 0.25) - (line_length[4] / 2.0);
    }
    break;
    }
    
    a_width = line_length[0];
    a_height = line_length[0];
    
    b_width = line_length[1];
    b_height = line_length[1];

    c_width = line_length[2];
    c_height = line_length[2];

    d_width = line_length[3];
    d_height = line_length[3];

    e_width = line_length[4];
    e_height = line_length[4];
    
    /* square a - #0 */
    cairo_rectangle(cr,
		    a_x0, a_y0,
		    a_width, a_height);
    cairo_fill(cr);

    /* square b - #0 */
    cairo_rectangle(cr,
		    b_x0, b_y0,
		    b_width, b_height);
    cairo_fill(cr);

    /* square c - #0 */
    cairo_rectangle(cr,
		    c_x0, c_y0,
		    c_width, c_height);
    cairo_fill(cr);

    /* square d - #0 */
    cairo_rectangle(cr,
		    d_x0, d_y0,
		    d_width, d_height);
    cairo_fill(cr);

    /* square e - #0 */
    cairo_rectangle(cr,
		    e_x0, e_y0,
		    e_width, e_height);
    cairo_fill(cr);  

    cairo_restore(cr);
  }

  cairo_restore(cr);
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

  gdouble correction;
  gdouble frequency;
  gdouble magnitude;  
  gdouble a_pos_x0, a_pos_y0;
  gdouble a_pos_x1, a_pos_y1;
  gdouble b_pos_x0, b_pos_y0;
  gdouble b_pos_x1, b_pos_y1;
  gdouble max_line_width;
  guint nth;
  guint i, k;

  width *= scale;
  height *= scale;
  
  max_line_width = scale * 4.0;

  for(i = 0; i < 5; i++){
    line_width[i] = max_line_width;
  }
  
  correction = (gdouble) samplerate / (gdouble) buffer_size;
  magnitude = 0.0;
  
  for(nth = buffer_start + 1, i = 0, k = 0; i < buffer_end - buffer_start && nth < buffer_size / 2; i++, nth++){
    frequency = nth * correction;
    
    magnitude += (magnitude_buffer[nth] / 100.0);
    k++;

    if(nth % ((buffer_end - buffer_start) / 5) == 0){
      if(magnitude != 0.0){
	line_width[(guint) floor((double) i / (double) (buffer_end - buffer_start) * 5.0)] *= magnitude;
      }

      magnitude = 0.0;
      k = 0;
    }
  }

  cairo_set_source_rgba(cr,
			r / 255.0, g / 255.0, b / 255.0, a / 255.0);

  cairo_save(cr);

  cairo_translate(cr,
		  width / 2.0, height / 2.0);
  cairo_rotate(cr,
	       angle);
  cairo_translate(cr,
		  -1.0 * width / 2.0, -1.0 * height / 2.0);

  for(i = 0; i < 5; i++){    
    /* line a */
    a_pos_x0 = 0.0;
    a_pos_y0 = 0.0;

    a_pos_x1 = (gdouble) width;
    a_pos_y1 = (gdouble) height;

    /* line b */
    b_pos_x0 = width;
    b_pos_y0 = 0.0;

    b_pos_x1 = 0.0;
    b_pos_y1 = (gdouble) height;

    /* line width */
    cairo_set_line_width(cr,
			 line_width[i]);
    
    cairo_save(cr);
    
    cairo_translate(cr,
		    width / 2.0, height / 2.0);
    cairo_rotate(cr,
		 ((i / 5.0) / 4.0) * (2.0 * M_PI));
    cairo_translate(cr,
		    -1.0 * width / 2.0, -1.0 * height / 2.0);

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

    cairo_restore(cr);
  }

  cairo_restore(cr);
}
