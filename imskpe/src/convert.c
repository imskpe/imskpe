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
#include <gtk/gtk.h>
#include "convert.h"

#include "loadfile.h"
#include "loadpar.h"

#include "klatt80/parwave.h"

/* defaults */

#define SAMPLE_FACTOR 0.00001

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

gboolean convert(char *filename)
{
  klatt_global_t kglobals;

/* Each frame's 40 values are placed in this array prior to calling parwave()*/
  klatt_frame_t kframe;

  int param,frame,samplesPerFrame,sample,noFrames;
  int startSample,endSample,noSamples;
  short *dataShortPtr,*waveformDestination,*tmpPtr;
  double *dataDoublePtr;
  struct waveInfo *wpi;
  struct fpInfo *fpi;
  long *lptr;
  FILE *outfp;
  unsigned char high_byte;
  unsigned char low_byte;
  short raw_type;
  unsigned int size;

  long i;
  int j;
  typValueList *pnt[PARAMETERS];
  typValueList *p_pnt[PARAMETERS];
  GList *vl[PARAMETERS];
  int y;

//   printf("convert start\n");
  
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

//   printf("DEBUG: \n");
//   printf("DU: %d\n",FileGetDuration());
//   printf("UI: %d\n",FileGetUpdateInterval());
//   printf("SR: %d\n",FileGetSamplingRate());
//   printf("VS: %d\n",FileGetVoiceSource());
//   printf("PP: %d\n",FileGetBranches());

  samplesPerFrame=(int)(((double)FileGetUpdateInterval()/1000.)*
 			(double)FileGetSamplingRate());
  
  /* calculate the number of samples per frame */
   kglobals.nspfr=(long)((float)FileGetSamplingRate()*
 			((float)FileGetUpdateInterval()/1000.0));
   parwave_init(&kglobals);

  /* calculate the number of samples*/
   noFrames=FileGetDuration()/FileGetUpdateInterval();
  
  for(j=0;j<PARAMETERS;j++)
  {
    vl[j]=(GList *) g_list_first (CurveSearchByNr(FileGetCurvesPointer(),j)->points);
    p_pnt[j]=(typValueList *)vl[j]->data;
    pnt[j]=(typValueList *)vl[j]->data;
  }

//   printf("\nframes: %d\n",noFrames);

  outfp = fopen(filename,"w");
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
  

  waveformDestination = (int*) malloc(sizeof(int)*20000);  // 20k = MAX_SAM
  if(waveformDestination==NULL)
  {
	perror("malloc failed");
        exit(1);
  }

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
//       printf("%d ",y);
      
    }
//     printf("\n");

    /* Change to Klatt's 1/10Hz steps */
    kframe.F0hz10 *=10;
    parwave(&kglobals,&kframe,waveformDestination);
    
    for(j=0;
	j<samplesPerFrame;
	j++)
    {
      low_byte = waveformDestination[j] & 0xff;
      high_byte = waveformDestination[j] >> 8;
      
      if(raw_type==1)
      {
	fprintf(outfp,"%c%c",high_byte,low_byte);
      }
      else
      {
	fprintf(outfp,"%c%c",low_byte,high_byte);
      }
//       printf("write value %5d [%2x%2x]\n",j,low_byte,high_byte);
    }
    
  }
//   printf("convert ended\n");

  fclose(outfp);

/* use later for wav-drawarea! */
//   /* calculate some new waveform display Xcoords */
//   startSample=(int)(Gstart*(float)GsampleRate);
//   endSample=(int)(Gend*(float)GsampleRate);
//   noSamples=(endSample-startSample);
  
//   GsampleStep=1;
//   while ( ((int)(float)noSamples/(float)GsampleStep)>MAXPOINTS)
//     GsampleStep++;
//   GdisplaySamples=noSamples/GsampleStep;
  
//   dataShortPtr=wpi->waveS+startSample;
//   dataDoublePtr=wpi->waveD;
//   for (sample=0;
//        sample<GdisplaySamples;
//        sample++,dataShortPtr+=GsampleStep,dataDoublePtr++)
//     *dataDoublePtr=(double)*dataShortPtr/MAXSAMPLE;
}
