/* For Emacs: -*-c-*- */
/*
    loadpar.l - Part of IMSKPE

    Copyright (c) 2004 Andreas Madsack

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

/**
 * @file   loadpar.c
 * @author Andreas Madsack
 * 
 * @brief  parser for PAR-Files (flex generated)
 * 
 * 
 */

%{
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "loadpar.h"
#include "loadfile.h"
#include "support.h"

  int datanr;

%}

%option noyywrap
%x comment
%x paramdata

%%

"/*"                            {BEGIN(comment);return COMMENT;}
<comment>[ :]                   ;
<comment>[A-Z]+                 return VARNAME;
<comment>[0-9]+                 return VALUE;
<comment>"*"+[^*/\n]*           return EOC;
<comment>\n                     return EOL;
<comment>"*"+"/"                {BEGIN(INITIAL);}
<comment>Produced[ ]by[ ]kpe80  return KPE;

^[ ]+                           {BEGIN(paramdata);}
<paramdata>[0-9]+":"            return LINEID;
<paramdata>[ ]+                 datanr++;
<paramdata>[0-9]+               return DITEM;
<paramdata>\n                   {datanr=0;BEGIN(INITIAL);}

.                               {return UNKNOWN;}
%%
/** 
 * LoadPar loads a PAR-File in curves-List 
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
  char var[3];

  void *params[PARARMETERS];
  GList *pointtmp=NULL;

  typParList *tmp;
  typValueList *point;

  // init
  for(i=0;i<PARARMETERS;i++)
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
    /** \todo use an errordialog ?! */
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
      case KPE:
      case COMMENT:
      case EOL:
	  break;
      case VARNAME:
	  // copy only first 2 chars!
	  strncpy(var,yytext,2);
	  var[2]=0;
	  break;
      case VALUE:
 	  if(!strcmp(var,"DU"))
	  {
	    if(FileSetDuration(atoi(yytext))==FALSE)
	    {
	      printf(_("PAR-Import-Error: "));
	      printf(_("DU not integer\n"));
	    }
	    GuiSetDuration(atoi(yytext));
	  }
 	  else 
	  if (!strcmp(var,"UI"))
	  {
 	    if(FileSetUpdateInterval(atoi(yytext))==FALSE)
 	    {
 	      printf(_("PAR-Import-Error: "));
 	      printf(_("UI not integer\n"));
 	    }
	    GuiSetUpdateInterval(atoi(yytext));
	  }
	  else
	  if (!strcmp(var,"SR"))
	  {
 	    if(FileSetSamplingRate(atoi(yytext))==FALSE)
 	    {
 	      printf(_("PAR-Import-Error: "));
 	      printf(_("SR not integer\n"));
 	    }
	    GuiSetSamplingRate(atoi(yytext));
	  }
	  else
	  if (!strcmp(var,"NF"))
	  {
 	    if(FileSetNumberFormants(atoi(yytext))==FALSE)
 	    {
 	      printf(_("PAR-Import-Error: "));
 	      printf(_("NF not integer\n"));
 	    }
	    GuiSetNumberFormants(atoi(yytext));
	  }
	  else
	  if (!strcmp(var,"SS"))
	  {
 	    if(FileSetVoiceSource(atoi(yytext))==FALSE)
 	    {
 	      printf(_("PAR-Import-Error: "));
 	      printf(_("SS not integer\n"));
 	    }
	    GuiSetVoiceSource(atoi(yytext));
	  }
	  else
	  if (!strcmp(var,"CP"))
	  {
 	    if(FileSetBranches(atoi(yytext))==FALSE)
 	    {
 	      printf(_("PAR-Import-Error: "));
 	      printf(_("CP not integer\n"));
 	    }
	    GuiSetBranches(atoi(yytext));
	  }
	  else
	  {
	    printf(_("not recognized variable in PAR-File: %s [%s]\n"),var,yytext);
	  }
	  break;

	  /* part2 parameters */
      case LINEID:
	  // new parameterline
	  pcount=0;
	  strncpy(x,yytext,strlen(yytext));
	  ptime=atoi(x);
	  break;
      case DITEM:
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
	  tmp->ptmp = (typValueList *) PointInsert(ptime,pvalue);
	  
	  /* write back list of points*/
	  tmp->points = pointtmp;
	  params[pcount]=tmp;
	  pcount++;
	  break;
      case UNKNOWN:
	  break;
      }
    }
  }
  fclose(infp);

// insert last values:
  for(i=0;i<PARARMETERS;i++)
  {
    tmp=params[i];
    pointtmp = (GList *) tmp->points;
    pointtmp = g_list_append(pointtmp,tmp->ptmp);
    tmp->points = pointtmp;
    params[i]=tmp;
  }

//  printf("------\n");

// free ...
  for(i=0;i<PARARMETERS;i++)
  {
    tmp=params[i];
    pointtmp = (GList *) g_list_first (tmp->points);
//    printf ("%2d / %3d\n",i,g_list_length (pointtmp));


/* par->curves:
   - curves auslagern in extra .c
   - getcurves in loadfile benutzen!
   - use curves searchfor (par, formant, button)
   - copy points ? (einfach pointer nehmen und nicht free-n)?
   
   - formants weg -> preferences colortable her!!

*/
//    printf("X-\n");
    CurveInsert(FileGetCurvesPointer(),i,pointtmp);

     while(pointtmp)
     {	
       point = (typValueList *) pointtmp->data;

       printf("%4d %4d %2d\n",point->time,point->value,i); 
      
//       if(point!=NULL)
//       free(point);

//       pointtmp = g_list_remove(pointtmp,point);
       pointtmp = pointtmp->next;
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
