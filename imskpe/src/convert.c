/*
    convert.c - Part of IMSKPE

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
 * @file   convert.c
 * @author Andreas Madsack
 * 
 * @brief  convert par->wav using klatt80/parwave
 * 
 * 
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <math.h>
#include "convert.h"

#include "loadfile.h"
#include "loadpar.h"

#include "klatt80/parwave.h"

#include "graphics.h"
#include "support.h"

/* defaults */

#define SAMPLE_FACTOR 0.00001
#define MAXSAMPLE 32767.0
#define MAXPOINTS 10000

static int naturalGlottalSamples[]=
{
  -310,-400,530,356,224,89,23,-10,-58,-16,461,599,536,701,770,
  605,497,461,560,404,110,224,131,104,-97,155,278,-154,-1165,
  -598,737,125,-592,41,11,-247,-10,65,92,80,-304,71,167,-1,122,
  233,161,-43,278,479,485,407,266,650,134,80,236,68,260,269,179,
  53,140,275,293,296,104,257,152,311,182,263,245,125,314,140,44,
  203,230,-235,-286,23,107,92,-91,38,464,443,176,98,-784,-2449,
  -1891,-1045,-1600,-1462,-1384,-1261,-949,-730
  };
#define NOGLOTTALSAMPLES ((sizeof(naturalGlottalSamples)/sizeof(int))-1)

static struct waveInfo {
  char *name;
   
  /* waveform variables */
  int used;
  short *waveS;
  double *waveD;
  int recalcSpec;
  
  /* Spectrum Variables */
  int specSize;
  double *spectrum;
  double *specX;
  
  /* Spectrogram Variables */
  int fftSize;
  float windowDuration;
  char *greyScales;
  float *sgram;
  double *sgramX,*sgramY;
  int recalcSgram;
  int recalcGreyScales;
  
} wfi[] = {{"original",FALSE},{"synthesised",TRUE},{NULL}};

/* globals */

typGraphics *g_wav = NULL;

/* procedures */

void configure_wavarea(GtkWidget *widget)
{
  /* --- Structure doesn't exist? --- */
  if (g_wav == NULL) {
    
    /* --- Create one --- */
    g_wav = NewGraphics ();
  }

  /* --- Existing pixmap?  --- */
  if (g_wav->pixmap != NULL) {
    
    /* --- Free it --- */
    gdk_pixmap_unref (g_wav->pixmap);
  } 

  /* --- Create a new pixmap --- */
  g_wav->pixmap = gdk_pixmap_new (widget->window,
				  widget->allocation.width,
				  widget->allocation.height,
				  -1);

  gdk_draw_rectangle (g_wav->pixmap,
		      widget->style->white_gc,
		      TRUE,
		      0, 0,
		      widget->allocation.width,
		      widget->allocation.height);
  if(GetMainWindow()!=NULL)
  repaint_wave();
}

void repaint_wave()
{
  convert("");
}

void redraw_wave_expose(GtkWidget *w,GdkEventExpose  *event)
{
  gdk_draw_pixmap(w->window,
            w->style->fg_gc[GTK_WIDGET_STATE (w)],
            g_wav->pixmap,
            event->area.x, event->area.y,
            event->area.x, event->area.y,
            event->area.width, event->area.height);
}

void init_wave(GtkWidget *widget)
{
  gdk_draw_rectangle (g_wav->pixmap,
		      widget->style->white_gc,
		      TRUE,
		      0, 0,
		      widget->allocation.width,
		      widget->allocation.height);

  gdk_draw_line (g_wav->pixmap, widget->style->black_gc, 
		 25, 
		 widget->allocation.height/2,
		 widget->allocation.width-10, 
		 widget->allocation.height/2);
  gdk_draw_line (g_wav->pixmap, widget->style->black_gc, 
		 25, 
		 0,
		 25, 
		 widget->allocation.height);
  gdk_draw_line (g_wav->pixmap, widget->style->black_gc, 
		 widget->allocation.width-10, 
		 0,
		 widget->allocation.width-10, 
		 widget->allocation.height);
}

void redraw_wave(GtkWidget *widget)
{
  GdkRectangle  update_rect;

  update_rect.x = 0;
  update_rect.y = 0;
  update_rect.width = widget->allocation.width;
  update_rect.height = widget->allocation.height;

  gtk_widget_draw (widget, &update_rect);
}

int width_wave(GtkWidget *widget)
{
  return widget->allocation.width-10-25;
}

int height_wave(GtkWidget *widget)
{
  return widget->allocation.height-2;
}

void drawline_wave(GtkWidget *widget, int x, int y1, int y2)
{
  gdk_draw_line (g_wav->pixmap, 
		 GetPenRGB (NULL, 0,0, 0xffff), 
		 x, 
		 y1,
		 x, 
		 y2);
}


/** 
 * convert par-data to wav-data
 *
 * if filename is "", only wav-draw-area is updated
 * 
 * @param filename 
 * 
 * @return 
 */
gboolean convert(char *filename)
{
  klatt_global_t kglobals;

/* Each frame's 40 values are placed in this array prior to calling parwave()*/
  klatt_frame_t kframe;

  int param,frame,samplesPerFrame,sample,noFrames;
  int startSample,endSample,noSamples;
  int GsampleStep=1;
  int GdisplaySamples;

  short *dataShortPtr,*waveformDestination,*tmpPtr;
  double *dataDoublePtr;
  struct waveInfo *wpi;
  struct fpInfo *fpi;
  long *lptr;
  FILE *outfp;
  unsigned char high_byte;
  unsigned char low_byte;
  short raw_type=0;
  unsigned int size;

  long i;
  int j;
  GList *vl[PARAMETERS];
  int y;
  typValueList *pnt[PARAMETERS];
  typValueList *p_pnt[PARAMETERS];

  // wav-area-varibles:
  GtkWidget *w = (GtkWidget *)lookup_widget (GTK_WIDGET (GetMainWindow()), "draw_wave");
  int wavewidth;
  int waveheight;
  int waveseg;
  short val;

  kglobals.sample_factor = 1.0;
  kglobals.f0_flutter = 0;
  kglobals.natural_samples = naturalGlottalSamples;
  kglobals.num_samples = NOGLOTTALSAMPLES;

  kglobals.synthesis_model = FileGetBranches();  // auslesen!!
  kglobals.samrate = FileGetSamplingRate();
  kglobals.glsource = FileGetVoiceSource();
  kglobals.natural_samples = naturalGlottalSamples;
  kglobals.num_samples = FileGetDuration()/FileGetUpdateInterval();
  kglobals.sample_factor = (float) SAMPLE_FACTOR;
  kglobals.nfcascade = 0;
  kglobals.outsl = 0;
  kglobals.f0_flutter = 0;

  samplesPerFrame=(int)((FileGetUpdateInterval()*FileGetSamplingRate())/1000);
  
  /* calculate the number of samples per frame */
  kglobals.nspfr=(long)((FileGetSamplingRate()*FileGetUpdateInterval())/1000);
  parwave_init(&kglobals);

  /* calculate the number of samples*/
   noFrames=FileGetDuration()/FileGetUpdateInterval();
  
  for(j=0;j<PARAMETERS;j++)
  {
    vl[j]=(GList *) g_list_first (CurveSearchByNr(FileGetCurvesPointer(),j)->points);
    p_pnt[j]=(typValueList *)vl[j]->data;
    pnt[j]=(typValueList *)vl[j]->data;
  }

  // only if filename is set, open file and write header!
  if(strlen(filename)>0)
  {
    outfp = fopen(filename,"wb");
    if(outfp==NULL)
    {
      printf("error opening out-file!\n");
    }

    /* write header of wave */
    /* 'RIFF'+4+'WAVE'+'fmt '+4+fmt(16)+'data'+4 == 44 bytes*/ 
    fprintf(outfp,"RIFF");
    size=36+FileGetDuration()*32;
    fprintf(outfp,"%c%c%c%c",size&0xff,(size>>8)&0xff,(size>>16)&0xff,(size>>24)&0xff);
    fprintf(outfp,"WAVEfmt \20%c%c%c%c%c",0,0,0,1,0);
    i=1;
    fprintf(outfp,"%c%c",i&0xff,(i>>8)&0xff);  // always one channel!
    i=FileGetSamplingRate();
    fprintf(outfp,"%c%c%c%c",i&0xff,(i>>8)&0xff,(i>>16)&0xff,(i>>24)&0xff);  // samplingrate
    i=FileGetSamplingRate()*1*2; // Bps=samplerate*channels*samplesize;
    fprintf(outfp,"%c%c%c%c",i&0xff,(i>>8)&0xff,(i>>16)&0xff,(i>>24)&0xff);  // Bps
    i=1*2;  // Bpsmp=channels*samplesize
    fprintf(outfp,"%c%c",i&0xff,(i>>8)&0xff);  // Bpsmp
    i=2<<3;
    fprintf(outfp,"%c%c",i&0xff,(i>>8)&0xff);  // samplebits
    fprintf(outfp,"data");
    size=FileGetDuration()*32;
    fprintf(outfp,"%c%c%c%c",size&0xff,(size>>8)&0xff,(size>>16)&0xff,(size>>24)&0xff);
  }

  waveformDestination = (short *) g_malloc(sizeof(short)*(samplesPerFrame+1));
  if(waveformDestination==NULL)
  {
	perror("malloc failed");
        exit(1);
  }

/* 
   init wave-area
*/
  init_wave(w);
  wavewidth=width_wave(w);
  waveheight=height_wave(w);
  waveseg=wavewidth*100/noFrames;

/* par2wav conversation */

  for (frame=0;
       frame<noFrames;
       frame++)
  {
    i=frame*FileGetUpdateInterval();
    
    lptr=(long *)&kframe;
    for(j=0;j<PARAMETERS;j++,lptr++)
    {
      if(pnt[j]->time<i)
      {
	p_pnt[j]=pnt[j];
	if(vl[j]->next != NULL)
	{
	  vl[j]=vl[j]->next;
	  pnt[j]=(typValueList *)vl[j]->data;
	}
      }
      
      if((pnt[j]->time-p_pnt[j]->time)==0)
      {
	/* to prevent from div by 0 */
	y=pnt[j]->value;
      }
      else
      {
	y=p_pnt[j]->value + (int)((float)(pnt[j]->value-p_pnt[j]->value)/(float)(pnt[j]->time-p_pnt[j]->time)*(float)(i-p_pnt[j]->time));
      }
      *lptr=(long)y;
      
    }

    /* Change to Klatt's 1/10Hz steps */
    kframe.F0hz10 *=10;
    parwave(&kglobals,&kframe,waveformDestination);

    for(j=0;
	j<samplesPerFrame;
	j++)
    {

// seems to be unnecessary?
//#ifdef WORDS_BIGENDIAN
//      raw_type=1;
//#else
      raw_type=0;
//#endif

      low_byte = waveformDestination[j] & 0xff;
      high_byte = (waveformDestination[j] >> 8) & 0xff;

//       printf("%5d: %6d => [%02x%02x | %02x%02x]\n",j,waveformDestination[j],low_byte,high_byte,high_byte,low_byte);
      if(strlen(filename)>0)
      {
	if(raw_type==1)
	{
	  fprintf(outfp,"%c%c",high_byte,low_byte);
	}
	else
	{
	  fprintf(outfp,"%c%c",low_byte,high_byte);
	}
      }
      // display wav
      val=waveformDestination[j]*(waveheight*2) / 32768;     // correct multiplicator: (waveheight/2)
                                                             // \todo howto get correct multiplicator?
      if(j%(100*samplesPerFrame/waveseg)==0)
      {
	if(val>0)
	drawline_wave(w, 25+(waveseg*frame)/100+(j/(100*samplesPerFrame/waveseg)), (waveheight/2)-val, waveheight/2);
	else
	drawline_wave(w, 25+(waveseg*frame)/100+(j/(100*samplesPerFrame/waveseg)), waveheight/2,(waveheight/2)+abs(val));
// 	printf("%4d | %3d | %3d | %4d \n",frame,j,waveseg,(waveseg*frame)+(j/(samplesPerFrame/waveseg)));
      }
    }
   
  }

  if(strlen(filename)>0)
  {
    fclose(outfp);
  }

  redraw_wave(w);
}

