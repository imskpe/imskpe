/*
    savefile.c - Part of IMSKPE

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
 * @file   savefile.c
 * @author Andreas Madsack
 * 
 * @brief  wrapper for save/export of files
 * 
 * 
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <stdio.h>
#include "support.h"

#include "loadfile.h"
//#include "curves.h"
#include "savefile.h"

#include "loadpar.h"



/** 
 * FileSave
 * 
 * Wrapper for saving files.
 * At this moment it only saves PAR-Files
 * 
 * @param filename 
 */

void FileSave(char *filename)
{
  char *tmp;
  FILE *outfp;
  
  if(!FileIsDefined())
  {
    // no file opened ??
    // no file created?
    // houston we have a ...
    
    printf("filedatapointer is NULL! why?\n");
  }

  if(strlen(filename)>1)
  {
    tmp=g_malloc (sizeof (char)*(strlen(filename)));
    strncpy(tmp,filename,strlen(filename));
  }
  else
  {
    tmp=g_malloc (sizeof (char)*(strlen(FileGetFilename())));
    strncpy(tmp,FileGetFilename(),strlen(FileGetFilename()));
  }
  
  tmp[strlen(tmp)]=0;
  FileSetFilename(tmp);
//   printf("-%s-\n",tmp);    

  outfp = fopen(tmp,"w");
  if(outfp==NULL)
  {
    DialogErrorOK (_("Can't open output parameter file."));

    free(tmp);
    return;
  }

  /* write "header"  */
  fprintf(outfp,"/* Produced by imskpe */\n");
  fprintf(outfp,"/*  DU : %d */\n",FileGetDuration());
  fprintf(outfp,"/*  UI : %d */\n",FileGetUpdateInterval());
  fprintf(outfp,"/*  SR : %d */\n",FileGetSamplingRate());
  fprintf(outfp,"/*  NF : %d */\n",FileGetNumberFormants());
  fprintf(outfp,"/*  SS : %d */\n",FileGetVoiceSource());
  fprintf(outfp,"/*  CP : %d */\n",FileGetBranches());

  /* write values */
  int i,j;
  typValueList *pnt[PARAMETERS];
  typValueList *p_pnt[PARAMETERS];
  GList *vl[PARAMETERS];
  int y;

  for(j=0;j<PARAMETERS;j++)
  {
    vl[j]=(GList *) g_list_first (CurveSearchByNr(FileGetCurvesPointer(),j)->points);
    p_pnt[j]=(typValueList *)vl[j]->data;
    pnt[j]=(typValueList *)vl[j]->data;
  }


  for(i=0;i<FileGetDuration();i+=FileGetUpdateInterval())
  {
    fprintf(outfp,"%5d: ",i);

    for(j=0;j<PARAMETERS;j++)
    {
      if(pnt[j]->time<i)
      {
	p_pnt[j]=pnt[j];
	if(vl[j]->next != NULL)
	{
	  vl[j]=vl[j]->next;
	  pnt[j]=(typValueList *)vl[j]->data;
	}
      }
      
      if((pnt[j]->time-p_pnt[j]->time)==0)
      {
	/* to prevent from div by 0 */
	y=pnt[j]->value;
      }
      else
      {
	y=p_pnt[j]->value + (int)((float)(pnt[j]->value-p_pnt[j]->value)/(float)(pnt[j]->time-p_pnt[j]->time)*(float)(i-p_pnt[j]->time));
      }
      
      fprintf(outfp,"%d ",y);
    }

    fprintf(outfp,"\n");
  }
  fclose(outfp);
}
