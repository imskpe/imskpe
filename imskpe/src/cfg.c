/*
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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib/gprintf.h>

#include "cfg.h"

GList *cfg;

void LoadConfig()
{

//  ConfigListInsertString(GO ON

}


void ConfigListInsertString(char *name, char *value, unsigned short type)
{
  typConfig *p;
  char *p_name;
  char *p_value;

  p = g_malloc (sizeof (typConfig));
  p_name = g_malloc (sizeof (strlen(name)+1));

  p_value = g_malloc (sizeof (strlen(value)+1));
  
  strcpy(p_value,value);
  strcpy(p_name,name);
  p->name = p_name;
  p->type = type;
  p->value = p_value;
  p->length;

}

void ConfigListInsert(char *name, int value, unsigned short type)
{
  char x[100];   // noch ein pointer muss nich sein ... oder?
  
  snprintf (x,sizeof(x),"%d",value);
  ConfigListInsertString(name, x, type);
}

char *ConfigGetString(char *name)
{
  typConfig *data;

  cfg=g_list_first (cfg);
  while(cfg)
  {
    data=(typConfig *)cfg->data;
    
    if(!strcmp(data->name,name))   /* strCASEcmp under win32? */
    {
      printf("found %s\n",data->value);
      return data->value;
    }

    cfg = cfg->next;
  }



}


void ConfigListFree()
{
  typConfig *data;

  cfg=g_list_first (cfg);
  while(cfg)
  {	
    data=(typConfig *)cfg->data;
    
    if(data->name!=NULL)
      free(data->name);
    
    if(data->value!=NULL)
      free(data->value);
    
    if(data!=NULL)
      free(data);
    
    cfg = g_list_remove(cfg,data);
  }
}

