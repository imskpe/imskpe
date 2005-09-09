/*
    graphics.h - Part of IMSKPE

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
 * @file   graphics.h
 * @author Andreas Madsack
 * 
 * @brief  all visual routines of IMSKPE
 * 
 */


#include <gtk/gtk.h>

/*
  structs
*/

/** struct for drawarea-pixmap */
typedef struct {
    GdkDrawable *pixmap;
    GdkGC *gc;
} typGraphics;

/** struct for list over formants (max 6) */
typedef struct {
  gchar *formantname;
  GdkColor color;    
} typFormantList;


/*
  procedures
*/

GtkWidget *GetMainWindow();
void SetMainWindow(GtkWidget *w);

void GuiSetDuration(unsigned int x);
void GuiSetUpdateInterval(unsigned int x);
void GuiSetSamplingRate(unsigned int x);
void GuiSetNumberFormants(unsigned int x);
void GuiSetVoiceSource(unsigned int x);
void GuiSetBranches(unsigned int x);
gboolean GuiGetToggleButtonState(char tmp[30]);
void GuiSetToolbarStyle(int style);

int CalcRealX(int dx, int maxx);
int CalcRealY(int dy, int maxy,diagramTyp dia);
void update_ruler(GtkWidget *widget, diagramTyp dia);
void configure_drawarea(GtkWidget *widget, diagramTyp dia);
GdkGC *GetPenRGB (GdkGC *gc, int nRed, int nGreen, int nBlue);
GdkGC *GetPenGdkColor (GdkGC *gc, GdkColor col);
GdkColor GetColor (gdouble Red, gdouble Green, gdouble Blue);
void Repaint(GtkWidget *d, diagramTyp dia);
void Redrawpixmap(GtkWidget *w, GdkEventExpose  *event);
typGraphics *NewGraphics ();
// void CurveListInit ();

void configure_page(int page);
void redraw_page(int page);

void SetStatusBar(char *sb, gchar *text);
void SetTitle(gchar *text);
void SetToggleButton(MouseActionTyp typ);
void DrawAreaMotion(int rx, int ry,   GdkModifierType state, diagramTyp dia);
void DrawButtonPressed(int rx, int ry,GdkEventButton  *event, diagramTyp dia);

int GetMousepressed();
void SetMousepressed(int foo);

void SetLabelColor(GtkWidget *w, char *cfgstr);
gboolean DialogYesNo (char *msg);
GtkWidget *DialogInfoOK (char *msg);
GtkWidget *DialogErrorOK (char *msg);
GtkWidget *DialogOK (char *msg, GtkMessageType mt);
char *filtertoken(char str[100], char token[1]);

gboolean SetAddMenuItem(char *buf);
void InitSplash();
