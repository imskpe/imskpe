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
 * @file   main.c
 * @author Andreas Madsack
 * 
 * @brief  main of IMSKPE
 * 
 * 
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "interface.h"
#include "callbacks.h"
#include "support.h"
#include "locale.h"
#include "loadfile.h"
#include "curves.h"
#include "cfg.h"

// #include "klatt80/parwave.h"

/** 
 * main
 *
 * mainfunction
 *
 * \todo add parsing of program-arguments
 * 
 * @param argc 
 * @param argv 
 * 
 * @return 
 */
int main (int argc, char *argv[])
{
  GtkWidget *imskpe_main;
  char *filename;

#ifdef ENABLE_NLS
#ifdef WIN32
  bindtextdomain (GETTEXT_PACKAGE, "..\\share\\locale");
#else
  bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
#endif
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
#endif

  gtk_set_locale ();
  gtk_init (&argc, &argv);

#ifdef WIN32
  add_pixmap_directory ("..\\share\\imskpe\\pixmaps");
#else
  add_pixmap_directory (PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps");
#endif

  ConfigLoad();

  SetMainWindow(NULL);

  MouseEventInit();

  FileInit();

  imskpe_main = create_imskpe_main ();
//   gtk_window_resize (GTK_WINDOW (imskpe_main), ConfigGetInteger("main_window_x"), ConfigGetInteger("main_window_y"));

  gtk_widget_show (imskpe_main);

  SetTitle("unnamed.par");

  if(argc>1)
  {
    filename=g_malloc (sizeof (char)*(strlen(argv[1])+1));
    strncpy(filename,argv[1],strlen(argv[1]));
    filename[strlen(argv[1])]=0;
    FileOpen(filename);
  }
  else
  {
    FileInit();
  }

  InitSplash();

  ConfigShowAllSets();

  gtk_main ();
  return 0;
}

/*! \mainpage IMSKPE Documentation
 *
 * \section intro Introduction
 *
 * IMSKPE is a GTK2-gui for klatt-speech-synthesis
 *
 */

// doxygen comments for generated files:

/**
 * @file   interface.c
 * @author Andreas Madsack
 * 
 * @brief  interface generated by glade-2
 */
/**
 * @file   interface.h
 * @author Andreas Madsack
 * 
 * @brief  interface generated by glade-2
 */
/**
 * @file   config.h
 * @author Andreas Madsack
 * 
 * @brief  global-defines, generated by autoconf
 */
/**
 * @file   support.h
 * @author Andreas Madsack
 * 
 * @brief  support-functions generated by glade-2
 */
/**
 * @file   support.c
 * @author Andreas Madsack
 * 
 * @brief  support-functions generated by glade-2
 */

