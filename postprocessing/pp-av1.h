/*! \file    pp-av1.h
 * \author   Lorenzo Miniero <lorenzo@meetecho.com>
 * \copyright GNU General Public License v3
 * \brief    Post-processing to generate .mp4 files out of AV1 frames (headers)
 * \details  Implementation of the post-processing code (based on FFmpeg)
 * needed to generate .mp4 files out of AV1 RTP frames.
 *
 * \ingroup postprocessing
 * \ref postprocessing
 */

#ifndef JANUS_PP_AV1
#define JANUS_PP_AV1

#include <stdio.h>

#include "pp-rtp.h"
#include "pp-ext-report.h"

/* AV1 stuff */
const char **janus_pp_av1_get_extensions(void);
int janus_pp_av1_create(char *destination, char *metadata, gboolean faststart, const char *extension);
int janus_pp_av1_preprocess(FILE *file, janus_pp_frame_packet *list, janus_pp_extension_report *report);
int janus_pp_av1_process(FILE *file, janus_pp_frame_packet *list, int *working);
void janus_pp_av1_close(void);


#endif
