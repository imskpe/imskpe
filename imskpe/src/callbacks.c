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
 * @brief  callback-ifdef
 * 
 * \todo problem with 'realize'-event and notebook-widgets! - realize isn't raised for notebook-tabs!=1 until selected!
 */


#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib/gprintf.h>
#include <sys/stat.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#include "cfg.h"
#include "loadfile.h"
#include "savefile.h"
#include "curves.h"
#include "graphics.h"
#include "convert.h"

/* ---------------------------------------------------------------------- */
/**
   @name load/save dialog
   @{ */

/** loadsaveType - maybe not the best solution */
typedef enum {NONE, LOAD, SAVE} lsTyp;

/** default value for loadsave */
lsTyp loadorsave=NONE;

/** 
 * init new file, redraw drawarea
 * 
 * @param menuitem 
 * @param user_data 
 */
void
on_new1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget *w;
  FileInit();

  w=(GtkWidget *)lookup_widget (GTK_WIDGET (GetMainWindow()), "nb_draw");
  redraw_page(gtk_notebook_get_current_page((GtkNotebook *)w));
}


/**
 * start fileopen dialog
 * 
 * @param menuitem 
 * @param user_data 
 */
void
on_open1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  InitDialogLoad();
}


/**
 * start filesave dialog
 * if file is new, ask for new filename else use default in aFile-struct
 * 
 * @param menuitem 
 * @param user_data 
 */
void
on_save1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  if(FileGetIsNew())
  {
    InitDialogSave();
  }
  else
  {
    // "" means use filename in aFile-struct
    FileSave("");
  }
}

/** 
 * start filesave dialog for filename asking
 * 
 * @param menuitem 
 * @param user_data 
 */
void
on_save_as1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  InitDialogSave();
}

void
on_ok_button2_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  char *filename;
  GtkWidget *w;
  struct stat st;
  
  int len;

// gtk_file_selection_get_filename (GTK_FILE_SELECTION (file_selector));
  len = strlen(gtk_file_selection_get_filename (GTK_FILE_SELECTION (gtk_widget_get_toplevel (GTK_WIDGET (button)))));

  filename = (char *) malloc(sizeof(char)*(len+2));
    
  filename = (char *) gtk_file_selection_get_filename 
      (GTK_FILE_SELECTION (gtk_widget_get_toplevel (GTK_WIDGET (button))));
  
  // test if file is directory!

//   path_part (file, last_dir, sizeof (last_dir));
  if (stat (filename, &st) != -1)
  {
    if (S_ISDIR (st.st_mode))
    {
      if (filename[strlen(filename)-1] != '/')
      {
	strcat (filename, "/");
      }
      gtk_file_selection_set_filename (GTK_FILE_SELECTION (GTK_FILE_SELECTION (gtk_widget_get_toplevel (GTK_WIDGET (button)))),filename);
      free (filename);
      return;
    }
  }

  if(loadorsave==LOAD)
  {
    gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
    
    // and start import ...
    SetTitle(filename);
    FileOpen(filename);

    w=(GtkWidget *)lookup_widget (GTK_WIDGET (GetMainWindow()), "nb_draw");
    redraw_page(gtk_notebook_get_current_page((GtkNotebook *)w));
  }
  else if(loadorsave==SAVE)
  {
    gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));

    SetTitle(filename);
    FileSave(filename);
  }
}


void
on_cancel_button2_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}

void
on_bn_new_clicked                      (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *w;
//  CurveInitStart();
  FileInit();
  FileSetIsNew(TRUE);

  w=(GtkWidget *)lookup_widget (GTK_WIDGET (GetMainWindow()), "nb_draw");
  redraw_page(gtk_notebook_get_current_page((GtkNotebook *)w));
}


void
on_bn_open_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  InitDialogLoad();
}


void InitDialogSave()
{
 static GtkWidget *fileopen = NULL;

 loadorsave=SAVE;

 if (fileopen == NULL) 
 {
   fileopen = create_imskpe_file ();
   /* set the widget pointer to NULL when the widget is destroyed */
   g_signal_connect (G_OBJECT (fileopen),
		     "destroy",
		     G_CALLBACK (gtk_widget_destroyed),
		     &fileopen);
   
   /* Make sure the dialog doesn't disappear behind the main window. */
   gtk_window_set_transient_for (GTK_WINDOW (fileopen), 
				 GTK_WINDOW (GetMainWindow()));
 }
 
 /* Make sure the dialog is visible. */
 gtk_window_present (GTK_WINDOW (fileopen));
}


void InitDialogLoad()
{
 static GtkWidget *fileopen = NULL;

 loadorsave=LOAD;

 if (fileopen == NULL) 
 {
   fileopen = create_imskpe_file ();
   /* set the widget pointer to NULL when the widget is destroyed */
   g_signal_connect (G_OBJECT (fileopen),
		     "destroy",
		     G_CALLBACK (gtk_widget_destroyed),
		     &fileopen);
   
   /* Make sure the dialog doesn't disappear behind the main window. */
   gtk_window_set_transient_for (GTK_WINDOW (fileopen), 
				 GTK_WINDOW (GetMainWindow()));
 }
 
 /* Make sure the dialog is visible. */
 gtk_window_present (GTK_WINDOW (fileopen));
}

void
on_bn_save_clicked                     (GtkToolButton   *toolbutton,
                                        gpointer         user_data)
{
  if(FileGetIsNew())
  {
    InitDialogSave();
  }
  else
  {
    // "" means use filename in aFile-struct
    FileSave("");
  }
}


void
on_bn_saveas_clicked                   (GtkToolButton   *toolbutton,
                                        gpointer         user_data)
{
  InitDialogSave();
}

/** @} */

/* ---------------------------------------------------------------------- */

/**
 @name about dialog
 @{ */ 

/** 
 * activates about dialog
 *
 * @param menuitem 
 * @param user_data 
 */
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
 * close about dialog (with cancel)
 *
 * @param button 
 * @param user_data 
 */
void
on_bn_about_close_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}


/** 
 * close about dialog (with ok)
 *
 * @param button 
 * @param user_data 
 */
void
on_bn_credits_ok_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}


/** 
 * activate credits-dialog
 *
 * @param button 
 * @param user_data 
 */
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

/** 
 * close splashscreen
 * 
 * @param button 
 * @param user_data 
 */
void
on_splash_close_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}

/** @} */

/* ---------------------------------------------------------------------- */

/**
 @name drawarea parts
 @{ */ 

/** 
 * look which notebook is selected and configure only this one!
 * except first start, when MainWindow isn't set
 * 
 * @param widget 
 * @param event 
 * @param user_data 
 * 
 * @return TRUE
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
    configure_drawarea(widget, FREQUENCIES);
  }

  return TRUE;
}

/** 
 * look which notebook is selected and configure only this one!
 * except first start, when MainWindow isn't set
 * 
 * @param widget 
 * @param event 
 * @param user_data 
 * 
 * @return TRUE
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
    configure_drawarea(widget, AMPLITUDE);
  }

  return TRUE;
}


/** 
 * look which notebook is selected and configure only this one!
 * except first start, when MainWindow isn't set
 * 
 * @param widget 
 * @param event 
 * @param user_data 
 * 
 * @return TRUE
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
    configure_drawarea(widget, BANDWIDTH);
  }

  return TRUE;
}


/** 
 * redraw after resize of the window
 * 
 * @param widget 
 * @param event 
 * @param user_data 
 * 
 * @return TRUE
 */
gboolean
on_draw_freq_expose_event              (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
  Redrawpixmap(widget,event);

  return TRUE;
}

/** 
 * redraw after resize of the window
 * 
 * @param widget 
 * @param event 
 * @param user_data 
 * 
 * @return TRUE
 */
gboolean
on_draw_amp_expose_event               (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
  Redrawpixmap(widget,event);

  return TRUE;
}

/** 
 * redraw after resize of the window
 * 
 * @param widget 
 * @param event 
 * @param user_data 
 * 
 * @return TRUE
 */
gboolean
on_draw_band_expose_event              (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
  Redrawpixmap(widget,event);

  return TRUE;
}

/** 
 * refresh on mousemotion the activated drawarea (incl. hovering)
 * 
 * @param widget 
 * @param event 
 * @param user_data 
 * 
 * @return TRUE
 */
gboolean
on_draw_freq_motion_notify_event       (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data)
{
  int x, y;
  int rx,ry;
  GdkModifierType state;
  char tmp[80];
  diagramTyp dia=FREQUENCIES;
  
  if (event->is_hint)
    gdk_window_get_pointer (event->window, &x, &y, &state);
  else
    {
      x = event->x;
      y = event->y;
      state = event->state;
    }
    
  rx=CalcRealX(x, widget->allocation.width);  
  ry=CalcRealY(y, widget->allocation.height,dia);  
  if(rx>=0 && ry>=0)
    {
      snprintf(tmp,30,_("%d ms - %d Hz "),rx,ry);
      SetStatusBar("sb_add",tmp);

      DrawAreaMotion(rx, ry, state, dia);
    }
  else
    {
      snprintf(tmp,30,"");
      SetStatusBar("sb_add",tmp);
    }
  return TRUE;
}

/** 
 * refresh on mousemotion the activated drawarea (incl. hovering)
 * 
 * @param widget 
 * @param event 
 * @param user_data 
 * 
 * @return TRUE
 */
gboolean
on_draw_amp_motion_notify_event        (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data)
{
  int x, y;
  int rx,ry;
  char tmp[80];
  GdkModifierType state;
  diagramTyp dia=AMPLITUDE;

  if (event->is_hint)
    gdk_window_get_pointer (event->window, &x, &y, &state);
  else
    {
      x = event->x;
      y = event->y;
      state = event->state;
    }
    
  rx=CalcRealX(x, widget->allocation.width);  
  ry=CalcRealY(y, widget->allocation.height,dia);  
  if(rx>=0 && ry>=0)
    {
      snprintf(tmp,30,_("%d ms - %d dB "),rx,ry);
      SetStatusBar("sb_add",tmp);

      DrawAreaMotion(rx, ry, state, dia);
    }
  else
    {
      snprintf(tmp,30,"");
      SetStatusBar("sb_add",tmp);
    }
  return TRUE;
}

/** 
 * refresh on mousemotion the activated drawarea (incl. hovering)
 * 
 * @param widget 
 * @param event 
 * @param user_data 
 * 
 * @return TRUE
 */
gboolean
on_draw_band_motion_notify_event       (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data)
{
  int x, y;
  int rx,ry;
  char tmp[80];
  GdkModifierType state;
  diagramTyp dia=BANDWIDTH;

  if (event->is_hint)
    gdk_window_get_pointer (event->window, &x, &y, &state);
  else
    {
      x = event->x;
      y = event->y;
      state = event->state;
    }
    
  rx=CalcRealX(x, widget->allocation.width);  
  ry=CalcRealY(y, widget->allocation.height,dia);  
  if(rx>=0 && ry>=0)
    {
      snprintf(tmp,30,_("%d ms - %d Hz "),rx,ry);
      SetStatusBar("sb_add",tmp);

      DrawAreaMotion(rx, ry, state, dia);
    }
  else
    {
      snprintf(tmp,30,"");
      SetStatusBar("sb_add",tmp);
    }
  return TRUE;
}

/** 
 * choose action for buttonpressed
 * 
 * @param widget 
 * @param event 
 * @param user_data 
 * 
 * @return TRUE
 */
gboolean
on_draw_freq_button_press_event        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  diagramTyp dia=FREQUENCIES;
  int x, y;
  int rx,ry;
  GdkModifierType state;
  
  x = event->x;
  y = event->y;

  rx=CalcRealX(x, widget->allocation.width);  
  ry=CalcRealY(y, widget->allocation.height,dia);  

  DrawButtonPressed(rx,ry,event, dia); 

  return TRUE;
}

/** 
 * choose action for buttonpressed
 * 
 * @param widget 
 * @param event 
 * @param user_data 
 * 
 * @return TRUE
 */
gboolean
on_draw_amp_button_press_event         (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  diagramTyp dia=AMPLITUDE;
  int x, y;
  int rx,ry;
  GdkModifierType state;
  
  x = event->x;
  y = event->y;

  rx=CalcRealX(x, widget->allocation.width);  
  ry=CalcRealY(y, widget->allocation.height,dia);  

  DrawButtonPressed(rx,ry,event, dia); 

  return TRUE;
}

/** 
 * choose action for buttonpressed
 * 
 * @param widget 
 * @param event 
 * @param user_data 
 * 
 * @return TRUE
 */
gboolean
on_draw_band_button_press_event        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  diagramTyp dia=BANDWIDTH;
  int x, y;
  int rx,ry;
  GdkModifierType state;
  
  x = event->x;
  y = event->y;

  rx=CalcRealX(x, widget->allocation.width);  
  ry=CalcRealY(y, widget->allocation.height,dia);  

  DrawButtonPressed(rx,ry,event, dia); 

  return TRUE;
}


/** 
 * set buttonpressedvar to 0
 * 
 * @param widget 
 * @param event 
 * @param user_data 
 * 
 * @return TRUE
 */
gboolean
on_draw_freq_button_release_event      (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  SetMousepressed(0);

  return TRUE;
}

/** 
 * set buttonpressedvar to 0
 * 
 * @param widget 
 * @param event 
 * @param user_data 
 * 
 * @return TRUE
 */
gboolean
on_draw_amp_button_release_event       (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  SetMousepressed(0);

  return TRUE;
}

/** 
 * set buttonpressedvar to 0
 * 
 * @param widget 
 * @param event 
 * @param user_data 
 * 
 * @return TRUE
 */
gboolean
on_draw_band_button_release_event      (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  SetMousepressed(0);

  return TRUE;
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


/** 
 * activate move-edit-dialog, for more accurate input
 * 
 * @param menuitem 
 * @param user_data 
 */
void
on_pm_movediag_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
 static GtkWidget *movediag = NULL;
 GtkWidget *w;

 int dia=1;
 int ymax=5000;

 typCurveList *cl=CurveSearchByNr(FileGetCurvesPointer(),MouseEventGetCurve());
 GList *vl=(GList *)g_list_first(cl->points);
 typValueList *v,*v2;  
 int ui=FileGetUpdateInterval();
 typValueList p_pnt;  
 typValueList pnt;  
 typValueList n_pnt;  
 int v2time;

 if (movediag == NULL) 
 {
   movediag = create_imskpe_move ();
   /* set the widget pointer to NULL when the widget is destroyed */
   g_signal_connect (G_OBJECT (movediag),
		     "destroy",
		     G_CALLBACK (gtk_widget_destroyed),
		     &movediag);
   
   
   p_pnt.time=0;
   n_pnt.time=-1;
   pnt.time=MouseEventGetPoint();
   pnt.value=-1;
   
   while(vl)
   {
     v=vl->data;
     if(v->time==pnt.time)
     { 
       if(vl->next!=NULL)
       {
	 v2=vl->next->data;
	 v2time=v2->time-ui;
       }
       {
	 v2time=pnt.time;
       }
       n_pnt.time=v2time;
       if(pnt.time!=0)
       {
	 p_pnt.time+=ui;
       }
       pnt.value=v->value;
       break;
     }
     else
     {
       p_pnt.time=v->time;
       p_pnt.value=v->value;
       vl=vl->next;
     }
   }
   
   w=(GtkWidget *)lookup_widget (GTK_WIDGET ((GtkWidget *)GetMainWindow()), "nb_draw");
   dia=gtk_notebook_get_current_page((GtkNotebook *)w)+1;

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
   
      // set values 
   w=lookup_widget (GTK_WIDGET (movediag), "spn_value");
   gtk_spin_button_set_range ((GtkSpinButton *) w, 0, ymax);
   gtk_spin_button_set_value ((GtkSpinButton *) w, pnt.value);
   
   w=lookup_widget (GTK_WIDGET (movediag), "spn_time");
   if(pnt.time==n_pnt.time)
   {
     p_pnt.time=n_pnt.time;  // don't change time of last point!
   }
   gtk_spin_button_set_range ((GtkSpinButton *) w, p_pnt.time, n_pnt.time);
   gtk_spin_button_set_value ((GtkSpinButton *) w, pnt.time);
   
   
//       SetMainWindow(lookup_widget (GTK_WIDGET (menuitem), "imskpe_main"));
   /* Make sure the dialog doesn't disappear behind the main window. */
   gtk_window_set_transient_for (GTK_WINDOW (movediag), 
				 GTK_WINDOW (GetMainWindow()));
 }
 
 /* Make sure the dialog is visible. */
 gtk_window_present (GTK_WINDOW (movediag));
 
}


void
on_bn_move_cancel_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *w;

  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));

  w=(GtkWidget *)lookup_widget (GTK_WIDGET (GetMainWindow()), "nb_draw");
  redraw_page(gtk_notebook_get_current_page((GtkNotebook *)w));
}


void
on_bn_move_ok_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *w;
  int val, time;

  // set Point
  w=lookup_widget (GTK_WIDGET (button), "spn_value");
  val=gtk_spin_button_get_value_as_int ((GtkSpinButton *) w);
  w=lookup_widget (GTK_WIDGET (button), "spn_time");
  time=gtk_spin_button_get_value_as_int ((GtkSpinButton *) w);

  PointMove(CurveSearchByNr(FileGetCurvesPointer(),MouseEventGetCurve()),MouseEventGetPoint(),time,val);

  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));

  w=(GtkWidget *)lookup_widget (GTK_WIDGET (GetMainWindow()), "nb_draw");
  redraw_page(gtk_notebook_get_current_page((GtkNotebook *)w));
}

/** @} */

/* ---------------------------------------------------------------------- */

/**
 @name gui control elements
 @{ */ 

/** 
 * number of formants changed -> decrease visible tabs
 * 
 * @param spinbutton 
 * @param user_data 
 */
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
on_lb_f1_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;

  col = ConfigGetColor("f1");
  gtk_widget_modify_fg (widget, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (widget, GTK_STATE_NORMAL, &col);
}


void
on_lb_f2_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;

  col = ConfigGetColor("f2");
  gtk_widget_modify_fg (widget, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (widget, GTK_STATE_NORMAL, &col);
}


void
on_lb_f3_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;

  col = ConfigGetColor("f3");
  gtk_widget_modify_fg (widget, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (widget, GTK_STATE_NORMAL, &col);
}



void
on_nb_draw_switch_page                 (GtkNotebook     *notebook,
                                        GtkNotebookPage *page,
                                        guint            page_num,
                                        gpointer         user_data)
{
//   printf("--on_nb_draw\n");
  redraw_page(page_num);
//   printf("--> tab: %2d\n",page_num);
}

void
on_lb_f4_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;

  col = ConfigGetColor("f4");
  gtk_widget_modify_fg (widget, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (widget, GTK_STATE_NORMAL, &col);
}


void
on_lb_f5_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;

  col = ConfigGetColor("f5");
  gtk_widget_modify_fg (widget, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (widget, GTK_STATE_NORMAL, &col);
}


void
on_lb_f6_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;

  col = ConfigGetColor("f6");
  gtk_widget_modify_fg (widget, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (widget, GTK_STATE_NORMAL, &col);
}


void
on_lb_nasals_realize                   (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;

  col = ConfigGetColor("nasals");
  gtk_widget_modify_fg (widget, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (widget, GTK_STATE_NORMAL, &col);
}


void
on_cm_vs_entry_changed                 (GtkEditable     *editable,
                                        gpointer         user_data)
{
  char x[10];
  strncpy(x,gtk_editable_get_chars(editable,0,-1),10);

  if(!strcmp(_("impulse"),x))
  {
    FileSetVoiceSource(1);
  }
  if(!strcmp(_("natural"),x))
  {
    FileSetVoiceSource(2);
  }
  if(!strcmp(_("sampled"),x))
  {
    FileSetVoiceSource(3);
  }
}


void
on_cm_cp_entry_changed                 (GtkEditable     *editable,
                                        gpointer         user_data)
{
  char x[20];
  strncpy(x,gtk_editable_get_chars(editable,0,-1),20);

  if(!strcmp(_("cascade + parallel"),x))
  {
    FileSetBranches(1);
//     printf("cascade + parallel: 1\n");
  }
  if(!strcmp(_("parallel only"),x))
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
on_spbn_duration_value_changed         (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  GtkWidget *w;
  unsigned int du;
  unsigned int o_du;
  int foo;
  
  o_du = FileGetDuration();
  du=(unsigned int)gtk_spin_button_get_value_as_int(spinbutton);

  if(du>o_du)
  {
    for(foo=0;foo<40;foo++)
    {
      PointSetDU(CurveSearchByNr(FileGetCurvesPointer(),foo),o_du, du);
    }
  }
  FileSetDuration(du);

  w=(GtkWidget *)lookup_widget (GTK_WIDGET ((GtkWidget *)GetMainWindow()), "nb_draw");
  foo=gtk_notebook_get_current_page((GtkNotebook *)w);
  configure_page(foo);
}


void
on_bn_f1_freq_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
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


/** @} */

/* ---------------------------------------------------------------------- */

/** 
    @name preferences parts 
    @{ */

/** bad style trick variable ... */
char *selected_color;

/** pointer to colordialog */
GtkWidget *colordiag = NULL;
/** pointer to prefs dialog */
GtkWidget *prefs = NULL;

/** 
 * init preferences dialog
 * 
 * @param button 
 * @param user_data 
 */
void
on_bn_prefs_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  InitDialogPrefs();
}

/** 
 * close preferences dialog (with cancel)
 * remove all temporary savings from config-list
 * 
 * @param button 
 * @param user_data 
 */
void
on_bn_prefs_cancel_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
/*   ConfigRename("klattcmd","tmp_klatt"); */
  ConfigRemove("color_f1_tmp");
  ConfigRemove("color_f2_tmp");
  ConfigRemove("color_f3_tmp");
  ConfigRemove("color_f4_tmp");
  ConfigRemove("color_f5_tmp");
  ConfigRemove("color_f6_tmp");
  ConfigRemove("color_nasals_tmp");
  ConfigRemove("color_vs_tmp");
  ConfigRemove("color_ea_tmp");

  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}


/** 
 * save all temporary savings by remove-old and rename-to-old
 * 
 * @param button 
 * @param user_data 
 */
void
on_bn_prefs_apply_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *w;
  int val;  
  char tmp[100];

  // save values
  printf("apply\n");
  if(ConfigFind("color_f1_tmp"))
  {
    ConfigRemove("color_f1");
    ConfigRename("color_f1_tmp","color_f1");
  }
  if(ConfigFind("color_f2_tmp"))
  {
    ConfigRemove("color_f2");
    ConfigRename("color_f2_tmp","color_f2");
  }
  if(ConfigFind("color_f3_tmp"))
  {
    ConfigRemove("color_f3");
    ConfigRename("color_f3_tmp","color_f3");
  }
  if(ConfigFind("color_f4_tmp"))
  {
    ConfigRemove("color_f4");
    ConfigRename("color_f4_tmp","color_f4");
  }
  if(ConfigFind("color_f5_tmp"))
  {
    ConfigRemove("color_f5");
    ConfigRename("color_f5_tmp","color_f5");
  }
  if(ConfigFind("color_f6_tmp"))
  {
    ConfigRemove("color_f6");
    ConfigRename("color_f6_tmp","color_f6");
  }

  if(ConfigFind("color_nasals_tmp"))
  {
    ConfigRemove("color_nasals");
    ConfigRename("color_nasals_tmp","color_nasals");
  }
  if(ConfigFind("color_vs_tmp"))
  {
    ConfigRemove("color_vs");
    ConfigRename("color_vs_tmp","color_vs");
  }
  if(ConfigFind("color_vs_tmp"))
  {
    ConfigRemove("color_vs");
    ConfigRename("color_vs_tmp","color_vs");
  }

  w=lookup_widget (GTK_WIDGET (button), "spn_max_freq");
  val=gtk_spin_button_get_value_as_int ((GtkSpinButton *) w);
  sprintf(tmp,"%d",val);
  printf("max_freq: %d\n",val);
  ConfigInsert("maxfreq",tmp);

  w=lookup_widget (GTK_WIDGET (button), "spn_max_amp");
  val=gtk_spin_button_get_value_as_int ((GtkSpinButton *) w);
  sprintf(tmp,"%d",val);
  ConfigInsert("maxamp",tmp);

  w=lookup_widget (GTK_WIDGET (button), "spn_max_band");
  val=gtk_spin_button_get_value_as_int ((GtkSpinButton *) w);
  sprintf(tmp,"%d",val);
  ConfigInsert("maxband",tmp);

/*   w=lookup_widget (GTK_WIDGET (button), "ent_klatt"); */
/*   strcpy(tmp,gtk_entry_get_text((GtkEntry *)w)); */
/*   ConfigInsert("klattcmd",tmp); */

  w=lookup_widget (GTK_WIDGET (button), "ent_play");
  strcpy(tmp,gtk_entry_get_text((GtkEntry *)w));
  ConfigInsert("playcmd",tmp);

  w=lookup_widget (GTK_WIDGET (button), "ent_tmp");
  strcpy(tmp,gtk_entry_get_text((GtkEntry *)w));
  ConfigInsert("tmpdir",tmp);

  w=lookup_widget (GTK_WIDGET (button), "cme_tool_style");
  strncpy(tmp,gtk_editable_get_chars((GtkEditable *)w,0,-1),6);

  if(!strcmp(_("icons"),tmp))
  {
    val=GTK_TOOLBAR_ICONS;
  }
  if(!strcmp(_("text"),tmp))
  {
    val=GTK_TOOLBAR_TEXT;
  }
  if(!strcmp(_("both"),tmp))
  {
    val=GTK_TOOLBAR_BOTH;
  }
  sprintf(tmp,"%d",val);
  ConfigInsert("toolbarstyle",tmp);
  GuiSetToolbarStyle(val);


  // redraw!!
  w=(GtkWidget *)lookup_widget (GTK_WIDGET (GetMainWindow()), "nb_draw");
  redraw_page(gtk_notebook_get_current_page((GtkNotebook *)w));

  printf("apply end\n");     
  return;
}


/** 
 * close preferences dialog and use apply-function to save all values
 * 
 * @param button 
 * @param user_data 
 */
void
on_bn_prefs_ok_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  // save values
  on_bn_prefs_apply_clicked (button,user_data);

  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));

  printf("prefs_ok end\n");     
}

void InitDialogPrefs()
{
  if (prefs == NULL) 
    {
      prefs = create_imskpe_prefs();
      /* set the widget pointer to NULL when the widget is destroyed */
      g_signal_connect (G_OBJECT (prefs),
			"destroy",
			G_CALLBACK (gtk_widget_destroyed),
			&prefs);

      /* Make sure the dialog doesn't disappear behind the main window. */
      gtk_window_set_transient_for (GTK_WINDOW (prefs), 
				    GTK_WINDOW (GetMainWindow()));
    }

  /* Make sure the dialog is visible. */
  gtk_window_present (GTK_WINDOW (prefs));
}

void InitDialogColor(char *searchstring)
{
  GtkWidget *w;
  GdkColor col;
  char tmp[40];

  strcpy(tmp,"color_");
  strcat(tmp,searchstring);
  strcat(tmp,"_tmp");
  if(ConfigFind(tmp))
  {
    strcpy(tmp,searchstring);
    strcat(tmp,"_tmp");
//    printf("found: %s\n",tmp);
    col=ConfigGetColor(tmp);
  }
  else
  {
    col=ConfigGetColor(searchstring);
  }
  GdkColor *xcol;

  xcol=(GdkColor *)malloc(sizeof(GdkColor));
  xcol->red=col.red;
  xcol->green=col.green;
  xcol->blue=col.blue;

  // risky!
  selected_color=searchstring;

//   printf("%2x %2x %2x\n",xcol->red,xcol->green,xcol->blue);

  if (colordiag == NULL) 
    {
      colordiag = create_imskpe_colorsel ();
      /* set the widget pointer to NULL when the widget is destroyed */
      g_signal_connect (G_OBJECT (colordiag),
			"destroy",
			G_CALLBACK (gtk_widget_destroyed),
			&colordiag);

      /* Make sure the dialog doesn't disappear behind the main window. */
      gtk_window_set_transient_for (GTK_WINDOW (colordiag), 
				    GTK_WINDOW ((GtkWidget *)GetMainWindow()));
      /* Do not allow user to resize the dialog */
      gtk_window_set_resizable (GTK_WINDOW (colordiag), FALSE);

      w=lookup_widget (GTK_WIDGET (colordiag), "color_selection1");
      gtk_color_selection_set_current_color((GtkColorSelection *)w,
					    xcol);

      g_free(xcol);
    }

  /* Make sure the dialog is visible. */
  gtk_window_present (GTK_WINDOW (colordiag));
}


void
on_spn_max_freq_realize                (GtkWidget       *widget,
                                        gpointer         user_data)
{
  gtk_spin_button_set_value ((GtkSpinButton *) widget, ConfigGetInteger("maxfreq"));
}


void
on_spn_max_amp_realize                 (GtkWidget       *widget,
                                        gpointer         user_data)
{
  gtk_spin_button_set_value ((GtkSpinButton *) widget, ConfigGetInteger("maxamp"));

}


void
on_spn_max_band_realize                (GtkWidget       *widget,
                                        gpointer         user_data)
{
  gtk_spin_button_set_value ((GtkSpinButton *) widget, ConfigGetInteger("maxband"));

}


void
on_ent_play_realize                    (GtkWidget       *widget,
                                        gpointer         user_data)
{
  gtk_entry_set_text ((GtkEntry *) widget, ConfigGetString("playcmd"));
}


void
on_ent_tmp_realize                     (GtkWidget       *widget,
                                        gpointer         user_data)
{
  gtk_entry_set_text ((GtkEntry *) widget, ConfigGetString("tmpdir"));
}

void
on_cme_tool_style_realize              (GtkWidget       *widget,
                                        gpointer         user_data)
{
  int foo=ConfigGetInteger("toolbarstyle");  

  switch(foo)
  {
  case GTK_TOOLBAR_ICONS:
      gtk_entry_set_text ((GtkEntry *)widget,_("icons"));
      break;
  case GTK_TOOLBAR_TEXT:
      gtk_entry_set_text ((GtkEntry *)widget,_("text"));
      break;
  case GTK_TOOLBAR_BOTH:
      gtk_entry_set_text ((GtkEntry *)widget,_("both"));
      break;
  }

}

void
on_lb_colors_realize                   (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;
  GtkWidget *w;

  col = ConfigGetColor("f1");
  w=lookup_widget (GTK_WIDGET (widget), "lb_f1");
  gtk_widget_modify_fg (w, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (w, GTK_STATE_NORMAL, &col);

  col = ConfigGetColor("f2");
  w=lookup_widget (GTK_WIDGET (widget), "lb_f2");
  gtk_widget_modify_fg (w, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (w, GTK_STATE_NORMAL, &col);

  col = ConfigGetColor("f3");
  w=lookup_widget (GTK_WIDGET (widget), "lb_f3");
  gtk_widget_modify_fg (w, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (w, GTK_STATE_NORMAL, &col);

  col = ConfigGetColor("f4");
  w=lookup_widget (GTK_WIDGET (widget), "lb_f4");
  gtk_widget_modify_fg (w, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (w, GTK_STATE_NORMAL, &col);

  col = ConfigGetColor("f5");
  w=lookup_widget (GTK_WIDGET (widget), "lb_f5");
  gtk_widget_modify_fg (w, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (w, GTK_STATE_NORMAL, &col);

  col = ConfigGetColor("f6");
  w=lookup_widget (GTK_WIDGET (widget), "lb_f6");
  gtk_widget_modify_fg (w, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (w, GTK_STATE_NORMAL, &col);

  col = ConfigGetColor("nasals");
  w=lookup_widget (GTK_WIDGET (widget), "lb_nasals");
  gtk_widget_modify_fg (w, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (w, GTK_STATE_NORMAL, &col);

  col = ConfigGetColor("vs");
  w=lookup_widget (GTK_WIDGET (widget), "lb_vs");
  gtk_widget_modify_fg (w, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (w, GTK_STATE_NORMAL, &col);

  col = ConfigGetColor("ea");
  w=lookup_widget (GTK_WIDGET (widget), "lb_ea");
  gtk_widget_modify_fg (w, GTK_STATE_ACTIVE, &col);
  gtk_widget_modify_fg (w, GTK_STATE_NORMAL, &col);
  
}


gboolean
on_color_selection1_configure_event    (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data)
{

  return FALSE;
}


void
on_ok_button1_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  // colordialog ok:
  gdouble color[3];
  GdkColor gdk_color;
  GtkWidget *w;
  char tmp[30];
  char tmp2[30];

  strcpy(tmp,"lb_");
  strcat(tmp,selected_color);

//   printf("-%s-\n",tmp);

  w = lookup_widget (GTK_WIDGET ((GtkWidget *)colordiag), "color_selection1");    
  gtk_color_selection_get_color ((GtkColorSelection *)w,color);
    
  /* Fit to a unsigned 16 bit integer (0..65535) and insert into the GdkColor structure */
  gdk_color=GetColor(color[0],color[1],color[2]);

  w = lookup_widget (GTK_WIDGET ((GtkWidget *)prefs), tmp);
  gtk_widget_modify_fg (w, GTK_STATE_ACTIVE, &gdk_color);
  gtk_widget_modify_fg (w, GTK_STATE_NORMAL, &gdk_color);


  // \todo farbwerte in #aabbcc umrechnen und abspeichern!
  sprintf(tmp,"#%02x%02x%02x",(int)(gdk_color.red/256),(int)(gdk_color.green/256),(int)(gdk_color.blue/256));

  strcpy(tmp2,"color_");
  strcat(tmp2,selected_color);
  strcat(tmp2,"_tmp");
  printf("update ...\n");
  ConfigInsert(tmp2,tmp);

//   if(selected_color!=NULL)
//   {
//     g_free(selected_color);
//   }

  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
//   colordiag=NULL;
  printf("color ok end\n");
}


void
on_cancel_button1_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}



void
on_bn_color_f1_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  InitDialogColor("f1");
}


void
on_bn_color_f2_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  InitDialogColor("f2");
}

void
on_bn_color_f3_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  InitDialogColor("f3");
}

void
on_bn_color_f4_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  InitDialogColor("f4");
}


void
on_bn_color_f5_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  InitDialogColor("f5");
}


void
on_bn_color_f6_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  InitDialogColor("f6");
}


void
on_bn_color_nasals_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  InitDialogColor("nasals");
}


void
on_bn_color_vc_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  InitDialogColor("vs");
}


void
on_bn_color_ea_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  InitDialogColor("ea");
}

void
on_toolbar1_realize                    (GtkWidget       *widget,
                                        gpointer         user_data)
{
  int foo=ConfigGetInteger("toolbarstyle");  
  gtk_toolbar_set_style ((GtkToolbar *)widget,foo);
}


void
on_toolbar4_realize                    (GtkWidget       *widget,
                                        gpointer         user_data)
{
  int foo=ConfigGetInteger("toolbarstyle");  
  gtk_toolbar_set_style ((GtkToolbar *)widget,foo);
}


void
on_toolbar2_realize                    (GtkWidget       *widget,
                                        gpointer         user_data)
{
  int foo=ConfigGetInteger("toolbarstyle");  
  gtk_toolbar_set_style ((GtkToolbar *)widget,foo);
}

/** @} */

/* ---------------------------------------------------------------------- */

/** 
    @name execute convert
    @{
 */

void
on_convert1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  char tmp[300];

  /* get par-filename */
  strcpy(tmp,FileGetFilename());
  tmp[strlen(tmp)-strlen(strrchr(tmp, '.'))]=0; /* delete .par at end */
  strcat(tmp,".wav");  /* and .wav*/

  convert(tmp);

  // wav-file written
  strcat(tmp,_(" written"));

  SetStatusBar ("sb_state",tmp);
}


void
on_execute1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  char *dir;
  char tmp[300];

  dir = (char *)g_malloc(sizeof(char)*(strlen(ConfigGetString("tmpdir"))+20));

  strcpy(dir,ConfigGetString("tmpdir"));
  strcat(dir,"/imskpe.");
  sprintf(tmp,"%d",getpid());
  strcat(dir,tmp);

  strcpy(tmp,dir);
  strcat(tmp,".wav");

  convert(tmp);
  strcpy(dir,ConfigGetString("playcmd"));
  strcat(dir," ");
  strcat(dir,tmp);
  system(dir);

  g_free(dir);
}


void
on_bn_convert_clicked                  (GtkToolButton   *toolbutton,
                                        gpointer         user_data)
{
  on_convert1_activate(NULL,NULL);
}


void
on_bn_execute_clicked                  (GtkToolButton   *toolbutton,
                                        gpointer         user_data)
{

  on_execute1_activate(NULL,NULL);

}

/** @} */

/* ---------------------------------------------------------------------- */

/** 
    @name activate and quit imskpe
    @{
 */

void
on_imskpe_main_activate_default        (GtkWindow       *window,
                                        gpointer         user_data)
{
 SetMainWindow(lookup_widget (GTK_WIDGET (window), "imskpe_main"));
}


gboolean
imskpe_quit                            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
//   FormantListFree();
  CurveListFree(FileGetCurvesPointer());
  ConfigSave();
  ConfigFree();
  gtk_main_quit ();
}

/** @} */



void
on_spn_max_freq_parent_set             (GtkWidget       *widget,
                                        GtkWidget       *old_parent,
                                        gpointer         user_data)
{
  ;

}

