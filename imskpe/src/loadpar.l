/* For Emacs: -*-c-*- */
/*
    loadpar.l - Part of IMSKPE
    Copyright (c) 2004  

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
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
%{
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "loadpar.h"

  int datanr;

%}

%option noyywrap
%x comment
%x paramdata

%%

"/*"                    {BEGIN(comment);return COMMENT;}
<comment>[ :]           ;
<comment>[A-Z]+         return VARNAME;
<comment>[0-9]+         return VALUE;
<comment>"*"+[^*/\n]*   return EOC;
<comment>\n             ;
<comment>"*"+"/"        {BEGIN(INITIAL);}

^[ ]+                   {BEGIN(paramdata);}
<paramdata>[0-9]+":"    return LINEID;
<paramdata>[ ]+         datanr++;
<paramdata>[0-9]+       return DITEM;
<paramdata>\n           {datanr=0;BEGIN(INITIAL);}
.                       {return UNKNOWN;}
%%
/** 
 * LoadPar loads a PAR-File in curves-List 
 * 
 * \todo remember filename and path in a struct?; what more? 
 *       an is_changed bool ... ?
 *       all global infos
 *
 * @param filename 
 */
void LoadPar(char *filename)
{
  FILE *infp;
  char buffer[1024];
  char x[100];
  int toktype;
  int pcount;
  int ptime;
  int pvalue;
  int i;

  void *params[PARARMETERS];
  GList *pointtmp=NULL;

  typParList *tmp;
  typListValue *point;

  // init
  for(i=1;i<PARARMETERS;i++)
  {
    tmp = g_malloc (sizeof (typParList));
    tmp->last1=-1;
    tmp->last2=-1;
    tmp->points=NULL;
    tmp->ptmp=NULL;
    params[i]=tmp;
    tmp=NULL;
  }

  printf("%s\n",filename);

  infp = fopen(filename,"r");
  if(infp==NULL)
  {
    /// \todo use an errordialog ?!
    printf("can't open parameter file");

    free(filename);
    return;
  }

  /* read from file, line by line */
  while (fgets (buffer, sizeof (buffer), infp)) 
  {
//    printf("%s",buffer);
    // ist es ein kommentar?
    yy_scan_string(buffer);
    while ((toktype = yylex()) != 0) 
    {
//      printf("\t%3d: %s\n", toktype, yytext);
      
      /* to be done */
      switch(toktype)
      {
	
	/* part1 commentinfo */

/*  DU:      duration of (selected) waveform
    UI:      update interval for parameter reset
    SR:      sampling frequency
    NF:      number of formants
    SS:      voice source; 1 = impulse, 2 = natural, 3 = sampled
    CP:      1 = cascade + parallel branches, 2 = parallel only
*/
	
      case COMMENT:
	  break;
      case VARNAME:
	  printf("var: %s ",yytext);
	  break;
      case VALUE:
	  printf(" = %s\n",yytext);
	  break;
	  /* part2 parameters */
	  
      case LINEID:
	  // new parameterline
	  pcount=0;
	  strncpy(x,yytext,strlen(yytext));
	  ptime=atoi(x);
	  break;
      case DITEM:
	  pcount++;
	  pvalue = atoi(yytext);
	  
	  tmp = params[pcount];
	  pointtmp = (GList *) tmp->points;
	  
	  /**
	     Idea for importfilter:
	     save point temporary in params-struct
	     if it's necessary save it.
	  */

//	  printf("- %4d %4d %2d - (%4d)(%4d) - %2d - #%3d \n",ptime,pvalue,pcount,tmp->last1,tmp->last2,pcount,g_list_length(pointtmp)); 
	  
	  if(tmp->last1!=pvalue && tmp->last2!=pvalue)
	  {
	    if(tmp->ptmp!=NULL)
	      pointtmp = g_list_append(pointtmp,tmp->ptmp);
	    tmp->last1 = pvalue;
	    tmp->last2 = -1;
	  }
	  else if(tmp->last1==pvalue && tmp->last2!=pvalue)
	  {
	    pointtmp = g_list_append(pointtmp,tmp->ptmp);
	    tmp->last2 = pvalue;
	  }
	  else if(tmp->last1==pvalue && tmp->last2==pvalue)
	  {
	    ;  /* it's not necessary! => don't append it */
	  }
	  /* aktual point; save it temporarily */
	  tmp->ptmp = (typListValue *) PointInsert(ptime,pvalue);
	  
	  /* write back list of points*/
	  tmp->points = pointtmp;
	  params[pcount]=tmp;
	  break;
      case UNKNOWN:
	  break;
      }
    }
  }
  fclose(infp);

// insert last values:
  for(i=1;i<PARARMETERS;i++)
  {
    tmp=params[i];
    pointtmp = (GList *) tmp->points;
    pointtmp = g_list_append(pointtmp,tmp->ptmp);
    tmp->points = pointtmp;
    params[i]=tmp;
  }

//  printf("------\n");

// free ...
  for(i=1;i<PARARMETERS;i++)
  {
    tmp=params[i];
    pointtmp = (GList *) g_list_first (tmp->points);
//    printf ("%2d / %3d\n",i,g_list_length (pointtmp));
    while(pointtmp)
    {	

      point = (typListValue *) pointtmp->data;

//      printf("%4d %4d %2d\n",point->time,point->value,i); 
      
      if(point!=NULL)
      free(point);

      pointtmp = g_list_remove(pointtmp,point);
    }
    free(tmp);
  }
  free(filename);
}

/*typParValue *ParamInsert (int t, int v, int pcnt)
{
  typParValue *pl;

  pl = g_malloc (sizeof (typParValue));
  pl->time=t;
  pl->value=v;
  pl->pcount=pcnt;
  return pl;
}
*/
