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

#define ORIG 0
#define SYNTH 1
// #define BOTH 2           // is maybe a problem!!

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif


#include <gtk/gtk.h>

#include "loadfile.h"

// #include "klatt80/parwave.h"

gboolean convert()
{
//   klatt_global_t kglobals;

//   /* Each frame's 40 values are placed in this array prior to calling parwave()*/
//   klatt_frame_t kframe;


//   int param,frame,samplesPerFrame,sample,noFrames;
//   int startSample,endSample,noSamples;
//   short *dataShortPtr,*waveformDestination;
//   double *dataDoublePtr;
//   struct waveInfo *wpi;
//   struct fpInfo *fpi;
//   long *lptr;

//   samplesPerFrame=(int)(((double)FileGetUpdatInterval()/1000.)*
// 			(double)FileGetSamplingRate());
  
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
//   kglobals.nspfr=(long)((float)FileGetSamplingRate()*
// 			((float)FileGetUpdatInterval()/1000.0));
//   parwave_init(&kglobals);

  /* calculate the number of samples*/
//  noFrames=variedParamInfo[0].noFrames;
  
  /* locate the waveform destination */
//   wpi=&wfi[SYNTH];
//   waveformDestination=wpi->waveS;
  
  /* mark that the waveform has changed */
//   wpi->recalcSpec=TRUE;
//   wpi->recalcSgram=TRUE;
//   wpi->recalcGreyScales=TRUE;
  
//   for (frame=0;
//        frame<noFrames;
//        frame++,waveformDestination+=samplesPerFrame)
//     {
//       /* insert values into a Klatt Frame */
//       for (param=0,lptr=(long *)&kframe;
// 	   param<NOVARIEDPARAMS;
// 	   param++,lptr++)
// 	{
// 	  *lptr=(long)(variedParamInfo[param].data[frame]*
// 		       (double)variedParamInfo[param].range);
// 	}
      
//       /* Change to Klatt's 1/10Hz steps */
//       kframe.F0hz10 *=10;
      
//       parwave(&kglobals,&kframe,waveformDestination);
//     }
  
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
