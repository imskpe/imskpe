/* For Emacs: -*-c-*- */
/*
    loadfile.h - Part of IMSKPE

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
 * @file   loadfile.h
 * @author Andreas Madsack

 * @brief  header for fileopen-wrapper
 * 
 * \todo   maybe use cfg-routines for saving DU,UI,SR,NF,SS,CP -> all in a list
 */

typedef enum {IMPULSE=1, NATURAL=2, SAMPLED=3} vcTyp;
typedef enum {BOTH, PARALLEL} branchTyp;

/** File-struct */
typedef struct {
    char *filename;	   /**< Name of File (incl. Path) */
    gboolean ischanged;    /**< if (file is changed): TRUE else FALSE */
    GList *curves;         /**< data for curves */

    /** \note are formants (the list) file data or preferences data?
     *        i think no -> formants list data must become cfg-data! 
     */
    /*    GList *formants;      // -> put in preferences ?! */
    
    /** DU, duration in ms, range: 0+, default: 500 */
    unsigned int duration;
    /** UI, update interval in ms, range: 2-20, default: 10 */
    unsigned int update_interval;
    /** SR, sampling rate in Hz, range: 5000-24000, default: 16000 */
    unsigned int sampling_rate;
    /** NF, number of formants, range 1-6, default: 6 */
    unsigned int formant_number;
    /** SS, voice source, values: 1 (impulse), 2 (natural), 3 (sampled) */
    vcTyp voice_source;
    /** CP, use cascade and or parallel branches,
        values: 1 (cascade + parallel branches), 2 (parallel only) */
    branchTyp branches;

} typFile;


void FileOpen(char *filename);

gboolean FileSetDuration(unsigned int d);

gboolean FileSetUpdateInterval(unsigned int ui);

gboolean FileSetSamplingRate(unsigned int sr);

gboolean FileSetNumberFormants(unsigned int f);

gboolean FileSetVoiceSource(unsigned int vs);

gboolean FileSetBranches(unsigned int cp);

void FileSetIsChanged(gboolean b);
gboolean FileGetIsChanged();

void FileSetCurvesPointer(GList *curves);
GList *FileGetCurvesPointer();
//GList *FileGetFormantsPointer();
