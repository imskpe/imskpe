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

/**
 * @file   callbacks.h
 * @author Andreas Madsack
 * 
 * @brief  callback-functions
 * 
 * 
 */



#include <gtk/gtk.h>


/* dialog inits*/
void InitDialogSave();
void InitDialogLoad();
void InitDialogPrefs();



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
on_spbn_numF_changed                   (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);

void
on_imskpe_main_activate_default        (GtkWindow       *window,
                                        gpointer         user_data);


// formant init ...
void FormantListInit ();


void
on_lb_f1_realize                       (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_lb_f2_realize                       (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_lb_f3_realize                       (GtkWidget       *widget,
                                        gpointer         user_data);

gboolean
imskpe_quit                            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_spbn_duration_changed               (GtkEditable     *editable,
                                        gpointer         user_data);

gboolean
on_draw_amp_configure_event            (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data);

gboolean
on_draw_amp_expose_event               (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data);

gboolean
on_draw_band_configure_event           (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data);

gboolean
on_draw_band_expose_event              (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data);

gboolean
on_draw_band_motion_notify_event       (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data);

void
on_nb_draw_switch_page                 (GtkNotebook     *notebook,
                                        GtkNotebookPage *page,
                                        guint            page_num,
                                        gpointer         user_data);

void
on_ok_button2_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_cancel_button2_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_bn_about_close_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_bn_open_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_bn_f1_freq_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_lb_f4_realize                       (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_lb_f5_realize                       (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_lb_f6_realize                       (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_lb_nasals_realize                   (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_cm_vs_entry_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_cm_cp_entry_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_spbn_sprate_value_changed           (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);

void
on_spbn_ui_value_changed               (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);

void
on_bn_new_clicked                      (GtkButton       *button,
                                        gpointer         user_data);

void
on_spbn_duration_value_changed         (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);

void
on_cm_voicesource_state_changed        (GtkWidget       *widget,
                                        GtkStateType     state,
                                        gpointer         user_data);

void
on_bn_prefs_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_bn_f1_amp_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_f1_band_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_f1_bandp_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_f2_freq_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_f2_amp_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_f2_band_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_f2_bandp_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_f3_freq_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_f3_amp_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_f3_band_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_f3_bandp_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_f4_freq_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_f4_amp_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_f4_band_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_f4_bandp_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_f5_freq_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_f5_amp_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_f5_band_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_f5_bandp_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_f6_freq_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_f6_amp_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_f6_band_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_f6_bandp_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_nasal_z_freq_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_nasal_z_band_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_nasal_p_freq_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_nasal_p_band_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_nasal_p_amp_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_vs_gopenquot_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_vs_breath_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_vs_spectilt_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_vs_skewness_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_vs_fundfreq_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_examp_voice_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_examp_asp_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_examp_fric_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_examp_bypass_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_examp_voicepar_toggled           (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_examp_siggain_toggled            (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bn_credits_ok_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_bn_about_credits_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_bn_quit_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_bn_open_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_bn_new_clicked                      (GtkButton       *button,
                                        gpointer         user_data);

void
on_bn_prefs_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_pm_move_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_pm_insert_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_pm_delete_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

gboolean
on_draw_freq_button_press_event        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

void
on_bn_move_toggled                     (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data);

void
on_bn_insert_toggled                   (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data);

void
on_bn_delete_toggled                   (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data);

void
on_bn_prefs_cancel_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_bn_prefs_apply_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_bn_prefs_ok_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_draw_freq_button_release_event      (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

void
on_pm_movediag_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_bn_move_cancel_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_bn_move_ok_clicked                  (GtkButton       *button,
                                        gpointer         user_data);




void
on_bn_save_clicked                     (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_bn_saveas_clicked                   (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_convert1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_execute1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_bn_convert_clicked                  (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_bn_execute_clicked                  (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_spn_max_freq_realize                (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_ent_klatt_realize                   (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_spn_max_amp_realize                 (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_spn_max_band_realize                (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_ent_play_realize                    (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_ent_tmp_realize                     (GtkWidget       *widget,
                                        gpointer         user_data);

gboolean
on_draw_amp_button_press_event         (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

gboolean
on_draw_amp_button_release_event       (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

gboolean
on_draw_band_button_press_event        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

gboolean
on_draw_band_button_release_event      (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

void
on_lb_colors_realize                   (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_bn_color_f1_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_bn_color_f2_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_bn_color_f4_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_bn_color_f5_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_bn_color_f6_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_bn_color_nasals_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_bn_color_vc_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_bn_color_ea_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_bn_color_f3_clicked                 (GtkButton       *button,
                                        gpointer         user_data);


void
on_splash_close_clicked                (GtkButton       *button,
                                        gpointer         user_data);
