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
 */


#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib/gprintf.h>
#include <sys/stat.h>
#include <string.h>
#include <malloc.h>

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
/* /\** pointer to open dialog *\/ */
/* GtkWidget *fileopen = NULL; */

/** pointer to save dialog */ 
GtkWidget *filesave = NULL; 

GtkWidget *cb_filesave_extra = NULL;

gint loadafter;

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
  on_bn_new_clicked(NULL,NULL);
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
  loadafter=0;

  if(FileGetIsChanged())
  {
    if(DialogYesNo(_("Save changed data?"))==TRUE)
    {
      InitDialogSave();
      loadafter=1;
    }
  }

  if(loadafter==0)
  {
    InitDialogLoad();
  }
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
  loadafter=0;
  InitDialogSave();
}

void
on_bn_file_open_ok_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  char *filename=NULL;
  GtkWidget *w;
  int len;
  char *tmp;
  
  tmp = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (gtk_widget_get_toplevel (GTK_WIDGET (button))));
  if(tmp)
  {
    len = strlen(tmp);
  }
  else
  {
    len = 0;
  }

  tmp = (char *) malloc(sizeof(char)*(500)); // should be no constant
  tmp = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER (gtk_widget_get_toplevel (GTK_WIDGET (button))));
  ConfigInsert("lastdir", tmp);

  if(len)
  {
    filename = (char *) malloc(sizeof(char)*(len+2));
    
    filename = (char *) gtk_file_chooser_get_filename 
	(GTK_FILE_CHOOSER (gtk_widget_get_toplevel (GTK_WIDGET (button))));
    filename[len]=0;
  }

  gtk_widget_hide(gtk_widget_get_toplevel (GTK_WIDGET (button)));

  if (filename)
  {
    // and start import ...
    if(FileOpen(filename)==TRUE)
    {
      SetTitle(filename);
      w=(GtkWidget *)lookup_widget (GTK_WIDGET (GetMainWindow()), "nb_draw");
      redraw_page(gtk_notebook_get_current_page((GtkNotebook *)w));

      convert("");
      
      w = (GtkWidget *)lookup_widget (GTK_WIDGET (GetMainWindow()), "draw_wave");
      redraw_wave(w);
    }
  }
}


/*
  cancel-button-event for save-dialog
 */
void
on_bn_file_save_cancel_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));

  if(loadafter==1)
  {
    InitDialogLoad();
  }
  if(loadafter==2)
  {
    on_bn_new_clicked(NULL,NULL);
  }
}

/*
  ok-button-event for save-dialog
 */
void
on_bn_file_save_ok_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  char *filename=NULL;
  GtkWidget *w;
  int len;
  int type;
  gchar *tmp;

  w = (GtkWidget *) gtk_widget_get_toplevel (GTK_WIDGET (button));

  tmp = (char *) malloc(sizeof(char)*(500)); // should be no constant
  tmp = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER (w));
  ConfigInsert("lastdir", tmp);
  free(tmp);

  type = gtk_combo_box_get_active((GtkComboBox *)cb_filesave_extra);
  tmp = (char *) malloc(sizeof(char)*(500)); // should be no constant
  tmp = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (w));
  if(tmp)
  {
    len = strlen(tmp);
    filename = (char *) malloc(sizeof(char)*(len+1));
    filename = (char *) gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (w));
    filename[len]=0;
    free(tmp);
  }
  else
  {
    len = 0;
  }
//  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
  gtk_widget_hide(gtk_widget_get_toplevel (GTK_WIDGET (button)));

  if(type==0) // filetype: par
  {
    if(filename)
    {
      SetTitle(filename);
      FileSave(filename);
  
      w = (GtkWidget *)lookup_widget (GTK_WIDGET (GetMainWindow()), "nb_draw");
      redraw_page(gtk_notebook_get_current_page((GtkNotebook *)w));
    }
    if(loadafter==1)
    {
      InitDialogLoad();
    }
    if(loadafter==2)
    {
      on_bn_new_clicked(NULL,NULL);
    }
  }
  else // filetype: wav
  {
    if(filename)
    {
      // convert writes file, if filename is set
      convert(filename);
      w = (GtkWidget *)lookup_widget (GTK_WIDGET (GetMainWindow()), "draw_wave");
      redraw_wave(w);
      // wav-file written
      strcpy(tmp,filename);
      strcat(tmp,_(" written"));
      SetStatusBar("sb_state",tmp);
    }
  }
}

void
on_bn_new_clicked                      (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *w;

  if(FileGetIsChanged() && loadafter!=2)
  {
    if(DialogYesNo(_("Save changed data?"))==TRUE)
    {
      InitDialogSave();
      loadafter=2;
      return;
    }
  }

  loadafter=0;
  
  FileInit();
  FileSetIsNew(TRUE);
  
  w=(GtkWidget *)lookup_widget (GTK_WIDGET (GetMainWindow()), "nb_draw");
  redraw_page(gtk_notebook_get_current_page((GtkNotebook *)w));
}


void
on_bn_open_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  loadafter=0;

  if(FileGetIsChanged())
  {
    if(DialogYesNo(_("Save changed data?"))==TRUE)
    {
      InitDialogSave();
      loadafter=1;
    }
  }

  if(loadafter==0)
  {
    InitDialogLoad();
  }
}

void
on_extra_changed               (GtkComboBox     *combobox,
				gpointer         user_data)
{
  int type;
  char *tmp;

  type=gtk_combo_box_get_active(combobox);
  if(strrchr(gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (gtk_widget_get_toplevel (GTK_WIDGET (combobox)))),'/')!=NULL)
  {
    tmp=strrchr(gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (gtk_widget_get_toplevel (GTK_WIDGET (combobox)))),'/');
  }
  else
  {
    tmp=strrchr(gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (gtk_widget_get_toplevel (GTK_WIDGET (combobox)))),'\\');
  }

  tmp++;
  if(strrchr(tmp, '.'))
  {
    tmp[strlen(tmp) - strlen(strrchr(tmp, '.'))] = 0; /* delete .par at end */
  }

  if(type==0)  // par
  {
    strcat(tmp,".par"); 
  }
  else
  {
    strcat(tmp,".wav"); 
  }

  gtk_file_chooser_set_current_name((GtkFileChooser *)filesave, tmp);
}

void InitDialogSave()
{
// static GtkWidget *filesave = NULL;
  char *tmp;
  char *tmp2;
  GtkWidget *lb, *hbox;

  if (filesave == NULL) 
  {
    filesave = create_imskpe_file_save ();
    /* set the widget pointer to NULL when the widget is destroyed */
    g_signal_connect (G_OBJECT (filesave),
    		      "destroy",
    		      G_CALLBACK (gtk_widget_destroyed),
    		      &filesave);

    // search for last /
    tmp = strdup(FileGetFilename());
#ifdef WIN32
    tmp2=strrchr(tmp,'\\');
#else
    tmp2=strrchr(tmp,'/');
#endif
    
    if(tmp2!=NULL)
    {
      // go to next found slash '/foo.par' -> 'foo.par'
      tmp++;
    }

    if (tmp)
    {
      gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(filesave), tmp);
//      gtk_file_chooser_set_filename (GTK_FILE_CHOOSER(filesave), tmp);
    }

    tmp=(char *)g_malloc(200); // should be not a constant!!
    strcpy(tmp,filtertoken(ConfigGetString("lastdir"),"\""));
    if (tmp)
    {
      gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (filesave), tmp);
      free(tmp);
    }

    hbox = gtk_hbox_new (FALSE, 0);

    lb = gtk_label_new (_("Filetype: "));

    cb_filesave_extra = gtk_combo_box_new_text ();
    gtk_widget_show (cb_filesave_extra);
    gtk_combo_box_append_text (GTK_COMBO_BOX (cb_filesave_extra), "par");
    gtk_combo_box_append_text (GTK_COMBO_BOX (cb_filesave_extra), "wav");
    gtk_combo_box_set_active (GTK_COMBO_BOX (cb_filesave_extra),0);

    gtk_widget_show(lb);
    gtk_box_pack_end((GtkBox *)hbox, (GtkWidget *)cb_filesave_extra, FALSE, FALSE, 0);
    gtk_box_pack_end((GtkBox *)hbox, (GtkWidget *)lb, FALSE, FALSE, 0);

    g_signal_connect ((gpointer) cb_filesave_extra, "changed",
    		      G_CALLBACK (on_extra_changed),
    		      NULL);

    gtk_file_chooser_set_extra_widget (GTK_FILE_CHOOSER (filesave), hbox);
   
    /* Make sure the dialog doesn't disappear behind the main window. */
    gtk_window_set_transient_for (GTK_WINDOW (filesave), 
				  GTK_WINDOW (GetMainWindow()));
  }

  /* Make sure the dialog is visible. */
  gtk_window_present (GTK_WINDOW (filesave));
}

void InitDialogLoad()
{
  static GtkWidget *fileopen = NULL;
  GtkFileFilter *filter;
  char tmp[200];  // not good!

  if (fileopen == NULL) 
  {
    fileopen = create_imskpe_file_open ();
    /* set the widget pointer to NULL when the widget is destroyed */
    g_signal_connect (G_OBJECT (fileopen),
    		      "destroy",
    		      G_CALLBACK (gtk_widget_destroyed),
    		      &fileopen);

    strcpy(tmp,filtertoken(ConfigGetString("lastdir"),"\""));
    if (tmp)
    {
      gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (fileopen), tmp);
    }

    /* Filters */
    filter = gtk_file_filter_new ();
    gtk_file_filter_set_name (filter, _("All Files"));
    gtk_file_filter_add_pattern (filter, "*");
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (fileopen), filter);

    filter = gtk_file_filter_new ();
    gtk_file_filter_set_name (filter, _("par-Files"));
    gtk_file_filter_add_pattern (filter, "*.par");
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (fileopen), filter);

    gtk_file_chooser_set_filter (GTK_FILE_CHOOSER (fileopen), filter);
   
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
  loadafter=0;
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
	     "<small>\302\251 2004-2010 Andreas Madsack &lt;bolsog@users.sf.net></small>",VERSION,_("Compiled"),__DATE__
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
  char tmp[30];
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
    snprintf(tmp,sizeof(tmp),_("%d ms - %d Hz "),rx,ry);
    SetStatusBar("sb_add",tmp);

    DrawAreaMotion(rx, ry, state, dia);
  }
  else
  {
    snprintf(tmp,sizeof(tmp),"");
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
  char tmp[30];
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
    snprintf(tmp,sizeof(tmp),_("%d ms - %d dB "),rx,ry);
    SetStatusBar("sb_add",tmp);

    DrawAreaMotion(rx, ry, state, dia);
  }
  else
  {
    snprintf(tmp,sizeof(tmp),"");
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
  char tmp[30];
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
    snprintf(tmp,sizeof(tmp),_("%d ms - %d Hz "),rx,ry);
    SetStatusBar("sb_add",tmp);

    DrawAreaMotion(rx, ry, state, dia);
  }
  else
  {
    snprintf(tmp,sizeof(tmp),"");
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
//  GdkModifierType state;
  
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
//  GdkModifierType state;
  
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
//  GdkModifierType state;
  
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
on_draw_button_release_event      (GtkWidget       *widget,
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


/** 
 * change mode with scrollwheel of 3+-button-mouse
 * 
 * @param widget 
 * @param event 
 * @param user_data 
 * 
 * @return 
 */
gboolean
on_draw_scroll_event              (GtkWidget       *widget,
				   GdkEvent        *event,
				   gpointer         user_data)
{
  if(gtk_toggle_tool_button_get_active((GtkToggleToolButton*)lookup_widget (GTK_WIDGET (GetMainWindow()), "bn_move")))
  {
    if(event->scroll.direction==0)
    {
      SetToggleButton(DELETE);
    }
    else
    {
      SetToggleButton(INSERT);
    }
    return TRUE;
  }
  
  if(gtk_toggle_tool_button_get_active((GtkToggleToolButton*)lookup_widget (GTK_WIDGET (GetMainWindow()), "bn_insert")))
  {
    if(event->scroll.direction==0)
    {
      SetToggleButton(MOVE);
    }
    else
    {
      SetToggleButton(DELETE);
    }
    return TRUE;
  }
  
  if(gtk_toggle_tool_button_get_active((GtkToggleToolButton*)lookup_widget (GTK_WIDGET (GetMainWindow()), "bn_delete")))
  {
    if(event->scroll.direction==0)
    {
      SetToggleButton(INSERT);
    }
    else
    {
      SetToggleButton(MOVE);
    }
    return TRUE;
  }

  return FALSE;
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
on_nb_draw_switch_page                 (GtkNotebook     *notebook,
                                        GtkNotebookPage *page,
                                        guint            page_num,
                                        gpointer         user_data)
{
  redraw_page(page_num);
}

void
on_lb_f1_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  SetLabelColor(widget,"f1");
}


void
on_lb_f2_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  SetLabelColor(widget,"f2");
}


void
on_lb_f3_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  SetLabelColor(widget,"f3");
}

void
on_lb_f4_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  SetLabelColor(widget,"f4");
}


void
on_lb_f5_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  SetLabelColor(widget,"f5");
}


void
on_lb_f6_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  SetLabelColor(widget,"f6");
}


void
on_lb_nasals_realize                   (GtkWidget       *widget,
                                        gpointer         user_data)
{
  SetLabelColor(widget,"nasals");
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
  }
  if(!strcmp(_("parallel only"),x))
  {
    FileSetBranches(2);
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
  GdkColor color;

  // save values
//   printf("apply\n");
  if(ConfigFind("prefs_tab1_tmp"))
  {
    w=lookup_widget (GTK_WIDGET (button), "cb_f1");
    g_return_if_fail(w != NULL);
    gtk_color_button_get_color ((GtkColorButton *)w, &color);
    ConfigInsertColor(&color, "f1");

    w=lookup_widget (GTK_WIDGET (button), "cb_f2");
    g_return_if_fail(w != NULL);
    gtk_color_button_get_color ((GtkColorButton *)w, &color);
    ConfigInsertColor(&color, "f2");

    w=lookup_widget (GTK_WIDGET (button), "cb_f3");
    g_return_if_fail(w != NULL);
    gtk_color_button_get_color ((GtkColorButton *)w, &color);
    ConfigInsertColor(&color, "f3");

    w=lookup_widget (GTK_WIDGET (button), "cb_f4");
    g_return_if_fail(w != NULL);
    gtk_color_button_get_color ((GtkColorButton *)w, &color);
    ConfigInsertColor(&color, "f4");

    w=lookup_widget (GTK_WIDGET (button), "cb_f5");
    g_return_if_fail(w != NULL);
    gtk_color_button_get_color ((GtkColorButton *)w, &color);
    ConfigInsertColor(&color, "f5");

    w=lookup_widget (GTK_WIDGET (button), "cb_f6");
    g_return_if_fail(w != NULL);
    gtk_color_button_get_color ((GtkColorButton *)w, &color);
    ConfigInsertColor(&color, "f6");

    w=lookup_widget (GTK_WIDGET (button), "cb_nasals");
    g_return_if_fail(w != NULL);
    gtk_color_button_get_color ((GtkColorButton *)w, &color);
    ConfigInsertColor(&color, "nasals");

    w=lookup_widget (GTK_WIDGET (button), "cb_vs");
    g_return_if_fail(w != NULL);
    gtk_color_button_get_color ((GtkColorButton *)w, &color);
    ConfigInsertColor(&color, "vs");

    w=lookup_widget (GTK_WIDGET (button), "cb_ea");
    g_return_if_fail(w != NULL);
    gtk_color_button_get_color ((GtkColorButton *)w, &color);
    ConfigInsertColor(&color, "ea");

    // set colors on gui-labels
    SetLabelColor(lookup_widget (GTK_WIDGET (GetMainWindow()), "lb_f1"),"f1");
    SetLabelColor(lookup_widget (GTK_WIDGET (GetMainWindow()), "lb_f2"),"f2");
    SetLabelColor(lookup_widget (GTK_WIDGET (GetMainWindow()), "lb_f3"),"f3");
    SetLabelColor(lookup_widget (GTK_WIDGET (GetMainWindow()), "lb_f4"),"f4");
    SetLabelColor(lookup_widget (GTK_WIDGET (GetMainWindow()), "lb_f5"),"f5");
    SetLabelColor(lookup_widget (GTK_WIDGET (GetMainWindow()), "lb_f6"),"f6");
    SetLabelColor(lookup_widget (GTK_WIDGET (GetMainWindow()), "lb_nasals"),"nasals");
  }

  if(ConfigFind("prefs_tab2_tmp"))
  {
    w=lookup_widget (GTK_WIDGET (button), "spn_max_freq");
    val=gtk_spin_button_get_value_as_int ((GtkSpinButton *) w);
    snprintf(tmp,sizeof(tmp),"%d",val);
    ConfigInsert("maxfreq",tmp);
    
    w=lookup_widget (GTK_WIDGET (button), "spn_max_amp");
    val=gtk_spin_button_get_value_as_int ((GtkSpinButton *) w);
    snprintf(tmp,sizeof(tmp),"%d",val);
    ConfigInsert("maxamp",tmp);
    
    w=lookup_widget (GTK_WIDGET (button), "spn_max_band");
    val=gtk_spin_button_get_value_as_int ((GtkSpinButton *) w);
    snprintf(tmp,sizeof(tmp),"%d",val);
    ConfigInsert("maxband",tmp);

    w=lookup_widget (GTK_WIDGET (button), "fb_font");
    snprintf(tmp,sizeof(tmp),"\"%s\"",gtk_font_button_get_font_name ((GtkFontButton *)w));
    ConfigInsert("rulerfont",tmp);
  }

  if(ConfigFind("prefs_tab3_tmp"))
  {
    w=lookup_widget (GTK_WIDGET (button), "ent_play");
    snprintf(tmp,sizeof(tmp),"\"%s\"",gtk_entry_get_text((GtkEntry *)w));
    ConfigInsert("playcmd",tmp);

    w=lookup_widget (GTK_WIDGET (button), "ent_tmp");
    snprintf(tmp,sizeof(tmp),"\"%s\"",gtk_entry_get_text((GtkEntry *)w));
    ConfigInsert("tmpdir",tmp);
  }

  if(ConfigFind("prefs_tab4_tmp"))
  {
    w=lookup_widget (GTK_WIDGET (button), "cb_tool_style");
    val=gtk_combo_box_get_active((GtkComboBox *)w);

    if(val!=-1)
    {
      snprintf(tmp,sizeof(tmp),"%d",val);
      ConfigInsert("toolbarstyle",tmp);
      GuiSetToolbarStyle(val);
    }

    w=(GtkWidget *)lookup_widget (GTK_WIDGET (prefs), "rb_quit");
    if(gtk_toggle_button_get_active ((GtkToggleButton *)w))
    {
      ConfigInsert("showquitdiag","1");
    }
    w=(GtkWidget *)lookup_widget (GTK_WIDGET (prefs), "rb_quit2");
    if(gtk_toggle_button_get_active ((GtkToggleButton *)w))
    {
      ConfigInsert("showquitdiag","0");
    }
  }

  // redraw!!
  w=(GtkWidget *)lookup_widget (GTK_WIDGET (GetMainWindow()), "nb_draw");
  redraw_page(gtk_notebook_get_current_page((GtkNotebook *)w));

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

  on_bn_prefs_cancel_clicked(button,user_data);
}


/** 
 * close preferences dialog (with cancel) and
 * remove all temporary savings from config-list
 * 
 * @param button 
 * @param user_data 
 */
void
on_bn_prefs_cancel_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  ConfigRemove("prefs_tab1_tmp");
  ConfigRemove("prefs_tab2_tmp");
  ConfigRemove("prefs_tab3_tmp");
  ConfigRemove("prefs_tab4_tmp");

  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}

/** 
 * restore default preferences
 * 
 * @param button 
 * @param user_data 
 */
void
on_bn_prefs_default_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
  if(DialogYesNo(_("Really reset preferences?"))==TRUE)
  {
    ConfigFree();
    ConfigNew();
  }
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


void
on_spn_max_freq_realize                (GtkWidget       *widget,
                                        gpointer         user_data)
{
  ConfigInsert("prefs_tab2_tmp","1"); // one for each tab (2)
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
  char tmp[200];  // not good!

  ConfigInsert("prefs_tab3_tmp","1");  // one for each tab (3)

  strcpy(tmp,filtertoken(ConfigGetString("playcmd"),"\""));
//   printf(">%s< - >%s<\n",tmp,g_utf8_normalize(tmp,strlen(tmp),G_NORMALIZE_DEFAULT));
  gtk_entry_set_text ((GtkEntry *) widget, g_utf8_normalize(tmp,strlen(tmp),G_NORMALIZE_DEFAULT));
}


void
on_ent_tmp_realize                     (GtkWidget       *widget,
                                        gpointer         user_data)
{
  char tmp[200];  // not good

  strcpy(tmp,filtertoken(ConfigGetString("tmpdir"),"\""));
  gtk_entry_set_text ((GtkEntry *) widget, g_utf8_normalize(tmp,strlen(tmp),G_NORMALIZE_DEFAULT));
}


void
on_cb_tool_style_realize               (GtkWidget       *widget,
                                        gpointer         user_data)
{
  int foo=ConfigGetInteger("toolbarstyle");  
  ConfigInsert("prefs_tab4_tmp","1"); // one for each tab (4)

  gtk_combo_box_set_active ((GtkComboBox *)widget,foo);
}


void
on_lb_colors_realize                   (GtkWidget       *widget,
                                        gpointer         user_data)
{
  ConfigInsert("prefs_tab1_tmp","1"); // one for each tab (1)
}

void
on_toolbar1_realize                    (GtkWidget       *widget,
                                        gpointer         user_data)
{
  int foo;

  foo=ConfigGetInteger("toolbarstyle");  
  gtk_toolbar_set_style ((GtkToolbar *)widget,foo);
}


void
on_toolbar4_realize                    (GtkWidget       *widget,
                                        gpointer         user_data)
{
  int foo;

  foo=ConfigGetInteger("toolbarstyle");  
  gtk_toolbar_set_style ((GtkToolbar *)widget,foo);
}


void
on_toolbar2_realize                    (GtkWidget       *widget,
                                        gpointer         user_data)
{
  int foo;

  foo=ConfigGetInteger("toolbarstyle");  
  gtk_toolbar_set_style ((GtkToolbar *)widget,foo);
}

void
on_cb_f1_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;

  col = ConfigGetColor("f1");
  gtk_color_button_set_color ((GtkColorButton *)widget, &col);
}

void
on_cb_f2_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;

  col = ConfigGetColor("f2");
  gtk_color_button_set_color ((GtkColorButton *)widget, &col);
}


void
on_cb_f3_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;

  col = ConfigGetColor("f3");
  gtk_color_button_set_color ((GtkColorButton *)widget, &col);
}


void
on_cb_f4_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;

  col = ConfigGetColor("f4");
  gtk_color_button_set_color ((GtkColorButton *)widget, &col);
}


void
on_cb_f5_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;

  col = ConfigGetColor("f5");
  gtk_color_button_set_color ((GtkColorButton *)widget, &col);
}


void
on_cb_f6_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;

  col = ConfigGetColor("f6");
  gtk_color_button_set_color ((GtkColorButton *)widget, &col);
}


void
on_cb_nasals_realize                   (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;

  col = ConfigGetColor("nasals");
  gtk_color_button_set_color ((GtkColorButton *)widget, &col);
}


void
on_cb_vs_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;

  col = ConfigGetColor("vs");
  gtk_color_button_set_color ((GtkColorButton *)widget, &col);
}


void
on_cb_ea_realize                       (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor col;

  col = ConfigGetColor("ea");
  gtk_color_button_set_color ((GtkColorButton *)widget, &col);
}


void
on_fb_font_realize                     (GtkWidget       *widget,
                                        gpointer         user_data)
{
  char tmp[100];
  strcpy(tmp,filtertoken(ConfigGetString("rulerfont"),"\""));
  gtk_font_button_set_font_name((GtkFontButton *)widget, tmp);
}

void
on_rb_quit_realize                     (GtkWidget       *widget,
                                        gpointer         user_data)
{
  if(ConfigGetInteger("showquitdiag")==1)
  {
    gtk_toggle_button_set_active ((GtkToggleButton *)widget,TRUE);
  }
}


void
on_rb_quit2_realize                    (GtkWidget       *widget,
                                        gpointer         user_data)
{
  if(ConfigGetInteger("showquitdiag")==0)
  {
    gtk_toggle_button_set_active ((GtkToggleButton *)widget,TRUE);
  }
}


/** @} */

/* ---------------------------------------------------------------------- */

/** 
    @name execute convert
    @{
*/

void
on_execute1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  char dir[500];  // not good
  char tmp[300];  // not good
  GtkWidget *w = (GtkWidget *)lookup_widget (GTK_WIDGET (GetMainWindow()), "draw_wave");

  strcpy(dir,filtertoken(ConfigGetString("tmpdir"),"\""));
  strcat(dir,"/imskpe.");
  snprintf(tmp,sizeof(tmp),"%d",getpid());
  strcat(dir,tmp);

  strcpy(tmp,dir);
  strcat(tmp,".wav");

  convert(tmp);
  redraw_wave(w);
  strcpy(dir,filtertoken(ConfigGetString("playcmd"),"\""));
  strcat(dir," ");
  strcat(dir,tmp);
  system(dir);
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
    @name set system
    @{
*/

void
on_add_set1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  static GtkWidget *set_add = NULL;

  if (set_add == NULL) 
  {
    set_add = create_imskpe_set_add ();
    /* set the widget pointer to NULL when the widget is destroyed */
    g_signal_connect (G_OBJECT (set_add),
		      "destroy",
		      G_CALLBACK (gtk_widget_destroyed),
		      &set_add);


    SetMainWindow(lookup_widget (GTK_WIDGET (menuitem), "imskpe_main"));
    /* Make sure the dialog doesn't disappear behind the main window. */
    gtk_window_set_transient_for (GTK_WINDOW (set_add), 
				  GTK_WINDOW (GetMainWindow()));
  }

  /* Make sure the dialog is visible. */
  gtk_window_present (GTK_WINDOW (set_add));
}


void
on_bn_set_add_cancel_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}


void
on_bn_set_add_ok_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
//  GtkWidget *menu_items;  
//  GtkWidget *menu;
  char buf[128];
  buf[0]=0;

  strcpy(buf,gtk_entry_get_text((GtkEntry *)lookup_widget (GTK_WIDGET (button), "ent_set_name")));

  if(strlen(buf)>0)
  {
    if(ConfigInsertSet(buf, (char *)GetCurvesShowArray())==TRUE)
    {
      SetAddMenuItem(buf);
    }      
  }
  else
  {
    DialogErrorOK (_("You need insert a name for the set!"));
  }
  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));

}

void
on_delete_set1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  static GtkWidget *set_del = NULL;
  GtkWidget *w;
  
  GtkWidget *menu = lookup_widget (GTK_WIDGET (GetMainWindow()), "sets1_menu");
  GList *l = gtk_container_get_children((GtkContainer *)menu);
  GtkWidget *item;
  GtkWidget *label;
  const char *text;
  
  if (set_del == NULL) 
  {
    set_del = create_imskpe_set_delete ();
    /* set the widget pointer to NULL when the widget is destroyed */
    g_signal_connect (G_OBJECT (set_del),
		      "destroy",
		      G_CALLBACK (gtk_widget_destroyed),
		      &set_del);
    
    /* insert values to cb_set_delete */
    w = lookup_widget (GTK_WIDGET (set_del), "cb_set_delete");
    gtk_combo_box_remove_text       ((GtkComboBox *) w, 0);
    
    // don't use first 3 elements!!
    l = l->next->next->next;
    
    while(l)
    {
      item = l->data;
      label = gtk_bin_get_child (GTK_BIN (item));
      if(label!=NULL)
      {
	text = gtk_label_get_text (GTK_LABEL (label));
	gtk_combo_box_append_text (GTK_COMBO_BOX (w), text);      
      }
      
      l = l->next;
    }
   
    gtk_combo_box_set_active (GTK_COMBO_BOX (w), 0);
   
    /* Make sure the dialog doesn't disappear behind the main window. */
    gtk_window_set_transient_for (GTK_WINDOW (set_del), 
				  GTK_WINDOW (GetMainWindow()));
  }
 
  /* Make sure the dialog is visible. */
  gtk_window_present (GTK_WINDOW (set_del));
}


void
on_bn_set_del_ok_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *item;
  GtkWidget *label;
  const char *text;
  GtkWidget *w;
  gint elem;
  gint cnt=0;
  GtkWidget *menu = lookup_widget (GTK_WIDGET (GetMainWindow()), "sets1_menu");
  GList *l = gtk_container_get_children((GtkContainer *)menu);
  char tmp[100];
  char *s;

  w = lookup_widget (GTK_WIDGET (button), "cb_set_delete");
  elem = gtk_combo_box_get_active ((GtkComboBox *)w);

  // don't use first 3 elements!!
  l = l->next->next->next;

  while(l)
  {
    item = l->data;
    label = gtk_bin_get_child (GTK_BIN (item));
    if(label!=NULL)
    {
      text = gtk_label_get_text (GTK_LABEL (label));
      if(cnt==elem)
      {
	gtk_container_remove((GtkContainer *)menu,(GtkWidget *)item);
	strcpy(tmp,"Set_");
	strcat(tmp,text);
	// convert spaces to _
	s = strchr(tmp,' ');
	while(s != NULL)
	{
	  s[0]='_';
	  s = strchr(tmp,' ');
	}
	ConfigRemove(tmp);
      }
    }
     
    l = l->next;
    cnt++;

  }

  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}

void
on_bn_set_del_cancel_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}

/** @} */

/* ---------------------------------------------------------------------- */

/** 
    @name wav-drawarea
    @{
*/

gboolean
on_draw_wave_configure_event           (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data)
{
  configure_wavarea(widget);

  return FALSE;
}


gboolean
on_draw_wave_expose_event              (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
  redraw_wave_expose(widget,event);

  return FALSE;
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
  if(ConfigGetInteger("showquitdiag")==1)
  {
    if(DialogYesNo(_("Really quit?"))==TRUE)
    {
      CurveListFree(FileGetCurvesPointer());
      ConfigSave();
      ConfigFree();
      gtk_main_quit ();
    }
  }
  else
  {
    CurveListFree(FileGetCurvesPointer());
    ConfigSave();
    ConfigFree();
    gtk_main_quit ();
  }
  return TRUE;
}

/** @} */

void
on_bn_close_allg                       (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}


void
on_bn_refresh_clicked                  (GtkToolButton   *toolbutton,
                                        gpointer         user_data)
{
  repaint_wave();
  GtkWidget *w = (GtkWidget *)lookup_widget (GTK_WIDGET (GetMainWindow()), "draw_wave");
  redraw_wave(w);
//   redraw_wave_expose(widget,event);
}


void
on_interpolate1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  if(DialogYesNo(_("Really interpolate?"))==TRUE)
  {
    CurveInterpolate();
  }
}


void
on_bn_interpol_clicked                 (GtkToolButton   *toolbutton,
                                        gpointer         user_data)
{
  if(DialogYesNo(_("Really interpolate?"))==TRUE)
  {
    CurveInterpolate();
  }
}

