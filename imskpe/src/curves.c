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
warning on AMD64:

curves.c: In function `SetCurveShow':
curves.c:272: warning: cast to pointer from integer of different size
curves.c:276: warning: cast to pointer from integer of different size
curves.c:276: warning: cast to pointer from integer of different size
curves.c: In function `CurveInitStart':
curves.c:535: warning: cast to pointer from integer of different size
curves.c:546: warning: cast to pointer from integer of different size

 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib/gprintf.h>

#include "curves.h"
#include "cfg.h"

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
  // Set curves
  FileSetCurvesPointer(curves);
}

/** 
 * CurvesListFree
 *
 * freeing a list of curves (and all points on each curve)
 * 
 * @param curves 
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
 * CurveSearchWidgetName
 *
 * returns searched element in curve-list with correct WidgetName
 * if not found returns NULL
 * 
 * @param curve 
 * @param wn 
 * 
 * @return 
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
 * CurveSearchByNr
 *
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
 * SetCurveShow
 * 
 * @param wn 
 * 
 * @return 
 */
gboolean SetCurveShow(char *wn)
{
  GtkWidget *w;

  typCurveList *c = CurveSearchWidgetName((GList *)FileGetCurvesPointer(),wn);

  if(c!=NULL)
  {
    w=(GtkWidget *)lookup_widget (GTK_WIDGET (GetMainWindow()), "nb_draw");
    if(c->show==TRUE)
    {
//       printf("%s - set false\n",c->widget_name);
      c->show=FALSE;
      redraw_page(gtk_notebook_get_current_page((GtkNotebook *)w));
      return FALSE;
    }
    else
    {
//       printf("%s - set true\n",c->widget_name);
      c->show=TRUE;
      redraw_page(gtk_notebook_get_current_page((GtkNotebook *)w));
      return TRUE;
    }
  }
  else
  {
    printf("curve not found: -%s-\n",wn);
    return FALSE;
  }
}


/** 
 * GenPoint
 *
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
 * PointDelete
 *
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
//       printf("%5d/%5d -> %5d/%5d\n",v->time,v->value,time,value);
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


gboolean PointInsert(typCurveList *cl, int time, int value)
{
  GList *vl=(GList *)g_list_first(cl->points);
  GList *vl2;
  typValueList *v;  
  typValueList *v2;  
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
 * Init default Curve
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
  typCurveList *curve;
  unsigned int du;
  GList *points=NULL;
  int i;

  CurveListFree((GList *)FileGetCurvesPointer()); /* ok here?  */

  du=FileGetDuration();


  for(i=0;i<40;i++)
  {
//     printf("- %2d - %4d\n",i,starttable[i]);
    points=NULL;
    points = g_list_append(points, (typValueList *)GenPoint(0,starttable[i]));
    points = g_list_append(points, (typValueList *)GenPoint(du,starttable[i]));
    CurveInsert((GList *)FileGetCurvesPointer(i), i, points);
  }

}


/* MouseEventStructureFunctions ...  */

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


