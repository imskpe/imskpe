/* For Emacs: -*-c-*- */
/*
    loadconf.l - Part of IMSKPE

    load of config-file (~/.imskpe on unix)
    has nothing to do with loadfile or loadpar!

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
%{
// #include <stdlib.h>
// #include <string.h>
#include <stdio.h>
// #include <gtk/gtk.h>
// #include <gdk/gdk.h>

#include "loadconf.h"

%}

%option noyywrap
%x comment
%x confdata

%%

^[ ]*"#"                     {BEGIN(comment);return COMMENT;}
<comment>.*
<comment>\n                  {BEGIN(INITIAL);return EOL;}  // end comment


^[ ]*[A-Za-z][-A-Za-z0-9_]*  {BEGIN(confdata);return VARNAME;}
<confdata>[ ]*               ;
<confdata>"="                return EQUAL;
<confdata>":"                return DPOINT;
<confdata>[-A-Za-z0-9_]*     return VALUE;
<confdata>"#"                {BEGIN(comment);return COMMENT;}
<confdata>\n                 {BEGIN(INITIAL);return EOL;}

.                            {return UNKNOWN;}
%%
/** 
 * LoadConf loads ~/.imskpe 
 * 
 * @param filename 
 */
void LoadConf(char *filename)
{
  int i;
  int equal;
  FILE *infp;
  char buffer[1024];
  int toktype;
  char val[100], var[100];  // das ist nicht optimal ... die k�nnen �berlaufen!

  printf("%s\n",filename);

  infp = fopen(filename,"r");
  if(infp==NULL)
  {
    /// \todo use an errordialog ?!
    printf("can't open config file");

    /// or touch a new one?

    free(filename);
    return;
  }

  /* read from file, line by line */
  i=0;equal=0;
  while (fgets (buffer, sizeof (buffer), infp)) 
  {
    i++;
    yy_scan_string(buffer);
    while ((toktype = yylex()) != 0) 
    {
//      printf("\t%3d: %s\n", toktype, yytext);
      
      switch(toktype)
      {
      case COMMENT:
	  printf("comment\n");
	  break;
      case VARNAME:
	  strcpy(var,yytext);   // hardening: strncpy!!
	  break;
      case VALUE:
	  strcpy(val,yytext);   // hardening: strncpy!!
	  break;
      case EQUAL:
      case DPOINT:
	  equal++;
	  break;
      case EOL:
	  if(equal>1)
	  {
	    printf("in configfile: 2 or more = or : in line %i.\n",i);
	  }    
	  else
	  if((strlen(val) > 0) && (strlen(var) > 0))
	  {
	    printf("var: %s = %s\n",var,val);
	  }
	  else
	  {
	    if(!(strlen(val)==0 && strlen(var)==0))
	    {
	      if(strlen(val) == 0)
	      {
		printf("in configfile: no value in line %i.\n",i);
	      }
	      if(strlen(var) == 0)
	      {
		printf("in configfile: no varname in line %i.\n",i);
	      }
	    }
	  }

	  // reset all
	  equal=0;
	  strcpy(val,"");
	  strcpy(var,"");
	  break;
      case UNKNOWN:
	  break;
      }
    }
  }
  fclose(infp);
}

int main()
{
  LoadConf("/home/bol/.imskpe");
}