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


typCurveList CurveInsert(GList *curves, int parid, GList *points)
{
  /** CurveMappings

  vs=voice-source  <- reicht evt. für die farben nicht ...
  ea=exp. amp.
  
  \todo manche linien vielleicht gestrichelt?!

  */
  struct CurveMapping curvetable [] = {
      {"f0","vs","bn_vs_fundfreq",1},
      {"av","ea","bn_examp_voice",2},
      {"f1","f1","bn_f1_freq",1},
      {"b1","f1","bn_f1_band",3},
      {"f2","f2","bn_f2_freq",1},
      {"b2","f2","bn_f2_band",3},
      {"f3","f3","bn_f3_freq",1},
      {"b3","f3","bn_f3_band",3},
      {"f4","f4","bn_f4_freq",1},
      {"b4","f4","bn_f4_band",3},
      {"f5","f5","bn_f5_freq",1},
      {"b5","f5","bn_f5_band",3},
      {"f6","f6","bn_f6_freq",1},
      {"b6","f6","bn_f6_band",3},
      {"fnz","nasal","bn_nasal_z_freq",1},
      {"bnz","nasal","bn_nasal_z_band",3},
      {"fnp","nasal","bn_nasal_p_freq",1},
      {"bnp","nasal","bn_nasal_p_band",3},
      {"asp","ea","bn_examp_asp",2},
      {"kopen","vs","bn_vs_gopenquot",2},  // no dia? -> 2
      {"aturb","vs","bn_vs_breath",2},
      {"tilt","vs","bn_vs_spectilt",2},
      {"af","ea","bn_examp_fric",2},
      {"skew","vs","bn_vs_skewness",2},  // no dia? -> 2
      {"a1","f1","bn_f1_amp",2},
      {"b1p","f1","bn_f1_bandp",3},
      {"a2","f2","bn_f2_amp",2},
      {"b2p","f2","bn_f2_bandp",3},
      {"a3","f3","bn_f3_amp",2},
      {"b3p","f3","bn_f3_bandp",3},
      {"a4","f4","bn_f4_amp",2},
      {"b4p","f4","bn_f4_bandp",3},
      {"a5","f5","bn_f5_amp",2},
      {"b5p","f5","bn_f5_bandp",3},
      {"a6","f6","bn_f6_amp",2},
      {"b6p","f6","bn_f6_bandp",3},
      {"anp","nasal","bn_nasal_p_amp",2},
      {"ab","ea","bn_examp_bypass",2},
      {"avp","ea","bn_examp_voicepar",2},
      {"gain","ae","bn_examp_siggain",2}
  };
  GList *pl=NULL;
  GList *el=NULL;
  typCurveList *eval=NULL;
  typValueList *pval=NULL;
  int tmp;
    
//   printf("%s / %s / %s / %d - %d\n",
// 	 curvetable[parid].parname,
// 	 curvetable[parid].formant,
// 	 curvetable[parid].widget_name,
// 	 curvetable[parid].dia,
// 	 g_list_length(points));

  /*
    todo:
    - check if curve exits (CurveSearchWidgetName) !
    - free pointlist
    - add new one ...
    - 
   */


//  printf("curves: %d\n",g_list_length(curves));

  // -> data ??!
  el=(GList *)CurveSearchWidgetName((GList *)curves,(char *)curvetable[parid].widget_name);

  if(el==NULL)
  {
//    printf("--\n");
    eval = g_malloc (sizeof (typCurveList));

    eval->points=points;

    tmp=strlen(curvetable[parid].formant);
    if(tmp>20) tmp=20;
    strncpy(eval->formant,curvetable[parid].formant,tmp);

    tmp=strlen(curvetable[parid].widget_name);
    if(tmp>30) tmp=30;
    strncpy(eval->widget_name,curvetable[parid].widget_name,tmp);

    eval->show=FALSE;
    curves = g_list_append(curves,eval);
  }
  else
  {
    /* free all points */
    eval= (typCurveList *) el->data;
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
    printf("-");
    while(pl)
    {	
      printf(".");
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
  printf("\n");
  FileSetCurvesPointer(NULL);
}

GList *CurveSearchWidgetName(GList *curve,char *wn)
{
  // über liste traversieren und das richtige element suchen ..
  // wenn gefunden zurückliefern, sonst NULL
  
  return NULL;

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
    points=NULL;

    points = g_list_append(points, (typValueList *)PointInsert(0,starttable[i]));
    points = g_list_append(points, (typValueList *)PointInsert(du,starttable[i]));
    CurveInsert((GList *)FileGetCurvesPointer(), i, points);
  }

}
