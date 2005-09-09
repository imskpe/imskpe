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

/** MouseActionTypes (move, insert, delete)*/
typedef enum {MOVE=1, INSERT=2, DELETE=3} MouseActionTyp;


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
/*    gchar *widget_name; */
    char widget_name[30];
    /** name of formant */
/*    gchar *formant; */
    char formant[20];
    /** show curve or not */
    gboolean show;
    /** in which diagramm paint this curve? */
    diagramTyp dia;
    /** which linestyle to use */
    GdkLineStyle ls;
    /** curvenumber in parfiles */
    int nr;
} typCurveList;


/** structure of lookuptable */
struct CurveMapping {
    char parname[7];
    char formant[20]; // only for colorcodes?
    char widget_name[30];
    diagramTyp dia;
    GdkLineStyle ls;
};

/** structure for mouseeventdata */
typedef struct {
    int thiscurve;
    int thispoint;

    MouseActionTyp thisaction;

/* only for move    */
    gboolean Button1Pressed;

} MouseEventData;

/* curve-functions  */
void CurveInsert(GList *curves,int parid, GList *points);
void CurveListFree(GList *curves);
typCurveList *CurveSearchWidgetName(GList *curve,char *wn);
typCurveList *CurveSearchByNr(GList *curve,int nr);
gboolean SetCurveShow(char *wn);
gboolean GetCurveShowByNum(int num);
gboolean SetCurveShowByNum(int num, gboolean val);
gboolean SetCurveShowArray(char *s);
char *GetCurvesShowArray();
typValueList *GenPoint (gint t, gint v);
gboolean PointDelete (typCurveList *vallist, int time);
gboolean PointInsert(typCurveList *cl, int time, int value);
int PointMove(typCurveList *cl, int otime, int time, int value);
gboolean PointSetDU(typCurveList *cl,int o_time, int time);
void CurveInitStart();
gboolean CurveInterpolate(); // typCurveList *cl);

/* mouseevent-functions  */

void MouseEventInit();
int MouseEventGetCurve();
int MouseEventGetPoint();
void MouseEventSetAction(MouseActionTyp typ);
MouseActionTyp MouseEventGetAction();
gboolean MouseEventSetCurve (int curve);
gboolean MouseEventCheckCurve(int curve);
gboolean MouseEventCheckPoint (int x, int curve);
gboolean MouseEventSetPoint(int x, int curve);
