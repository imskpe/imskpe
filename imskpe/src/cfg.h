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

#include <gtk/gtk.h>

/*
  structs
*/

typedef struct {
    char *name;
    unsigned short type;
    char *value;
    gint length;
} typConfig;

/*
  constants
*/

#define TYPE_STR 0
#define TYPE_INT 1
#define TYPE_BOOL 2

/*
procedures
*/

void LoadConfig();

