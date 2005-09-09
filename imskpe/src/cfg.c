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
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>

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

  // todo: write log to debug-file!

  if(g_get_home_dir()!=NULL)
  {
    homelen=strlen(g_get_home_dir());
    tmp=(char *)g_malloc(homelen+20);

    strncpy(tmp,g_get_home_dir(),homelen);
    tmp[homelen]=0;
    tmp=strcat(tmp,"/.imskpe");
    tmp[homelen+strlen("/.imskpe")]=0;
    LoadConf(tmp);
  }
  else
  {
//     DialogErrorOK (_("get_home_dir failed!"));
    ;
  }
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
      if(isdigit(value[i])>0)
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
	  if(hexdigit_value(value[i])!=-1)
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
}

void ConfigInsertString(char *name, char *value, unsigned short type)
{
  typConfig *p=NULL;
  char *p_name;
  char *p_value;

//  printf("-CIS- %d - %s[%d] / %s[%d] / %d \n",sizeof(typConfig),name,strlen(name),value,strlen(value),type);
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
  return 0;
}

/** 
 * returns Integervalue from Configfile
 *
 * if not found returnvalue is 0.
 * bad solution!!
 * 
 * @param name 
 * 
 * @return integer
 */
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
  return 0;
}

gboolean ConfigInsertColor(GdkColor *color, char *selected_color)
{
  char tmp[100];  // not good
  char tmp2[100];  // not good

  sprintf(tmp,"#%02x%02x%02x",(int)(color->red/256),(int)(color->green/256),(int)(color->blue/256));
  strcpy(tmp2,"color_");
  strcat(tmp2,selected_color);
  ConfigInsert(tmp2,tmp);

  return TRUE;
}

/** 
 * returns color-struct from config-file
 *
 * \todo needs return-value in error-case
 * 
 * @param name 
 * 
 * @return 
 */
GdkColor ConfigGetColor(char *name)
{
  typConfig *data;
  GList *cl;
  int i,a[3];
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
	a[i]=hexdigit_value(data->value[(i*2)+2]);
	a[i]+=(hexdigit_value(data->value[(i*2)+1]))*16;
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
//       printf("T %s\n",name);
      return TRUE;
    }

    cl = cl->next;
  }
//   printf("F %s\n",name);
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
}

void ConfigNew()
{

  ConfigInsert("maxfreq","5000");
  ConfigInsert("maxamp","100");
  ConfigInsert("maxband","2000");


#ifdef WIN32
  ConfigInsert("playcmd","\"sndrec32 /play /close\"");
  ConfigInsert("tmpdir","\"c:\\\"");
#else
  ConfigInsert("playcmd","\"play\"");
  ConfigInsert("tmpdir","\"/tmp\"");
#endif

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

  ConfigInsertSet("Clear_All","0000000000000000000000000000000000000000");

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

  // delete a few items. only as insurance
  ConfigRemove("prefs_tab1_tmp");
  ConfigRemove("prefs_tab2_tmp");
  ConfigRemove("prefs_tab3_tmp");
  ConfigRemove("prefs_tab4_tmp");

  // now real saving
  if(g_get_home_dir()!=NULL)
  {
    homelen=strlen(g_get_home_dir());
    tmp=(char *)g_malloc(homelen+20);

    strncpy(tmp,g_get_home_dir(),homelen);
    tmp[homelen]=0;
    tmp=strcat(tmp,"/.imskpe");
    tmp[homelen+strlen("/.imskpe")]=0;

    outfp = fopen(tmp,"w");
    if(outfp==NULL)
    {
      DialogErrorOK (_("Problems saving config-file!"));
      
      free(tmp);
      return;
    }
    
    /* write "header"  */
    fprintf(outfp,"# please don't edit this file!!\n");
    
    cl=g_list_first (cfg);
    while(cl)
    {	
      data=(typConfig *)cl->data;
      
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

int hexdigit_value(unsigned char digit)
{
    return isxdigit(digit)
               ? isdigit(digit)
                    ? digit - '0'
                    : tolower(digit) - 'a' + 10    /* Don't forget + 10 */
               : -1;
}

void ConfigShowAllSets()
{
  typConfig *data;
  GList *cl;
  char buf[100];
  char *s;

  cl=g_list_first (cfg);
  while(cl)
  {
    data=(typConfig *)cl->data;
    strcpy(buf,data->name);

    if(buf[0]=='S' && buf[1]=='e' && buf[2]=='t' && buf[3]=='_')
    {
      s = strchr(buf,'_');
      while(s != NULL)
      {
	s[0]=' ';
	s = strchr(buf,'_');
      }
      SetAddMenuItem(buf+4); // ignore 'Set_'
    }

    cl = cl->next;
  }
}

gboolean ConfigInsertSet(char *name, char val[50])
{
  char tmp[100];  // not good
  char tmp2[100];  // not good
  char *s;
  int i;

  strcpy(tmp,"Set_");
  strcat(tmp,name);

  // convert spaces to _
  s = strchr(tmp,' ');
  while(s != NULL)
  {
    s[0]='_';
    s = strchr(tmp,' ');
  }
  
  // are all values in name: [-A-Za-z0-9_] 
  for(i=0;i<strlen(tmp);i++)
  {
    if(isalnum(tmp[i])==0)
    {
      if(tmp[i]!='_' && tmp[i]!='-')
      {
	DialogErrorOK (_("Only Characters, Numbers, Space and - are allowed!"));
	return FALSE;
      }
    }
  }

  strcpy(tmp2,"B");
  strcat(tmp2,val);

  ConfigInsert(tmp,tmp2);

  return TRUE;
}

