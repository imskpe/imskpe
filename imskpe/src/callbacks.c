/*
    callbacks.c - Part of IMSKPE

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
 * @file   callbacks.c
 * @author Andreas Madsack
 * 
 * @brief  callback-functions
 * 
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

#include "cfg.h"
#include "loadfile.h"
#include "curves.h"
#include "graphics.h"

/*** procedures:

***/


/* ----------------------------------------------- */


void
on_new1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  CurveInitStart();
}


void
on_open1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
 static GtkWidget *fileopen = NULL;

  if (fileopen == NULL) 
    {
      fileopen = create_imskpe_file ();
      /* set the widget pointer to NULL when the widget is destroyed */
      g_signal_connect (G_OBJECT (fileopen),
			"destroy",
			G_CALLBACK (gtk_widget_destroyed),
			&fileopen);

      SetMainWindow(lookup_widget (GTK_WIDGET (menuitem), "imskpe_main"));
      /* Make sure the dialog doesn't disappear behind the main window. */
      gtk_window_set_transient_for (GTK_WINDOW (fileopen), 
				    GTK_WINDOW (GetMainWindow()));
    }

  /* Make sure the dialog is visible. */
  gtk_window_present (GTK_WINDOW (fileopen));
}


void
on_save1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_save_as1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
 static GtkWidget *about = NULL;
 GtkWidget *w;
 char buf[512];

  if (about == NULL) 
    {
      about = create_imskpe_about ();
      /* set the widget pointer to NULL when the widget is destroyed */
      g_signal_connect (G_OBJECT (about),
			"destroy",
			G_CALLBACK (gtk_widget_destroyed),
			&about);


      w = lookup_widget (GTK_WIDGET (about), "about_label");    
      /** is snprintf really portable? */
      snprintf(buf,sizeof(buf),
	       "\n<span size=\"x-large\"><b>IMS-KPE %s</b></span>\n\n"
	       "\n\n"
	       "<b>%s</b>: %s\n\n"
	       "<small>\302\251 2004 Andreas Madsack &lt;bolsog@users.sf.net></small>",VERSION,_("Compiled"),__DATE__
	  );
      gtk_label_set_markup (GTK_LABEL (w), buf);  

      SetMainWindow(lookup_widget (GTK_WIDGET (menuitem), "imskpe_main"));
      /* Make sure the dialog doesn't disappear behind the main window. */
      gtk_window_set_transient_for (GTK_WINDOW (about), 
				    GTK_WINDOW (GetMainWindow()));
    }

  /* Make sure the dialog is visible. */
  gtk_window_present (GTK_WINDOW (about));
}


/** 
 * on_draw_freq_configure_event
 *
 * look which notebook is selected and configure only this one!
 * except first start, when MainWindow isn't set
 * 
 * @param widget 
 * @param event 
 * @param user_data 
 * 
 * @return 
 */
gboolean
on_draw_freq_configure_event           (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data)
{
  GtkWidget *w=(GtkWidget *)GetMainWindow();

  if(w!=NULL)  
  {
    w=(GtkWidget *)lookup_widget (GTK_WIDGET (w), "nb_draw");
    configure_page(gtk_notebook_get_current_page((GtkNotebook *)w));
  }
  else
  {
    configure_drawarea(widget, 1);
  }

  return TRUE;
}

/** 
 * on_draw_amp_configure_event
 *
 * look which notebook is selected and configure only this one!
 * except first start, when MainWindow isn't set
 * 
 * @param widget 
 * @param event 
 * @param user_data 
 * 
 * @return 
 */
gboolean
on_draw_amp_configure_event            (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data)
{
  GtkWidget *w=(GtkWidget *)GetMainWindow();

  if(w!=NULL)  
  {
    w=(GtkWidget *)lookup_widget (GTK_WIDGET (w), "nb_draw");
    configure_page(gtk_notebook_get_current_page((GtkNotebook *)w));
  }
  else
  {
    configure_drawarea(widget, 2);
  }

  return TRUE;
}


/** 
 * on_draw_band_configure_event
 *
 * look which notebook is selected and configure only this one!
 * except first start, when MainWindow isn't set
 * 
 * @param widget 
 * @param event 
 * @param user_data 
 * 
 * @return 
 */
gboolean
on_draw_band_configure_event           (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data)
{
  GtkWidget *w=(GtkWidget *)GetMainWindow();

  if(w!=NULL)  
  {
    w=(GtkWidget *)lookup_widget (GTK_WIDGET (w), "nb_draw");
    configure_page(gtk_notebook_get_current_page((GtkNotebook *)w));
  }
  else
  {
    configure_drawarea(widget, 3);
  }

  return TRUE;
}



gboolean
on_draw_freq_expose_event              (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
  Redrawpixmap(widget,event);

  return TRUE;
}

gboolean
on_draw_amp_expose_event               (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
/*  gdk_draw_pixmap(widget->window,
            widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
            g->pixmap,
            event->area.x, event->area.y,
            event->area.x, event->area.y,
            event->area.width, event->area.height);
*/
  Redrawpixmap(widget,event);

  return TRUE;
}




gboolean
on_draw_band_expose_event              (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
/*  gdk_draw_pixmap(widget->window,
            widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
            g->pixmap,
            event->area.x, event->area.y,
            event->area.x, event->area.y,
            event->area.width, event->area.height);
*/
  Redrawpixmap(widget,event);

  return TRUE;
}


gboolean
on_draw_freq_motion_notify_event       (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data)
{
  int x, y;
  int rx,ry;
  GdkModifierType state;
  char tmp[80];


  if (event->is_hint)
    gdk_window_get_pointer (event->window, &x, &y, &state);
  else
    {
      x = event->x;
      y = event->y;
      state = event->state;
    }
    
  rx=CalcRealX(x, widget->allocation.width);  
  ry=CalcRealY(y, widget->allocation.height);  
  snprintf(tmp,30,"%d ms - %d Hz ",rx,ry);
  SetStatusBar("sb_add",tmp);

//   Repaint(widget, 1);

/* testarea :)
 */
//   printf("s----------\n");
  
  // get segment
    GList *cv=(GList *)FileGetCurvesPointer();
    typCurveList *c;
    int dia=1;
    int setp=0;
    int setc=0;

    int lastpoint=MouseEventGetPoint();
    int lastcurve=MouseEventGetCurve();

    while(cv)
    {	
      c=cv->data;
      if(c->show==TRUE && c->dia==dia)
      {
	GList *val;
	typValueList p_pnt;
	typValueList pp_pnt;
	typValueList pnt;
	typValueList n_pnt;
	typValueList *v;
    
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

	  if(rx>p_pnt.time && rx<pnt.time) {
	    // berechnen des y-wertes

// 	    printf("time pp: %5d  p: %5d  %5d  pos: %5d \n",pp_pnt.time,p_pnt.time,pnt.time,rx);
// 	    printf("val  pp: %5d  p: %5d  %5d  pos: %5d \n",pp_pnt.value,p_pnt.value,pnt.value,ry);
// 	    printf(" | %3.2f %3.2f %5d"
// 		   ,((double)(pnt.value-p_pnt.value)/(double)(pnt.time-p_pnt.time))
// 		   ,
// 		   ,p_pnt.value
// 		);
	    double grad=((double)(pnt.value-p_pnt.value)/(double)(pnt.time-p_pnt.time))*(double)(rx-p_pnt.time);
	    int yval = (int)((double)p_pnt.value+(grad));
//  	    printf("yv: %5d  g: %5.0f | x:%5d / y:%5d\n",yval,grad,ry,rx);

	    if(yval>ry-(50) && yval<ry+(50)) // +abs((int)grad))
	    {
	      if(MouseEventSetCurve(c->nr)) {
// 		printf("--r--\n");
		redraw_page(dia-1);
	      }
	      setc++;
	    }
	    if((pnt.value>ry-40 && pnt.value<ry+40) && (rx>pnt.time-20 && rx<pnt.time+20))
	    {
	      if(state & GDK_BUTTON1_MASK)
	      {
		if(MouseEventGetAction()==MOVE)
		{
		  PointMove(CurveSearchByNr(FileGetCurvesPointer(),c->nr),pnt.time,rx,ry);
		}
	      }


// ??? start
	      if(MouseEventSetCurve(c->nr)) {
		redraw_page(dia-1);
	      }
	      setc++;
// ??? end

	      if(MouseEventSetPoint(pnt.time,c->nr)) {
// 		printf("--r--\n");
		redraw_page(dia-1);
	      }
	      setp++;
	    }
	    
// 	    printf("\n");
	  }
	  

	  val=val->next;
	}
      }
      cv=cv->next;
    }
/** \todo solve redrawproblem / check for mouse not overline or not over point */
    if(lastcurve!=-1 && setc==0)
    {
      if(MouseEventSetCurve(-1)) {
//  	printf("c ");
 	redraw_page(dia-1);
      }
    }
// MouseEventCheckPoint(lastpoint,lastcurve) && 
    if(setp==0)
    {
      if(MouseEventSetPoint(-1,-1)) {
//  	printf("p ");
	redraw_page(dia-1);
      }
    }
    
//     printf("e---------- %2d\n",set);

  return TRUE;
}

gboolean
on_draw_freq_button_press_event        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  GtkWidget *pmenu;
  int point,curve;
  MouseActionTyp action;
  int dia=1;

  if (event->type == GDK_BUTTON_PRESS && event->button==1) {
    point=MouseEventGetPoint();
    curve=MouseEventGetCurve();
    action=MouseEventGetAction();

    if(curve>0)
    {
      if(point==-1)
      {
	// only insert available:
	if(action==INSERT)
	{
	  int x, y;
	  int rx,ry;
	  GdkModifierType state;

// 	  if (event->is_hint)
// 	  gdk_window_get_pointer (event->window, &x, &y, &state);
// 	  else
// 	  {
	    x = event->x;
	    y = event->y;
// 	    state = event->state;
// 	  }
	  
	  rx=CalcRealX(x, widget->allocation.width);  
	  ry=CalcRealY(y, widget->allocation.height);  
 	  PointInsert(CurveSearchByNr(FileGetCurvesPointer(),curve),rx,ry);
 	  redraw_page(dia-1);
// 	  printf("%d / %d\n",rx,ry);
// 	  printf("insert\n");
	}
      }
      else
      {
	if(action==DELETE)
	{
	  PointDelete(CurveSearchByNr(FileGetCurvesPointer(),curve),point);
	  redraw_page(dia-1);
// 	  printf("delete\n");
	}
	if(action==MOVE)
	{
	  printf("move\n");
	}
      }
    }
  }

  if (event->type == GDK_BUTTON_PRESS && event->button==3) {
    
    pmenu=create_pmenu();
//       if(point==-1)
//       {
// 	gtk_widget_hide ((GtkWidget*)lookup_widget (GTK_WIDGET (pmenu), "pm_delete"));
// 	gtk_widget_hide ((GtkWidget*)lookup_widget (GTK_WIDGET (pmenu), "pm_move"));
//       }
//       else
//       {
// 	gtk_widget_hide ((GtkWidget*)lookup_widget (GTK_WIDGET (pmenu), "pm_insert"));
//       }

    GdkEventButton *bevent = (GdkEventButton *) event;       
    gtk_menu_popup (GTK_MENU (pmenu), NULL, NULL, NULL, NULL,
		      bevent->button, bevent->time);
    return TRUE;
  }

  return TRUE;
}


gboolean
on_draw_amp_motion_notify_event        (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data)
{
  int x, y;
  int rx,ry;
  char tmp[80];
  GdkModifierType state;

  if (event->is_hint)
    gdk_window_get_pointer (event->window, &x, &y, &state);
  else
    {
      x = event->x;
      y = event->y;
      state = event->state;
    }
    
  rx=CalcRealX(x, widget->allocation.width);  
  ry=CalcRealY(y, widget->allocation.height);  
  snprintf(tmp,30,"%d ms - %d dB ",rx,ry);
  SetStatusBar("sb_add",tmp);
  return TRUE;
}

gboolean
on_draw_band_motion_notify_event       (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data)
{
  int x, y;
  int rx,ry;
  char tmp[80];
  GdkModifierType state;

  if (event->is_hint)
    gdk_window_get_pointer (event->window, &x, &y, &state);
  else
    {
      x = event->x;
      y = event->y;
      state = event->state;
    }
    
  rx=CalcRealX(x, widget->allocation.width);  
  ry=CalcRealY(y, widget->allocation.height);  
  snprintf(tmp,30,"%d ms - %d Hz ",rx,ry);
  SetStatusBar("sb_add",tmp);
  return TRUE;
}

gboolean
on_imskpe_main_delete_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
}

gboolean
on_color_selection1_configure_event    (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data)
{

void gtk_color_selection_set_color( GtkColorSelection *colorsel,
                                    gdouble           *color );

  return FALSE;
}


void
on_color_selection1_color_changed      (GtkColorSelection *colorselection,
                                        gpointer         user_data)
{
  GtkWidget *mainwindow = GetMainWindow();
  if(mainwindow != NULL)
  {
    gdouble color[3];
    GdkColor gdk_color;
  
    GtkNotebook *nb=(GtkNotebook*) lookup_widget (GTK_WIDGET (mainwindow), "nb_resonators");
    GtkWidget *child=gtk_notebook_get_nth_page(nb, gtk_notebook_get_current_page(nb));
    GtkWidget *widget=gtk_notebook_get_tab_label(nb,child);
    GtkWidget *w= lookup_widget (GTK_WIDGET (mainwindow), "lb_f1");    

/* Get current color */
    
    gtk_color_selection_get_color (colorselection,color);
    
  /* Fit to a unsigned 16 bit integer (0..65535) and insert into the GdkColor structure */
    gdk_color=GetColor(color[0],color[1],color[2]);


/*    g_printf("%d / %d / %d\n",gdk_color.red,gdk_color.green,gdk_color.blue);
      g_printf("%d\n",gtk_notebook_get_current_page(nb));
*/
    
/*    g_printf("%d ?= %d\n",&widget,&w); */

    gtk_widget_modify_fg (widget, GTK_STATE_ACTIVE, &gdk_color);
    gtk_widget_modify_fg (widget, GTK_STATE_NORMAL, &gdk_color);
  }
}


void
on_ok_button1_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}


void
on_cancel_button1_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}



gboolean
on_imskpe_colorsel_destroy_event       (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  gtk_widget_destroy (widget);
  return TRUE;
}



void
on_bn_fX_color_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  static GtkWidget *color = NULL;

  if (color == NULL) 
    {
      color = create_imskpe_colorsel ();
      /* set the widget pointer to NULL when the widget is destroyed */
      g_signal_connect (G_OBJECT (color),
			"destroy",
			G_CALLBACK (gtk_widget_destroyed),
			&color);

      SetMainWindow(lookup_widget (GTK_WIDGET (button), "imskpe_main"));
      /* Make sure the dialog doesn't disappear behind the main window. */
      gtk_window_set_transient_for (GTK_WINDOW (color), 
				    GTK_WINDOW ((GtkWidget *)GetMainWindow()));
      /* Do not allow user to resize the dialog */
      gtk_window_set_resizable (GTK_WINDOW (color), FALSE);
    }

  /* Make sure the dialog is visible. */
  gtk_window_present (GTK_WINDOW (color));
}


void
on_spbn_numF_changed                   (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  gint val=gtk_spin_button_get_value_as_int(spinbutton);
  GtkWidget *w;

  if(val<6)
  {
    w = lookup_widget (GTK_WIDGET (spinbutton), "tbl_f6");
    gtk_widget_hide (w);
  }
  else
  {
    w = lookup_widget (GTK_WIDGET (spinbutton), "tbl_f6");
    gtk_widget_show (w);
  }

  if(val<5)
  {
    w = lookup_widget (GTK_WIDGET (spinbutton), "tbl_f5");
    gtk_widget_hide (w);
  }
  else
  {
    w = lookup_widget (GTK_WIDGET (spinbutton), "tbl_f5");
    gtk_widget_show (w);
  }

  if(val<4)
  {
    w = lookup_widget (GTK_WIDGET (spinbutton), "tbl_f4");
    gtk_widget_hide (w);
  }
  else
  {
    w = lookup_widget (GTK_WIDGET (spinbutton), "tbl_f4");
    gtk_widget_show (w);
  }

  if(val<3)
  {
    w = lookup_widget (GTK_WIDGET (spinbutton), "tbl_f3");
    gtk_widget_hide (w);
  }
  else
  {
    w = lookup_widget (GTK_WIDGET (spinbutton), "tbl_f3");
    gtk_widget_show (w);
  }

  if(val<2)
  {
    w = lookup_widget (GTK_WIDGET (spinbutton), "tbl_f2");
    gtk_widget_hide (w);
  }
  else
  {
    w = lookup_widget (GTK_WIDGET (spinbutton), "tbl_f2");
    gtk_widget_show (w);
  }


  FileSetNumberFormants((unsigned int)val);

}



void
on_imskpe_main_activate_default        (GtkWindow       *window,
                                        gpointer         user_data)
{
  SetMainWindow(lookup_widget (GTK_WIDGET (window), "imskpe_main"));
}


void
on_lb_f1_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;

  col = GetFormantListColor("f1");
  gtk_widget_modify_fg (widget, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (widget, GTK_STATE_NORMAL, &col);
}


void
on_lb_f2_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;

  col = GetFormantListColor("f2");
  gtk_widget_modify_fg (widget, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (widget, GTK_STATE_NORMAL, &col);
}


void
on_lb_f3_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;

  col = GetFormantListColor("f3");
  gtk_widget_modify_fg (widget, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (widget, GTK_STATE_NORMAL, &col);
}


gboolean
imskpe_quit                            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  FormantListFree();
  ConfigListFree();
  CurveListFree(FileGetCurvesPointer());

  gtk_main_quit ();
  return FALSE;
}


void
on_nb_draw_switch_page                 (GtkNotebook     *notebook,
                                        GtkNotebookPage *page,
                                        guint            page_num,
                                        gpointer         user_data)
{
//   printf("--on_nb_draw\n");
  redraw_page(page_num);
  printf("--> tab: %2d\n",page_num);
}



void
on_ok_button2_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  // set filename 
  char *filename;
  GtkWidget *w;

  int len = strlen(gtk_file_selection_get_filename (GTK_FILE_SELECTION (gtk_widget_get_toplevel (GTK_WIDGET (button)))));

  filename = (char *) malloc(sizeof(char)*(len+2));
 
  filename = (char *) gtk_file_selection_get_filename 
      (GTK_FILE_SELECTION (gtk_widget_get_toplevel (GTK_WIDGET (button))));
  

  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));

  // and start import ...
  SetTitle(filename);
  FileOpen(filename);

  w=(GtkWidget *)lookup_widget (GTK_WIDGET (GetMainWindow()), "nb_draw");
  redraw_page(gtk_notebook_get_current_page((GtkNotebook *)w));
}


void
on_cancel_button2_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}


void
on_bn_about_close_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}


void
on_bn_open_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
 static GtkWidget *fileopen = NULL;

  if (fileopen == NULL) 
    {
      fileopen = create_imskpe_file ();
      /* set the widget pointer to NULL when the widget is destroyed */
      g_signal_connect (G_OBJECT (fileopen),
			"destroy",
			G_CALLBACK (gtk_widget_destroyed),
			&fileopen);

      SetMainWindow(lookup_widget (GTK_WIDGET (button), "imskpe_main"));
      /* Make sure the dialog doesn't disappear behind the main window. */
      gtk_window_set_transient_for (GTK_WINDOW (fileopen), 
				    GTK_WINDOW (GetMainWindow()));
    }

  /* Make sure the dialog is visible. */
  gtk_window_present (GTK_WINDOW (fileopen));
}


void
on_lb_f4_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;

  col = GetFormantListColor("f4");
  gtk_widget_modify_fg (widget, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (widget, GTK_STATE_NORMAL, &col);
}


void
on_lb_f5_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;

  col = GetFormantListColor("f5");
  gtk_widget_modify_fg (widget, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (widget, GTK_STATE_NORMAL, &col);
}


void
on_lb_f6_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;

  col = GetFormantListColor("f6");
  gtk_widget_modify_fg (widget, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (widget, GTK_STATE_NORMAL, &col);
}


void
on_lb_nasals_realize                   (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;

  col = GetFormantListColor("nasals");
  gtk_widget_modify_fg (widget, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (widget, GTK_STATE_NORMAL, &col);
}


void
on_cm_vs_entry_changed                 (GtkEditable     *editable,
                                        gpointer         user_data)
{
  char x[10];
  strncpy(x,gtk_editable_get_chars(editable,0,-1),10);

  if(!strcmp("impulse",x))
  {
    FileSetVoiceSource(1);
//     printf("impulse: 1\n");
  }
  if(!strcmp("natural",x))
  {
    FileSetVoiceSource(2);
//     printf("natural: 2\n");
  }
  if(!strcmp("sampled",x))
  {
    FileSetVoiceSource(3);
//     printf("sampled: 3\n");
  }
}


void
on_cm_cp_entry_changed                 (GtkEditable     *editable,
                                        gpointer         user_data)
{
  char x[20];
  strncpy(x,gtk_editable_get_chars(editable,0,-1),20);

  if(!strcmp("cascade + parallel",x))
  {
    FileSetBranches(1);
//     printf("cascade + parallel: 1\n");
  }
  if(!strcmp("parallel only",x))
  {
    FileSetBranches(2);
//     printf("parallel only: 2\n");
  }
}


void
on_spbn_sprate_value_changed           (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  FileSetSamplingRate((unsigned int)gtk_spin_button_get_value_as_int(spinbutton));
}


void
on_spbn_ui_value_changed               (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  FileSetUpdateInterval((unsigned int)gtk_spin_button_get_value_as_int(spinbutton));
}


void
on_bn_new_clicked                      (GtkButton       *button,
                                        gpointer         user_data)
{
  CurveInitStart();
}


void
on_spbn_duration_value_changed         (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  GtkWidget *w;
  int foo;
  
  w=(GtkWidget *)lookup_widget (GTK_WIDGET ((GtkWidget *)GetMainWindow()), "nb_draw");
  foo=gtk_notebook_get_current_page((GtkNotebook *)w);
//   printf("foo_sp: %d\n",foo);
  configure_page(foo);

  FileSetDuration((unsigned int)gtk_spin_button_get_value_as_int(spinbutton));
}



void
on_bn_prefs_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
 static GtkWidget *prefs = NULL;

  if (prefs == NULL) 
    {
      prefs = create_imskpe_prefs ();
      /* set the widget pointer to NULL when the widget is destroyed */
      g_signal_connect (G_OBJECT (prefs),
			"destroy",
			G_CALLBACK (gtk_widget_destroyed),
			&prefs);

      SetMainWindow(lookup_widget (GTK_WIDGET (button), "imskpe_main"));
      /* Make sure the dialog doesn't disappear behind the main window. */
      gtk_window_set_transient_for (GTK_WINDOW (prefs), 
				    GTK_WINDOW (GetMainWindow()));
    }

  /* Make sure the dialog is visible. */
  gtk_window_present (GTK_WINDOW (prefs));

}

void
on_bn_f1_freq_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  printf("'f1_freq' - state: - %d\n",gtk_toggle_button_get_active ((GtkToggleButton *)togglebutton));
  SetCurveShow("bn_f1_freq");
}

void
on_bn_f1_amp_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_f1_amp");
}


void
on_bn_f1_band_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_f1_band");
}


void
on_bn_f1_bandp_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_f1_bandp");
}


void
on_bn_f2_freq_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_f2_freq");
}


void
on_bn_f2_amp_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_f2_amp");
}


void
on_bn_f2_band_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_f2_band");
}


void
on_bn_f2_bandp_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_f2_bandp");
}


void
on_bn_f3_freq_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_f3_freq");
}


void
on_bn_f3_amp_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_f3_amp");
}


void
on_bn_f3_band_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_f3_band");
}


void
on_bn_f3_bandp_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_f3_bandp");
}


void
on_bn_f4_freq_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_f4_freq");
}


void
on_bn_f4_amp_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_f4_amp");
}


void
on_bn_f4_band_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_f4_band");
}


void
on_bn_f4_bandp_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_f4_bandp");
}


void
on_bn_f5_freq_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_f5_freq");
}


void
on_bn_f5_amp_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_f5_amp");
}


void
on_bn_f5_band_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_f5_band");
}


void
on_bn_f5_bandp_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_f5_bandp");
}


void
on_bn_f6_freq_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_f6_freq");
}


void
on_bn_f6_amp_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_f6_amp");
}


void
on_bn_f6_band_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_f6_band");
}


void
on_bn_f6_bandp_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_f6_bandp");
}


void
on_bn_nasal_z_freq_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_nasal_z_freq");
}


void
on_bn_nasal_z_band_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_nasal_z_band");
}


void
on_bn_nasal_p_freq_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_nasal_p_freq");
}


void
on_bn_nasal_p_band_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_nasal_p_band");
}


void
on_bn_nasal_p_amp_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_nasal_p_amp");
}


void
on_bn_vs_gopenquot_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_vs_gopenquot");
}


void
on_bn_vs_breath_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_vs_breath");
}


void
on_bn_vs_spectilt_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_vs_spectilt");
}


void
on_bn_vs_skewness_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_vs_skewness");
}


void
on_bn_vs_fundfreq_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_vs_fundfreq");
}


void
on_bn_examp_voice_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_examp_voice");
}


void
on_bn_examp_asp_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_examp_asp");
}


void
on_bn_examp_fric_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_examp_fric");
}


void
on_bn_examp_bypass_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_examp_bypass");
}


void
on_bn_examp_voicepar_toggled           (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_examp_voicepar");
}


void
on_bn_examp_siggain_toggled            (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  SetCurveShow("bn_examp_siggain");
}


void
on_bn_credits_ok_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}


void
on_bn_about_credits_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
 static GtkWidget *credits = NULL;

  if (credits == NULL) 
    {
      credits = create_imskpe_credits ();
      /* set the widget pointer to NULL when the widget is destroyed */
      g_signal_connect (G_OBJECT (credits),
			"destroy",
			G_CALLBACK (gtk_widget_destroyed),
			&credits);

      /* Make sure the dialog doesn't disappear behind the main window. */
      gtk_window_set_transient_for (GTK_WINDOW (credits), 
				    GTK_WINDOW (GetMainWindow()));
    }

  /* Make sure the dialog is visible. */
  gtk_window_present (GTK_WINDOW (credits));
}


void
on_pm_move_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  SetToggleButton(MOVE);
}


void
on_pm_insert_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  SetToggleButton(INSERT);
}


void
on_pm_delete_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  SetToggleButton(DELETE);
}


void
on_bn_move_toggled                     (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data)
{
   if(gtk_toggle_tool_button_get_active (toggletoolbutton) == TRUE)
   {
     SetToggleButton(MOVE);
   }
}


void
on_bn_insert_toggled                   (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data)
{
   if(gtk_toggle_tool_button_get_active (toggletoolbutton) == TRUE)
   {
     SetToggleButton(INSERT);
   }
}


void
on_bn_delete_toggled                   (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data)
{
   if(gtk_toggle_tool_button_get_active (toggletoolbutton) == TRUE)
   {
     SetToggleButton(DELETE);
   }
}


void
on_bn_prefs_cancel_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}


void
on_bn_prefs_apply_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_bn_prefs_ok_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}

