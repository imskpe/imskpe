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
typedef enum {BOTH=1, PARALLEL=2} branchTyp;

/** File-struct */
typedef struct {
    char *filename;	   /**< Name of File (incl. Path) */
    gboolean ischanged;    /**< if (file is changed): TRUE else FALSE */
    gboolean isnew;        /**< if (file is new): TRUE else FALSE */
    GList *curves;         /**< data for curves */

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
gboolean FilePrepare(char *filename);

gboolean FileIsDefined();

char *FileGetFilename();
gboolean FileSetFilename(char *foo);

gboolean FileSetDuration(unsigned int d);
unsigned int FileGetDuration();
gboolean FileSetUpdateInterval(unsigned int ui);
unsigned int FileGetUpdateInterval();
gboolean FileSetSamplingRate(unsigned int sr);
unsigned int FileGetSamplingRate();
gboolean FileSetNumberFormants(unsigned int f);
unsigned int FileGetNumberFormants();
gboolean FileSetVoiceSource(unsigned int vs);
unsigned int FileGetVoiceSource();
gboolean FileSetBranches(unsigned int cp);
unsigned int FileGetBranches();

void FileSetIsNew(gboolean b);
gboolean FileGetIsNew();
void FileSetIsChanged(gboolean b);
gboolean FileGetIsChanged();

void FileSetCurvesPointer(GList *curves);
GList *FileGetCurvesPointer();
void FileInit();
