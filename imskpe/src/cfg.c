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

#include "curves.h"
#include "graphics.h"
#include "loadconf.h"
#include "cfg.h"

GList *cfg;

void ConfigLoad()
{
  char *tmp;
  int homelen;

  if(g_get_home_dir()!=NULL)
  {
    homelen=strlen(g_get_home_dir());
    tmp=(char *)g_malloc(homelen+20);

    strncpy(tmp,g_get_home_dir(),homelen);
    tmp[homelen]=0;
    tmp=strcat(tmp,"/.imskpe");
    tmp[homelen+strlen("/.imskpe")]=0;
//     printf(">%s<\n",tmp);
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
  typConfig *data;
  GList *cl;
  char *p_value;
  int i,j;

  /* first look if name is already in list:  */
  cl=g_list_first (cfg);
  while(cl)
  {
    data=(typConfig *)cl->data;
    
    if(!strcmp(data->name,name))
    {
      if(data->value!=NULL)
      {
	g_free(data->value);
      }
      p_value = g_malloc (sizeof(char)*(strlen(value)+1));
      strcpy(p_value,value);
      data->value = p_value;
      printf("up %30s = %30s \n",name,value);
      return;
    }
    
    cl = cl->next;
  }

  /* its not in list: */

  // test type:
  if(atoi(value)!=0)
  {
    type=TYPE_INT;
  }
  else
  {
    if(strlen(value)==7)
    {
      if(value[0]=='#')
      {
	j=0;
	for(i=1;i<8;i++)
	{
	  if(isxdigit(value[i])==4096)
	  {
	    j++;
	  }
	}
      }
    }

    // \todo mabe add test with isdigit !!
    if(j==6)
    {
      type=TYPE_COLOR;
    }
    else
    {
      type=TYPE_STR;
    }
  }

  printf("ins %30s = %30s [%d]\n",name,value,type);

  ConfigListInsertString(name,value,type);
}

void ConfigListInsertString(char *name, char *value, unsigned short type)
{
  typConfig *p=NULL;
  char *p_name;
  char *p_value;

//    printf("-1- %d - %s[%d] / %s[%d] / %d \n",sizeof(typConfig),name,strlen(name),value,strlen(value),type);
  p = (typConfig *)g_malloc (sizeof (typConfig));
  p_name = g_malloc (strlen(name));
  p_value = g_malloc (strlen(value));

  strcpy(p_value,value);
  strcpy(p_name,name);
  p_value[strlen(value)]=0;
  p_name[strlen(name)]=0;

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

GdkColor ConfigGetColor(char *name)
{
  typConfig *data;
  GList *cl;
  int i,x,a[3];
  char tmp[30];

  strcpy(tmp,"color_");
  strcat(tmp,name);

  cl=g_list_first (cfg);
  while(cl)
  {
    data=(typConfig *)cl->data;
    
    if(!strcmp(data->name,tmp) && data->type==TYPE_COLOR)
    {
      // dirty hack!!!!

      for(i=0;i<3;i++)
      {
 	if(isdigit(data->value[(i*2)+2])==2048)
 	{
 	  x=data->value[(i*2)+2]-48;
 	}
 	else
 	{
	  switch(data->value[(i*2)+2])
	  {
	  case 'a':case 'A':
	      x=10;
	      break;
	  case 'b':case 'B':
	      x=11;
	      break;
	  case 'c':case 'C':
	      x=12;
	    break;
	  case 'd':case 'D':
	      x=13;
	      break;
	  case 'e':case 'E':
	      x=14;
	      break;
	  case 'f':case 'F':
	      x=15;
	      break;
	  }
 	}
	a[i]=x;
	
	if(isdigit(data->value[(i*2)+1])==2048)
	{
	  x=data->value[(i*2)+1]-48;
	}
	else
	{
	  switch(data->value[(i*2)+1])
	  {
	  case 'a':case 'A':
	      x=10;
	      break;
	  case 'b':case 'B':
	      x=11;
	      break;
	  case 'c':case 'C':
	      x=12;
	    break;
	  case 'd':case 'D':
	      x=13;
	      break;
	  case 'e':case 'E':
	      x=14;
	      break;
	  case 'f':case 'F':
	      x=15;
	      break;
	  }
	}
	a[i]+=x*16;
      }
      return GetColor((float)a[0]/(float)255,(float)a[1]/(float)255,(float)a[2]/(float)255);
    }

    cl = cl->next;
  }
}

gboolean ConfigRename(char *name, char *ziel)
{
  typConfig *data;
  GList *cl;
  char *tmp;

  cl=g_list_first (cfg);
  while(cl)
  {
    data=(typConfig *)cl->data;
    
    if(!strcmp(data->name,name))
    {
      if(data->name!=NULL)
      {
	g_free(data->name);
      }
      tmp = g_malloc (sizeof(char)*(strlen(ziel)+1));
      strcpy(tmp,ziel);
      data->name = tmp;
      return TRUE;
    }

    cl = cl->next;
  }
  return FALSE;
}

gboolean ConfigFind(char *name)
{
  typConfig *data;
  GList *cl;

  cl=g_list_first (cfg);
  while(cl)
  {
    data=(typConfig *)cl->data;
    
    if(!strcmp(data->name,name))
    {
      return TRUE;
    }

    cl = cl->next;
  }
  return FALSE;
}

gboolean ConfigRemove(char *name)
{
  typConfig *data;
  GList *cl;

  cl=g_list_first (cfg);
  while(cl)
  {
    data=(typConfig *)cl->data;
    
    if(!strcmp(data->name,name))
    {
      if(data->name!=NULL)
	g_free(data->name);
      
      if(data->value!=NULL)
	g_free(data->value);
      
      if(data!=NULL)
	g_free(data);

      cl = g_list_remove(cl,data);
      return TRUE;
    }

    cl = cl->next;
  }
  return FALSE;
}

void ConfigListFree()
{
  typConfig *data;
  GList *cl;

  printf("-configlistfree start-\n");

  cl=g_list_first (cfg);
  while(cl)
  {	
    data=(typConfig *)cl->data;

    if(data->name!=NULL)
      g_free(data->name);

    if(data->value!=NULL)
      g_free(data->value);

    if(data!=NULL)
      g_free(data);

    cl = g_list_remove(cl,data);
  }
  printf("-configlistfree end-\n");
}

void ConfigNew()
{
  ConfigListInsert("maxfreq","5000");
  ConfigListInsert("maxamp","100");
  ConfigListInsert("maxband","2000");

  ConfigListInsert("klattcmd","klatt");
  ConfigListInsert("playcmd","play");
  ConfigListInsert("tmpdir","/tmp");

  ConfigListInsert("color_f1","#ff0000");
  ConfigListInsert("color_f2","#00ff00");
  ConfigListInsert("color_f3","#0000ff");
  ConfigListInsert("color_f4","#00ffff");
  ConfigListInsert("color_f5","#ffff00");
  ConfigListInsert("color_f6","#ff00ff");
  ConfigListInsert("color_nasals","#ffaaaa");
  ConfigListInsert("color_vs","#aaffaa");
  ConfigListInsert("color_ea","#aaaaff");

//  ConfigListInsert("main_window_x","740");
//  ConfigListInsert("main_window_y","540");
}

void ConfigSave()
{
  char *tmp;
  FILE *outfp;
  int homelen;

  typConfig *data;
  GList *cl;

  if(g_get_home_dir()!=NULL)
  {
    homelen=strlen(g_get_home_dir());
    tmp=(char *)g_malloc(homelen+20);

    strncpy(tmp,g_get_home_dir(),homelen);
    tmp[homelen]=0;
    tmp=strcat(tmp,"/.imskpe");
    tmp[homelen+strlen("/.imskpe")]=0;
//     printf(">%s<\n",tmp);

    outfp = fopen(tmp,"w");
    if(outfp==NULL)
    {
      /** \todo use an errordialog ?! */
      printf("can't open output parameter file");
      
      free(tmp);
      return;
    }
    
    /* write "header"  */
    fprintf(outfp,"# please don't edit this file!!\n");
    
    printf("save config\n");
    cl=g_list_first (cfg);
    while(cl)
    {	
      data=(typConfig *)cl->data;
      
//       printf("%s=%s\n",data->name,data->value);
      fprintf(outfp,"%s = %s\n",data->name,data->value);
      
      cl = cl->next;
    }
    
    fclose(outfp);
  }
  else
  {
    printf("get_home_dir failed!!\n");
    return;
  }

}
