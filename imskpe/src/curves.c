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
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib/gprintf.h>

#include "curves.h"
#include "cfg.h"

/*   - curves searchfor (par, button) */


void CurveInsert(GList *curves, int parid, GList *points)
{
  /** CurveMappings

  vs=voice-source  <- reicht evt. f�r die farben nicht ...
  ea=exp. amp.
  
  \todo manche linien vielleicht gestrichelt?!

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
      {"gain","ae","bn_examp_siggain",2,GDK_LINE_SOLID}
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

  /*
    todo:
    - check if curve exits (CurveSearchWidgetName) !
    - free pointlist
    - add new one ...
    - 
   */


//  printf("curves: %d\n",g_list_length(curves));

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

/*
*/
    eval->show=GuiGetToggleButtonState(curvetable[parid].widget_name);
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

      pl = g_list_remove(pl,pval);
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
 * returns searched element in curve-list
 * 
 * @param curve 
 * @param wn 
 * 
 * @return 
 */
typCurveList *CurveSearchWidgetName(GList *curve,char *wn)
{
  // �ber liste traversieren und das richtige element suchen ..
  // wenn gefunden zur�ckliefern, sonst NULL
  
  typCurveList *thiscurve;  
  GList *cl;
  char tmp[30];

  cl=g_list_first (curve);

  while(cl)
  {	
    thiscurve = cl->data;

    strncpy(tmp,thiscurve->widget_name,30);
//     printf("------------- -%s-\n",thiscurve->widget_name);
    if(!strcasecmp(tmp,wn))
    {
//        printf("found %s\n",wn);
      return thiscurve;
    }

    cl=cl->next;
  }
  return NULL;

}


gboolean SetCurveShow(char *wn)
{
  GtkWidget *w;

  typCurveList *c = CurveSearchWidgetName((GList *)FileGetCurvesPointer(),wn);

  if(c!=NULL)
  {
    w=(GtkWidget *)lookup_widget (GTK_WIDGET (GetMainWindow()), "nb_draw");
    if(c->show==TRUE)
    {
      printf("%s - set false\n",c->widget_name);
      c->show=FALSE;
      redraw_page(gtk_notebook_get_current_page((GtkNotebook *)w));
      return FALSE;
    }
    else
    {
      printf("%s - set true\n",c->widget_name);
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
 * Inserts a Point in an typCurveList/typParList and 
 * returns the pointer to it.
 *
 * @param t 
 * @param v 
 * 
 * @return 
 */
typValueList *PointInsert (gint t, gint v)
{
  typValueList *vl;

  vl = g_malloc (sizeof (typValueList));
  vl->time=t;
  vl->value=v;
  return vl;
}

/** 
 * Init default Curve
 *
 * using DU from File-struct to get 2nd point!
 * setting no other values, except curve!
 * 
 * @param curves 
 */
void CurveInitStart()
{
  int starttable [] = {
      100,60,500,60,1500,
      90,1500,150,3250,200,
      3700,200,4990,500,6680,
      439,6680,489,0,40,
      0,0,0,0,0,
      60,0,90,0,150,
      0,200,0,200,0,
      500,0,0,60,60};
  typCurveList *curve;
  unsigned int du;
  GList *points=NULL;
  int i;

  CurveListFree((GList *)FileGetCurvesPointer()); /* ok here?  */

  du=FileGetDuration();


  for(i=0;i<40;i++)
  {
    printf("- %2d - %4d\n",i,starttable[i]);
    points=NULL;
    points = g_list_append(points, (typValueList *)PointInsert(0,starttable[i]));
    points = g_list_append(points, (typValueList *)PointInsert(du,starttable[i]));
    CurveInsert((GList *)FileGetCurvesPointer(), i, points);
  }

}