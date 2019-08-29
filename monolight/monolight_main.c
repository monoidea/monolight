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

#include <glib.h>
#include <glib-object.h>

#define _GNU_SOURCE
#include <locale.h>

#include <monolight/monolight_main.h>

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include "config.h"

void sig_handler(int signo)
{
  if(signo == SIGKILL){
    g_main_loop_quit();
  }
}

int
main(int argc, char **argv)
{
  MonolightRGBMatrix *rgb_matrix;

  GMainLoop *main_loop;
  
  putenv("TZ=UTC");
  
  setlocale(LC_ALL, "");
  bindtextdomain(PACKAGE, LOCALEDIR);
  textdomain(PACKAGE);

  signal(SIGKILL, sig_handler);
  
  main_loop = g_main_loop_new(NULL,
			      TRUE);

  rgb_matrix = monolight_rgb_matrix_new();
  monolight_rgb_matrix_start(rgb_matrix);

  g_main_loop_run();

  monolight_rgb_matrix_stop(rgb_matrix);
  
  return(0);
}
