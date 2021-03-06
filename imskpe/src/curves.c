/*
  curves.c - Part of IMSKPE

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
 * @file   curves.c
 * @author Andreas Madsack
 * 
 * @brief  curve functions
 *
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib/gprintf.h>
#include <string.h>
#include <math.h>
#include <malloc.h>

#include "loadfile.h"
#include "curves.h"
#include "cfg.h"
#include "support.h"
#include "graphics.h"

/** 
 * Insert one Curve to curvelist
 * 
 * @param curves List of curves
 * @param parid the curve is for which parameter
 * @param points Points to add to this curve
 */
void CurveInsert(GList *curves, int parid, GList *points)
{
  /** CurveMappings

  2nd column:
  vs=voice-source  <- maybe not enough colors ...
  ea=exp. amp.
  
  */
  struct CurveMapping curvetable [] = {
      {"f0","vs","bn_vs_fundfreq",1,GDK_LINE_SOLID},
      {"av","ea","bn_examp_voice",2,GDK_LINE_SOLID},
      {"f1","f1","bn_f1_freq",1,GDK_LINE_SOLID},
      {"b1","f1","bn_f1_band",3,GDK_LINE_SOLID},
      {"f2","f2","bn_f2_freq",1,GDK_LINE_SOLID},
      {"b2","f2","bn_f2_band",3,GDK_LINE_SOLID},
      {"f3","f3","bn_f3_freq",1,GDK_LINE_SOLID},
      {"b3","f3","bn_f3_band",3,GDK_LINE_SOLID},
      {"f4","f4","bn_f4_freq",1,GDK_LINE_SOLID},
      {"b4","f4","bn_f4_band",3,GDK_LINE_SOLID},
      {"f5","f5","bn_f5_freq",1,GDK_LINE_SOLID},
      {"b5","f5","bn_f5_band",3,GDK_LINE_SOLID},
      {"f6","f6","bn_f6_freq",1,GDK_LINE_SOLID},
      {"b6","f6","bn_f6_band",3,GDK_LINE_SOLID},
      {"fnz","nasal","bn_nasal_z_freq",1,GDK_LINE_SOLID},
      {"bnz","nasal","bn_nasal_z_band",3,GDK_LINE_SOLID},
      {"fnp","nasal","bn_nasal_p_freq",1,GDK_LINE_SOLID},
      {"bnp","nasal","bn_nasal_p_band",3,GDK_LINE_SOLID},
      {"asp","ea","bn_examp_asp",2,GDK_LINE_SOLID},
      {"kopen","vs","bn_vs_gopenquot",2,GDK_LINE_SOLID},  // no dia? -> 2
      {"aturb","vs","bn_vs_breath",2,GDK_LINE_SOLID},
      {"tilt","vs","bn_vs_spectilt",2,GDK_LINE_SOLID},
      {"af","ea","bn_examp_fric",2,GDK_LINE_SOLID},
      {"skew","vs","bn_vs_skewness",2,GDK_LINE_SOLID},  // no dia? -> 2
      {"a1","f1","bn_f1_amp",2,GDK_LINE_SOLID},
      {"b1p","f1","bn_f1_bandp",3,GDK_LINE_ON_OFF_DASH},
      {"a2","f2","bn_f2_amp",2,GDK_LINE_SOLID},
      {"b2p","f2","bn_f2_bandp",3,GDK_LINE_ON_OFF_DASH},
      {"a3","f3","bn_f3_amp",2,GDK_LINE_SOLID},
      {"b3p","f3","bn_f3_bandp",3,GDK_LINE_ON_OFF_DASH},
      {"a4","f4","bn_f4_amp",2,GDK_LINE_SOLID},
      {"b4p","f4","bn_f4_bandp",3,GDK_LINE_ON_OFF_DASH},
      {"a5","f5","bn_f5_amp",2,GDK_LINE_SOLID},
      {"b5p","f5","bn_f5_bandp",3,GDK_LINE_ON_OFF_DASH},
      {"a6","f6","bn_f6_amp",2,GDK_LINE_SOLID},
      {"b6p","f6","bn_f6_bandp",3,GDK_LINE_ON_OFF_DASH},
      {"anp","nasal","bn_nasal_p_amp",2,GDK_LINE_SOLID},
      {"ab","ea","bn_examp_bypass",2,GDK_LINE_SOLID},
      {"avp","ea","bn_examp_voicepar",2,GDK_LINE_SOLID},
      {"gain","ea","bn_examp_siggain",2,GDK_LINE_SOLID}
  };
  GList *pl=NULL;
  typCurveList *eval=NULL;
  typValueList *pval=NULL;
  int tmp;
    
//    printf("%s / %s / %s / %d - %d\n",
//  	 curvetable[parid].parname,
//  	 curvetable[parid].formant,
//  	 curvetable[parid].widget_name,
//  	 curvetable[parid].dia,
//  	 g_list_length(points));

  eval=(typCurveList *)CurveSearchWidgetName((GList *)curves,(char *)curvetable[parid].widget_name);

  if(eval==NULL)
  {
    eval = g_malloc (sizeof (typCurveList));
    eval->points=points;

    tmp=strlen(curvetable[parid].formant);
    curvetable[parid].formant[tmp]=0;
    if(tmp>20) tmp=20;
    strncpy(eval->formant,curvetable[parid].formant,tmp+1);

    tmp=strlen(curvetable[parid].widget_name);
    curvetable[parid].widget_name[tmp]=0;
    if(tmp>30) tmp=30;
    strncpy(eval->widget_name,curvetable[parid].widget_name,tmp+1);
    
    eval->dia=curvetable[parid].dia;
    eval->ls=curvetable[parid].dia;

    eval->show=GuiGetToggleButtonState(curvetable[parid].widget_name);

    eval->nr=parid;

    curves = g_list_append(curves,eval);
  }
  else
  {
    /* free all points */
    pl=g_list_first(eval->points);
    while(pl)
    {	
      pval = (typValueList *) pl->data;

      if(pval!=NULL)
      {
	free(pval);
      }

      pl = g_list_remove(pl,pval);
    }

    /* new points */
    eval->points=points;        
    /* no need to set formant/widget_name/show */
  }
  // Set curvespointer
  FileSetCurvesPointer(curves);
}

/** 
 * freeing a list of curves (and ALL points on EACH curve)
 * 
 * @param curves List of curves to be freed
 */
void CurveListFree(GList *curves)
{
  typCurveList *cdata;
  GList *cl;
  typValueList *pval;
  GList *pl;

  cl=g_list_first(curves);
  while(cl)
  {
    cdata = (typCurveList *) cl->data; 
    
    pl=g_list_first(cdata->points);
    while(pl)
    {	
      pval = (typValueList *) pl->data;

      if(pval!=NULL)
      {
	free(pval);
      }
      pl = (GList *)g_list_remove(pl,pval);
    }  

    if(cdata!=NULL)
    {
      free(cdata);
    }

    cl = g_list_remove(cl,cdata);
  }
  FileSetCurvesPointer(NULL);
}

/** 
 * returns searched element in curve-list with correct WidgetName
 * if not found returns NULL
 * 
 * @param curve List of Curves to search on
 * @param wn search for this widgetname
 * 
 * @return a Curvelist
 */
typCurveList *CurveSearchWidgetName(GList *curve,char *wn)
{
  typCurveList *thiscurve;  
  GList *cl;
  char tmp[30];

  cl=g_list_first (curve);

  while(cl)
  {	
    thiscurve = cl->data;

    strncpy(tmp,thiscurve->widget_name,30);
    if(!strcasecmp(tmp,wn))
    {
      return thiscurve;
    }

    cl=cl->next;
  }
  return NULL;

}

/** 
 * searches searched element in curvelist with correct nr
 * if not found returns NULL
 * 
 * @param curve 
 * @param nr 
 * 
 * @return (typCurveList *)
 */
typCurveList *CurveSearchByNr(GList *curve,int nr)
{
  
  typCurveList *thiscurve;  
  GList *cl;

  cl=g_list_first (curve);

  while(cl)
  {	
    thiscurve = cl->data;

    if(nr==thiscurve->nr)
    {
      return thiscurve;
    }

    cl=cl->next;
  }
  return NULL;
}

/** 
 * sets a curve-show-flag
 * 
 * @param wn 
 * 
 * @return 
 */
gboolean SetCurveShow(char *wn)
{
  GtkWidget *w;
  int page;

  typCurveList *c = CurveSearchWidgetName((GList *)FileGetCurvesPointer(),wn);

  if(c!=NULL)
  {
    w=(GtkWidget *)lookup_widget (GTK_WIDGET (GetMainWindow()), "nb_draw");
    if(c->show==TRUE)
    {
      c->show=FALSE;
      redraw_page(gtk_notebook_get_current_page((GtkNotebook *)w));
      return TRUE;
    }
    else
    {
      c->show=TRUE;
      page=c->dia-1;
      gtk_notebook_set_current_page((GtkNotebook *)w,page);
      redraw_page(page);
      return TRUE;
    }
  }

  printf("curve not found: -%s-\n",wn);
  return FALSE;
}

/** 
 * returns TRUE if curve is shown.
 * 
 * @param num 
 * 
 * @return 
 */
gboolean GetCurveShowByNum(int num)
{
  typCurveList *c = CurveSearchByNr((GList *)FileGetCurvesPointer(),num);

  if(c!=NULL)
  {
    return c->show;
  }

  printf("curve not found: -%d-\n",num);
  return FALSE;
}

/** 
 * sets if a curve is shown on number of curve using gtk_toggle_button_set_active.
 * 
 * @param num 
 * @param val 
 * 
 * @return 
 */
gboolean SetCurveShowByNum(int num, gboolean val)
{
  typCurveList *c = CurveSearchByNr((GList *)FileGetCurvesPointer(),num);

  if(val==TRUE || val==FALSE)
  {
    if(c!=NULL)
    {
      if(c->show!=val)
      {
	// the toggled-event sets value in curves-list!!
	gtk_toggle_button_set_active((GtkToggleButton*)lookup_widget (GTK_WIDGET (GetMainWindow()), c->widget_name),val);
      }
      return TRUE;
    }
  }
  printf("curve not found: -%d-\n",num);
  return FALSE;
}

/** 
 * uses array of 40x 0/1-values to enable/disable curves
 * 
 * @param s 
 * 
 * @return 
 */
gboolean SetCurveShowArray(char *s)
{
  GtkWidget *w;
  int i;

  // first char is a 'B' after that 40x 0 or 1
  for(i=1;i<41;i++)
  {  
    if(SetCurveShowByNum(i-1,s[i]-48)==FALSE)
    {
      return FALSE;
    }
  }

  w=(GtkWidget *)lookup_widget (GTK_WIDGET (GetMainWindow()), "nb_draw");
  redraw_page(gtk_notebook_get_current_page((GtkNotebook *)w));

  return TRUE;
}

/** 
 * returns an array with curves-show-true/false-values (0 or 1)
 * i.e. 0000000000000000000000000000000000000000 for all false
 * 
 * @return 
 */
char *GetCurvesShowArray()
{
  int i;
  char *buf;

  /* yes 10 bytes to much */
  buf = (char *) g_malloc (sizeof (char)*50);

  for(i=0;i<50;i++)
  {
    buf[i]=0; 
  }

  for(i=0;i<40;i++)
  {  
    if(GetCurveShowByNum(i)==TRUE)
    {
      buf[i]='1';
    }
    else
    {
      buf[i]='0';
    }
  }
  return buf;
}


/** 
 * Inserts a Point in an typCurveList/typParList and 
 * returns the pointer to it.
 *
 * @param t 
 * @param v 
 * 
 * @return 
 */
typValueList *GenPoint (gint t, gint v)
{
  typValueList *vl;

  vl = g_malloc (sizeof (typValueList));
  vl->time=t;
  vl->value=v;
  return vl;
}

/** 
 * deletes an point in value-list
 * 
 * @param cl 
 * @param time 
 */
gboolean PointDelete (typCurveList *cl, int time)
{
  GList *vl=(GList *)g_list_first(cl->points);
  typValueList *v;  
  int du = FileGetDuration();
  int ui=FileGetUpdateInterval();

  while(vl)
  {
    v=vl->data;
    if(v->time==time)
    {
      /* test if its the first point  */
      if(time==0)
      {
	return FALSE;
      }
      if(time>=du-ui && vl->next==NULL)
      {
	return FALSE;
      }

      if(v!=NULL)
      {
	free(v);
      }
      FileSetIsChanged(TRUE);
      
      vl = g_list_remove(vl,v);
      return TRUE;
    }
    else
    {
      vl=vl->next;
    }
  }
  return FALSE;
}

/** 
 * Move one Point on a curvelists-points
 * 
 * @param cl 
 * @param otime 
 * @param time 
 * @param value 
 * 
 * @return 
 */
int PointMove(typCurveList *cl, int otime, int time, int value)
{
  GList *vl=(GList *)g_list_first(cl->points);
  typValueList *v,*v2;  
  int ui=FileGetUpdateInterval();
  typValueList p_pnt;  
  p_pnt.time=0;
  p_pnt.value=-1;
  int v2time;

  while(vl)
  {
    v=vl->data;
    if(v->time==otime)
    {    
      // point found
      
      // check if time is between range!

      if(vl->next!=NULL)
      {
	v2=vl->next->data;
	v2time=v2->time;
      }
      else
      {
	v2time=v->time;
      }

      if((((p_pnt.time+ui)<time) && (v2time>(time+ui))))
      {
	if(otime!=0 && otime!=v2time)
	{
	  v->time=time;
	}
      }
      
      if(value>=0)
      {
	v->value=value;
      }
      FileSetIsChanged(TRUE);
      return v->time;
    }
    else
    {
      p_pnt.time=v->time;
      p_pnt.value=v->value;
      vl=vl->next;
    }
  }
  return -1;
}

/** 
 * sets new duration-maximum. all time values bigger as new duration 
 * were set to ew duration
 * not the best solution. needs to be improved!
 * especially, if more than one point is bigger as new DU, 
 * only one is needed, or not?
 * 
 * @param cl 
 * @param o_time 
 * @param time 
 * 
 * @return TRUE if ok, else FALSE
 */
gboolean PointSetDU(typCurveList *cl,int o_time, int time)
{
  GList *vl=(GList *)g_list_first(cl->points);
  typValueList *v;  
  typValueList p_pnt;  

  while(vl)
  {
    v=vl->data;
    if(v->time==o_time)
    {    
      v->time=time;

      FileSetIsChanged(TRUE);
      return TRUE;
    }
    else
    {
      p_pnt.time=v->time;
      p_pnt.value=v->value;
      vl=vl->next;
    }
  }
  return FALSE;
}


/** 
 * Insert one point to a curvelists-points
 * 
 * @param cl 
 * @param time 
 * @param value 
 * 
 * @return 
 */
gboolean PointInsert(typCurveList *cl, int time, int value)
{
  GList *vl=(GList *)g_list_first(cl->points);
//  GList *vl2;
  typValueList *v;  
//  typValueList *v2;  
  int ui=FileGetUpdateInterval();

  typValueList p_pnt;
  typValueList pnt;
  typValueList n_pnt;
   
  p_pnt.time=-1;
  pnt.time=-1;
  n_pnt.time=-1;
   
  while(vl)
  {
    v=vl->data;

    if(p_pnt.time<0) {
      if(pnt.time<0) {
	pnt.time=v->time;
	pnt.value=v->value;
      }
      else {
	p_pnt.time=pnt.time;
	p_pnt.value=pnt.value;
	pnt.time=v->time;
	pnt.value=v->value;
      }
    }
    else {
      p_pnt.time=pnt.time;
      p_pnt.value=pnt.value;
      pnt.time=v->time;
      pnt.value=v->value;
    }

    if(time>p_pnt.time && time<pnt.time) 
    {
      if(!(p_pnt.time+ui>time) && !(pnt.time-ui<time))
      {
	g_list_insert_before((GList *)g_list_first(cl->points),vl,(typValueList *)GenPoint (time,value));
	FileSetIsChanged(TRUE);
      }
      else
      {
	SetStatusBar ("sb_state","last msg: ui-delta not enough for new point");
      }
      return TRUE;
    }

    vl=vl->next;
  }
  return FALSE;
}


/** 
 * Init default Curves
 *
 * using DU from File-struct to get 2nd point!
 * setting no other values, except curve!
 */
void CurveInitStart()
{
  int starttable [] = {
      100,59,500,59,1500,89,
      2500,149,3250,200,3700,
      200,4990,500,280,89,280,
      89,0,40,0,0,0,0,59,59,59,
      89,59,149,59,200,59,200,
      59,500,0,0,59,59};
//       100,59,500,59,1500,89,
//       1500,149,3250,200,3700,
//       200,4990,500,6680,439,6680,
//       489,0,40,0,0,0,0,0,60,0,90,
//       0,150,0,200,0,200,
//       0,500,0,0,60,60};
//  typCurveList *curve;
  unsigned int du;
  GList *points=NULL;
  int i;

  CurveListFree((GList *)FileGetCurvesPointer()); /* ok here?  */

  du=FileGetDuration();


  for(i=0;i<40;i++)
  {
    points=NULL;
    points = g_list_append(points, (typValueList *)GenPoint(0,starttable[i]));
    points = g_list_append(points, (typValueList *)GenPoint(du,starttable[i]));
    CurveInsert((GList *)FileGetCurvesPointer(i), i, points);
  }

}


/** 
 * interpolates all unnecessary points on all curves
 * 
 * idea: calculating gradient for pp_pnt/pnt vs pp_pnt/p_pnt
 * \todo add some kind of configurable-option in preferences for the tolerance of the interpolation. In dB and Hz!
 *
 * @param cl curvelist to operate on, if NULL get curvelist from active file
 * 
 * @return 
 */
gboolean CurveInterpolate()
{
  typCurveList *cdata;
  GList *cl;
  typValueList *pv;
  typValueList *p_pv;
  GList *pl;
  GList *p_pl;

  typValueList pnt;
  typValueList p_pnt;
  typValueList pp_pnt;

  double a,b;

  cl=g_list_first((GList *)FileGetCurvesPointer());
  while(cl)
  {
    cdata = (typCurveList *) cl->data; 
    
    pnt.time=-1;
    p_pnt.time=-1;
    pp_pnt.time=-1;

    pl=g_list_first(cdata->points);
    while(pl)
    {	
      pv = (typValueList *) pl->data;

      if(pp_pnt.time<0) {
	if(p_pnt.time<0) {
	  if(pnt.time<0) {
	    pnt.time=pv->time;
	    pnt.value=pv->value;
	  }
	  else {
	    p_pnt.time=pnt.time;
	    p_pnt.value=pnt.value;
	    pnt.time=pv->time;
	    pnt.value=pv->value;
	  }
	}
	else {
	  pp_pnt.time=p_pnt.time;
	  pp_pnt.value=p_pnt.value;
	  p_pnt.time=pnt.time;
	  p_pnt.value=pnt.value;
	  pnt.time=pv->time;
	  pnt.value=pv->value;
	}
      }
      else {
	pp_pnt.time=p_pnt.time;
	pp_pnt.value=p_pnt.value;
	p_pnt.time=pnt.time;
	p_pnt.value=pnt.value;
	pnt.time=pv->time;
	pnt.value=pv->value;
      }
      
      if(pp_pnt.time>0)
      {
// 	printf("%20s %4.2f %4.2f ",
// 	       cdata->widget_name,
// 	       ((double)(pnt.value-pp_pnt.value)/(double)(pnt.time-pp_pnt.time)),
// 		((double)(p_pnt.value-pp_pnt.value)/(double)(p_pnt.time-pp_pnt.time))
// 	    );
	a = ((double)(pnt.value-pp_pnt.value)/(double)(pnt.time-pp_pnt.time));
	b = ((double)(p_pnt.value-pp_pnt.value)/(double)(p_pnt.time-pp_pnt.time));
//	printf(" - %4.2f %4.2f - ",a,b);

	if(fabs(fabs(a)-fabs(b)) < 0.05)  /* tolerance-value  */
	{
	  /* delete previous point */
	  p_pl = pl->prev;
	  p_pv = (typValueList *) p_pl->data;

	  if(p_pv!=NULL)
	  {
	    free(p_pv);
	  }
// 	  printf("D");

	  p_pl = (GList *)g_list_remove(p_pl,p_pv);
	  pl=p_pl->next;
	}
	else
	{
	  pl=pl->next;
	}
//	printf("\n");
      }
      else
      {
	pl=pl->next;
      }
    }  
    cl=cl->next;
  }
  redraw_page(0);
  redraw_page(1);
  redraw_page(2);
  
  return TRUE;
}



/**
 * \todo put this stuff in an extra c-file?
 * 
 */
/**
 * MouseEventStructureFunctions ...
   @name mouse events
   @{ */ 


MouseEventData mouseevent;


void MouseEventInit()
{
  mouseevent.thiscurve=-1;
  mouseevent.thispoint=-1;
  
  mouseevent.thisaction=MOVE;
}

int MouseEventGetCurve()
{
  return mouseevent.thiscurve;
}

int MouseEventGetPoint()
{
  return mouseevent.thispoint;
}

void MouseEventSetAction(MouseActionTyp typ)
{
  mouseevent.thisaction=typ;
}

MouseActionTyp MouseEventGetAction()
{
  return mouseevent.thisaction;
}

gboolean MouseEventSetCurve (int curve)
{
  if(mouseevent.thiscurve!=curve) {
    mouseevent.thiscurve=curve;
    return TRUE;
  } else {
    return FALSE;
  }
}

gboolean MouseEventCheckCurve(int curve)
{
  if(curve==mouseevent.thiscurve)
  {
    return TRUE;
  }
  return FALSE;
}

gboolean MouseEventSetPoint (int x, int curve)
{
  if(mouseevent.thispoint!=x) {
    mouseevent.thispoint=x;
//    mouseevent.thiscurve=curve;
    return TRUE;
  } else {
    return FALSE;
  }
}

gboolean MouseEventCheckPoint(int x, int curve)
{
  if(x==mouseevent.thispoint && curve==mouseevent.thiscurve)
  {
    return TRUE;
  }
  return FALSE;
}


/** @} */

