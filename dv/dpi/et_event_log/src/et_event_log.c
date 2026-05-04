/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <svdpi.h>
#include <vpi_user.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>

#define EVL_DPI_EVENT_SIZE (256 / 32)


#ifndef uint64_t
   #define uint64_t __uint64_t
#endif


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static int  next_fd             = 0;
static FILE *dpi_event_fd[32]   = { (FILE *) NULL, (FILE *) NULL, (FILE *) NULL, (FILE *) NULL,
                                    (FILE *) NULL, (FILE *) NULL, (FILE *) NULL, (FILE *) NULL,
                                    (FILE *) NULL, (FILE *) NULL, (FILE *) NULL, (FILE *) NULL,
                                    (FILE *) NULL, (FILE *) NULL, (FILE *) NULL, (FILE *) NULL,
                                    (FILE *) NULL, (FILE *) NULL, (FILE *) NULL, (FILE *) NULL,
                                    (FILE *) NULL, (FILE *) NULL, (FILE *) NULL, (FILE *) NULL,
                                    (FILE *) NULL, (FILE *) NULL, (FILE *) NULL, (FILE *) NULL,
                                    (FILE *) NULL, (FILE *) NULL, (FILE *) NULL, (FILE *) NULL };
static char *dpi_event_file[32] = { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
                                    "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" };


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
uint64_t
et_get_time_of_day () {
   struct timeval tv;

   gettimeofday(&tv, (struct timezone *) NULL);
   return (uint64_t) tv.tv_sec;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
uint64_t
et_tcl_get_cur_time () {
   time_t    cur_time;
   struct tm tm;

   if (time(&cur_time) != (time_t) -1) {
      if (localtime_r(&cur_time, &tm) != (struct tm *) NULL) {
         return ( (((uint64_t) tm.tm_year & 0xff) << 40) |
                  (((uint64_t) tm.tm_mon & 0x7f)  << 32) |
                  (((uint64_t) tm.tm_mday & 0x7f) << 24) |
                  (((uint64_t) tm.tm_hour & 0x7f) << 16) |
                  (((uint64_t) tm.tm_min & 0x7f)  << 8)  |
                   ((uint64_t) tm.tm_sec & 0x7f) );
      }
   }
   return (uint64_t) 0;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
char *
et_get_env (char *env_name) {
   if (getenv(env_name) == (char *) NULL) {
      return "";
   }
   return getenv(env_name);
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
int
et_open_dpi_event_file (char *file_name) {
   int  ii;
   FILE *fd;

   for (ii = 0; ii < 32; ii++) {
      if (strcmp(file_name, dpi_event_file[ii]) == 0) {
         return ii;
      }
   }
   if (next_fd > 32) {
      vpi_printf("ERROR: cannot open DPI event file \"%s\" ... too many open files\n", file_name);
      return -1;
   }
   if ((fd = fopen(file_name, "w")) == (FILE *) NULL) {
      vpi_printf("ERROR: cannot open DPI event file \"%s\"\n", file_name);
      return -1;
   }
   if ((dpi_event_file[next_fd] = (char *) malloc(sizeof(char) * (strlen(file_name) + 2))) == (char *) NULL) {
      vpi_printf("ERROR: cannot open DPI event file \"%s\" ... out of memory\n", file_name);
      return -1;
   }
   dpi_event_fd[next_fd] = fd;
   strcpy(dpi_event_file[next_fd], file_name);
   next_fd++;
   return (next_fd - 1);
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
void
et_close_dpi_event_files () {
   int ii;

   for (ii = 0; ii < 32; ii++) {
      if (dpi_event_fd[ii] != (FILE *) NULL) {
         fclose(dpi_event_fd[ii]);
      }
   }
   return;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
void
et_write_dpi_event (int fd_in, svBitVec32 *packed_data) {
/*
   int      ar_valid;
   int      aw_valid;
   int      r_valid;
   int      b_valid;
   int      ax_addr_hi;
   int      ax_addr_lo;
   int      ax_id;
   int      ax_user;
   int      ax_size;
   int      ax_qos;
   int      ax_resp;
   char line[1024];

   ar_valid = packed_data[2] & 1;
   aw_valid = packed_data[2] & 2;
   r_valid  = packed_data[2] & 4;
   b_valid  = packed_data[2] & 8;
   *line = (char) 0;
   sprintf(line, "S%02d:N%d:C%d:T%d:A%d %d (cycle %0d)", (packed_data[1] >> 24) & 0xff, (packed_data[1] >> 20) & 0xf, (packed_data[1] >> 16) & 0xf, (packed_data[1] >> 14) & 0x3, (packed_data[1] >> 8) & 0x3f, packed_data[1] & 0xff, packed_data[0] * 2);
   if (ar_valid != 0) {
      ax_addr_hi = ((packed_data[3] >> 4) & 0xff);
      ax_addr_lo = ((packed_data[3] & 0xf) << 28) | ((packed_data[2] >> 4) & 0xfffffff);
      ax_id      = ((packed_data[3] >> 12) & 0xfffff);
      ax_size    = 1 << (packed_data[4] & 0xf);
      ax_qos     = ((packed_data[4] >> 8) & 0xf);
      sprintf(line, "%s - AXI_READ 0x%02x%08x 0x%05x %0d %d", line, ax_addr_hi, ax_addr_lo, ax_id, ax_size, ax_qos);
   }
   if (aw_valid != 0) {
      ax_addr_hi = (packed_data[5] >> 8) & 0xff;
      ax_addr_lo = ((packed_data[5] & 0xff) << 24) | ((packed_data[4] >> 8) & 0xffffff);
      ax_id      = ((packed_data[6] & 0xf) << 16) | ((packed_data[5] >> 16) & 0xffff);
      ax_size    = 1 << ((packed_data[6] >> 4) & 0xf);
      ax_user    = ((packed_data[6] >> 8) & 0xffff);
      ax_qos     = ((packed_data[6] >> 24) & 0xf);
      sprintf(line, "%s - AXI_WRITE 0x%02x%08x 0x%05x %0d 0x%04x %d", line, ax_addr_hi, ax_addr_lo, ax_id, ax_size, ax_user, ax_qos);
   }
   if (r_valid != 0) {
      ax_resp = ((packed_data[6] >> 28) & 0x3);
      ax_id   = (packed_data[7] & 0xfffff);
      sprintf(line, "%s - R %d 0x%05x", line, ax_resp, ax_id);
   }
   if (b_valid != 0) {
      ax_resp = ((packed_data[7] >> 20) & 0x3);
      ax_id   = ((packed_data[8] & 0xfff) << 8) | ((packed_data[7] >> 24) & 0xff);
      sprintf(line, "%s - B %d 0x%05x", line, ax_resp, ax_id);
   }
   vpi_printf("== et_write_dpi_event(): 0x%08x_%08x_%08x_%08x_%08x_%08x_%08x_%08x\n", packed_data[7], packed_data[6], packed_data[5], packed_data[4], packed_data[3], packed_data[2], packed_data[1], packed_data[0]);
   vpi_printf("== et_write_dpi_event(): %s\n", line);
*/
   if ((fd_in < 0) || (fd_in > 31) || (dpi_event_fd[fd_in] == (FILE *) NULL)) {
      return;
   }
   fwrite(packed_data, sizeof(svBitVec32), EVL_DPI_EVENT_SIZE, dpi_event_fd[fd_in]);
}
