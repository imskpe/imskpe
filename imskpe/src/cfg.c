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

#include "support.h"
#include "curves.h"
#include "graphics.h"
#include "loadconf.h"
#include "cfg.h"

GList *cfg;

void ConfigLoad()
{
  char *tmp;
  int homelen;

  // schreibe log messages in debug-file !!!

//   printf("--- start configload\n");
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
//     DialogErrorOK (_("get_home_dir failed!"));
    ;
  }
//   printf("--- end configload\n");
}

void ConfigInsert(char *name, char *value)
{
  unsigned short type=TYPE_STR;;
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
//       printf("up %30s = %30s \n",name,value);
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
    j=0;
    for(i=0;i<strlen(value);i++)
    {
#ifdef WIN32
      if(isdigit(value[i])==128)
#else
      if(isdigit(value[i])==2048)
#endif
      {
	j++;
      }
    }
    if(j==strlen(value) && j!=0)
    {
      type=TYPE_INT;
    }

    if(strlen(value)==7)
    {
      if(value[0]=='#')
      {
	j=0;
	for(i=1;i<8;i++)
	{
#ifdef WIN32
	  if(isxdigit(value[i])==128)
#else
	  if(isxdigit(value[i])==4096)
#endif
	  {
	    j++;
	  }
	}
	if(j==6)
	{
	  type=TYPE_COLOR;
	}
	else
	{
	  type=TYPE_STR;
	}
      }
    }
  }

  if(type==TYPE_INT)
  {
    ConfigInsertInteger(name,atoi(value),type);
  }
  else
  {
    ConfigInsertString(name,value,type);
  }

//   printf("ins %30s = %30s [%d]\n",name,value,type);
}

void ConfigInsertString(char *name, char *value, unsigned short type)
{
  typConfig *p=NULL;
  char *p_name;
  char *p_value;

//   printf("-CIS- %d - %s[%d] / %s[%d] / %d \n",sizeof(typConfig),name,strlen(name),value,strlen(value),type);
  p = (typConfig *)g_malloc (sizeof (typConfig));
  p_name = g_malloc (strlen(name)+1);
  p_value = g_malloc (strlen(value)+1);

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

void ConfigInsertInteger(char *name, int value, unsigned short type)
{
  char x[100];
  
  snprintf (x,sizeof(x),"%d",value);
  ConfigInsertString(name, x, type);
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
    
    if(!strcmp(data->name,name))
    {
      if(data->type==TYPE_INT)
      {
	return atoi(data->value);
      }
      else
      {
	return 0;  // not the best return value!
      }
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
	x=0;
#ifdef WIN32
 	if(isdigit(data->value[(i*2)+2])==128)
#else
 	if(isdigit(data->value[(i*2)+2])==2048)
#endif
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
	
	x=0;
#ifdef WIN32
 	if(isdigit(data->value[(i*2)+1])==128)
#else
 	if(isdigit(data->value[(i*2)+1])==2048)
#endif
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
//       printf("- colorcode: %3d %3d %3d\n",a[0],a[1],a[2]);
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
      tmp = g_malloc (sizeof(char)*(strlen(ziel)));
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

void ConfigFree()
{
  typConfig *data;
  GList *cl;

//   printf("-configlistfree start-\n");

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
  cfg=NULL;
//   printf("-configlistfree end-\n");
}

void ConfigNew()
{

  ConfigInsert("maxfreq","5000");
  ConfigInsert("maxamp","100");
  ConfigInsert("maxband","2000");

  ConfigInsert("playcmd","play");
  ConfigInsert("tmpdir","/tmp");

  ConfigInsert("color_f1","#ff0000");
  ConfigInsert("color_f2","#00ff00");
  ConfigInsert("color_f3","#0000ff");
  ConfigInsert("color_f4","#00ffff");
  ConfigInsert("color_f5","#ffff00");
  ConfigInsert("color_f6","#ff00ff");
  ConfigInsert("color_nasals","#ffaaaa");
  ConfigInsert("color_vs","#aaffaa");
  ConfigInsert("color_ea","#aaaaff");

  ConfigInsert("toolbarstyle","0");
  ConfigInsert("showquitdiag","1");
  ConfigInsert("rulerfont","\"Sans 6\"");

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
      DialogErrorOK (_("Problems saving config-file!"));
      
      free(tmp);
      return;
    }
    
    /* write "header"  */
    fprintf(outfp,"# please don't edit this file!!\n");
    
//    printf("save config\n");
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
    DialogErrorOK (_("get_home_dir failed!"));
    return;
  }

}
