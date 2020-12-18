
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "nvutility.h"

#include "pfm.h"

#include "version.h"


#define PARSE_OFFSET 1000


/*****************************************************************************

    Program:    pfm_deconflict

    Purpose:    See Usage message below.

    Programmer: Jan C. Depner

    Date:       09/19/01

*****************************************************************************/


int32_t main (int32_t argc, char **argv)
{
  int32_t             i, j, m, n, percent = 0, old_percent = -1, pfm_handle; 
  uint32_t            k;
  int16_t             type[50], type_count, parse_count, *file_type, file_count;
  PFM_OPEN_ARGS       open_args;
  float               total;
  NV_I32_COORD2       coord;
  DEPTH_RECORD        *dep;
  BIN_RECORD          bin;
  uint8_t             type_hit[50], recomp, alpha, hit;
  uint32_t            flag = PFM_FILTER_INVAL;
  char                path[512], parse[50][50];



  fprintf (stderr, "\n\n%s\n\n", VERSION);
  fflush (stderr);


  if (argc < 3)
    {
      fprintf (stderr, "\n\nUsage: pfm_deconflict <PFM_HANDLE_FILE or PFM_LIST_FILE> [-r] <data type> <data type> ...\n\n");
      fprintf (stderr, "Marks as filter edited all points in a bin that\n");
      fprintf (stderr, "are of lower priority based on a list of data\n");
      fprintf (stderr, "type priorities.  If the -r option is used the data\n");
      fprintf (stderr, "will be marked as PFM_REFERENCE data.  This way the\n");
      fprintf (stderr, "will not be invalidated but will not be used for\n");
      fprintf (stderr, "computing the surface or selecting soundings.\n");
      fprintf (stderr, "An example for the priority list:\n");
      fprintf (stderr, "If the priority list is 3 2 5 1 then LIDAR data (type 3)\n");
      fprintf (stderr, "will override GSF data (2), TEXT data (5), and merge\n");
      fprintf (stderr, "data (1).  If there is no LIDAR data then the\n");
      fprintf (stderr, "sequence will start with GSF data.  All data in\n");
      fprintf (stderr, "the bin that is not of the highest priority type\n");
      fprintf (stderr, "will be marked as PFM_FILTER_INVAL (or PFM_REFERENCE).\n");
      fprintf (stderr, "If you only list one data type then that type will be\n");
      fprintf (stderr, "marked as PFM_FILTER_INVAL (or PFM_REFERENCE) if there\n");
      fprintf (stderr, "is data of any other type in the bin.  You may also use\n");
      fprintf (stderr, "a text string instead of a data type.  The text string\n");
      fprintf (stderr, "will be searched for in the file names.  For example you\n");
      fprintf (stderr, "could use mba to indicate just EM121a GSF data files as\n");
      fprintf (stderr, "follows:\n");
      fprintf (stderr, "pfm_deconflict FILE mba 3 2 would remove GSF in favor of\n");
      fprintf (stderr, "LIDAR except in the case of EM121a files.  The text strings\n");
      fprintf (stderr, "take precedence over the data type.  Important note:\n");
      fprintf (stderr, "text strings must contain at least one non-numeric character.\n\n");
      fprintf (stderr, "Press 'Enter' to see a list of PFM data types:");
      fflush (stderr);

      if (fgets (path, sizeof (path), stdin) == NULL)
	{
	  fprintf (stderr, "Error reading from STDIN in file %s, function %s, line %d.  This shouldn't happen!\n", __FILE__, __FUNCTION__, __LINE__ - 2);
	  exit (-1);
	}

      fprintf (stderr, "\nPFM_CHRTR_DATA        1\n");
      fprintf (stderr, "PFM_GSF_DATA            2\n");
      fprintf (stderr, "PFM_SHOALS_OUT_DATA     3\n");
      fprintf (stderr, "PFM_CHARTS_HOF_DATA     4\n");
      fprintf (stderr, "PFM_NAVO_ASCII_DATA     5\n");
      fprintf (stderr, "PFM_HTF_DATA            6\n");
      fprintf (stderr, "PFM_WLF_DATA            7\n");
      fprintf (stderr, "PFM_DTM_DATA            8\n");
      fprintf (stderr, "PFM_HDCS_DATA           9\n");
      fprintf (stderr, "PFM_ASCXYZ_DATA         10\n");
      fprintf (stderr, "PFM_CNCBIN_DATA         11\n");
      fprintf (stderr, "PFM_STBBIN_DATA         12\n");
      fprintf (stderr, "PFM_XYZBIN_DATA         13\n");
      fprintf (stderr, "PFM_OMG_DATA            14\n");
      fprintf (stderr, "PFM_CNCTRACE_DATA       15\n");
      fprintf (stderr, "PFM_NEPTUNE_DATA        16\n");
      fprintf (stderr, "PFM_SHOALS_1K_DATA      17\n");
      fprintf (stderr, "PFM_SHOALS_ABH_DATA     18\n");
      fprintf (stderr, "PFM_SURF_DATA           19\n");
      fprintf (stderr, "PFM_SMF_DATA            20\n");
      fprintf (stderr, "PFM_VISE_DATA           21\n");
      fprintf (stderr, "PFM_PFM_DATA            22\n");
      fprintf (stderr, "PFM_MIF_DATA            23\n");
      fprintf (stderr, "PFM_SHOALS_TOF_DATA     24\n");
      fprintf (stderr, "PFM_UNISIPS_DEPTH_DATA  25\n");
      fprintf (stderr, "PFM_HYD93B_DATA         26\n");
      fprintf (stderr, "PFM_LADS_DATA           27\n");
      fprintf (stderr, "PFM_HS2_DATA            28\n");
      fprintf (stderr, "PFM_9COLLIDAR_DATA      29\n");
      fprintf (stderr, "PFM_FGE_DATA            30\n");
      fprintf (stderr, "PFM_PIVOT_DATA          31\n");
      fprintf (stderr, "PFM_MBSYSTEM_DATA       32\n");
      fprintf (stderr, "PFM_LAS_DATA            33\n");
      fprintf (stderr, "PFM_BDI_DATA            34\n");
      fprintf (stderr, "PFM_NAVO_LLZ_DATA       35\n");
      fprintf (stderr, "PFM_LADSDB_DATA         36\n");
      fprintf (stderr, "PFM_DTED_DATA           37\n");
      fprintf (stderr, "PFM_HAWKEYE_HYDRO_DATA  38\n");
      fprintf (stderr, "PFM_HAWKEYE_TOPO_DATA   39\n");
      fprintf (stderr, "PFM_BAG_DATA            40\n");
      fprintf (stderr, "PFM_CZMIL_DATA          41\n\n");
      exit (-1);
    }


  /*  We try to read the data type as a number.  If it isn't a number we read it as a text string to
      be parsed for in the file names.  */

  parse_count = 0;
  type_count = 0;
  for (i = 2 ; i < argc ; i++)
    {
      alpha = NVFalse;
      for (k = 0 ; k < strlen (argv[i]) ; k++)
        {
          if (!isdigit (argv[i][k]))
            {
              alpha = NVTrue;
              break;
            }
        }

      if (alpha)
        {
          /*  Check for the 'reference' option.  */

          if (!strcmp (argv[i], "-r"))
            {
              flag = PFM_REFERENCE;
            }
          else
            {
              strcpy (parse[parse_count], argv[i]);
              type[type_count] = PARSE_OFFSET + parse_count;
              parse_count++;
              type_count++;
            }
        }
      else
        {
          sscanf (argv[i], "%hd", &type[type_count]);
          if (type[type_count] < 0 || type[type_count] > PFM_DATA_TYPES - 1)
            {
              strcpy (parse[parse_count], argv[i]);
              type[type_count] = PARSE_OFFSET + parse_count;
              parse_count++;
            }
          type_count++;
        }
    }


  strcpy (open_args.list_path, argv[1]);


  open_args.checkpoint = 0;
  pfm_handle = open_existing_pfm_file (&open_args);


  if (pfm_handle < 0) pfm_error_exit (pfm_error);


  /*  Allocate the memory for the file types.  */

  file_count = get_next_list_file_number (pfm_handle);

  file_type = (int16_t *) calloc (file_count, sizeof (int16_t));

  if (file_type == NULL)
    {
      perror ("Allocating memory for file types");
      return (-1);
    }


  /*  Read all of the PFM source file names to check for file type and see if they match any of the text strings
      that were input on the command line.  */

  for (i = 0 ; i < file_count ; i++) 
    {
      /*  Grab the file type from the PFM .ctl file.  */

      read_list_file (pfm_handle, i, path, &file_type[i]);


      /*  Compare the file name to the text strings (if any).  */

      for (j = 0 ; j < parse_count ; j++)
        {
          /*  If we get a match we want to replace the file type with an offset type so that the real file type
              (ie 2 for GSF) doesn't get considered for files that match.  */

          if (strstr (path, parse[j])) 
            {
              file_type[i] = PARSE_OFFSET + j;
              break;
            }
        }
    }


  total = open_args.head.bin_height;

  fprintf (stderr, "\n\n");
  fflush (stderr);


  /*  Loop through the PFM.  */

  for (i = 0; i < open_args.head.bin_height; i++)
    {
      coord.y = i;

      for (j = 0; j < open_args.head.bin_width; j++)
        {
          coord.x = j;

          recomp = NVFalse;

          bin.coord = coord;


          /*  Read the depth array.  */

          if (!read_bin_depth_array_index (pfm_handle, &bin, &dep))
            {
              for (m = 0 ; m < type_count ; m++) type_hit[m] = NVFalse;

              for (k = 0 ; k < bin.num_soundings ; k++)
                {
                  /*  If it's already invalid, don't mess with it.  */

                  if (!(dep[k].validity & (PFM_INVAL | PFM_DELETED | PFM_REFERENCE)))
                    {
                      /*  Loop through all file/parse types.  */

                      for (m = 0 ; m < type_count ; m++)
                        {
                          /*  If we had only one type or string on the command line we want to invalidate that
                              in deference to any other data type.  */

                          if (type_count == 1)
                            {
                              if (file_type[dep[k].file_number] != type[m])
                                {
                                  type_hit[m] = NVTrue;
                                  break;
                                }
                            }


                          /*  We had multiple types/strings.  */

                          else
                            {
                              if (file_type[dep[k].file_number] == type[m]) 
                                {
                                  type_hit[m] = NVTrue;
                                  if (!m) break;
                                }
                            }
                        }
                    }
                }


              /*  Check each type.  */

              for (m = 0 ; m < type_count ; m++)
                {
                  /*  If we had a "hit" on this file/parse type when we checked the record (above), loop through
                      the depth array and set the appropriate validity bits.  */

                  if (type_hit[m])
                    {
                      for (k = 0 ; k < bin.num_soundings ; k++)
                        {
                          if (!(dep[k].validity & (PFM_INVAL | PFM_DELETED | PFM_REFERENCE)))
                            {
                              /*  If we had only one type or string on the command line we want to invalidate that
                                  in deference to any other data type.  */

                              if (type_count == 1)
                                {
                                  if (file_type[dep[k].file_number] == type[m])
                                    {
                                      dep[k].validity |= flag;
                                      update_depth_record_index (pfm_handle, &dep[k]);
                                      recomp = NVTrue;
                                    }
                                }


                              /*  We had multiple types/strings.  */

                              else
                                {
                                  if (file_type[dep[k].file_number] != type[m])
                                    {
                                      /*  If the data type wasn't on the command line, ignore it completely.  */

                                      hit = NVFalse;
                                      for (n = 0 ; n < type_count ; n++)
                                        {
                                          if (file_type[dep[k].file_number] == type[n])
                                            {
                                              hit = NVTrue;
                                              break;
                                            }
                                        }

                                      if (hit)
                                        {
                                          dep[k].validity |= flag;
                                          update_depth_record_index (pfm_handle, &dep[k]);
                                          recomp = NVTrue;
                                        }
                                    }
                                }
                            }
                        }
                      break;
                    }
                }


              /*  If we modified anything, recompute the bin values.  */

              if (recomp) 
                {
                  bin.validity &= ~PFM_CHECKED;
                  recompute_bin_values_from_depth_index (pfm_handle, &bin, PFM_CHECKED, dep);
                }
              free (dep);
            }
        }

      percent = (i / total) * 100.0;
      if (percent != old_percent)
        {
          old_percent = percent;
          fprintf (stderr, "Deconflicting bins : %03d%%          \r", percent);
          fflush (stderr);
        }
    }

  fprintf (stderr, "100%% bins deconflicted                       \n\n");
  fflush (stderr);

  close_pfm_file (pfm_handle);


  /*  Please ignore the following line.  It is useless.  Except...

      On some versions of Ubuntu, if I compile a program that doesn't use the math
      library but it calls a shared library that does use the math library I get undefined
      references to some of the math library functions even though I have -lm as the last
      library listed on the link line.  This happens whether I use qmake to build the
      Makefile or I have a pre-built Makefile.  Including math.h doesn't fix it either.
      The following line forces the linker to bring in the math library.  If there is a
      better solution please let me know at area.based.editor AT gmail DOT com.  */

  float ubuntu; ubuntu = 4.50 ; ubuntu = fmod (ubuntu, 1.0);


  return (0);
}
