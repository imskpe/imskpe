/*
    loadfile.c - Part of IMSKPE

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
 * @file   loadfile.c
 * @author Andreas Madsack
 * 
 * @brief  wrapper for open/import of files
 * 
 * 
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <stdio.h>

#include "loadfile.h"
#include "loadpar.h"

/** global-File-struct */
typFile *aFile=NULL;

/** 
 * FileOpen
 * 
 * Wrapper for opening of files.
 * At this moment it only opens PAR-Files
 * 
 * @param filename 
 */
void FileOpen(char *filename)
{
  char *tmp;
  
  /** \todo look if old file isChanged and needs to be saved. */

  if(aFile != NULL)
  {
    g_free(aFile->filename);
    g_free(aFile);
    // free curves!!
    CurveListFree(aFile->curves);
  }

  aFile=g_malloc (sizeof (typFile));

  tmp=g_malloc (sizeof (char)*(strlen(filename)+1));
  strncpy(tmp,filename,strlen(filename));

  aFile->filename=tmp;
  aFile->ischanged=FALSE;
  aFile->curves=NULL;

  /** \todo test for filename extension!! */
  LoadPar(filename);

  /** \todo actualize the widgets !! */

  printf("%s\n",aFile->filename);

//  printf("x- %d\n",aFile->duration);
  /** \todo free aFile at quit! -> in need of FileGetFilename*/
}

gboolean FileSetDuration(unsigned int d)
{
  if(aFile!=NULL)
  {
    if(d>=0)
    {
      aFile->duration=d;
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }
  else
  {
    return FALSE;
  }
}

gboolean FileSetUpdateInterval(unsigned int ui)
{
  if(aFile!=NULL)
  {
    if(ui>=2 && ui<=20)
    {
      aFile->update_interval=ui;
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }
  else
  {
    return FALSE;
  }
}

gboolean FileSetSamplingRate(unsigned int sr)
{
  if(aFile!=NULL)
  {
    if(sr>=5000 && sr<=24000)   // more?
    {
      aFile->sampling_rate=sr;
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }
  else
  {
    return FALSE;
  }
}

gboolean FileSetNumberFormants(unsigned int f)
{
  if(aFile!=NULL)
  {
    if(f>0 && f<7)
    {
      aFile->formant_number=f;
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }
  else
  {
    return FALSE;
  }
}

gboolean FileSetVoiceSource(unsigned int vs)
{
  if(aFile!=NULL)
  {
    switch(vs)
    {
    case IMPULSE:
    case NATURAL:
    case SAMPLED:
	aFile->voice_source=vs;
	return TRUE;
    default:
	return FALSE;
    }
  }
}

gboolean FileSetBranches(unsigned int cp)
{
  if(aFile!=NULL)
  {
    switch(cp)
    {
    case BOTH:
    case PARALLEL:
	aFile->branches=cp;
	return TRUE;
    default:
	return FALSE;
    }
  }
}

void FileSetIsChanged(gboolean b)
{
  if(aFile!=NULL)
  {
    aFile->ischanged=b;
  }
}

gboolean FileGetIsChanged()
{
  if(aFile!=NULL)
  {
    return aFile->ischanged;
  }
}

void FileSetCurvesPointer(GList *curves)
{
  aFile->curves=curves;
}

GList *FileGetCurvesPointer()
{
  if(aFile!=NULL)
  {
    if(aFile->curves!=NULL)
    {
      return g_list_first(aFile->curves);
    }
  }
}



// GList *FileGetFormantsPointer()
// {
//   if(aFile!=NULL)
//   {
//     return aFile->formants;
//   }
// }
