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
#include <glib.h> 
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "support.h"

#include "loadfile.h"
#include "loadpar.h"
#include "graphics.h"

/* only for InitDialogSave :( */
#include "callbacks.h"


/** global-File-struct */
typFile *aFile=NULL;

/** 
 * Wrapper for opening of files.
 * At this moment it only opens PAR-Files
 * 
 * @param filename 
 */
gboolean FileOpen(char *filename)
{
  // check if filename is a file
  if (!g_file_test(filename, G_FILE_TEST_IS_DIR))
  {
    LoadPar(filename);
    return TRUE;
  }
  return FALSE;
}

/** 
 * set all values in file-struct, for newly loaded file.
 * 
 * @param filename 
 * 
 * @return 
 */
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
  tmp[strlen(filename)]=0;

  aFile->filename=tmp;
  aFile->ischanged=FALSE;
  aFile->isnew=FALSE;
  aFile->curves=NULL;
  
  return TRUE;
}

/** 
 * returns if File is defined in file-struct
 *
 * if not set FALSE is returned.
 * 
 * @return 
 */
gboolean FileIsDefined()
{
  if(aFile!=NULL)
  {
    return TRUE;
  }
  return FALSE;
}

/** 
 * returns Filename of actual file in file-struct
 * 
 * returns "\0" if not defined
 * 
 * @return 
 */
char *FileGetFilename()
{
  if(aFile!=NULL)
  {
    return aFile->filename;
  }
  return "\0";
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
  return FALSE;
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
  return FALSE;
}

/** 
 * returns durations from file-struct
 * 
 * returns 0 if not defined
 * 
 * @return 
 */
unsigned int FileGetDuration()
{
  if(aFile!=NULL)
  {
    return aFile->duration;
  }
  return 0;
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
  return FALSE;
}

/** 
 * returns updateinterval from file-struct
 *
 * returns 0 if not defined
 * 
 * 
 * @return 
 */
unsigned int FileGetUpdateInterval()
{
  if(aFile!=NULL)
  {
    return aFile->update_interval;
  }
  return 0;
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
  return FALSE;
}

/** 
 * returns Samplingrate defined in file-struct
 * 
 * returns 0 if not defined
 * 
 * @return 
 */
unsigned int FileGetSamplingRate()
{
  if(aFile!=NULL)
  {
    return aFile->sampling_rate;
  }
  return 0;
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
  return FALSE;
}

/** 
 * returns number of formants defined in file-struct
 * 
 * returns 0 if not defined
 * 
 * @return 
 */
unsigned int FileGetNumberFormants()
{
  if(aFile!=NULL)
  {
    return aFile->formant_number;
  }
  return 0;
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
    }
  }
  return FALSE;
}

/** 
 * returns VoiceSource from file-struct
 *
 * returns 0 if not defined
 * 
 * @return 
 */
unsigned int FileGetVoiceSource()
{
  if(aFile!=NULL)
  {
    return aFile->voice_source;
  }
  return 0;
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
  return FALSE;
}

/** 
 * returns Branches from file-struct
 * 
 * returns 0 if not defined
 * 
 * @return 
 */
unsigned int FileGetBranches()
{
  if(aFile!=NULL)
  {
    return aFile->branches;
  }
  return 0;
}

void FileSetIsNew(gboolean b)
{
  if(aFile!=NULL)
  {
    aFile->isnew=b;
  }
}

/** 
 * returns if file is new
 *
 * if not defined, file is new (result is TRUE)
 * 
 * 
 * @return 
 */
gboolean FileGetIsNew()
{
  if(aFile!=NULL)
  {
    return aFile->isnew;
  }
  return TRUE;
}

void FileSetIsChanged(gboolean b)
{
  if(aFile!=NULL)
  {
    aFile->ischanged=b;
  }
}

/** 
 * returns if file is changes
 *
 * not defined should not occur.
 * if it occurs, FALSE will be returned.
 * 
 * 
 * @return 
 */
gboolean FileGetIsChanged()
{
  if(aFile!=NULL)
  {
    return aFile->ischanged;
  }
  return FALSE;
}

void FileSetCurvesPointer(GList *curves)
{
  aFile->curves=curves;
}

/** 
 * returns curves-pointer in file-struct
 * 
 * if not defined NULL is returned
 * 
 * @return 
 */
GList *FileGetCurvesPointer()
{
  if(aFile!=NULL)
  {
    if(aFile->curves!=NULL)
    {
      return g_list_first(aFile->curves);
    }
  }
  return NULL;
}

/** 
 * Initialize File.
 *
 * \todo look if old file isChanged and needs to be saved. 
 * 
 */
void FileInit()
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
  tmp=g_malloc (sizeof (char)*(strlen("unnamed.par")+1));
  strncpy(tmp,"unnamed.par",strlen("unnamed.par"));
  tmp[strlen("unnamed.par")]=0;

  aFile->filename=tmp;
  aFile->ischanged=FALSE;
  aFile->curves=NULL;

  FileSetDuration(500);
  GuiSetDuration(500);
  FileSetUpdateInterval(10);
  GuiSetUpdateInterval(10);
  FileSetSamplingRate(16000);
  GuiSetSamplingRate(16000);
  FileSetNumberFormants(5);
  GuiSetNumberFormants(5);
  FileSetVoiceSource(2);
  GuiSetVoiceSource(2);
  FileSetBranches(1);
  GuiSetBranches(1);

  CurveInitStart();
}

