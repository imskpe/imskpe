#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"


/***

 erstmal hier ablegen ...
 mal sehen wo es später hinkommt ...

***/

typedef struct {

    GdkDrawable *pixmap;
    GdkGC *gc;

} typGraphics;

typGraphics *NewGraphics ();
typGraphics *g = NULL;

int nScreenWidth = 200;
int nScreenHeight = 200;

GtkWidget *main_window;

/*
 * NewGraphics
 *
 * Allocate space for our graphics object
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
GdkGC *GetPen (int nRed, int nGreen, int nBlue)
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

void Repaint(GtkWidget *d)
{
    GdkRectangle  update_rect;

    /* --- clear pixmap --- */
/*    gdk_draw_rectangle (g->pixmap,
			GetPen (0xafff, 0xafff, 0xafff),
			TRUE,
			0, 0,
			d->allocation.width,
			d->allocation.height);
*/
    /* --- Draw molecule in the background --- */
/*    paint (g);*/

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


/* ----------------------------------------------- */


void
on_new1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_open1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

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
on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
}


void
on_button2_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  GdkColor color;
  GtkWidget *widget= lookup_widget (GTK_WIDGET (button), "lb_f1");
  gdk_color_parse ("red", &color);
  gtk_widget_modify_fg (widget, GTK_STATE_ACTIVE, &color);
  gtk_widget_modify_fg (widget, GTK_STATE_NORMAL, &color);
}


gboolean
on_draw_freq_configure_event           (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data)
{
  /* --- Structure doesn't exist? --- */
  if (g == NULL) {
    
    /* --- Create one --- */
    g = NewGraphics ();
  }

  /* --- Existing pixmap?  --- */
  if (g->pixmap) {
    
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

  /* --- Redraw molecule --- */
  Repaint (widget);

  return TRUE;

}


gboolean
on_draw_freq_expose_event              (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
  gdk_draw_pixmap(widget->window,
            widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
            g->pixmap,
            event->area.x, event->area.y,
            event->area.x, event->area.y,
            event->area.width, event->area.height);

  return TRUE;
}


gboolean
on_draw_freq_motion_notify_event       (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data)
{
  int x, y;
  GdkModifierType state;

  /* --- If it's a hint... (combining several events) --- */
  if (event->is_hint) {
    
    /* --- Get new position --- */
    gdk_window_get_pointer (event->window, &x, &y, &state);
  } else {
    
    /* --- Get new position --- */
    x = event->x;
    y = event->y;
    state = event->state;
  }
/*  g_printf("%d / %d\n",x,y); */
  
  /* --- If the mouse button is down --- */
  if (state & GDK_BUTTON1_MASK && g->pixmap != NULL) {
    
    /* --- Calculate the mouse drag effect on the  
     *     molecule.
     */
    gdk_draw_line (g->pixmap, GetPen (0xffff, 0, 0) ,
		   widget->allocation.width/2, 
		   widget->allocation.height/2,
		   (int) x, 
		   (int) y);
    Repaint(widget);
  } 
  
  return TRUE;
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
  gtk_main_quit ();
  return FALSE;
}

gboolean
on_draw_amp_motion_notify_event        (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data)
{
  int x,y;
  GdkModifierType state;

  /* --- If it's a hint... (combining several events) --- */
  if (event->is_hint) {
    
    /* --- Get new position --- */
    gdk_window_get_pointer (event->window, &x, &y, &state);
  } else {
    
    /* --- Get new position --- */
    x = event->x;
    y = event->y;
    state = event->state;
  }
  g_printf("%d / %d\n",x,y);
  
  return TRUE;
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
  if(main_window != NULL)
  {
    gdouble color[3];
    GdkColor gdk_color;
  
    GtkNotebook *nb=(GtkNotebook*) lookup_widget (GTK_WIDGET (main_window), "nb_resonators");
    GtkWidget *child=gtk_notebook_get_nth_page(nb, gtk_notebook_get_current_page(nb));
    GtkWidget *widget=gtk_notebook_get_tab_label(nb,child);
    

/* Get current color */
    
    gtk_color_selection_get_color (colorselection,color);
    
  /* Fit to a unsigned 16 bit integer (0..65535) and insert into the GdkColor structure */

    gdk_color.red = (guint16)(color[0]*65535.0);
    gdk_color.green = (guint16)(color[1]*65535.0);
    gdk_color.blue = (guint16)(color[2]*65535.0);

/*    g_printf("%d / %d / %d\n",gdk_color.red,gdk_color.green,gdk_color.blue);
      g_printf("%d\n",gtk_notebook_get_current_page(nb));
*/
    
    GtkWidget *w= lookup_widget (GTK_WIDGET (main_window), "lb_f1");

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

      main_window = lookup_widget (GTK_WIDGET (button), "imskpe_main");
      /* Make sure the dialog doesn't disappear behind the main window. */
      gtk_window_set_transient_for (GTK_WINDOW (color), 
				    GTK_WINDOW (main_window));
      /* Do not allow user to resize the dialog */
      gtk_window_set_resizable (GTK_WINDOW (color), FALSE);
    }

  /* Make sure the dialog is visible. */
  gtk_window_present (GTK_WINDOW (color));
}

