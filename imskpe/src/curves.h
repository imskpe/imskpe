/*
    curves.h - Part of IMSKPE

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
 * @file   curves.h
 * @author Andreas Madsack
 * 
 * @brief  curve functions
 */

/** the 3 diagram-types */
typedef enum {FREQUENCIES=1, AMPLITUDE=2, BANDWIDTH=3} diagramTyp;


/** struct for points on curves */
typedef struct {
    gint time;
    gint value;
} typValueList;


/** struct for list over curves 
 *
 * \todo sizes of char-arrays static?! sizes ok?
*/
typedef struct {
    /** list over points */
    GList *points;
    /** name of togglebutton */
//    gchar *widget_name; 
    char widget_name[30];
    /* name in PAR-Files */
//    gchar *parname;
//    char parname[7];
    /** name of formant */
//    gchar *formant;
    char formant[20];
    /** show curve or not */
    gboolean show;
} typCurveList;


/** structure of lookuptable */
struct CurveMapping {
    char parname[7];
    char formant[20]; // only for colorcodes?
    char widget_name[30];
    diagramTyp dia;
};


typCurveList CurveInsert(GList *curves,int parid, GList *points);
void CurveListFree(GList *curves);
GList *CurveSearchWidgetName(GList *curve,char *wn);
typValueList *PointInsert (gint t, gint v);
void CurveInitStart();
