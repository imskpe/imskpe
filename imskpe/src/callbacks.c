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
#include "graphics.h"
#include "loadfile.h"
#include "curves.h"


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


gboolean
on_draw_freq_configure_event           (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data)
{
  configure_drawarea(widget);
  return TRUE;
}

gboolean
on_draw_amp_configure_event            (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data)
{
  configure_drawarea(widget);
  return TRUE;
}


gboolean
on_draw_band_configure_event           (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data)
{
  configure_drawarea(widget);
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
  
  return TRUE;
}

gboolean
on_draw_amp_motion_notify_event        (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data)
{
  return FALSE;
}

gboolean
on_draw_band_motion_notify_event       (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data)
{

  return FALSE;
}

void
on_bn_move_toggled                     (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active (togglebutton) == TRUE)
  {
    gtk_toggle_button_set_active((GtkToggleButton*)lookup_widget (GTK_WIDGET (togglebutton), "bn_insert"),FALSE);
    gtk_toggle_button_set_active((GtkToggleButton*)lookup_widget (GTK_WIDGET (togglebutton), "bn_delete"),FALSE);
  }
/*  else
  {
    gtk_toggle_button_set_active(togglebutton,TRUE);
    }*/
}


void
on_bn_insert_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active (togglebutton) == TRUE)
  {
    gtk_toggle_button_set_active((GtkToggleButton*)lookup_widget (GTK_WIDGET (togglebutton), "bn_move"),FALSE);
    gtk_toggle_button_set_active((GtkToggleButton*)lookup_widget (GTK_WIDGET (togglebutton), "bn_delete"),FALSE);
  }
/*  else
  {
    gtk_toggle_button_set_active(togglebutton,TRUE);
    }*/
}


void
on_bn_delete_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if(gtk_toggle_button_get_active (togglebutton) == TRUE)
  {
    gtk_toggle_button_set_active((GtkToggleButton*)lookup_widget (GTK_WIDGET (togglebutton), "bn_insert"),FALSE);
    gtk_toggle_button_set_active((GtkToggleButton*)lookup_widget (GTK_WIDGET (togglebutton), "bn_move"),FALSE);
  }
/*  else
  {
    gtk_toggle_button_set_active(togglebutton,TRUE);
    }*/
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
				    GTK_WINDOW (GetMainWindow()));
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
  redraw_all_drawareas();
}



void
on_ok_button2_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  // set filename 
  char *filename;

  int len = strlen(gtk_file_selection_get_filename (GTK_FILE_SELECTION (gtk_widget_get_toplevel (GTK_WIDGET (button)))));

  filename = (char *) malloc(sizeof(char)*(len+2));
 
  filename = (char *) gtk_file_selection_get_filename 
      (GTK_FILE_SELECTION (gtk_widget_get_toplevel (GTK_WIDGET (button))));
  

  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));

  // and start import ...
  SetStatusBar("sb_file", filename);
  FileOpen(filename);
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
on_bn_f1_freq_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

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
  redraw_all_drawareas();
  FileSetDuration((unsigned int)gtk_spin_button_get_value_as_int(spinbutton));
}


