/*
    convert.h - Part of IMSKPE

    Copyright (C) 2004 Andreas Madsack

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
/**
 * @file   convert.h
 * @author Andreas Madsack
 * 
 * @brief  convert par->wav using klatt80/parwave
 * 
 * 
 */

#define ORIG 0
#define SYNTH 1
// #define BOTH 2           // is maybe a problem!!

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

void configure_wavarea(GtkWidget *widget);
void repaint_wave();
void redraw_wave_expose(GtkWidget *w,GdkEventExpose  *event);
void init_wave(GtkWidget *widget);
void redraw_wave(GtkWidget *widget);
int width_wave(GtkWidget *widget);
int height_wave(GtkWidget *widget);
void drawline_wave(GtkWidget *widget, int x, int y1, int y2);

gboolean convert(char *filename);

