#include <gtk/gtk.h>


gboolean
on_imskpe_main_delete_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_new1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_open1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_save1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_save_as1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_bn_move_toggled                     (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_insert_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_delete_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_fX_color_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_draw_freq_configure_event           (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data);

gboolean
on_draw_freq_expose_event              (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data);

gboolean
on_draw_freq_motion_notify_event       (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data);

gboolean
on_draw_amp_motion_notify_event        (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data);

gboolean
on_imskpe_colorsel_destroy_event       (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_ok_button1_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_cancel_button1_clicked              (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_color_selection1_configure_event    (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data);

void
on_color_selection1_color_changed      (GtkColorSelection *colorselection,
                                        gpointer         user_data);

void
on_spbn_numF_changed                   (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);

void
on_imskpe_main_activate_default        (GtkWindow       *window,
                                        gpointer         user_data);
