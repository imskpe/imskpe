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


#include <gtk/gtk.h>
#include "convert.h"

#include "loadfile.h"

#include "klatt80/parwave.h"

/* defaults */

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

gboolean convert()
{
  klatt_global_t kglobals;

/* Each frame's 40 values are placed in this array prior to calling parwave()*/
  klatt_frame_t kframe;

  int param,frame,samplesPerFrame,sample,noFrames;
  int startSample,endSample,noSamples;
  short *dataShortPtr,*waveformDestination;
  double *dataDoublePtr;
  struct waveInfo *wpi;
  struct fpInfo *fpi;
  long *lptr;

  printf("convert start\n");
  
  kglobals.sample_factor = 1.0;
  kglobals.f0_flutter = 0;
  kglobals.natural_samples = naturalGlottalSamples;
  kglobals.num_samples = NOGLOTTALSAMPLES;


 //  /* calculate the desired duration */
//   GframeDuration= (float)updateInterval/1000.;
//   Gduration=GframeDuration * (float)noFrames;
//   fixedParamInfo[DU].currentValue=(int)(Gduration * 1000.);

//   /* Each frame's 40 values are placed in this array prior to calling parwave()*/
//   klatt_frame_t kframe;

  samplesPerFrame=(int)(((double)FileGetUpdateInterval()/1000.)*
 			(double)FileGetSamplingRate());
  
  /* fill in the correct values for the global parameters */
//   for (fpi=fixedParamInfo;
//        fpi->name!=NULL;
//        fpi++)
//     {
//       /* fill in current value in the frame */
//       if (fpi->kframePtr!=NULL)
// 	*(fpi->kframePtr)=fpi->currentValue;
//     }
  
  /* calculate the number of samples per frame */
   kglobals.nspfr=(long)((float)FileGetSamplingRate()*
 			((float)FileGetUpdateInterval()/1000.0));
   parwave_init(&kglobals);

  /* calculate the number of samples*/
   noFrames=FileGetDuration()/FileGetUpdateInterval();
  
  /* locate the waveform destination */
   wpi=&wfi[1]; // [SYNTH];
   waveformDestination=wpi->waveS;
  
  /* mark that the waveform has changed */
   wpi->recalcSpec=TRUE;
   wpi->recalcSgram=TRUE;
   wpi->recalcGreyScales=TRUE;
  
   for (frame=0;
        frame<noFrames;
        frame++,waveformDestination+=samplesPerFrame)
     {
       // todo: calculate values like in savefile and write to this struct!
       // where does the memory comes from?
       // lptr must be malloced? where does this happen in kpe.c ?
       // hmm IN: one frame ... out one wavedest ...
       // memory for wavedest?

       /* insert values into a Klatt Frame */
//        for (param=0,lptr=(long *)&kframe;
//  	   param<;
//  	   param++,lptr++)
//  	{
// 	  *lptr=(long)(variedParamInfo[param].data[frame]*
// 		       (double)variedParamInfo[param].range);
	  ;
//  	}
      
       /* Change to Klatt's 1/10Hz steps */
       kframe.F0hz10 *=10;
      
       parwave(&kglobals,&kframe,waveformDestination);
     }
  
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
