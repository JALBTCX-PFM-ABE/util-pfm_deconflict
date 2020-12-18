
/*********************************************************************************************

    This is public domain software that was developed by or for the U.S. Naval Oceanographic
    Office and/or the U.S. Army Corps of Engineers.

    This is a work of the U.S. Government. In accordance with 17 USC 105, copyright protection
    is not available for any work of the U.S. Government.

    Neither the United States Government, nor any employees of the United States Government,
    nor the author, makes any warranty, express or implied, without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, or assumes any liability or
    responsibility for the accuracy, completeness, or usefulness of any information,
    apparatus, product, or process disclosed, or represents that its use would not infringe
    privately-owned rights. Reference herein to any specific commercial products, process,
    or service by trade name, trademark, manufacturer, or otherwise, does not necessarily
    constitute or imply its endorsement, recommendation, or favoring by the United States
    Government. The views and opinions of authors expressed herein do not necessarily state
    or reflect those of the United States Government, and shall not be used for advertising
    or product endorsement purposes.

*********************************************************************************************/


/*********************************************************************************************

    This program is public domain software that was developed by 
    the U.S. Naval Oceanographic Office.

    This is a work of the US Government. In accordance with 17 USC 105,
    copyright protection is not available for any work of the US Government.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*********************************************************************************************/

#ifndef VERSION

#define     VERSION     "PFM Software - pfm_deconflict V1.29 - 07/23/14"

#endif

/*

    Version 1.0
    Jan C. Depner
    09/19/01


    Version 1.1
    Jan C. Depner
    12/23/03

    Remove a single type if only one is listed on the command line.


    Version 1.11
    Jan C. Depner
    12/16/04

    Changed Usage message for PFM 4.5 directory input.


    Version 1.12
    Jan C. Depner
    02/25/05

    Switched to open_existing_pfm_file from open_pfm_file.


    Version 1.13
    Jan C. Depner
    10/26/05

    Changed usage for PFM 4.6 handle file use.


    Version 1.14
    Jan C. Depner
    08/31/06

    Added PFM_NAVO_LLZ_DATA to comments and Usage.


    Version 1.2
    Jan C. Depner
    09/14/06

    Added ability to parse the filenames for text strings and deconflict based on that.


    Version 1.21
    Jan C. Depner
    09/27/06

    Added -r option (PFM_REFERENCE instead of PFM_FILTER_INVAL).


    Version 1.22
    Jan C. Depner
    10/22/07

    Added fflush calls after prints to stderr since flush is not automatic in Windows.


    Version 1.23
    Jan C. Depner
    04/07/08

    Replaced single .h files from utility library with include of nvutility.h


    Version 1.24
    Jan C. Depner
    01/23/09

    Replaced PFM_MRG_DATA with PFM_CHRTR_DATA.


    Version 1.24
    Jan C. Depner
    01/29/09

    Set checkpoint to 0 prior to calling open_existing_pfm_file.


    Version 1.25
    Jan C. Depner
    03/20/09

    Replaced PFM_DEMO_DATA with PFM_WLF_DATA.


    Version 1.26
    Jan C. Depner
    06/11/09

    Replaced PFM_SHOALS_XY2_DATA with PFM_CHARTS_HOF_DATA.


    Version 1.27
    Jan C. Depner (PFM Software)
    03/17/14

    Brought list of PFM data types up to date (to include CZMIL).


    Version 1.28
    Jan C. Depner (PFM Software)
    05/07/14

    Check return from fgets call.


    Version 1.29
    Jan C. Depner (PFM Software)
    07/23/14

    - Switched from using the old NV_INT64 and NV_U_INT32 type definitions to the C99 standard stdint.h and
      inttypes.h sized data types (e.g. int64_t and uint32_t).

*/
