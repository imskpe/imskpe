/*
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

#include <gtk/gtk.h>

/*
  structs
*/

/// struct for drawarea-pixmap
typedef struct {
    GdkDrawable *pixmap;
    GdkGC *gc;
} typGraphics;

/// struct for points on curves
typedef struct {
    gint time;
    gint value;
} typListValue;

/// struct for list over formants (max 6)
typedef struct {
  gchar *formantname;
  GdkColor color;    
} typListFormants;

/// struct for list over curves
typedef struct {
    /// list over points
    GList *points;
    /// name of togglebutton
    gchar *widget_name; 
    /// name of formant
    gchar *formant;
    /// show curve or not
    gboolean show;
} typListCurve;


/*
  procedures
*/
GtkWidget *get_main_window();
void set_main_window(GtkWidget *w);

GdkGC *GetPenRGB (int nRed, int nGreen, int nBlue);
GdkGC *GetPenGdkColor (GdkColor col);
GdkColor GetColor (gdouble Red, gdouble Green, gdouble Blue);
GdkColor GetFormantListColor (gchar *searchstring);
void Repaint(GtkWidget *d);
void Redrawpixmap(GtkWidget *w, GdkEventExpose  *event);
typGraphics *NewGraphics ();
void CurveListInit ();

void redraw_all_drawareas();
typListValue *PointInsert (gint t, gint v);
