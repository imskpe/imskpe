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

#include "support.h"

#include "loadfile.h"
#include "loadpar.h"

/* only for InitDialogSave :( */
#include "callbacks.h"


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
  LoadPar(filename);
}

gboolean FilePrepare(char *filename)
{
  char *tmp;
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
  aFile->isnew=FALSE;
  aFile->curves=NULL;
}

gboolean FileIsDefined()
{
  if(aFile!=NULL)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}


char *FileGetFilename()
{
  if(aFile!=NULL)
  {
    return aFile->filename;
  }
  else
  {
    return 0;
  }
}

gboolean FileSetFilename(char *foo)
{
  char *tmp;

  if(aFile!=NULL)
  {
    free(aFile->filename);
    tmp=g_malloc (sizeof (char)*(strlen(foo)+1));
    strncpy(tmp,foo,strlen(foo));

    aFile->filename=tmp;
    return TRUE;
  }
  else
  {
    return FALSE;
  }
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

unsigned int FileGetDuration()
{
  if(aFile!=NULL)
  {
    return aFile->duration;
  }
  else
  {
    return 0;
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

unsigned int FileGetUpdateInterval()
{
  if(aFile!=NULL)
  {
    return aFile->update_interval;
  }
  else
  {
    return 0;
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

unsigned int FileGetSamplingRate()
{
  if(aFile!=NULL)
  {
    return aFile->sampling_rate;
  }
  else
  {
    return 0;
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

unsigned int FileGetNumberFormants()
{
  if(aFile!=NULL)
  {
    return aFile->formant_number;
  }
  else
  {
    return 0;
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

unsigned int FileGetVoiceSource()
{
  if(aFile!=NULL)
  {
    return aFile->voice_source;
  }
  else
  {
    return 0;
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

unsigned int FileGetBranches()
{
  if(aFile!=NULL)
  {
    return aFile->branches;
  }
  else
  {
    return 0;
  }
}

void FileSetIsNew(gboolean b)
{
  if(aFile!=NULL)
  {
    aFile->isnew=b;
  }
}

gboolean FileGetIsNew()
{
  if(aFile!=NULL)
  {
    return aFile->isnew;
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

void FileInit()
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
  tmp=g_malloc (sizeof (char)*(strlen("unnamed.par")+1));
  strncpy(tmp,"unnamed.par",strlen("unnamed.par"));
  tmp[strlen("unnamed.par")]=0;

  aFile->filename=tmp;
  aFile->ischanged=FALSE;
  aFile->curves=NULL;

  FileSetDuration(500);
  FileSetUpdateInterval(10);
  FileSetSamplingRate(16000);
  FileSetNumberFormants(5);
  FileSetVoiceSource(2);
  FileSetBranches(1);

  CurveInitStart();

  /** \todo free aFile at quit! -> in need of FileGetFilename*/
}

