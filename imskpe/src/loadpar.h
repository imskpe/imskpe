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
 * @file   loadpar.h
 * @author Andreas Madsack
 * 
 * @brief  parser for PAR-Files
 * 
 * 
 */


// #include <gtk/gtk.h>
#include "curves.h" // for typValueList

/*
procedures
*/

#define COMMENT 257
#define EOC 258
#define VARNAME 259
#define VALUE 260
#define LINEID 261
#define DITEM 262
#define EOL 263
#define KPE 264
#define IMSKPE 265
#define UNKNOWN 299

#define PARAMETERS 40

/** ParList-struct */
typedef struct {
    int last1;
    int last2;
    GList *points;  /**< typValueList Liste */
    typValueList *ptmp; /**< maybe next to insert File-struct*/
} typParList;


void LoadPar (char *filename);
