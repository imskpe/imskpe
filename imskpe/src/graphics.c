/*
    graphics.c - Part of IMSKPE

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
 * @file   graphics.c
 * @author Andreas Madsack
 * 
 * @brief  all visual routines of IMSKPE
 * 
 * \todo put part of functions in other files (formant/curves-routines, math-stuff, statusbar, ...)
 * 
 * \todo color muss auch in Curves ... -> keine formantsliste mehr ... / abspeichern der farbwerte in den preferences ... 
 *
 * \note idea: 
 * - one list for formants (in preferences)
 *   - specify color of label and curves
 * - list of curves (in file-struct)
 *   - inhabits list of points
 * 
 * -  skala muss flexibler werden .. 
 *    und die daten m�ssen abgreifbar sein ...
 *
 * - routine, die die linien im repaint malt ...
 *   incl. rechtecken auf den punkten ...
 *   sofern diese show=TRUE haben ...
 * 
 * - garbage collector f�r die ganze schose ...
 * 
 * - eventroutinen 
 *   - wenn maus dr�ber linie dicker
 *   - wenn maus �ber punkt, dann auch dot dicker
 *   - aktuelle koordinaten in status-right anzeigen
 *   - aktuelle kurve in status-left (oder neuer middle) anzeigen
 *   - file evt. in title ...
 *
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib/gprintf.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "graphics.h"
#include "cfg.h"
#include "curves.h"


/*
  global variables
*/
GtkWidget *main_window;
typGraphics *g = NULL;
int nScreenWidth = 200;
int nScreenHeight = 200;

//GList *curves = NULL;  // put in file struct!!
GList *formants = NULL;  // put in preferences ?

/** \todo put in another file - not graphic relevant! */
GtkWidget *GetMainWindow()
{
  return (GtkWidget *)main_window;
}

void SetMainWindow(GtkWidget *w)
{
  main_window=(GtkWidget *)w;

  return;
}

void GuiSetDuration(unsigned int x)
{
  GtkWidget *w = lookup_widget (GTK_WIDGET (GetMainWindow()), "spbn_duration");
  gtk_spin_button_set_value ((GtkSpinButton *)w,
			     (float)x);
}

void GuiSetUpdateInterval(unsigned int x)
{
  GtkWidget *w = lookup_widget (GTK_WIDGET (GetMainWindow()), "spbn_ui");
  gtk_spin_button_set_value ((GtkSpinButton *)w,
			     (float)x);
}

void GuiSetSamplingRate(unsigned int x)
{
  GtkWidget *w = lookup_widget (GTK_WIDGET (GetMainWindow()), "spbn_sprate");
  gtk_spin_button_set_value ((GtkSpinButton *)w,
			     (float)x);
}

void GuiSetNumberFormants(unsigned int x)
{
  GtkWidget *w = lookup_widget (GTK_WIDGET (GetMainWindow()), "spbn_numF");
  gtk_spin_button_set_value ((GtkSpinButton *)w,
			     (float)x);
}

void GuiSetVoiceSource(unsigned int x)
{
  GtkWidget *w = lookup_widget (GTK_WIDGET (GetMainWindow()), "cm_vs_entry");
  switch(x)
  {
  case 1:
      gtk_entry_set_text ((GtkEntry *)w,"impulse");
      break;
  case 2:
      gtk_entry_set_text ((GtkEntry *)w,"natural");
      break;
  case 3:
      gtk_entry_set_text ((GtkEntry *)w,"sampled");
      break;
  }
}

void GuiSetBranches(unsigned int x)
{
  GtkWidget *w = lookup_widget (GTK_WIDGET (GetMainWindow()), "cm_cp_entry");
  switch(x)
  {
  case 1:
      gtk_entry_set_text ((GtkEntry *)w,"cascade + parallel");
      break;
  case 2:
      gtk_entry_set_text ((GtkEntry *)w,"parallel only");
      break;
  }
}


/* ------------------------------------------------------------  */


void FormantListInit ()
{
  typFormantList *oneformant;
  gchar *x;

/* F1 */
  oneformant = g_malloc (sizeof (typFormantList));

  oneformant->formantname = g_malloc (sizeof (gchar)*5);
  strcpy(oneformant->formantname,"f1");

  oneformant->color=GetColor(1.0,0,0);

  formants = g_list_append (formants, oneformant);

/* F2 */
  oneformant = g_malloc (sizeof (typFormantList));

  oneformant->formantname = g_malloc (sizeof (gchar)*5);
  strcpy(oneformant->formantname,"f2");

  oneformant->color=GetColor(0,0,1.0);

  formants = g_list_append (formants, oneformant);

/* F3 */
  oneformant = g_malloc (sizeof (typFormantList));

  oneformant->formantname = g_malloc (sizeof (gchar)*5);
  strcpy(oneformant->formantname,"f3");

  oneformant->color=GetColor(0,1.0,0);

  formants = g_list_append (formants, oneformant);

/* F4 */
  oneformant = g_malloc (sizeof (typFormantList));

  oneformant->formantname = g_malloc (sizeof (gchar)*5);
  strcpy(oneformant->formantname,"f4");

  oneformant->color=GetColor(1.0,1.0,0);

  formants = g_list_append (formants, oneformant);

/* F5 */
  oneformant = g_malloc (sizeof (typFormantList));

  oneformant->formantname = g_malloc (sizeof (gchar)*5);
  strcpy(oneformant->formantname,"f5");

  oneformant->color=GetColor(0,1.0,1.0);

  formants = g_list_append (formants, oneformant);

/* F6 */
  oneformant = g_malloc (sizeof (typFormantList));

  oneformant->formantname = g_malloc (sizeof (gchar)*5);
  strcpy(oneformant->formantname,"f6");

  oneformant->color=GetColor(1.0,0,1.0);

  formants = g_list_append (formants, oneformant);

/* nasals */
  oneformant = g_malloc (sizeof (typFormantList));

  /** \todo save maxsize of of formantname!! */
  oneformant->formantname = g_malloc (sizeof (gchar)*(strlen("nasals")+1));
  strcpy(oneformant->formantname,"nasals");

  oneformant->color=GetColor(0.5,0.5,1.0);

  formants = g_list_append (formants, oneformant);


/* wohin mit dieser initroutine?! */
  LoadConfig();

/* not best position ... :) */
//  CurveListInit ();
}

GdkColor GetFormantListColor (gchar *searchstring)
{
  typFormantList *oneformant;  

/*  printf ("%s\n",searchstring);*/
/*  printf("--\nsearch: %s\n",searchstring);*/
  formants=g_list_first (formants);
  while(formants)
  {	
    gchar *fname;
    fname = g_malloc (sizeof (gchar)*8);

//    oneformant = g_malloc (sizeof (typFormantList));
    oneformant = formants->data;

    strncpy(fname,oneformant->formantname,8);
    if(!strcmp(fname,searchstring))   /* strCASEcmp under win32? */
    {
      printf("found %s\n",fname);
      return oneformant->color;
    }

    formants=formants->next;

    free(fname);
//    free(oneformant);
  }

/*  printf("problem?! %s\n",searchstring);*/
  /* return FALSE?? */
}


void FormantListFree()
{
  typFormantList *data;

  formants=g_list_first (formants);
  while(formants)
  {	
    data=(typFormantList *)formants->data;
    
    if(data->formantname!=NULL)
      free(data->formantname);
    
    if(data!=NULL)
      free(data);
    
    formants = g_list_remove(formants,data);
  }
//  g_list_free(formants);
}

/* ------------------------------------------------ */

/** 
 * draw ruler
 *
 * \todo export values to preferences!
 * 
 * @param widget 
 */
void update_ruler(GtkWidget *widget)
{
  gint duration;
  int i;
  PangoLayout *layout;
  PangoFontDescription *fontdesc;
  gchar *x;

  int xsplits=15;  /* export in preferences */
  int ysplits=10;  /* export in preferences */
//  int ymax=3000;   /* put in preferences and/or calculate it! */
  int ymax=500;   /* put in preferences and/or calculate it! */

  duration = gtk_spin_button_get_value_as_int((GtkSpinButton *)lookup_widget (GTK_WIDGET (main_window), "spbn_duration"));

  x = g_malloc (sizeof (gchar)*10);
  
  /* \todo choose another font ...  and/or export in preferences */
  fontdesc = pango_font_description_from_string ("Luxi Mono 6");  
  

/* x-achse */
  gdk_draw_line (g->pixmap, GetPenRGB (0, 0, 0) ,
		 20, 
		 widget->allocation.height-25,
		 widget->allocation.width, 
		 widget->allocation.height-25);

  for(i=0;i<=xsplits;i++)
  {
    int mod=5;  /* -> preferences  */
/*
    int mod=xsplits;
    if(i==0)
    mod=(xsplits/3);
    if(i==1)
    mod=(xsplits/3)*2;
*/
    sprintf(x,"%d",i*duration/xsplits);
    layout = gtk_widget_create_pango_layout (widget, x);

    pango_layout_set_font_description (layout, fontdesc); 
    gdk_draw_layout (g->pixmap,GetPenRGB(0xffff,0,0),
		     ((widget->allocation.width-25)/xsplits)*i+25-mod, 
		     widget->allocation.height-15, layout);

    gdk_draw_line (g->pixmap, GetPenRGB (0, 0, 0xffff) ,
		   ((widget->allocation.width-25)/xsplits)*i+25, 
		   widget->allocation.height-30,
		   ((widget->allocation.width-25)/xsplits)*i+25, 
		   widget->allocation.height-20);
    
  }

/* y-achse */
  gdk_draw_line (g->pixmap, GetPenRGB (0, 0, 0) ,
		 25, 
		 0,
		 25, 
		 widget->allocation.height-20);

  for(i=0;i<=ysplits;i++)
  {
    sprintf(x,"%d",(ysplits-i)*(ymax)/ysplits);  

    layout = gtk_widget_create_pango_layout (widget, x);

    pango_layout_set_font_description (layout, fontdesc); 
    gdk_draw_layout (g->pixmap,GetPenRGB(0xffff,0,0),
		     0, 
		     ((widget->allocation.height-25)/ysplits)*i, layout);

    gdk_draw_line (g->pixmap, GetPenRGB (0, 0, 0xffff) ,
		   20,
		   ((widget->allocation.height-25)*i)/ysplits,
		   30,
		   ((widget->allocation.height-25)*i)/ysplits
		   );
  }

  pango_font_description_free (fontdesc);
  g_object_unref (layout);
  free(x);
}

void configure_drawarea(GtkWidget *widget)
{
  main_window = lookup_widget (GTK_WIDGET (widget), "imskpe_main");

  /* --- Structure doesn't exist? --- */
  if (g == NULL) {
    
    /* --- Create one --- */
    g = NewGraphics ();
  }

  /* --- Existing pixmap?  --- */
  if (g->pixmap == NULL) {
    
    /* --- Free it --- */
    gdk_pixmap_unref (g->pixmap);
  } 

  /* --- Create a new pixmap --- */
  g->pixmap = gdk_pixmap_new (widget->window,
			      widget->allocation.width,
			      widget->allocation.height,
			      -1);
  /* --- Get height and width to clear screen --- */
  nScreenWidth = widget->allocation.width;
  nScreenHeight = widget->allocation.height;

  /* --- Create a new pixmap --- */
  gdk_draw_rectangle (g->pixmap,
		      widget->style->white_gc,
		      TRUE,
		      0, 0,
		      widget->allocation.width,
		      widget->allocation.height);


  update_ruler(widget); 
  
  Repaint(widget);

}



/* ------------------------------------------------ */

/** 
 * NewGraphics allocates space for the global graphics object
 * 
 * 
 * @return 
 */
typGraphics *NewGraphics ()
{
    typGraphics *gfx;

    gfx = (typGraphics *) g_malloc (sizeof (typGraphics));
    gfx->gc = NULL;
    gfx->pixmap = NULL;
    return (gfx);
}


/*
 * GetPen
 *
 * Get a GdkGC (pen) based on the colors passed 
 * in.  This is used to change the color of the 
 * items being drawn.
 *
 * nRed - Red component of pen
 * nGreen - Green component of pen
 * nBlue - Blue component of pen
 */
GdkGC *GetPenRGB (int nRed, int nGreen, int nBlue)
{
    GdkColor *c;
    GdkGC *gc;

    c = (GdkColor *) g_malloc (sizeof (GdkColor));
    c->red = nRed;
    c->green = nGreen;
    c->blue = nBlue;

    gdk_color_alloc (gdk_colormap_get_system (), c);

    gc = gdk_gc_new (g->pixmap);

    gdk_gc_set_foreground (gc, c);

    return (gc);
}
/*
 * GetPen
 *
 */
GdkGC *GetPenGdkColor (GdkColor col)
{
    GdkColor *c;
    GdkGC *gc;

    c = (GdkColor *) g_malloc (sizeof (GdkColor));
    c->red = col.red;
    c->green = col.green;
    c->blue = col.blue;

    gdk_color_alloc (gdk_colormap_get_system (), c);

    gc = gdk_gc_new (g->pixmap);

    gdk_gc_set_foreground (gc, c);

    return (gc);
}

GdkColor GetColor (gdouble Red, gdouble Green, gdouble Blue)
{
  GdkColor col;

  col.red = (guint16)(Red*65535.0);
  col.green = (guint16)(Green*65535.0);
  col.blue = (guint16)(Blue*65535.0);
  
  return col;
}

/** 
 * Repaint
 * 
 * \todo 
 * - calculation of virtualcoordinates in extra function
 * - split in a few functions
 * - many values must be put in preferences
 *
 * @param d 
 */
void Repaint(GtkWidget *d)
{
    GdkRectangle  update_rect;
    typCurveList *c;
    int lastx=-1,lasty=-1;
    int x=-1,y=-1;
    typValueList *v;
//    GList *cv=g_list_first (curves);

/* muss alles ausgelagert werden: */
    int xsplits=15;  /**< export in preferences */
    int ysplits=10;  /**< export in preferences */
    int ymax=500;   /**< put in preferences and/or calculate it! */

    int xmax = FileGetDuration();
//gtk_spin_button_get_value_as_int((GtkSpinButton *)lookup_widget (GTK_WIDGET (main_window), "spbn_duration"));


    /* --- clear pixmap --- */
    gdk_draw_rectangle (g->pixmap,
			GetPenRGB (0xffff, 0xffff, 0xffff),
			TRUE,
			0, 0,
			d->allocation.width,
			d->allocation.height);

    /* --- Draw molecule in the background --- */
/*    paint (g);*/
    update_ruler(d);

    /***************************************
      erst hier, dann auslagern ... 
    ****************************************/
    
    printf("repaint\n");
/*
//    printf(".. %d\n",cv);
    while(cv)
    {	
      c=cv->data;
//      printf("-\n");
      if(c->show==TRUE)
      {
	GList *val;
	printf("show it\n");

	lastx=-1;lasty=-1;x=-1;y=-1;
	val=c->points;
	val=g_list_first(val);
	while(val)
	{	
	  v=val->data;

	  if(lastx<0)
	  {
	    lastx=v->time;
	  }
	  else
	  {
	    x=v->time;
	  }
	  if(lasty<0)
	  {
	    lasty=v->value;
	  }
	  else
	  {
	    y=v->value;
	  }
	  if(x>0 && y>0)
	  {
*/

/*	    printf ("lastx %d %d %d\n",d->allocation.width-25,xmax,lastx);
	    printf ("    x %d %d %d\n",d->allocation.width-25,xmax,x);
	    printf ("lasty %d %d %d\n",d->allocation.height-25,ymax,lasty);
	    printf ("    y %d %d %d\n",d->allocation.height-25,ymax,y);

	    printf ("%d/%d - %d/%d\n",
		 (d->allocation.width-25)/xmax*lastx+25,
		 d->allocation.height-((d->allocation.height-25)*lasty/ymax)-25,
		 (d->allocation.width-25)*x/xmax+25,
		 d->allocation.height-((d->allocation.height-25)*y/ymax)-25);
*/

// es reicht den farbwert einmal nachzuschlagen ...
// auch die 25 sollte ausgelagert werden ...
// nun fehlt noch das rechteck ...
// wobei das 0er rechteck und evt. auch das max rechteck sich nur in der y-achse bewegen sollten ...
/*	    gdk_draw_line 
	    (g->pixmap, GetPenGdkColor (GetFormantListColor(c->formant)) ,
		 (d->allocation.width-25)*lastx/xmax+25,
		 d->allocation.height-((d->allocation.height-25)*lasty/ymax)-25,
		 (d->allocation.width-25)*x/xmax+25,
		 d->allocation.height-((d->allocation.height-25)*y/ymax)-25);
	    lastx=x;
	    lasty=y;
	  }
//	  printf("%5d / %5d\n",x,y);
	  val=val->next;
	}
      }
      cv=cv->next;
    }
*/

/* ****************************** */
    /* --- The whole screen --- */
    update_rect.x = 0;
    update_rect.y = 0;
    update_rect.width = d->allocation.width;
    update_rect.height = d->allocation.height;

    /* --- Call the expose event - which copies the background 
     *     into the widget
     */
    gtk_widget_draw (d, &update_rect);
}

void Redrawpixmap(GtkWidget *w, GdkEventExpose  *event)
{
  gdk_draw_pixmap(w->window,
            w->style->fg_gc[GTK_WIDGET_STATE (w)],
            g->pixmap,
            event->area.x, event->area.y,
            event->area.x, event->area.y,
            event->area.width, event->area.height);
}

/** 
  \todo repaint only actual drawable .. 
*/
void redraw_all_drawareas()
{
  GtkWidget *widget;

  widget=(GtkWidget *)lookup_widget (GTK_WIDGET (main_window), "draw_freq");
/*  configure_drawarea(widget);*/
  Repaint(widget);

  widget=(GtkWidget *)lookup_widget (GTK_WIDGET (main_window), "draw_amp");
  Repaint(widget);

  widget=(GtkWidget *)lookup_widget (GTK_WIDGET (main_window), "draw_band");
  Repaint(widget);
}


void SetStatusBar(char *sb, gchar *text)
{
  GtkStatusbar *s;
//  gchar *tmp;
//  guint context_id;

//   tmp = g_strdup_printf(text);
  
  s=(GtkStatusbar *) lookup_widget (GTK_WIDGET (GetMainWindow()), sb);
//  context_id = gtk_statusbar_get_context_id(
//                          GTK_STATUSBAR (s), "Statusbar example");

  gtk_statusbar_pop (s, 0);

  gtk_statusbar_push (s, 0, text);

  printf("%s -> %s\n",sb,text);

//  g_free(tmp);
}
