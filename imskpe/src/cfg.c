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

/**
 * @file   cfg.c
 * @author Andreas Madsack
 * 
 * @brief  listsystem for internal cfg-list
 * 
 * \note   use strCASEcmp for compares?
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib/gprintf.h>

#include "loadconf.h"
#include "cfg.h"

GList *cfg;

void ConfigLoad()
{
  char *tmp;
  int homelen;



//  ConfigListInsertString(GO ON
  if(g_get_home_dir()!=NULL)
  {
    homelen=strlen(g_get_home_dir());
    tmp=(char *)g_malloc(sizeof(homelen)+20);

    strncpy(tmp,g_get_home_dir(),homelen);
    tmp[homelen]=0;
    tmp=strcat(tmp,"/.imskpe");
    tmp[homelen+strlen("/.imskpe")]=0;
    printf(">%s<\n",tmp);
    LoadConf(tmp);
  }
  else
  {
    printf("get_home_dir failed!!\n");
  }
}

void ConfigListInsert(char *name, char *value)
{
  unsigned short type;

  // test type:
  if(atoi(value)!=0)
  {
    type=TYPE_INT;
  }
  else
  {
    // \todo add test with isdigit !!
    type=TYPE_STR;
  }

//   printf("%30s = %30s [%d]\n",name,value,type);

  ConfigListInsertString(name,value,type);
}

void ConfigListInsertString(char *name, char *value, unsigned short type)
{
  typConfig *p=NULL;
  char *p_name;
  char *p_value;


//   printf("-1- %d - %s[%d] / %s[%d] / %d \n",sizeof(typConfig),name,strlen(name),value,strlen(value),type);
  p = (typConfig *)g_malloc (sizeof (typConfig));
  p_name = g_malloc (sizeof (strlen(name)+1));
  p_value = g_malloc (sizeof (strlen(value)+1));

  strcpy(p_value,value);
  strcpy(p_name,name);

  p->name = p_name;
  p->type = type;
  p->value = p_value;

  cfg = g_list_append(cfg,p);

  return;
}

void ConfigListInsertInteger(char *name, int value, unsigned short type)
{
  char x[100];
  
  snprintf (x,sizeof(x),"%d",value);
  ConfigListInsertString(name, x, type);
}


char *ConfigGetString(char *name)
{
  typConfig *data;
  GList *cl;

  cl=g_list_first (cfg);
  while(cl)
  {
    data=(typConfig *)cl->data;
    
    if(!strcmp(data->name,name))
    {
      return data->value;
    }

    cl = cl->next;
  }
}

int ConfigGetInteger(char *name)
{
  typConfig *data;
  GList *cl;

  cl=g_list_first (cfg);
  while(cl)
  {
    data=(typConfig *)cl->data;
    
    if(!strcmp(data->name,name) && data->type==TYPE_INT)
    {
      return atoi(data->value);
    }

    cl = cl->next;
  }
}

void ConfigListFree()
{
  typConfig *data;
  GList *cl;

  cl=g_list_first (cfg);
  while(cl)
  {	
    data=(typConfig *)cl->data;
    
    if(data->name!=NULL)
      free(data->name);
    
    if(data->value!=NULL)
      free(data->value);
    
    if(data!=NULL)
      free(data);
    
    cl = g_list_remove(cl,data);
  }
}

void ConfigNew()
{
  ConfigListInsert("maxfreq","5000");
  ConfigListInsert("maxamp","100");
  ConfigListInsert("maxband","2000");

  ConfigListInsert("klattcmd","klatt");
  ConfigListInsert("playcmd","play");
  ConfigListInsert("tmpdir","/tmp");

  ConfigListInsert("main_window_x","740");
  ConfigListInsert("main_window_y","540");
}

void ConfigSave()
{

}
