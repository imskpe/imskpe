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
 * \todo maybe put part of functions in other files 
 *       (formant-routines, math-stuff, statusbar, ...)
 * \todo subgrouping like in callbacks.c
 *
 * \todo add dynamic scale ... zooming?
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib/gprintf.h>
#include <string.h>
#include <malloc.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "curves.h"
#include "graphics.h"
#include "cfg.h"
#include "loadfile.h"

/*
  global variables
*/
GtkWidget *main_window;
typGraphics *g = NULL;

int mousepressed=0;

/* ---------------------------------------------------------------------- */

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
  if(GetMainWindow())
  {
    GtkWidget *w = lookup_widget (GTK_WIDGET (GetMainWindow()), "spbn_duration");
    gtk_spin_button_set_value ((GtkSpinButton *)w,
			       (float)x);
  }
}

void GuiSetUpdateInterval(unsigned int x)
{
  if(GetMainWindow())
  {
    GtkWidget *w = lookup_widget (GTK_WIDGET (GetMainWindow()), "spbn_ui");
    gtk_spin_button_set_value ((GtkSpinButton *)w,
			       (float)x);
  }
}

void GuiSetSamplingRate(unsigned int x)
{
  if(GetMainWindow())
  {
    GtkWidget *w = lookup_widget (GTK_WIDGET (GetMainWindow()), "spbn_sprate");
    gtk_spin_button_set_value ((GtkSpinButton *)w,
			       (float)x);
  }
}

void GuiSetNumberFormants(unsigned int x)
{
  if(GetMainWindow())
  {
    GtkWidget *w = lookup_widget (GTK_WIDGET (GetMainWindow()), "spbn_numF");
    gtk_spin_button_set_value ((GtkSpinButton *)w,
			       (float)x);
  }
}

void GuiSetVoiceSource(unsigned int x)
{
  if(GetMainWindow())
  {
    GtkWidget *w = lookup_widget (GTK_WIDGET (GetMainWindow()), "cm_vs_entry");

    switch(x)
    {
    case 1:
	gtk_entry_set_text ((GtkEntry *)w,_("impulse"));
	break;
    case 2:
	gtk_entry_set_text ((GtkEntry *)w,_("natural"));
	break;
    case 3:
	gtk_entry_set_text ((GtkEntry *)w,_("sampled"));
	break;
    }
  }
}

void GuiSetBranches(unsigned int x)
{
  if(GetMainWindow())
  {
    GtkWidget *w = lookup_widget (GTK_WIDGET (GetMainWindow()), "cm_cp_entry");

    switch(x)
    {
    case 1:
	gtk_entry_set_text ((GtkEntry *)w,_("cascade + parallel"));
	break;
    case 2:
	gtk_entry_set_text ((GtkEntry *)w,_("parallel only"));
	break;
    }
  }
}

gboolean GuiGetToggleButtonState(char tmp[30])
{
  GtkWidget *w;

  if(GetMainWindow())
  {
    /* MainWindow isn't initialized yet!  */
    w = lookup_widget (GTK_WIDGET (GetMainWindow()), tmp);
    
    if(w!=NULL)
    {
      return gtk_toggle_button_get_active ((GtkToggleButton *)w);
    }
    else
    {
      printf("GuiGetToggleButtonState: %s not found\n",tmp);
      return FALSE;
    }
  }
  return FALSE;
}

void GuiSetToolbarStyle(int style)
{
  GtkWidget *w;

  w = lookup_widget (GTK_WIDGET (GetMainWindow()), "toolbar1");
  gtk_toolbar_set_style ((GtkToolbar *)w,style);

  w = lookup_widget (GTK_WIDGET (GetMainWindow()), "toolbar2");
  gtk_toolbar_set_style ((GtkToolbar *)w,style);

  w = lookup_widget (GTK_WIDGET (GetMainWindow()), "toolbar4");
  gtk_toolbar_set_style ((GtkToolbar *)w,style);
}



/* ------------------------------------------------ */


int CalcRealX(int dx, int maxx)
{
  int du=FileGetDuration();
  int rulerdiff=25;

  int ox=(int)((double)((dx-rulerdiff)*du)/(double)(maxx-rulerdiff-10));

  return ox;
}

int CalcRealY(int dy, int maxy,diagramTyp dia)
{
//  int du=FileGetDuration();
  int rulerdiff=25;
  int max=5000;
  int oy;

  switch(dia)
  {
  case FREQUENCIES:
	max=ConfigGetInteger("maxfreq");
	break;
  case AMPLITUDE:
  	max=ConfigGetInteger("maxamp");
	break;
  case BANDWIDTH:
  	max=ConfigGetInteger("maxband");
	break;
  }    

  oy=(int)((double)((-dy+maxy-25)*max)/(double)(maxy-rulerdiff));

  return oy;
}


/** 
 * draw ruler
 * 
 * @param widget 
 * @param dia 
 */
void update_ruler(GtkWidget *widget, diagramTyp dia)
{
  gint duration;
  int i;
  PangoLayout *layout;
  PangoFontDescription *fontdesc;
  gchar *x;
  char tmp[100];

  int xsplits=15;  /* export in preferences */
  int ysplits=10;  /* export in preferences */

  int ymax=5000;   

  int mod=5;  /* -> preferences  | modificator for rulernumbers (x-axis)*/

  switch(dia)
  {
  case FREQUENCIES:
	ymax=ConfigGetInteger("maxfreq");
	break;
  case AMPLITUDE:
  	ymax=ConfigGetInteger("maxamp");
	break;
  case BANDWIDTH:
  	ymax=ConfigGetInteger("maxband");
	break;
  }    


  duration = FileGetDuration();

  x = g_malloc (sizeof (gchar)*10);
  
  strcpy(tmp,filtertoken(ConfigGetString("rulerfont"),"\""));
  fontdesc = pango_font_description_from_string (tmp);  

/* x-axis */
  gdk_draw_line (g->pixmap, GetPenRGB (NULL, 0, 0, 0) ,
		 20, 
		 widget->allocation.height-25,
		 widget->allocation.width, 
		 widget->allocation.height-25);

  for(i=0;i<=xsplits;i++)
  {
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
    gdk_draw_layout (g->pixmap,GetPenRGB(NULL, 0xffff,0,0),
		     ((widget->allocation.width-25-10)*i/xsplits)+25-mod, 
		     widget->allocation.height-15, layout);

    gdk_draw_line (g->pixmap, GetPenRGB (NULL, 0, 0, 0xffff) ,
		   ((widget->allocation.width-25-10)*i/xsplits)+25, 
		   widget->allocation.height-30,
		   ((widget->allocation.width-25-10)*i/xsplits)+25, 
		   widget->allocation.height-20);
    
  }

/* y-axis */
  gdk_draw_line (g->pixmap, GetPenRGB (NULL, 0, 0, 0) ,
		 25, 
		 0,
		 25, 
		 widget->allocation.height-20);

  for(i=0;i<=ysplits;i++)
  {
    sprintf(x,"%d",(ysplits-i)*(ymax)/ysplits);  

    layout = gtk_widget_create_pango_layout (widget, x);

    pango_layout_set_font_description (layout, fontdesc); 
    gdk_draw_layout (g->pixmap,GetPenRGB(NULL, 0xffff,0,0),
		     0, 
		     ((widget->allocation.height-25)/ysplits)*i, layout);

    gdk_draw_line (g->pixmap, GetPenRGB (NULL, 0, 0, 0xffff) ,
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

void configure_drawarea(GtkWidget *widget, diagramTyp dia)
{
  main_window = lookup_widget (GTK_WIDGET (widget), "imskpe_main");

  /* --- Structure doesn't exist? --- */
  if (g == NULL) {
    
    /* --- Create one --- */
    g = NewGraphics ();
  }

  /* --- Existing pixmap?  --- */
   if (g->pixmap != NULL) {
    
     /* --- Free it --- */
     gdk_pixmap_unref (g->pixmap);
   } 

  /* --- Create a new pixmap --- */
  g->pixmap = gdk_pixmap_new (widget->window,
			      widget->allocation.width,
			      widget->allocation.height,
			      -1);

  /* --- Create a new pixmap --- */
  gdk_draw_rectangle (g->pixmap,
		      widget->style->white_gc,
		      TRUE,
		      0, 0,
		      widget->allocation.width,
		      widget->allocation.height);

  Repaint(widget,dia);
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

/** 
 * Get a GdkGC (pen) based on the colors passed 
 * in.  This is used to change the color of the 
 * items being drawn.
 * 
 * @param gc 
 * @param nRed 
 * @param nGreen 
 * @param nBlue 
 * 
 * @return 
 */
GdkGC *GetPenRGB (GdkGC *gc, int nRed, int nGreen, int nBlue)
{
    GdkColor *c;

    c = (GdkColor *) g_malloc (sizeof (GdkColor));
    c->red = nRed;
    c->green = nGreen;
    c->blue = nBlue;

    gdk_color_alloc (gdk_colormap_get_system (), c);

    gc = gdk_gc_new (g->pixmap);

    gdk_gc_set_foreground (gc, c);

    return (gc);
}

/** 
 * used to add GdkColor as foreground-color to an GdkGC
 * 
 * @param gc 
 * @param col 
 * 
 * @return 
 */
GdkGC *GetPenGdkColor (GdkGC *gc, GdkColor col)
{
    GdkColor *c;

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
 * \todo 
 * - calculation of virtualcoordinates in extra function
 * - split in a few functions
 * - put more values in preferences
 * 
 * @param d 
 * @param dia 
 */
void Repaint(GtkWidget *d, diagramTyp dia)
{
    GdkRectangle  update_rect;
    typCurveList *c;
    int lastx=-1,lasty=-1;
    int x=-1,y=-1;
    typValueList *v;

    GList *cv=(GList *)FileGetCurvesPointer();

/* muss alles ausgelagert werden: */
//    int xsplits=15;  /**< export in preferences */
//    int ysplits=10;  /**< export in preferences */
    int ymax=5000;

    int xmax = FileGetDuration();

    int ui=FileGetUpdateInterval();
    char statusbarcurvemessage[100];
    gboolean statusshown=FALSE;
    GList *val;
    int linewidth=1;
    int mod;
    char s[80];

    GdkGC *gc;
    
    strcpy(statusbarcurvemessage,"\0");

    switch(dia)
    {
    case FREQUENCIES:
	ymax=ConfigGetInteger("maxfreq");
	break;
    case AMPLITUDE:
  	ymax=ConfigGetInteger("maxamp");
	break;
    case BANDWIDTH:
  	ymax=ConfigGetInteger("maxband");
	break;
    }    

    /* --- clear pixmap --- */
    gdk_draw_rectangle (g->pixmap,
			GetPenRGB (NULL, 0xffff, 0xffff, 0xffff),
			TRUE,
			0, 0,
			d->allocation.width,
			d->allocation.height);

/*    paint (g);*/
    update_ruler(d, dia);

    /***************************************
    ****************************************/
    
    while(cv)
    {	
      c=cv->data;
      if(c->show==TRUE && c->dia==dia)
      {
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
	  if(x>=0 && y>=0)
	  {
	    linewidth=1;
	    if(MouseEventCheckCurve(c->nr)) {
	      linewidth=2;

	      strncpy(statusbarcurvemessage,strstr(c->widget_name,"_"),strlen(strstr(c->widget_name,"_"))+1);
	      statusbarcurvemessage[0]=' '; /** cheat!! */
	      
	    }
	    gc=GetPenGdkColor (NULL,ConfigGetColor(c->formant));
	    gdk_gc_set_line_attributes (gc,linewidth,GDK_LINE_SOLID,GDK_CAP_NOT_LAST,GDK_JOIN_MITER );

	    gdk_draw_line 
		(g->pixmap, gc ,
		 (d->allocation.width-25-10)*lastx/xmax+25,
		 d->allocation.height-((d->allocation.height-25)*lasty/ymax)-25,
		 (d->allocation.width-25-10)*x/xmax+25,
		 d->allocation.height-((d->allocation.height-25)*y/ymax)-25);
	    
	    mod=2;
	    if(MouseEventCheckPoint(lastx,c->nr))
	    {

	      mod=3;
	      snprintf(s,15," (%5d/%5d)",lastx,lasty);
	      strncat(statusbarcurvemessage,s,strlen(s)+1);
	      SetStatusBar ("sb_curve",statusbarcurvemessage);
	      statusshown=TRUE;
	    }

	    gdk_draw_rectangle
		(g->pixmap,
		 GetPenGdkColor (NULL,ConfigGetColor(c->formant)),
		 FALSE,
		 ((d->allocation.width-25-10)*lastx/xmax+25)-mod,
		 (d->allocation.height-((d->allocation.height-25)*lasty/ymax)-25)-mod,
		 mod*2+1,
		 mod*2+1);

 	    if(x>=xmax-ui)
 	    {
	      if(MouseEventCheckPoint(x,c->nr))
	      {
		mod=3;
		snprintf(s,15," (%5d/%5d)",x,y);
		strncat(statusbarcurvemessage,s,strlen(s)+1);
		SetStatusBar ("sb_curve",statusbarcurvemessage);
		statusshown=TRUE;
	      }
	      else
	      {
		mod=2;
	      }
 	      gdk_draw_rectangle
 		  (g->pixmap,
 		   GetPenGdkColor (NULL,ConfigGetColor(c->formant)),
 		   FALSE,
 		   ((d->allocation.width-25-10)*x/xmax+25)-mod,
 		   (d->allocation.height-((d->allocation.height-25)*y/ymax)-25)-mod,
 		   mod*2+1,
 		   mod*2+1);
 	    }

	    lastx=x;
	    lasty=y;
	  }
	  val=val->next;
	}
      }
      cv=cv->next;
    }
    if(statusshown==FALSE)
    {
      SetStatusBar ("sb_curve",statusbarcurvemessage);
    }

    update_rect.x = 0;
    update_rect.y = 0;
    update_rect.width = d->allocation.width;
    update_rect.height = d->allocation.height;

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

void configure_page(int page)
{
  GtkWidget *widget;

  switch(page)
  {
  case 0:
      widget=(GtkWidget *)lookup_widget (GTK_WIDGET ((GtkWidget *)GetMainWindow()), "draw_freq");
      configure_drawarea(widget, 1);
      break;
  case 1:
      widget=(GtkWidget *)lookup_widget (GTK_WIDGET ((GtkWidget *)GetMainWindow()), "draw_amp");
      configure_drawarea(widget, 2);
      break;
  case 2:
      widget=(GtkWidget *)lookup_widget (GTK_WIDGET ((GtkWidget *)GetMainWindow()), "draw_band");
      configure_drawarea(widget, 3);
      break;
  default:
      printf("nb-error, tab out of range?!\n");
  }
}

void redraw_page(int page)
{
  GtkWidget *widget;

  switch(page)
  {
  case 0:
      widget=(GtkWidget *)lookup_widget (GTK_WIDGET ((GtkWidget *)GetMainWindow()), "draw_freq");
      Repaint(widget, 1);
      break;
  case 1:
      widget=(GtkWidget *)lookup_widget (GTK_WIDGET ((GtkWidget *)GetMainWindow()), "draw_amp");
      Repaint(widget, 2);
      break;
  case 2:
      widget=(GtkWidget *)lookup_widget (GTK_WIDGET ((GtkWidget *)GetMainWindow()), "draw_band");
      Repaint(widget, 3);
      break;
  default:
      printf("nb-error, tab out of range?!\n");
  }
}

void SetStatusBar(char *sb, gchar *text)
{
  GtkStatusbar *s;
  
  s=(GtkStatusbar *) lookup_widget (GTK_WIDGET (GetMainWindow()), sb);

  gtk_statusbar_pop (s, 0);
  gtk_statusbar_push (s, 0, text);
}

void SetTitle(gchar *text)
{
  GtkWindow *w;
  gchar tmp[200];

  snprintf(tmp,200,"IMSKPE - %s",text);
  
  w=(GtkWindow *) GetMainWindow();

  gtk_window_set_title (w,tmp);

}

void SetToggleButton(MouseActionTyp typ)
{
  switch(typ)
  {
  case MOVE:
      gtk_toggle_tool_button_set_active((GtkToggleToolButton*)lookup_widget (GTK_WIDGET (GetMainWindow()), "bn_move"),TRUE);
      gtk_toggle_tool_button_set_active((GtkToggleToolButton*)lookup_widget (GTK_WIDGET (GetMainWindow()), "bn_insert"),FALSE);
      gtk_toggle_tool_button_set_active((GtkToggleToolButton*)lookup_widget (GTK_WIDGET (GetMainWindow()), "bn_delete"),FALSE);
      MouseEventSetAction(MOVE);
      break;
  case INSERT:
      gtk_toggle_tool_button_set_active((GtkToggleToolButton*)lookup_widget (GTK_WIDGET (GetMainWindow()), "bn_move"),FALSE);
      gtk_toggle_tool_button_set_active((GtkToggleToolButton*)lookup_widget (GTK_WIDGET (GetMainWindow()), "bn_insert"),TRUE);
      gtk_toggle_tool_button_set_active((GtkToggleToolButton*)lookup_widget (GTK_WIDGET (GetMainWindow()), "bn_delete"),FALSE);
      MouseEventSetAction(INSERT);
      break;
  case DELETE:
      gtk_toggle_tool_button_set_active((GtkToggleToolButton*)lookup_widget (GTK_WIDGET (GetMainWindow()), "bn_insert"),FALSE);
      gtk_toggle_tool_button_set_active((GtkToggleToolButton*)lookup_widget (GTK_WIDGET (GetMainWindow()), "bn_move"),FALSE);
      gtk_toggle_tool_button_set_active((GtkToggleToolButton*)lookup_widget (GTK_WIDGET (GetMainWindow()), "bn_delete"),TRUE);
      MouseEventSetAction(DELETE);
      break;
  }
}

/** 
 * draw values on motion (incl. hovering ...)
 * 
 * @param rx 
 * @param ry 
 * @param state 
 * @param dia 
 */
void DrawAreaMotion(int rx, int ry,   GdkModifierType state, diagramTyp dia)
{
  GList *cv=(GList *)FileGetCurvesPointer();
  typCurveList *c;
  int setp=0;
  int setc=0;
  int ui=FileGetUpdateInterval();
  int tmp;

  int ymax=5000;

//  int lastpoint=MouseEventGetPoint();
  int lastcurve=MouseEventGetCurve();
  
  GList *val;
  typValueList p_pnt;
  typValueList pp_pnt;
  typValueList pnt;
  typValueList n_pnt;
  typValueList *v;
  typValueList *v2;

  double grad;
  int yval;
  int ytol;

  switch(dia)
  {
  case FREQUENCIES:
	ymax=ConfigGetInteger("maxfreq");
	break;
  case AMPLITUDE:
  	ymax=ConfigGetInteger("maxamp");
	break;
  case BANDWIDTH:
  	ymax=ConfigGetInteger("maxband");
	break;
  }    

  while(cv)
  {	
    c=cv->data;
    if(c->show==TRUE && c->dia==dia)
    {
      pp_pnt.time=-1;
      p_pnt.time=-1;
      pnt.time=-1;
      n_pnt.time=-1;
      
      val=c->points;
      val=g_list_first(val);
      while(val)
      {	
	v=val->data;
	if(pp_pnt.time<0) {
	  if(p_pnt.time<0) {
	    if(pnt.time<0) {
	      pnt.time=v->time;
	      pnt.value=v->value;
	    }
	    else {
	      p_pnt.time=pnt.time;
	      p_pnt.value=pnt.value;
	      pnt.time=v->time;
	      pnt.value=v->value;
	    }
	  }
	  else {
	    pp_pnt.time=p_pnt.time;
	    pp_pnt.value=p_pnt.value;
	    p_pnt.time=pnt.time;
	    p_pnt.value=pnt.value;
	    pnt.time=v->time;
	    pnt.value=v->value;
	  }
	}
	else {
	  pp_pnt.time=p_pnt.time;
	  pp_pnt.value=p_pnt.value;
	  p_pnt.time=pnt.time;
	  p_pnt.value=pnt.value;
	  pnt.time=v->time;
	  pnt.value=v->value;
	}
	

	if(val->next!=NULL)
	{
	  v2=val->next->data;
	  n_pnt.time=v2->time;
	}
	
	if(state & GDK_BUTTON1_MASK)
	{
	  if(MouseEventGetAction()==MOVE && pnt.time==MouseEventGetPoint() && c->nr==MouseEventGetCurve())
	  {
	    tmp=PointMove(CurveSearchByNr(FileGetCurvesPointer(),c->nr),pnt.time,rx,ry);
	    MouseEventSetPoint(tmp,c->nr);
	    setp++;
	    
	    redraw_page(dia-1);
	  }
	}
	else
	{
	  if(rx>p_pnt.time && rx<=n_pnt.time) {

	    grad=((double)(pnt.value-p_pnt.value)/(double)(pnt.time-p_pnt.time))*(double)(rx-p_pnt.time);
	    yval = (int)((double)p_pnt.value+(grad));
	    
	    ytol=(ymax/100);
	    if(ytol<10)
	    ytol=10;
	    
	    if(yval>ry-ytol && yval<ry+ytol)
	    {
	      if(MouseEventSetCurve(c->nr)) {
	      redraw_page(dia-1);
	      }
	      setc++;
	    }
	    
	    if((pnt.value>ry-ytol && pnt.value<ry+ytol) && (rx>pnt.time-(ui-1) && rx<pnt.time+(ui+1)))
	    {
	      if(MouseEventSetCurve(c->nr)) {
		redraw_page(dia-1);
	      }
	      setc++;
	      
	      if(MouseEventSetPoint(pnt.time,c->nr)) {
		redraw_page(dia-1);
	      }
	      setp++;
	    }
	  }
	}
	  

	val=val->next;
      }
    }
    cv=cv->next;
  }
/* ?? solve redrawproblem / check for mouse not overline or not over point */
  if(lastcurve!=-1 && setc==0 && !(state & GDK_BUTTON1_MASK))
  {
    if(MouseEventSetCurve(-1)) {
      redraw_page(dia-1);
    }
  }
  if(setp==0 && !(state & GDK_BUTTON1_MASK))
  {
    if(MouseEventSetPoint(-1,-1)) {
    }
  }
  
  if(setp>0 && !(state & GDK_BUTTON1_MASK))
  {
    redraw_page(dia-1);
  }
}


void DrawButtonPressed(int rx, int ry, GdkEventButton  *event, diagramTyp dia)
{
  GtkWidget *pmenu;
  int point,curve;
  MouseActionTyp action;
  int x, y;
//  GdkModifierType state;
  GdkEventButton *bevent;

  if (event->type == GDK_BUTTON_PRESS && event->button==1) {
    point=MouseEventGetPoint();
    curve=MouseEventGetCurve();
    action=MouseEventGetAction();

    if(curve>0)
    {
      // if NOT over a point
      if(point==-1)
      {
	if(action==INSERT)
	{
	  x = event->x;
	  y = event->y;
	  
 	  PointInsert(CurveSearchByNr(FileGetCurvesPointer(),curve),rx,ry);
 	  redraw_page(dia-1);
	}
      }
      // if over a point
      else
      {
	if(action==DELETE)
	{
	  PointDelete(CurveSearchByNr(FileGetCurvesPointer(),curve),point);
	  redraw_page(dia-1);
	}
	if(action==MOVE)
	{
	  SetMousepressed(1);
	}
      }
    }
  }

  if (event->type == GDK_BUTTON_PRESS && event->button==2) {
    point=MouseEventGetPoint();
    curve=MouseEventGetCurve();
    action=MouseEventGetAction();
    if(curve>0)
    {
      // if NOT over a point
      if(point==-1)
      {
	x = event->x;
	y = event->y;
	  
	PointInsert(CurveSearchByNr(FileGetCurvesPointer(),curve),rx,ry);
	redraw_page(dia-1);
      }
    }
  }

  if (event->type == GDK_BUTTON_PRESS && event->button==3) {
    
    pmenu=create_pmenu();
    point=MouseEventGetPoint();
    if(point==-1)
    {
      gtk_widget_hide ((GtkWidget*)lookup_widget (GTK_WIDGET (pmenu), "pm_movediag"));
    }
    
    bevent = (GdkEventButton *) event;       
    gtk_menu_popup (GTK_MENU (pmenu), NULL, NULL, NULL, NULL,
		      bevent->button, bevent->time);
    return;
  }
}

int GetMousepressed()
{
  return mousepressed;
}

void SetMousepressed(int foo)
{
   mousepressed=foo;
}


void SetLabelColor(GtkWidget *w, char *cfgstr)
{
  GdkColor col;

  col = ConfigGetColor(cfgstr);
  gtk_widget_modify_fg (w, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (w, GTK_STATE_NORMAL, &col);
}


GtkWidget *DialogInfoOK(char *msg)
{
  return DialogOK(msg, GTK_MESSAGE_INFO);
}

GtkWidget *DialogErrorOK(char *msg)
{
  return DialogOK(msg, GTK_MESSAGE_ERROR);
}

GtkWidget *DialogOK (char *msg, GtkMessageType mt)
{
  GtkWidget *dialog;

  dialog = gtk_message_dialog_new (NULL, 0, mt, GTK_BUTTONS_OK,
				   "%s", msg);
  g_signal_connect (G_OBJECT (dialog), "response",
		    G_CALLBACK (gtk_widget_destroy), 0);
  gtk_window_set_resizable (GTK_WINDOW (dialog), FALSE);
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_MOUSE);
  gtk_widget_show (dialog);

  return dialog;
}


gboolean DialogYesNo (char *msg)
{
  GtkWidget *dialog;
  gint foo;

  dialog = gtk_message_dialog_new (NULL, 0, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
				   "%s", msg);

  foo=gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);   
  switch(foo)
  {
  case GTK_RESPONSE_YES:
    return TRUE;
  case GTK_RESPONSE_NO:
    return FALSE;
  }
  return FALSE;
}

/** 
 * filter tokens out of strings.
 * needs hardening! 
 * 
 * @param str 
 * @param token 
 * 
 * @return 
 */
char *filtertoken(char str[100], char token[1])
{
  char *result = NULL;
  char s[100]; // not good!

  strcpy(s,str);
  result = (char *)strtok( s, token ); 
  return result;
}


// maybe use a set.c ??
static void set_action_response(gchar *string)
{
  char *s;
  char tmp[100];

  s = strchr(string,' ');
  while(s != NULL)
  {
    s[0]='_';
    s = strchr(string,' ');
  }

  strcpy(tmp,"Set_");
  strcat(tmp,string);

  strcpy(tmp,ConfigGetString(tmp));

  SetCurveShowArray(tmp);
}


gboolean SetAddMenuItem(char *buf)
{
  GtkWidget *menu_items;  
  GtkWidget *menu;

  menu_items = gtk_menu_item_new_with_label (buf);
  menu=lookup_widget (GTK_WIDGET (GetMainWindow()), "sets1_menu");

  /* ...and add it to the menu. */
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_items);
  
  /* Do something interesting when the menuitem is selected */
   g_signal_connect_swapped (G_OBJECT (menu_items), "activate",
 			    G_CALLBACK (set_action_response), 
 			    (gpointer) g_strdup (buf));
  
  /* Show the widget */
  gtk_widget_show (menu_items);
  return TRUE;
}

/** 
 * initialize and set splashscreen!
 * 
 */
void InitSplash()
{
  static GtkWidget *splash = NULL;
  GtkWidget *w;
  char buf[512];

  if (splash == NULL) 
  {
    splash = create_imskpe_splash ();
    /* set the widget pointer to NULL when the widget is destroyed */
    g_signal_connect (G_OBJECT (splash),
		      "destroy",
		      G_CALLBACK (gtk_widget_destroyed),
		      &splash);


    w = lookup_widget (GTK_WIDGET (splash), "splash_label");    
    snprintf(buf,sizeof(buf),
	     "\n<span size=\"x-large\"><b>IMS-KPE %s</b></span>\n\n"
	     "\n\n"
	     "<b>%s</b>\n<span size=\"small\">%s</span>\n\n"
	     "<b>%s</b>\n<span size=\"small\">%s</span>\n\n"
	     ,VERSION,_("write bugreports here: "),"http://sourceforge.net/tracker/?group_id=94548",
	     _("get announcements:"),"http://lists.sourceforge.net/lists/listinfo/imskpe-announce"
	);
    gtk_label_set_markup (GTK_LABEL (w), buf);  

    /* Make sure the dialog doesn't disappear behind the main window. */
    gtk_window_set_transient_for (GTK_WINDOW (splash), 
				  GTK_WINDOW (GetMainWindow()));
  }

  /* Make sure the dialog is visible. */
  gtk_window_present (GTK_WINDOW (splash));
}
