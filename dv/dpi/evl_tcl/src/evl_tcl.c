/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <svdpi.h>
#include <sv_vpi_user.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <tcl.h>

#define EVL_DPI_EVENT_SIZE (256 / 32)

#define EVL_TCL_LINE_LIMIT (8192 - ((int) sizeof(struct script_data_s *)))
#define EVL_MIN_ARG_LENGTH 128

#define EVL_MAX_TCL_THREAD      4096
#define EVL_MAX_TCL_ARG         64

#define EVL_TCL_RESULT_INT      0
#define EVL_TCL_RESULT_INT_LIST 1
#define EVL_TCL_RESULT_STR      2
#define EVL_TCL_RESULT_STR_LIST 3

#define EVL_TCL_PARSE_NONE          0
#define EVL_TCL_PARSE_AGENT         1
#define EVL_TCL_PARSE_AGENT_DEFAULT 2

#define EVL_TCL_END_OF_THREAD        -1
#define EVL_TCL_CANNOT_CREATE_THREAD -2
#define EVL_TCL_CANNOT_START_THREAD  -3
#define EVL_TCL_THREAD_TIMEOUT       -4
#define EVL_TCL_MALLOC_ERROR         -5
#define EVL_TCL_ALLOC_ERROR          -6
#define EVL_TCL_EVAL_ERROR           -7
#define EVL_TCL_ARG_COUNT_ERROR      -8

#ifndef uint64_t
   #define uint64_t __uint64_t
#endif

static int    process_error();


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
typedef struct tcl_file_path_s {
      char                   *path_name;
      struct tcl_file_path_s *next_ptr;
   } tcl_file_path_s;


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
typedef struct script_data_s {
      char                 line[EVL_TCL_LINE_LIMIT];
      struct script_data_s *next_ptr;
   } script_data_s;


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
typedef struct tcl_str_obj_s {
      char                 *str_ptr;
      int                  str_len;
      struct tcl_str_obj_s *next_ptr;
   } tcl_str_obj_s;


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
typedef struct tcl_int_obj_s {
      uint64_t             value;
      int                  valid;
      struct tcl_int_obj_s *next_ptr;
   } tcl_int_obj_s;


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
typedef struct tcl_cmd_enc_s {
      struct tcl_cmd_enc_s *next_cmd;
      char                 *cmd_name;
      char                 *usage;
      int                  cmd_encoding;
      int                  parse_type;
      int                  min_arg_count;
      int                  max_arg_count;
      int                  return_type;
   } tcl_cmd_enc_s;


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
typedef struct tcl_thread_s {
      int             thread_num;
      int             id;
      Tcl_ThreadId    *tcl_thread_id;
      Tcl_Interp      *tcl_interp;
      char            *file_name;
      FILE            *fd;
      FILE            *replay_fd;
      char            *tcl_script;
      char            *eval_str;
      int             next_cmd_encoding;
      int             thread_active;
      int             cmd_encoding;
      int             str_arg_count;
      int             int_arg_count;
      int             str_res_count;
      int             int_res_count;
      int             arg_count;
      int             res_count;
      int             lock;
      int             in_error;
      tcl_cmd_enc_s   *tcl_cmds;
      tcl_str_obj_s   *first_str_arg;
      tcl_int_obj_s   *first_int_arg;
      tcl_str_obj_s   *last_str_arg;
      tcl_int_obj_s   *last_int_arg;
      tcl_str_obj_s   *first_str_res;
      tcl_int_obj_s   *first_int_res;
      tcl_str_obj_s   *last_str_res;
      tcl_int_obj_s   *last_int_res;
      pthread_mutex_t mutex;
      pthread_cond_t  xfr_to_verilog;
      pthread_cond_t  xfr_to_dpi;
      char            last_error[128];
   } tcl_thread_s;


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static char            *null_str              = "";
static int             plus_args_extracted    = 0;
static int             plus_args_count        = 0;
static int             last_thread            = -1;
static char            last_global_error[256] = "";
static char            net_value_str[1024]    = "";
static int             have_time              = 0;
static char            **plus_arg_ptrs;
static char            **next_plus_arg_ptr;
static char            time_str[64];
static struct tm       tm;
static tcl_file_path_s *include_paths         = (tcl_file_path_s *) NULL;
static tcl_file_path_s *last_include_path     = (tcl_file_path_s *) NULL;
static tcl_file_path_s *file_paths            = (tcl_file_path_s *) NULL;
static tcl_file_path_s *last_file_path        = (tcl_file_path_s *) NULL;
static tcl_thread_s    *thread_ptrs[EVL_MAX_TCL_THREAD];


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static uint64_t
decimal_convert (char *begp) {
   uint64_t value;
   uint64_t cur_value;
   char     c;

   value = 0;
   while ((c = *begp) != (char) 0) {
      if ((c < '0') || (c > '9')) {
         break;
      }
      begp++;
      cur_value = (uint64_t) (c - '0');
      value = (value * 10) + cur_value;
   }
   return (uint64_t) value;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static uint64_t
hex_convert (char *begp) {
   uint64_t value;
   uint64_t cur_value;
   char     c;

   value = 0;
   while ((c = *begp) != (char) 0) {
      if ((c >= '0') && (c <= '9')) {
         cur_value = (uint64_t) (c - '0');
      } else if ((c >= 'a') && (c <= 'f')) {
         cur_value = (uint64_t) (c - 'a' + 10);
      } else if ((c >= 'A') && (c <= 'F')) {
         cur_value = (uint64_t) (c - 'A' + 10);
      } else {
         break;
      }
      begp++;
      value <<= 4;
      value |= cur_value;
   }
   return (uint64_t) value;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static uint64_t
octal_convert (char *begp) {
   uint64_t value;
   uint64_t cur_value;
   char     c;

   value = 0;
   while ((c = *begp) != (char) 0) {
      if ((c < '0') || (c > '7')) {
         break;
      }
      begp++;
      cur_value = (uint64_t) (c - '0');
      value <<= 3;
      value |= cur_value;
   }
   return value;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static uint64_t
binary_convert (char *begp) {
   uint64_t value;
   uint64_t cur_value;
   char     c;

   value = 0;
   while ((c = *begp) != (char) 0) {
      if ((c < '0') || (c > '1')) {
         break;
      }
      begp++;
      cur_value = (uint64_t) (c - '0');
      value <<= 1;
      value |= cur_value;
   }
   return value;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static int
get_wide_value (Tcl_Interp *tcl_interp, Tcl_Obj *tcl_obj, uint64_t *wide_value) {
   char     c;
   char     *p;
   char     *endp;
   int64_t  sign;
   uint64_t value;

   /* Before evaluating as a string expression, look at the string to see if it is of the form "0?..." */
   p = (char *) Tcl_GetString(tcl_obj);
   c = *p;
   if ((c != '+') && (c != '-') && ((c < '0') || (c > '9'))) {
      int eval_result;

      eval_result = Tcl_ExprString(tcl_interp, Tcl_GetString(tcl_obj));
      if (eval_result != TCL_OK) {
         *wide_value = -1;
         return eval_result;
      }
      p = (char *) Tcl_GetStringResult(tcl_interp);
   }
   endp = null_str;
   if (*p == '-') {
      sign = -1;
      p++;
   } else if (*p == '+') {
      sign = 1;
      p++;
   } else {
      sign = 1;
   }
   if (*p == '0') {
      switch (*(p + 1)) {
         case 'x':
         case 'X':
            p += 2;
            *wide_value = (uint64_t) 0;
            while (*p == '0') {
               p++;
            }
            if (*p == 0) {
               return TCL_OK;
            }
            value = hex_convert(p); /*strtoull(p, &endp, 16);*/
            break;
         case 'o':
         case 'O':
            p += 2;
            *wide_value = (uint64_t) 0;
            while (*p == '0') {
               p++;
            }
            if (*p == 0) {
               return TCL_OK;
            }
            value = octal_convert(p); /*strtoull(p, &endp, 8);*/
            break;
         case 'b':
         case 'B':
            p += 2;
            *wide_value = (uint64_t) 0;
            while (*p == '0') {
               p++;
            }
            if (*p == 0) {
               return TCL_OK;
            }
            value = binary_convert(p); /*strtoull(p, &endp, 1);*/
            break;
         default:
            *wide_value = (uint64_t) 0;
            while (*p == '0') {
               p++;
            }
            if (*p == 0) {
               return TCL_OK;
            }
            value = strtoull(p, &endp, 10);
            if (value == ((uint64_t) 0xffffffffffffffff)) {
               value = decimal_convert(p);
            }
            break;
      }
   } else {
      value = strtoull(p, &endp, 10);
      if (value == ((uint64_t) 0xffffffffffffffff)) {
         value = decimal_convert(p);
      }
   }
   *wide_value = sign * ((uint64_t) value);
   if (*endp == 0) {
      return TCL_OK;
   }
   Tcl_SetObjResult(tcl_interp, Tcl_ObjPrintf("expected integer but got \"%s\"", Tcl_GetString(tcl_obj)));
   return TCL_ERROR;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static char *
append_str (char *prefix_ptr, char *append_str, char *postfix_ptr) {
   char *new_str;

   new_str = null_str;
   if (prefix_ptr == (char *) NULL) {
      if (append_str == (char *) NULL) {
         append_str = null_str;
      }
      if (postfix_ptr == (char *) NULL) {
         postfix_ptr = null_str;
      }
      if ((new_str = Tcl_Alloc(sizeof(char) * (strlen(append_str) + strlen(postfix_ptr) + 1))) != (char *) NULL) {
         strcpy(new_str, append_str);
         strcat(new_str, postfix_ptr);
      }
   } else {
      if (append_str == (char *) NULL) {
         append_str = null_str;
      }
      if (postfix_ptr == (char *) NULL) {
         postfix_ptr = null_str;
      }
      if ((new_str = Tcl_Alloc(sizeof(char) * (strlen(prefix_ptr) + strlen(append_str) + strlen(postfix_ptr) + 1))) != (char *) NULL) {
         strcpy(new_str, prefix_ptr);
         strcat(new_str, append_str);
         strcat(new_str, postfix_ptr);
      }
   }
   return new_str;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static int
add_str_arg (tcl_thread_s *tcl_thread, int index, char *arg) {
   char          *str_arg;
   int           ii;
   int           true_len;
   int           str_len;
   tcl_str_obj_s *tcl_str_obj;

   str_arg  = null_str;
   true_len = strnlen(arg, 8191);
   str_len  = true_len;
   if (str_len < (EVL_MIN_ARG_LENGTH - 1)) {
      str_len = (EVL_MIN_ARG_LENGTH - 1);
   } else if (str_len > 8191) {
      str_len  = 8191;
      true_len = 8191;
   }
   while (tcl_thread->str_arg_count <= index) {
      if (tcl_thread->str_arg_count < index) {
         if ((str_arg = (char *) Tcl_Alloc(sizeof(char) * EVL_MIN_ARG_LENGTH)) == (char *) NULL) {
            sprintf(tcl_thread->last_error, "could not Tcl_Alloc %d bytes for str_arg (in add_str_arg())", EVL_MIN_ARG_LENGTH);
            return process_error(tcl_thread, EVL_TCL_ALLOC_ERROR, tcl_thread->last_error, 0);
         }
      } else {
         if ((str_arg = (char *) Tcl_Alloc(sizeof(char) * (str_len + 1))) == (char *) NULL) {
            sprintf(tcl_thread->last_error, "could not Tcl_Alloc %d bytes for str_arg (in add_str_arg())", str_len + 1);
            return process_error(tcl_thread, EVL_TCL_ALLOC_ERROR, tcl_thread->last_error, 0);
         }
      }
      if ((tcl_str_obj = (tcl_str_obj_s *) Tcl_Alloc(sizeof(tcl_str_obj_s))) == (tcl_str_obj_s *) NULL) {
         sprintf(tcl_thread->last_error, "could not Tcl_Alloc %d bytes for tcl_str_obj (in add_str_arg())", (int) sizeof(tcl_str_obj_s));
         return process_error(tcl_thread, EVL_TCL_ALLOC_ERROR, tcl_thread->last_error, 0);
      }
      if (tcl_thread->first_str_arg == (tcl_str_obj_s *) NULL) {
         tcl_thread->first_str_arg = tcl_str_obj;
      } else {
         tcl_thread->last_str_arg->next_ptr = tcl_str_obj;
      }
      *str_arg = (char) 0;
      tcl_str_obj->str_ptr = str_arg;
      if (tcl_thread->str_arg_count < index) {
         tcl_str_obj->str_len = EVL_MIN_ARG_LENGTH;
      } else {
         tcl_str_obj->str_len = str_len;
      }
      tcl_str_obj->next_ptr    = (tcl_str_obj_s *) NULL;
      tcl_thread->last_str_arg = tcl_str_obj;
      tcl_thread->str_arg_count++;
   }

   tcl_str_obj = tcl_thread->first_str_arg;
   for (ii = 1; ii <= index; ii++) {
      tcl_str_obj = tcl_str_obj->next_ptr;
   }
   if (tcl_str_obj->str_len >= str_len) {
      str_arg = tcl_str_obj->str_ptr;
   } else {
      Tcl_Free((char *) tcl_str_obj->str_ptr);
      if ((str_arg = (char *) Tcl_Alloc(sizeof(char) * (str_len + 1))) == (char *) NULL) {
         sprintf(tcl_thread->last_error, "could not Tcl_Alloc %d bytes for str_arg (in add_str_arg())", str_len + 1);
         return process_error(tcl_thread, EVL_TCL_ALLOC_ERROR, tcl_thread->last_error, 0);
      }
      tcl_str_obj->str_ptr = str_arg;
      tcl_str_obj->str_len = str_len;
   }
   strncpy(str_arg, arg, str_len);
   str_arg[true_len] = (char) 0;
   if (tcl_thread->arg_count < (index + 1)) {
      tcl_thread->arg_count = index + 1;
   }
   return 0;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static int
add_int_arg (tcl_thread_s *tcl_thread, int index, uint64_t wide_value, int valid_in) {
   tcl_int_obj_s *tcl_int_obj;

   if (tcl_thread->int_arg_count <= index) {
      if ((tcl_int_obj = (tcl_int_obj_s *) Tcl_Alloc(sizeof(tcl_int_obj_s))) == (tcl_int_obj_s *) NULL) {
         sprintf(tcl_thread->last_error, "could not Tcl_Alloc %d bytes for int_arg (in add_int_arg())", (int) sizeof(tcl_int_obj_s));
         return process_error(tcl_thread, EVL_TCL_ALLOC_ERROR, tcl_thread->last_error, 0);
      }
      if (tcl_thread->last_int_arg != (tcl_int_obj_s *) NULL) {
         tcl_thread->last_int_arg->next_ptr = tcl_int_obj;
      }
      tcl_int_obj->next_ptr    = (tcl_int_obj_s *) NULL;;
      tcl_thread->last_int_arg = tcl_int_obj;
      tcl_thread->int_arg_count++;
      if (tcl_thread->first_int_arg == (tcl_int_obj_s *) NULL) {
         tcl_thread->first_int_arg = tcl_int_obj;
      }
   } else {
      int ii;

      tcl_int_obj = tcl_thread->first_int_arg;
      for (ii = 1; ii <= index; ii++) {
         tcl_int_obj = tcl_int_obj->next_ptr;
      }
   }
   tcl_int_obj->valid = valid_in;
   tcl_int_obj->value = wide_value;
   return 0;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static char *
get_str_arg (tcl_thread_s *tcl_thread, int arg_index_in) {
   int arg_index;

   arg_index = arg_index_in & 0xefffffff;
   if (arg_index < tcl_thread->arg_count) {
      int           count;
      tcl_str_obj_s *str_obj;

      count = arg_index;
      str_obj = tcl_thread->first_str_arg;
      while ((count-- > 0) && (str_obj != (tcl_str_obj_s *) NULL)) {
         str_obj = str_obj->next_ptr;
      }
      if (str_obj == (tcl_str_obj_s *) NULL) {
         return null_str;
      }
      return str_obj->str_ptr;
   }
   return null_str;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static int
add_str_res (tcl_thread_s *tcl_thread, Tcl_Interp *tcl_interp, int index, char *str_in) {
   char          *str_res;
   int           ii;
   int           str_len;
   tcl_str_obj_s *tcl_str_obj;

   str_len = strnlen(str_in, 8191);
   if (str_len < (EVL_MIN_ARG_LENGTH - 1)) {
      str_len = (EVL_MIN_ARG_LENGTH - 1);
   }
   while (tcl_thread->str_res_count <= index) {
      if (tcl_thread->str_res_count < index) {
         if ((str_res = (char *) Tcl_Alloc(sizeof(char) * EVL_MIN_ARG_LENGTH)) == (char *) NULL) {
            sprintf(tcl_thread->last_error, "could not Tcl_Alloc %d bytes for str_res (in add_str_res())", EVL_MIN_ARG_LENGTH);
            return process_error(tcl_thread, EVL_TCL_ALLOC_ERROR, tcl_thread->last_error, 0);
         }
      } else {
         if ((str_res = (char *) Tcl_Alloc(sizeof(char) * (str_len + 1))) == (char *) NULL) {
            sprintf(tcl_thread->last_error, "could not Tcl_Alloc %d bytes for str_res (in add_str_res())", str_len + 1);
            return process_error(tcl_thread, EVL_TCL_ALLOC_ERROR, tcl_thread->last_error, 0);
         }
      }
      if ((tcl_str_obj = (tcl_str_obj_s *) Tcl_Alloc(sizeof(tcl_str_obj_s))) == (tcl_str_obj_s *) NULL) {
         sprintf(tcl_thread->last_error, "could not Tcl_Alloc %d bytes for tcl_str_obj (in add_str_res())", (int) sizeof(tcl_str_obj_s));
         return process_error(tcl_thread, EVL_TCL_ALLOC_ERROR, tcl_thread->last_error, 0);
      }
      if (tcl_thread->first_str_res == (tcl_str_obj_s *) NULL) {
         tcl_thread->first_str_res = tcl_str_obj;
      } else {
         tcl_thread->last_str_res->next_ptr = tcl_str_obj;
      }
      *str_res = (char) 0;
      tcl_str_obj->str_ptr = str_res;
      if (tcl_thread->str_res_count < index) {
         tcl_str_obj->str_len = EVL_MIN_ARG_LENGTH;
      } else {
         tcl_str_obj->str_len = str_len;
      }
      tcl_str_obj->next_ptr    = (tcl_str_obj_s *) NULL;
      tcl_thread->last_str_res = tcl_str_obj;
      tcl_thread->str_res_count++;
   }

   tcl_str_obj = tcl_thread->first_str_res;
   for (ii = 1; ii <= index; ii++) {
      tcl_str_obj = tcl_str_obj->next_ptr;
   }
   if (tcl_str_obj->str_len >= str_len) {
      str_res = tcl_str_obj->str_ptr;
   } else {
      Tcl_Free((char *) tcl_str_obj->str_ptr);
      if ((str_res = (char *) Tcl_Alloc(sizeof(char) * (str_len + 1))) == (char *) NULL) {
         sprintf(tcl_thread->last_error, "could not Tcl_Alloc %d bytes for str_res (in add_str_res())", str_len + 1);
         return process_error(tcl_thread, EVL_TCL_ALLOC_ERROR, tcl_thread->last_error, 0);
      }
      tcl_str_obj->str_ptr = str_res;
      tcl_str_obj->str_len = str_len;
   }
   strncpy(str_res, str_in, str_len);
   if (tcl_thread->res_count < (index + 1)) {
      tcl_thread->res_count = index + 1;
   }
   return 0;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static int
add_int_res (tcl_thread_s *tcl_thread, Tcl_Interp *tcl_interp, int index, uint64_t int_in) {
   int           ii;
   tcl_int_obj_s *tcl_int_obj;

   while (tcl_thread->int_res_count <= index) {
      if ((tcl_int_obj = (tcl_int_obj_s *) Tcl_Alloc(sizeof(tcl_int_obj_s))) == (tcl_int_obj_s *) NULL) {
         sprintf(tcl_thread->last_error, "could not Tcl_Alloc %d bytes for int_res (in add_int_res())", (int) sizeof(tcl_int_obj_s));
         return process_error(tcl_thread, EVL_TCL_ALLOC_ERROR, tcl_thread->last_error, 0);
      }
      if (tcl_thread->last_int_res != (tcl_int_obj_s *) NULL) {
         tcl_thread->last_int_res->next_ptr = tcl_int_obj;
      }
      tcl_int_obj->value       = -1;
      tcl_int_obj->next_ptr    = (tcl_int_obj_s *) NULL;
      tcl_thread->last_int_res = tcl_int_obj;
      if (tcl_thread->first_int_res == (tcl_int_obj_s *) NULL) {
         tcl_thread->first_int_res = tcl_int_obj;
      }
      tcl_thread->int_res_count++;
   }

   tcl_int_obj = tcl_thread->first_int_res;
   for (ii = 1; ii <= index; ii++) {
      tcl_int_obj = tcl_int_obj->next_ptr;
   }
   tcl_int_obj->value = int_in;
   if (tcl_thread->res_count < (index + 1)) {
      tcl_thread->res_count = index + 1;
   }
   return 0;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static char *
get_str_res (tcl_thread_s *tcl_thread, int index) {
   int           ii;
   tcl_str_obj_s *tcl_str_obj;

   if (tcl_thread->str_res_count <= index) {
      return null_str;
   }
   tcl_str_obj = tcl_thread->first_str_res;
   for (ii = 1; ii <= index; ii++) {
      tcl_str_obj = tcl_str_obj->next_ptr;
   }
   return tcl_str_obj->str_ptr;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static Tcl_WideInt
get_int_res (tcl_thread_s *tcl_thread, int index) {
   int           ii;
   tcl_int_obj_s *tcl_int_obj;

   if (tcl_thread->int_res_count <= index) {
      return (Tcl_WideInt) -1;
   }
   tcl_int_obj = tcl_thread->first_int_res;
   for (ii = 1; ii <= index; ii++) {
      tcl_int_obj = tcl_int_obj->next_ptr;
   }
   return (Tcl_WideInt) tcl_int_obj->value;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static tcl_cmd_enc_s *
find_tcl_cmd(tcl_thread_s *tcl_thread, char *cmd_name) {
   tcl_cmd_enc_s *tcl_cmd_enc;

   tcl_cmd_enc = tcl_thread->tcl_cmds;
   while (tcl_cmd_enc != (tcl_cmd_enc_s *) NULL) {
      if (strcmp(cmd_name, tcl_cmd_enc->cmd_name) == 0) {
         return tcl_cmd_enc;
      }
      tcl_cmd_enc = tcl_cmd_enc->next_cmd;
   }
   return tcl_cmd_enc;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static void
transfer_to_verilog (int thread_index_in, int wait_for_verilog) {
   int          thread_index;
   tcl_thread_s *tcl_thread;

   thread_index = thread_index_in & 0xefffffff;
   if ((thread_index <= last_thread) && (thread_ptrs[thread_index] != (tcl_thread_s *) NULL)) {
      tcl_thread            = thread_ptrs[thread_index];
      tcl_thread->res_count = 0;
      if ((tcl_thread->lock & 0xf0) == 0x00) {
         (void) pthread_mutex_lock(&(tcl_thread->mutex));
         tcl_thread->lock |= 0x10;
      }
      (void) pthread_cond_broadcast(&(tcl_thread->xfr_to_verilog));
      tcl_thread->lock &= 0x0f;
      if (wait_for_verilog < 0) {
         (void) pthread_mutex_unlock(&(tcl_thread->mutex));
      } else {
         (void) pthread_cond_wait(&(tcl_thread->xfr_to_dpi), &(tcl_thread->mutex));
         (void) pthread_mutex_unlock(&(tcl_thread->mutex));
      }
   }
   return;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static int
transfer_to_dpi (int thread_index_in) {
   int             thread_index;
   int             thread_result;
   tcl_thread_s    *tcl_thread;
   struct timespec thread_time;

   thread_index = thread_index_in & 0xefffffff;
   if ((thread_index <= last_thread) && (thread_ptrs[thread_index] != (tcl_thread_s *) NULL)) {
      tcl_thread = thread_ptrs[thread_index];
      if ((tcl_thread->lock & 0x0f) == 0x00) {
         (void) pthread_mutex_lock(&(tcl_thread->mutex));
         tcl_thread->lock |= 0x01;
      }
      (void) pthread_cond_broadcast(&(tcl_thread->xfr_to_dpi));
      tcl_thread->lock &= 0xf0;

      clock_gettime(CLOCK_REALTIME, &thread_time);
      thread_time.tv_sec += 300;
      thread_result = pthread_cond_timedwait(&(tcl_thread->xfr_to_verilog), &(tcl_thread->mutex), &thread_time);
      if (thread_result == ETIMEDOUT) {
         return process_error(tcl_thread, EVL_TCL_ALLOC_ERROR, "timed out waiting for the TCL DPI to respond", 0);
      }
      (void) pthread_mutex_unlock(&(tcl_thread->mutex));
   }
   return 0;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static int
process_error (tcl_thread_s *tcl_thread, int error_code, char *error_msg, int transfer_control) {
   if (tcl_thread->in_error != 0) {
      if (transfer_control != 0) {
         transfer_to_verilog(tcl_thread->thread_num, 0);
      }
      return error_code;
   }
   tcl_thread->cmd_encoding = error_code;
   tcl_thread->in_error     = 1;
   if (error_msg != (char *) NULL) {
      tcl_thread->arg_count = 1;
      add_str_arg(tcl_thread, 0, error_msg);
   } else {
      char message[128];

      tcl_thread->arg_count = 1;
      sprintf(message, "result is %d", error_code);
      add_str_arg(tcl_thread, 0, message);
   }
   if (transfer_control != 0) {
      transfer_to_verilog(tcl_thread->thread_num, 0);
   }
   tcl_thread->in_error = 0;
   return error_code;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static void
parse_agent_name(char *agent_name, int *shire_out, int *neighborhood_out, int *core_out, int *agent_id_out) {
   int  shire;
   int  neighborhood;
   int  core;
   int  agent_id;
   char c;
   char *p;
   char *start;

   shire        = -1;
   neighborhood = -1;
   core         = -1;
   agent_id     = -1;
   p            = agent_name;
   start        = agent_name;
   while ((c = *p) != (char) 0) {
      if ((c >= 'a') && (c <= 'a')) {
         *p = c - ('a' - 'A');
      } else if (c == '-') {
         *p++ = (char) 0;
         agent_id = atoi(p);
         if ((agent_id == 0) && (*p != '0')) {
            agent_id = -1;
         }
         break;
      } else if (c == ':') {
         *p = (char) 0;
         if (*start == 'S') {
            shire = atoi(start + 1);
            if ((shire == 0) && (*(start + 1) != '0')) {
               shire = -1;
            }
         } else if (*start == 'N') {
            neighborhood = atoi(start + 1);
            if ((neighborhood == 0) && (*(start + 1) != '0')) {
               neighborhood = -1;
            }
         } else if (*start == 'M') {
            core = atoi(start + 1);
            if ((core == 0) && (*(start + 1) != '0')) {
               core = -1;
            }
         }
         start = p + 1;
      }
      ++p;
   }
   if (start != agent_name) {
      p = agent_name;
      while ((*p++ = *start++) != (char) 0);
   }
   *agent_id_out     = agent_id;
   *core_out         = core;
   *neighborhood_out = neighborhood;
   *shire_out        = shire;
}


/***************************************************************************************************
 *
 * exec_get_env()
 *
 **************************************************************************************************/
static int
exec_get_env (ClientData client_data, Tcl_Interp *tcl_interp, int objc, Tcl_Obj *objv[]) {
   int thread_index;

   thread_index = ((int) ((uint64_t) client_data)) & 0xefffffff;
   if ((thread_index <= last_thread) && (thread_ptrs[thread_index] != (tcl_thread_s *) NULL)) {
      char *env_ptr;

      if (objc != 2) {
         char message[128];

         sprintf(message, "\" is not 1 (it is %d)", objc - 1);
         return process_error(thread_ptrs[thread_index], EVL_TCL_ARG_COUNT_ERROR, append_str("the number of arguments for \"", Tcl_GetString(objv[0]), message), 1);
      }
      env_ptr = getenv(Tcl_GetString(objv[1]));
      if (env_ptr == (char *) NULL) {
         Tcl_SetObjResult(tcl_interp, Tcl_NewStringObj("", -1));
      } else {
         Tcl_SetObjResult(tcl_interp, Tcl_NewStringObj(env_ptr, -1));
      }
   }
   return 0;
}


/***************************************************************************************************
 *
 * exec_verilog_cmd()
 *
 **************************************************************************************************/
static int
exec_verilog_cmd (ClientData client_data, Tcl_Interp *tcl_interp, int objc, Tcl_Obj *objv[]) {
   int           thread_index;
   tcl_thread_s  *tcl_thread;
   tcl_cmd_enc_s *tcl_cmd_enc;
   int           index;
   uint64_t      wide_value;
   int           result;

   thread_index = ((int) ((uint64_t) client_data)) & 0xefffffff;
   if ((thread_index <= last_thread) && (thread_ptrs[thread_index] != (tcl_thread_s *) NULL)) {
      tcl_thread = thread_ptrs[thread_index];
      if ((tcl_cmd_enc = find_tcl_cmd(tcl_thread, Tcl_GetString(objv[0]))) == (tcl_cmd_enc_s *) NULL) {
         return process_error(tcl_thread, EVL_TCL_EVAL_ERROR, append_str("could not find encoding for \"", Tcl_GetString(objv[0]), "\""), 1);
      }
      if ((objc < (tcl_cmd_enc->min_arg_count + 1)) || (objc > (tcl_cmd_enc->max_arg_count + 1))) {
         char message[128];

         if (tcl_cmd_enc->min_arg_count == tcl_cmd_enc->max_arg_count) {
            sprintf(message, "\" is not %d (it is %d)", tcl_cmd_enc->min_arg_count, objc - 1);
         } else {
            sprintf(message, "\" is not between %d and %d (it is %d)", tcl_cmd_enc->min_arg_count, tcl_cmd_enc->max_arg_count, objc - 1);
         }
         return process_error(tcl_thread, EVL_TCL_ARG_COUNT_ERROR, append_str("the number of arguments for \"", Tcl_GetString(objv[0]), message), 1);
      }
      tcl_thread->arg_count    = objc - 1;
      tcl_thread->cmd_encoding = tcl_cmd_enc->cmd_encoding;
      for (index = 1; index < objc; index++) {
         int valid;

         if ((result = add_str_arg(tcl_thread, index - 1, Tcl_GetString(objv[index]))) != 0) {
            return process_error(tcl_thread, result, get_str_arg(tcl_thread, 0), 1);
         }
         if (get_wide_value(tcl_interp, objv[index], &wide_value) == TCL_OK) {
            valid = 1;
         } else {
            valid      = 0;
            wide_value = ~((uint64_t) 0);
         }
         if ((result = add_int_arg(tcl_thread, index - 1, wide_value, valid)) != 0) {
            return process_error(tcl_thread, result, get_str_arg(tcl_thread, 0), 1);
         }
      }
      if (((tcl_cmd_enc->parse_type == EVL_TCL_PARSE_AGENT) || (tcl_cmd_enc->parse_type == EVL_TCL_PARSE_AGENT_DEFAULT)) && (objc == 2)) {
         int shire;
         int neighborhood;
         int core;
         int agent_id;

         parse_agent_name(get_str_arg(tcl_thread, 0), &shire, &neighborhood, &core, &agent_id);
         if (tcl_cmd_enc->parse_type == EVL_TCL_PARSE_AGENT_DEFAULT) {
            if (shire < 0) {
               shire = 0;
            }
            if (neighborhood < 0) {
               neighborhood = 0;
            }
            if (core < 0) {
               core = 0;
            }
            if (agent_id < 0) {
               agent_id = 0;
            }
         }
         tcl_thread->arg_count = 1;
         for (tcl_thread->arg_count = 1; tcl_thread->arg_count < 5; tcl_thread->arg_count++) {
            if ((result = add_str_arg(tcl_thread, tcl_thread->arg_count, "")) != 0) {
               return process_error(tcl_thread, result, get_str_arg(tcl_thread, 0), 1);
            }
         }
         if ((result = add_int_arg(tcl_thread, 1, (uint64_t) agent_id, 1)) != 0) {
            return process_error(tcl_thread, result, get_str_arg(tcl_thread, 0), 1);
         }
         if ((result = add_int_arg(tcl_thread, 2, (uint64_t) shire, 1)) != 0) {
            return process_error(tcl_thread, result, get_str_arg(tcl_thread, 0), 1);
         }
         if ((result = add_int_arg(tcl_thread, 3, (uint64_t) neighborhood, 1)) != 0) {
            return process_error(tcl_thread, result, get_str_arg(tcl_thread, 0), 1);
         }
         if ((result = add_int_arg(tcl_thread, 4, (uint64_t) core, 1)) != 0) {
            return process_error(tcl_thread, result, get_str_arg(tcl_thread, 0), 1);
         }
      }
      transfer_to_verilog(tcl_thread->thread_num, 0);
      if (tcl_cmd_enc->return_type == EVL_TCL_RESULT_INT) {
         Tcl_SetObjResult(tcl_interp, Tcl_NewWideIntObj(get_int_res(tcl_thread, 0)));
      } else if (tcl_cmd_enc->return_type == EVL_TCL_RESULT_INT_LIST) {
         int     ii;
         Tcl_Obj *list_obj;
         int     list_length;

         Tcl_SetObjResult(tcl_interp, Tcl_NewObj());
         list_obj = Tcl_GetObjResult(tcl_interp);
         Tcl_ListObjLength(tcl_interp, list_obj, &list_length);
         Tcl_ListObjReplace(tcl_interp, list_obj, 0, list_length, 0, NULL);
         for (ii = 0; ii < tcl_thread->res_count; ii++) {
            result = Tcl_ListObjAppendElement(tcl_interp, list_obj, Tcl_NewWideIntObj(get_int_res(tcl_thread, ii)));
         }
         Tcl_SetObjResult(tcl_interp, list_obj);
      } else if (tcl_cmd_enc->return_type == EVL_TCL_RESULT_STR) {
         Tcl_SetObjResult(tcl_interp, Tcl_NewStringObj(get_str_res(tcl_thread, 0), -1));
      } else if (tcl_cmd_enc->return_type == EVL_TCL_RESULT_STR_LIST) {
         int     ii;
         Tcl_Obj *list_obj;
         int     list_length;

         Tcl_SetObjResult(tcl_interp, Tcl_NewObj());
         list_obj = Tcl_GetObjResult(tcl_interp);
         Tcl_ListObjLength(tcl_interp, list_obj, &list_length);
         Tcl_ListObjReplace(tcl_interp, list_obj, 0, list_length, 0, NULL);
         for (ii = 0; ii < tcl_thread->res_count; ii++) {
            Tcl_ListObjAppendElement(tcl_interp, list_obj, Tcl_NewStringObj(get_str_res(tcl_thread, ii), -1));
         }
         Tcl_SetObjResult(tcl_interp, list_obj);
      }
      return 0;
   }
   return 0;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static void
run_tcl_thread (ClientData client_data) {
   int result;
   int thread_index;

   thread_index = ((int) ((uint64_t) client_data)) & 0xefffffff;
   if ((thread_index <= last_thread) && (thread_ptrs[thread_index] != (tcl_thread_s *) NULL)) {
      tcl_thread_s *tcl_thread;
      Tcl_Interp   *tcl_interp;

      tcl_thread = thread_ptrs[thread_index];
      tcl_interp = tcl_thread->tcl_interp;
      tcl_thread->thread_active = 1;
      transfer_to_verilog(tcl_thread->thread_num, 0);

      while ((tcl_thread->eval_str != (char *) NULL) && (*(tcl_thread->eval_str) != (char) 0) && (*(tcl_thread->eval_str) != ' ')) {
         (void) Tcl_EvalEx(tcl_interp, tcl_thread->eval_str, -1, TCL_EVAL_GLOBAL);
         tcl_thread->eval_str = null_str;
         transfer_to_verilog(tcl_thread->thread_num, 0);
      }

      result = Tcl_EvalEx(tcl_interp, tcl_thread->tcl_script, -1, TCL_EVAL_GLOBAL);
      if (result != TCL_OK) {
         process_error(tcl_thread, EVL_TCL_EVAL_ERROR, Tcl_GetString(Tcl_GetObjResult(tcl_interp)), 0);
      }

      transfer_to_verilog(tcl_thread->thread_num, 0);
   }
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static tcl_thread_s *
create_tcl_thread (int id, char *file_name, FILE *fd) {
   tcl_thread_s *tcl_thread;
   Tcl_Interp   *tcl_interp;

   if ((tcl_thread = (tcl_thread_s *) malloc(sizeof(tcl_thread_s))) == (tcl_thread_s *) NULL) {
      sprintf(last_global_error, "create_tcl_thread() in evl_tcl.c failed ... the malloc failed");
      fclose(fd);
      return (tcl_thread_s *) NULL;
   }
   tcl_interp = Tcl_CreateInterp();
   if (tcl_interp == (Tcl_Interp *) NULL) {
      sprintf(last_global_error, "create_tcl_thread() in evl_tcl.c failed ... Tcl_CreateInterp() failed");
      fclose(fd);
      return (tcl_thread_s *) NULL;
   }
   Tcl_Init(tcl_interp);

   last_thread++;
   thread_ptrs[last_thread]      = tcl_thread;
   tcl_thread->thread_num        = last_thread;
   tcl_thread->id                = id;
   tcl_thread->tcl_thread_id     = (Tcl_ThreadId *) NULL;
   tcl_thread->tcl_interp        = tcl_interp;
   tcl_thread->file_name         = file_name;
   tcl_thread->fd                = fd;
   tcl_thread->replay_fd         = (FILE *) NULL;
   tcl_thread->tcl_script        = null_str;
   tcl_thread->eval_str          = null_str;
   tcl_thread->next_cmd_encoding = 1;
   tcl_thread->thread_active     = 0;
   tcl_thread->cmd_encoding      = 0;
   tcl_thread->str_arg_count     = 0;
   tcl_thread->int_arg_count     = 0;
   tcl_thread->str_res_count     = 0;
   tcl_thread->int_res_count     = 0;
   tcl_thread->arg_count         = 0;
   tcl_thread->res_count         = 0;
   tcl_thread->lock              = 0;
   tcl_thread->in_error          = 0;
   tcl_thread->tcl_cmds          = (tcl_cmd_enc_s *) NULL;
   tcl_thread->first_str_arg     = (tcl_str_obj_s *) NULL;
   tcl_thread->first_int_arg     = (tcl_int_obj_s *) NULL;
   tcl_thread->last_str_arg      = (tcl_str_obj_s *) NULL;
   tcl_thread->last_int_arg      = (tcl_int_obj_s *) NULL;
   tcl_thread->first_str_res     = (tcl_str_obj_s *) NULL;
   tcl_thread->first_int_res     = (tcl_int_obj_s *) NULL;
   tcl_thread->last_str_res      = (tcl_str_obj_s *) NULL;
   tcl_thread->last_int_res      = (tcl_int_obj_s *) NULL;
   tcl_thread->last_error[0]     = (char) 0;

   Tcl_CreateObjCommand(tcl_thread->tcl_interp, "get_env", (Tcl_ObjCmdProc *) exec_get_env, (ClientData) ((long) tcl_thread->thread_num), (Tcl_CmdDeleteProc *) NULL);

   /*
    * Create a mutex and conditions for transferring control.
    */
   (void) pthread_mutex_init(&(tcl_thread->mutex), NULL);
   (void) pthread_cond_init(&(tcl_thread->xfr_to_verilog), NULL);
   (void) pthread_cond_init(&(tcl_thread->xfr_to_dpi), NULL);

   return tcl_thread;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static int
start_tcl_thread (tcl_thread_s *tcl_thread) {
   /*
    * Create a thread and wake it up.
    */
   (void) pthread_mutex_lock(&(tcl_thread->mutex));
   Tcl_CreateThread((Tcl_ThreadId *) &(tcl_thread->tcl_thread_id), (Tcl_ThreadCreateProc *) &run_tcl_thread, (ClientData) ((long) last_thread), TCL_THREAD_STACK_DEFAULT, TCL_THREAD_NOFLAGS);
   (void) pthread_cond_wait(&(tcl_thread->xfr_to_verilog), &(tcl_thread->mutex));
   (void) pthread_mutex_unlock(&(tcl_thread->mutex));

   /*
    * If the thread has not been successfully started, return an error indication.
    */
   if (tcl_thread->thread_active == 0) {
      return process_error(tcl_thread, EVL_TCL_CANNOT_START_THREAD, "TCL thread could not be started successfully", 0);
   }

   return 0;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
static int
load_tcl_file (tcl_thread_s *tcl_thread) {
   int           char_count;
   int           cur_char_count;
   FILE          *fd;
   script_data_s *script_data_start;
   script_data_s *script_data;
   char          input_line[EVL_TCL_LINE_LIMIT];

   char_count     = 0;
   cur_char_count = 0;
   fd             = tcl_thread->fd;

   input_line[EVL_TCL_LINE_LIMIT - 2] = '\n';
   input_line[EVL_TCL_LINE_LIMIT - 1] = 0;
   if ((script_data_start = (script_data_s *) malloc(sizeof(script_data_s))) == (script_data_s *) NULL) {
      return process_error(tcl_thread, EVL_TCL_MALLOC_ERROR, "could not allocate memory for script_data_start (in load_tcl_file())", 1);
   }

   script_data = script_data_start;
   script_data->line[0]  = (char) 0;
   script_data->next_ptr = (script_data_s *) NULL;

   /*
    * Read the TCL file, storing the entire script in a string object.  By reading in the script
    * this way, it is possible for script writers to include comments within "{ }" blocks (we cheat
    * and parse out comments).
    */
   while (fgets(input_line, EVL_TCL_LINE_LIMIT - 1, fd) != (char *) NULL) {
      char c;
      char *p;
      int  ii;
      int  last_char;
      int  last_escape;
      int  escaped;
      int  in_quote;
      int  in_bracket;
      int  found_eol;
      int  first_comment;

      p             = input_line;
      ii            = 0;
      last_char     = -1;
      last_escape   = -1;
      escaped       = 0;
      in_quote      = 0;
      in_bracket    = 0;
      found_eol     = 0;
      first_comment = -1;
      input_line[EVL_TCL_LINE_LIMIT - 1] = (char) 0;
      while ((c = *p++) != (char) 0) {
         if (c == '\n') {
            found_eol = 1;
            if ((last_escape >= 0) && (last_escape != (ii - 1))) {
               last_escape = -1;
            }
         }
         else if (c == '\\') {
            escaped   = (escaped + 1) & 0x1;
            last_char = ii;
            if (escaped > 0) {
               last_escape = ii;
            }
         }
         else if (first_comment < 0) {
            if (isspace(c) == 0) {
               last_char = ii;
            }
            if ((escaped & 1) == 0) {
               if (in_quote == 1) {
                  if (c == '"') {
                     in_quote      = 0;
                     first_comment = -1;
                  }
               } else if (in_bracket == 1) {
                  if (c == '}') {
                     in_bracket    = 0;
                     first_comment = -1;
                  }
               } else if (c == '"') {
                  in_quote = 1;
               } else if (c == '{') {
                  in_bracket = 1;
               }
               if ((c == '#') && (first_comment < 0) && (in_quote == 0)) {
                  first_comment = ii;
               }
            }
            escaped = 0;
         } else {
            escaped = 0;
         }
         ii++;
      }
      if (found_eol == 0) {
         first_comment = -1;
      }
      if (first_comment == 0) {
         last_char = -1;
      } else if (first_comment > 0) {
         p         = &input_line[first_comment - 1];
         last_char = first_comment - 1;
         while ((p != input_line) && (isspace(*p) != 0)) {
            last_char--;
            p--;
         }
         if ((p == input_line) && (isspace(*p) != 0)) {
            last_char = -1;
         }
      }
      if (found_eol == 1) {
         if ((last_escape >= 0) && (last_escape > last_char)) {
            input_line[last_char + 1] = '\\';
            input_line[last_char + 2] = '\n';
            input_line[last_char + 3] = (char) 0;
         } else {
            input_line[last_char + 1] = '\n';
            input_line[last_char + 2] = (char) 0;
         }
      } else {
         input_line[last_char + 1] = (char) 0;
      }
      last_char = strlen(input_line);
      if (last_char > 0) {
         if ((last_char + cur_char_count) >= EVL_TCL_LINE_LIMIT) {
            if ((script_data->next_ptr = (script_data_s *) malloc(sizeof(script_data_s))) == (script_data_s *) NULL) {
               return process_error(tcl_thread, EVL_TCL_MALLOC_ERROR, "could not allocate memory for script_data (in load_tcl_file())", 1);
            }
            script_data = script_data->next_ptr;
            script_data->line[0]  = (char) 0;
            script_data->next_ptr = (script_data_s *) NULL;
            cur_char_count = 0;
         }
         strcat(script_data->line, input_line);
      }
      char_count     += last_char;
      cur_char_count += last_char;
   }

   if ((tcl_thread->tcl_script = Tcl_Alloc(sizeof(char) * (char_count + 1))) == (char *) NULL) {
      return process_error(tcl_thread, EVL_TCL_ALLOC_ERROR, "could not allocate TCL memory for the script (in load_tcl_file())", 1);
   }

   *tcl_thread->tcl_script = (char) 0;
   script_data = script_data_start;
   while (script_data != (script_data_s *) NULL) {
      script_data_s *prev_data;

      prev_data = script_data;
      strcat(tcl_thread->tcl_script, script_data->line);
      script_data = script_data->next_ptr;
      free(prev_data);
   }

   return 0;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
void
extract_plus_args_from_file (char *ptr, int count_only) {
   char **argv;
   char *p;

   if (ptr == (char *) NULL) {
      return;
   }
   argv = (char **) ptr;
   while ((p = *argv) != (char *) NULL) {
      p = *argv++;
      if (*p == (char) '+') {
         if (*(p+1) != (char) 0) {
            if (count_only != 0) {
               plus_args_count++;
            } else {
               *next_plus_arg_ptr++ = p + 1;
            }
         }
      } else if ((*p == (char) '-') && ((*(p+1) == (char) 'f') || (*(p+1) == (char) 'F')) && (*(p+2) == (char) 0)) {
         extract_plus_args_from_file(*argv++, count_only);
      }
   }
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
void
extract_plus_args () {
   s_vpi_vlog_info vlog_info;
   int             argc;
   char            **argv;
   char            *p;

   if (plus_args_extracted++ > 0) {
      plus_args_extracted = 1;
      return;
   }
   if (vpi_get_vlog_info(&vlog_info) == 0) {
      *plus_arg_ptrs    = null_str;
      next_plus_arg_ptr = plus_arg_ptrs;
      return;
   }

   argc = (int)     vlog_info.argc;
   argv = (char **) vlog_info.argv;
   argv++;
   while (argc > 1) {
      p = *argv++;
      argc--;
      if (*p == (char) '+') {
         if (*(p+1) != (char) 0) {
            plus_args_count++;
         }
      } else if ((*p == (char) '-') && ((*(p+1) == (char) 'f') || (*(p+1) == (char) 'F')) && (*(p+2) == (char) 0)) {
         p = *argv++;
         argc--;
         extract_plus_args_from_file(p, 1);
      }
   }
   if ((plus_arg_ptrs = (char **) malloc(sizeof(char *) * (plus_args_count + 2))) == (char **) NULL) {
      *plus_arg_ptrs    = null_str;
      next_plus_arg_ptr = plus_arg_ptrs;
      return;
   }
   next_plus_arg_ptr = plus_arg_ptrs;
   argc = (int)     vlog_info.argc;
   argv = (char **) vlog_info.argv;
   argv++;
   while (argc > 1) {
      p = *argv++;
      argc--;
      if (*p == (char) '+') {
         if (*(p+1) != (char) 0) {
            *next_plus_arg_ptr++ = p + 1;
         }
      } else if ((*p == (char) '-') && ((*(p+1) == (char) 'f') || (*(p+1) == (char) 'F')) && (*(p+2) == (char) 0)) {
         p = *argv++;
         argc--;
         extract_plus_args_from_file(p, 0);
      }
   }
   *next_plus_arg_ptr = null_str;
   next_plus_arg_ptr  = plus_arg_ptrs;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
char *
add_file_name_to_file_paths(char *file_name) {
   tcl_file_path_s *file_path;

   file_path = file_paths;
   while (file_path != (tcl_file_path_s *) NULL) {
      if (strcmp(file_path->path_name, file_name) == 0) {
         return file_path->path_name;
      }
      file_path = file_path->next_ptr;
   }
   if ((file_path = (tcl_file_path_s *) malloc(sizeof(tcl_file_path_s))) == (tcl_file_path_s *) NULL) {
      return (char *) NULL;
   }
   if ((file_path->path_name = (char *) malloc(sizeof(char) * ((int) strlen(file_name) + 1))) == (char *) NULL) {
      return (char *) NULL;
   }
   if (last_file_path == (tcl_file_path_s *) NULL) {
      file_paths = file_path;
   } else {
      last_file_path->next_ptr = file_path;
   }
   strcpy(file_path->path_name, file_name);
   file_path->next_ptr = (tcl_file_path_s *) NULL;
   last_file_path      = file_path;
   return file_path->path_name;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
char *
evl_tcl_get_next_plus_arg () {
   char *plus_arg_ptr;

   extract_plus_args();
   plus_arg_ptr = *next_plus_arg_ptr++;
   if (*plus_arg_ptr == (char) 0) {
      next_plus_arg_ptr = plus_arg_ptrs;
   }
   return plus_arg_ptr;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
void
evl_tcl_add_include_path (char *path_name_in) {
   int             path_len;
   char            *full_path_name;
   char            *cur_path_name;
   char            *path_name;
   tcl_file_path_s *include_path;

   if (*path_name_in == (char) 0) {
      return;
   }
   if ((full_path_name = (char *) malloc(sizeof(char) * ((int) strlen(path_name_in) + 2))) == (char *) NULL) {
      return;
   }
   strcpy(full_path_name, path_name_in);
   cur_path_name = full_path_name;
   while (*cur_path_name != (char) 0) {
      int  ignore_path;
      char c;
      char *p;

      p = cur_path_name;
      ignore_path = 0;
      while ((c = *p++) != (char) 0) {
         if (c == ':') {
            *(p - 1) = (char) 0;
            if ((path_name = (char *) malloc(sizeof(char) * ((int) strlen(cur_path_name) + 2))) == (char *) NULL) {
               return;
            }
            strcpy(path_name, cur_path_name);
            cur_path_name = p;
            break;
         }
      }
      if (c == (char) 0) {
         if ((path_name = (char *) malloc(sizeof(char) * ((int) strlen(cur_path_name) + 2))) == (char *) NULL) {
            return;
         }
         strcpy(path_name, cur_path_name);
         cur_path_name = p - 1;
      }
      if (*path_name != (char) 0) {
         path_len = strlen(path_name);
         while ((path_len > 0) && (path_name[path_len - 1] == '/')) {
            path_name[path_len - 1] = (char) 0;
            path_len = strlen(path_name);
         }
         strcat(path_name, "/");
         include_path = include_paths;
         while (include_path != (tcl_file_path_s *) NULL) {
            if (strcmp(path_name, include_path->path_name) == 0) {
               ignore_path = 1;
               break;
            }
            include_path = include_path->next_ptr;
         }
         if ((ignore_path == 1) || (access(path_name, R_OK | X_OK) != 0)) {
            free(path_name);
         } else if ((include_path = (tcl_file_path_s *) malloc(sizeof(tcl_file_path_s))) == (tcl_file_path_s *) NULL) {
            free(path_name);
         } else {
            if (last_include_path == (tcl_file_path_s *) NULL) {
               include_paths = include_path;
            } else {
               last_include_path->next_ptr = include_path;
            }
            include_path->path_name = path_name;
            include_path->next_ptr  = (tcl_file_path_s *) NULL;
            last_include_path       = include_path;
         }
      }
   }
   free(full_path_name);
   return;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
char *
evl_tcl_get_root_name (char *name_in) {
   char c;
   char *p;
   char *start;
   char root_name[8192];

   if (*name_in == (char) 0) {
      return (char *) NULL;
   }
   strncpy(root_name, name_in, 8191);
   root_name[8191] = (char) 0;
   p = &root_name[strlen(root_name) - 1];
   while (--p != root_name) {
      if (*p == (char) '/') {
         break;
      }
   }
   if (*p == (char) '/') {
      p++;
   }
   if (*p == (char) 0) {
      return (char *) NULL;
   }
   start = p;
   while ((c = *p++) != (char) 0) {
      if (!( ((c >= (char) '0') && (c <= (char) '9')) ||
             ((c >= (char) 'A') && (c <= (char) 'Z')) ||
             ((c >= (char) 'a') && (c <= (char) 'z')) ||
             (c == (char) '_') || (c == (char) '-') ) ) {
         *(p - 1) = (char) 0;
         break;
      }
   }
   return start;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
uint64_t
evl_tcl_get_cur_time () {
   time_t cur_time;

   if (time(&cur_time) != (time_t) -1) {
      if (localtime_r(&cur_time, &tm) != (struct tm *) NULL) {
         have_time = 1;
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
uint64_t
evl_tcl_get_time_of_day () {
   struct timeval tv;

   evl_tcl_get_cur_time();
   gettimeofday(&tv, (struct timezone *) NULL);
   return (uint64_t) tv.tv_sec;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
char *
evl_tcl_get_hhmmss () {
   if (have_time == 0) {
      evl_tcl_get_cur_time();
   }
   if (have_time == 0) {
      return "??:??:??";
   }
   strftime(time_str, 63, "%r", &tm);
   return time_str;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
uint64_t
evl_tcl_get_vpi_time () {
   s_vpi_time cur_time;

   cur_time.type = vpiSimTime;
   vpi_get_time(NULL, &cur_time);

   return ( ((uint64_t) cur_time.high  << 32) |
            ((uint64_t) cur_time.low ) );
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
char *
evl_tcl_get_env (char *env_name) {
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
evl_tcl_set_env (char *env_string, char *env_value) {
   return setenv(env_string, env_value, 1);
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
int
evl_tcl_system (char *system_call) {
   int result;

   result = system(system_call);
   if (WIFEXITED(result) == 1) {
      return WEXITSTATUS(result);
   }
   return -1;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
char *
evl_tcl_find_file (char *file_name_in, int use_include_paths) {
   int             path_length;
   char            file_name[8192];
   char            *env_ptr;
   tcl_file_path_s *include_path;

   if (*file_name_in == (char) 0) {
      return (char *) NULL;
   }
   if (access(file_name_in, R_OK) == 0) {
      return file_name_in;
   }
   if (*file_name_in == '/') {
      return (char *) NULL;
   }

   if (use_include_paths != 0) {
      include_path = include_paths;
      while (include_path != (tcl_file_path_s *) NULL) {
         path_length = strlen(include_path->path_name);
         strcpy(file_name, include_path->path_name);
         strncat(file_name, file_name_in, 8191 - path_length);
         if (access(file_name, R_OK) == 0) {
            return add_file_name_to_file_paths(file_name);
         }
         include_path = include_path->next_ptr;
      }
   }

   env_ptr = getenv("REPOROOT");
   if (env_ptr == (char *) NULL) {
      return (char *) NULL;
   }

   strncpy(file_name, env_ptr, 4095);
   while (1) {
      path_length = strlen(file_name) - 1;
      if (path_length < 1) {
         break;
      } else if (file_name[path_length] != '/') {
         file_name[path_length + 1] = (char) '/';
         file_name[path_length + 2] = (char) 0;
         break;
      } else {
         file_name[path_length] = (char) 0;
      }
   }
   path_length = strlen(file_name);
   strncat(file_name, file_name_in, 8191 - path_length);
   if (access(file_name, R_OK) == 0) {
      return add_file_name_to_file_paths(file_name);
   }
   return (char *) NULL;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
int
evl_tcl_prepare_tcl_script (int thread_id, char *file_name) {
   int          result;
   FILE         *fd;
   tcl_thread_s *tcl_thread;

   if (last_thread < 0) {
      int ii;

      for (ii = 0; ii < EVL_MAX_TCL_THREAD; ii++) {
         thread_ptrs[ii] = (tcl_thread_s *) NULL;
      }
   }

   if (last_thread >= (EVL_MAX_TCL_THREAD - 1)) {
      sprintf(last_global_error, "cannot create a TCL thread ... the maximum number of threads have been allocated");
      return EVL_TCL_CANNOT_CREATE_THREAD;
   }

   if ((fd = fopen(file_name, "r")) == (FILE *) NULL) {
      sprintf(last_global_error, "cannot create a TCL thread ... the file cannot be read");
      return EVL_TCL_CANNOT_CREATE_THREAD;
   }

   if ((tcl_thread = create_tcl_thread(thread_id, file_name, fd)) == (tcl_thread_s *) NULL) {
      return EVL_TCL_CANNOT_CREATE_THREAD;
   }

   if ((result = load_tcl_file(tcl_thread)) < 0) {
      return result;
   }
   fclose(fd);

   return tcl_thread->thread_num;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
int
evl_tcl_add_cmd_enc (int thread_index_in, char *cmd_name, int min_arg_count, int max_arg_count, char *return_type, char *parse_type) {
   int thread_index;

   thread_index = thread_index_in & 0xefffffff;
   if ((thread_index <= last_thread) && (thread_ptrs[thread_index] != (tcl_thread_s *) NULL)) {
      tcl_thread_s  *tcl_thread;
      tcl_cmd_enc_s *tcl_cmd_enc;

      tcl_thread = thread_ptrs[thread_index];
      if ((tcl_cmd_enc = find_tcl_cmd(tcl_thread, cmd_name)) != (tcl_cmd_enc_s *) NULL) {
         return tcl_cmd_enc->cmd_encoding;
      } else {
         if ((tcl_cmd_enc = (tcl_cmd_enc_s *) Tcl_Alloc(sizeof(tcl_cmd_enc_s))) != (tcl_cmd_enc_s *) NULL) {
            tcl_cmd_enc->cmd_name = append_str((char *) NULL, cmd_name, (char *) NULL);
            if (strcmp(cmd_name, tcl_cmd_enc->cmd_name) == 0) {
               tcl_cmd_enc->cmd_encoding  = tcl_thread->next_cmd_encoding++;
               tcl_cmd_enc->min_arg_count = min_arg_count;
               tcl_cmd_enc->max_arg_count = max_arg_count;
               if (strcmp("string", return_type) == 0) {
                  tcl_cmd_enc->return_type = EVL_TCL_RESULT_STR;
               } else if (strcmp("string list", return_type) == 0) {
                  tcl_cmd_enc->return_type = EVL_TCL_RESULT_STR_LIST;
               } else if (strcmp("int list", return_type) == 0) {
                  tcl_cmd_enc->return_type = EVL_TCL_RESULT_INT_LIST;
               } else {
                  tcl_cmd_enc->return_type = EVL_TCL_RESULT_INT;
               }
               if (strcmp("agent name", parse_type) == 0) {
                  tcl_cmd_enc->parse_type = EVL_TCL_PARSE_AGENT;
               } else if (strcmp("agent default", parse_type) == 0) {
                  tcl_cmd_enc->parse_type = EVL_TCL_PARSE_AGENT;
               } else {
                  tcl_cmd_enc->parse_type = EVL_TCL_PARSE_NONE;
               }
               tcl_cmd_enc->next_cmd = tcl_thread->tcl_cmds;
               tcl_thread->tcl_cmds  = tcl_cmd_enc;

               /* Create a new command that can be interpreted by the TCL script */
               Tcl_CreateObjCommand(tcl_thread->tcl_interp, tcl_cmd_enc->cmd_name, (Tcl_ObjCmdProc *) exec_verilog_cmd, (ClientData) ((long) tcl_thread->thread_num), (Tcl_CmdDeleteProc *) NULL);
               return tcl_cmd_enc->cmd_encoding;
            }
            Tcl_Free((char *) tcl_cmd_enc);
         }
      }
   }
   return EVL_TCL_END_OF_THREAD;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
int
evl_tcl_start_tcl_script (int thread_index_in) {
   int thread_index;
   int result;

   thread_index = thread_index_in & 0xefffffff;
   if ((thread_index <= last_thread) && (thread_ptrs[thread_index] != (tcl_thread_s *) NULL)) {
      if ((result = start_tcl_thread(thread_ptrs[thread_index])) < 0) {
         return result;
      }
      return 0;
   }
   return EVL_TCL_END_OF_THREAD;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
int
evl_tcl_eval_string (int thread_index_in, char *eval_str) {
   int result;
   int thread_index;

   thread_index = thread_index_in & 0xefffffff;
   if ((thread_index <= last_thread) && (thread_ptrs[thread_index] != (tcl_thread_s *) NULL) && (eval_str != (char *) NULL)) {
      thread_ptrs[thread_index]->cmd_encoding = 0;
      thread_ptrs[thread_index]->eval_str = eval_str;
      if ((result = transfer_to_dpi(thread_index)) < 0) {
         return result;
      }
      thread_ptrs[thread_index]->eval_str = null_str;
      return thread_ptrs[thread_index]->cmd_encoding;
   }
   return EVL_TCL_END_OF_THREAD;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
int
evl_tcl_get_next_cmd (int thread_index_in) {
   int thread_index;
   int result;

   thread_index = thread_index_in & 0xefffffff;
   if ((thread_index <= last_thread) && (thread_ptrs[thread_index] != (tcl_thread_s *) NULL)) {
      tcl_thread_s *tcl_thread;

      tcl_thread = thread_ptrs[thread_index];
      tcl_thread->arg_count    = 0;
      tcl_thread->cmd_encoding = -1;
      if ((result = transfer_to_dpi(thread_index)) < 0) {
         return result;
      }
      return tcl_thread->cmd_encoding;
   }
   return EVL_TCL_END_OF_THREAD;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
int
evl_tcl_get_cmd_encoding (int thread_index_in) {
   int thread_index;

   thread_index = thread_index_in & 0xefffffff;
   if ((thread_index <= last_thread) && (thread_ptrs[thread_index] != (tcl_thread_s *) NULL)) {
      return thread_ptrs[thread_index]->cmd_encoding;
   }
   return EVL_TCL_END_OF_THREAD;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
int
evl_tcl_get_arg_count (int thread_index_in) {
   int thread_index;

   thread_index = thread_index_in & 0xefffffff;
   if ((thread_index <= last_thread) && (thread_ptrs[thread_index] != (tcl_thread_s *) NULL)) {
      return thread_ptrs[thread_index]->arg_count;
   }
   return 0;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
int
evl_tcl_get_arg_is_string (int thread_index_in, int arg_index_in) {
   int thread_index;
   int arg_index;

   thread_index = thread_index_in & 0xefffffff;
   arg_index    = arg_index_in & 0xefffffff;
   if ((thread_index <= last_thread) && (thread_ptrs[thread_index] != (tcl_thread_s *) NULL)) {
      if (arg_index < thread_ptrs[thread_index]->arg_count) {
         int           count;
         tcl_int_obj_s *int_obj;

         count = arg_index;
         int_obj = thread_ptrs[thread_index]->first_int_arg;
         while ((count-- > 0) && (int_obj != (tcl_int_obj_s *) NULL)) {
            int_obj = int_obj->next_ptr;
         }
         if (int_obj == (tcl_int_obj_s *) NULL) {
            return -1;
         }
         if (int_obj->valid != 0) {
            return 1;
         }
         return 0;
      }
   }
   return -1;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
char *
evl_tcl_get_str_arg (int thread_index_in, int arg_index_in) {
   int thread_index;

   thread_index = thread_index_in & 0xefffffff;
   if ((thread_index <= last_thread) && (thread_ptrs[thread_index] != (tcl_thread_s *) NULL)) {
      return get_str_arg(thread_ptrs[thread_index], arg_index_in);
   } else if (thread_index_in < 0) {
      return last_global_error;
   }
   return null_str;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
uint64_t
evl_tcl_get_int_arg (int thread_index_in, int arg_index_in) {
   int thread_index;
   int arg_index;

   thread_index = thread_index_in & 0xefffffff;
   arg_index    = arg_index_in & 0xefffffff;
   if ((thread_index <= last_thread) && (thread_ptrs[thread_index] != (tcl_thread_s *) NULL)) {
      if (arg_index < thread_ptrs[thread_index]->arg_count) {
         int           count;
         tcl_int_obj_s *int_obj;

         count = arg_index;
         int_obj = thread_ptrs[thread_index]->first_int_arg;
         while ((count-- > 0) && (int_obj != (tcl_int_obj_s *) NULL)) {
            int_obj = int_obj->next_ptr;
         }
         if (int_obj == (tcl_int_obj_s *) NULL) {
            return -1;
         }
         return int_obj->value;
      }
   }
   return -1;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
uint64_t
evl_tcl_parse_agent_name (int thread_index_in, char *agent_name_in, int default_is_0) {
   int thread_index;

   thread_index = thread_index_in & 0xefffffff;
   if ((thread_index <= last_thread) && (thread_ptrs[thread_index] != (tcl_thread_s *) NULL)) {
      tcl_thread_s *tcl_thread;
      int          shire;
      int          neighborhood;
      int          core;
      int          agent_id;
      int          result;

      tcl_thread = thread_ptrs[thread_index];
      tcl_thread->arg_count = 1;
      if ((result = add_str_arg(tcl_thread, 0, agent_name_in)) != 0) {
         return process_error(tcl_thread, result, get_str_arg(tcl_thread, 0), 1);
      }
      for (tcl_thread->arg_count = 1; tcl_thread->arg_count < 5; tcl_thread->arg_count++) {
         if ((result = add_str_arg(tcl_thread, tcl_thread->arg_count, "")) != 0) {
            return process_error(tcl_thread, result, get_str_arg(tcl_thread, 0), 1);
         }
      }
      parse_agent_name(get_str_arg(tcl_thread, 0), &shire, &neighborhood, &core, &agent_id);
      if (default_is_0 != 0) {
         if (shire < 0) {
            shire = 0;
         }
         if (neighborhood < 0) {
            neighborhood = 0;
         }
         if (core < 0) {
            core = 0;
         }
         if (agent_id < 0) {
            agent_id = 0;
         }
      }
      if ((result = add_int_arg(tcl_thread, 1, (uint64_t) agent_id, 1)) != 0) {
         return process_error(tcl_thread, result, get_str_arg(tcl_thread, 0), 1);
      }
      if ((result = add_int_arg(tcl_thread, 2, (uint64_t) shire, 1)) != 0) {
         return process_error(tcl_thread, result, get_str_arg(tcl_thread, 0), 1);
      }
      if ((result = add_int_arg(tcl_thread, 3, (uint64_t) neighborhood, 1)) != 0) {
         return process_error(tcl_thread, result, get_str_arg(tcl_thread, 0), 1);
      }
      if ((result = add_int_arg(tcl_thread, 4, (uint64_t) core, 1)) != 0) {
         return process_error(tcl_thread, result, get_str_arg(tcl_thread, 0), 1);
      }
      return 0;
   }
   return -1;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
int
evl_tcl_set_str_res (int thread_index_in, int res_index_in, char *str_res) {
   int thread_index;
   int res_index;

   thread_index = thread_index_in & 0xefffffff;
   res_index    = res_index_in & 0xefffffff;
   if ((thread_index <= last_thread) && (thread_ptrs[thread_index] != (tcl_thread_s *) NULL)) {
      return add_str_res(thread_ptrs[thread_index], thread_ptrs[thread_index]->tcl_interp, res_index, str_res);
   }
   return EVL_TCL_END_OF_THREAD;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
int
evl_tcl_set_int_res (int thread_index_in, int res_index_in, uint64_t value) {
   int thread_index;
   int res_index;

   thread_index = thread_index_in & 0xefffffff;
   res_index    = res_index_in & 0xefffffff;
   if ((thread_index <= last_thread) && (thread_ptrs[thread_index] != (tcl_thread_s *) NULL)) {
      return add_int_res(thread_ptrs[thread_index], thread_ptrs[thread_index]->tcl_interp, res_index, value);
   }
   return EVL_TCL_END_OF_THREAD;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
int
evl_tcl_open_replay_file (int thread_index_in, char *file_name) {
   int thread_index;

   thread_index = thread_index_in & 0xefffffff;
   if ((thread_index <= last_thread) && (thread_ptrs[thread_index] != (tcl_thread_s *) NULL)) {
      if ((thread_ptrs[thread_index]->replay_fd = fopen(file_name, "w")) != (FILE *) NULL) {
         return 1;
      }
   }
   return 0;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
void
evl_tcl_close_replay_file (int thread_index_in) {
   int thread_index;

   thread_index = thread_index_in & 0xefffffff;
   if ((thread_index <= last_thread) && (thread_ptrs[thread_index] != (tcl_thread_s *) NULL)) {
      if (thread_ptrs[thread_index]->replay_fd != (FILE *) NULL) {
         fclose(thread_ptrs[thread_index]->replay_fd);
         thread_ptrs[thread_index]->replay_fd = (FILE *) NULL;
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
int
evl_tcl_replay_file_write (int thread_index_in, char *message) {
   int thread_index;

   thread_index = thread_index_in & 0xefffffff;
   if ((thread_index <= last_thread) && (thread_ptrs[thread_index] != (tcl_thread_s *) NULL)) {
      if (thread_ptrs[thread_index]->replay_fd != (FILE *) NULL) {
         fprintf(thread_ptrs[thread_index]->replay_fd, "%s\n", message);
      }
   }
   return 0;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
char *
evl_tcl_get_net_binary_value (char *path) {
   int       prop;
   vpiHandle net;

   net  = vpi_handle_by_name(path, NULL);
   prop = (int) vpi_get(vpiType, net);
   if ((prop == vpiNet)        || (prop == vpiPort)      ||
       (prop == vpiReg)        || (prop == vpiNetBit)    ||
       (prop == vpiPortBit)    || (prop == vpiRegBit)    || (prop == vpiVarSelect)         ||
       (prop == vpiPartSelect) || (prop == vpiBitSelect) || (prop == vpiIndexedPartSelect) ||
       (prop == vpiBitVar)     || (prop == vpiByteVar)   || (prop == vpiEnumVar)           ||
       (prop == vpiStructVar)  || (prop == vpiUnionVar)  || (prop == vpiPackedArrayVar)) {
      s_vpi_value value;
      int         vector_size;

      value.format = vpiBinStrVal;
      vector_size  = (int) vpi_get(vpiSize, net);
      if (vector_size >= 1024) {
         sprintf(net_value_str, "too long");
      } else {
         vpi_get_value(net, &value);
         sprintf(net_value_str, "%s", value.value.str);
      }
   } else {
      sprintf(net_value_str, "%s", "");
   }
   vpi_free_object(net);
   return net_value_str;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
DPI_DLLESPEC
char *
evl_tcl_get_net_hex_value (char *path) {
   int       prop;
   vpiHandle net;

   net  = vpi_handle_by_name(path, NULL);
   prop = (int) vpi_get(vpiType, net);
   if ((prop == vpiNet)        || (prop == vpiPort)      ||
       (prop == vpiReg)        || (prop == vpiNetBit)    ||
       (prop == vpiPortBit)    || (prop == vpiRegBit)    || (prop == vpiVarSelect)         ||
       (prop == vpiPartSelect) || (prop == vpiBitSelect) || (prop == vpiIndexedPartSelect) ||
       (prop == vpiBitVar)     || (prop == vpiByteVar)   || (prop == vpiEnumVar)           ||
       (prop == vpiStructVar)  || (prop == vpiUnionVar)  || (prop == vpiPackedArrayVar)) {
      s_vpi_value value;
      int         vector_size;

      value.format = vpiHexStrVal;
      vector_size  = (int) vpi_get(vpiSize, net);
      if (vector_size >= 4093) {
         sprintf(net_value_str, "too long");
      } else {
         vpi_get_value(net, &value);
         sprintf(net_value_str, "%s", value.value.str);
      }
   } else {
      sprintf(net_value_str, "%s", "");
   }
   vpi_free_object(net);
   return net_value_str;
}


/***************************************************************************************************
 *
 *
 *
 **************************************************************************************************/
int
evl_tcl_check_depth (vpiHandle moduleHandle, int depth) {
   char *fullName;

   if ((moduleHandle == NULL) || (depth <= 0)) {
      return 0;
   }
   fullName = vpi_get_str(vpiFullName, moduleHandle);
   while (*fullName != (char) 0) {
      if (*fullName++ == (char) '.') {
         if (depth-- <= 1) {
            return 0;
         }
      }
   }
   return 1;
}


void
evl_tcl_scan_submodule (vpiHandle moduleHandle, int depth) {
   vpiHandle moduleItr;
   vpiHandle submoduleHandle;
   vpiHandle paramItr;
   vpiHandle paramHandle;
   char      moduleInfo[8192];
   char      paramInfo[4096];
   char      space;

   if (evl_tcl_check_depth(moduleHandle, depth) == 0) {
      return;
   }
   sprintf(moduleInfo, "%s (", vpi_get_str(vpiFullName, moduleHandle));
   strcat(moduleInfo, vpi_get_str(vpiDefName, moduleHandle));
   paramInfo[0] = (char) 0;
   if (NULL != (paramItr = vpi_iterate(vpiParameter, moduleHandle))) {
      sprintf(paramInfo, "#");
      space = '(';
      while (NULL != (paramHandle = vpi_scan(paramItr))) {
         int limit;

         limit = 4094 - strlen(paramInfo);
         if (limit > 0) {
            s_vpi_value value;
            char        localInfo[1024];

            value.format = vpiObjTypeVal;
            vpi_get_value(paramHandle, &value);
            if (value.format == vpiStringVal) {
               sprintf(localInfo, "%c%s=%s", space, vpi_get_str(vpiName, paramHandle), value.value.str);
               strncat(paramInfo, localInfo, limit);
               space = ',';
            } else if (value.format == vpiIntVal) {
               sprintf(localInfo, "%c%s=%d", space, vpi_get_str(vpiName, paramHandle), value.value.integer);
               strncat(paramInfo, localInfo, limit);
               space = ',';
            } else {
               value.format = vpiHexStrVal;
               vpi_get_value(paramHandle, &value);
               sprintf(localInfo, "%c%s=%s", space, vpi_get_str(vpiName, paramHandle), value.value.str);
               strncat(paramInfo, localInfo, limit);
               space = ',';
            }
         }
      }
      strcat(paramInfo, ")");
   }
   vpi_printf("%s%s, %s)\n", moduleInfo, paramInfo, vpi_get_str(vpiFile, moduleHandle));
   moduleItr = vpi_iterate(vpiModule, moduleHandle);
   while (NULL != (submoduleHandle = vpi_scan(moduleItr))) {
      evl_tcl_scan_submodule(submoduleHandle, depth);
   }
   return;
}


DPI_DLLESPEC
void
evl_tcl_scan_module (char *path, int depth_in) {
   int       depth;
   vpiHandle moduleHandle;
   char      *fullName;

   if (depth_in < 0) {
      depth = 1;
   } else {
      depth = depth_in;
   }
   if (NULL == (moduleHandle = vpi_handle_by_name(path, NULL))) {
      return;
   }

   //
   // Set the depth relative to the depth of the specified module.
   //
   fullName = vpi_get_str(vpiFullName, moduleHandle);
   while (*fullName != (char) 0) {
      if (*fullName++ == (char) '.') {
         depth++;
      }
   }
   depth++;
   evl_tcl_scan_submodule(moduleHandle, depth);
   return;
}
