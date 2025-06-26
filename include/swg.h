// swg.h

#ifndef SWG_H
#define SWG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <sysexits.h>

#define PROMPT_THRESHOLD    500e6
#define MAX_WORD_LEN        1024
#define MAX_WORDS           1024
#define OUTPUT_BUF_SIZE     ((size_t)8 * 1024 * 1024)

extern char         *words[MAX_WORDS];
extern size_t       word_count;
extern char         *outbuf;
extern size_t       outbuf_used;
extern size_t       max_depth_opt;
extern bool         quiet;
extern bool         dry_run;
extern const char   *ifile, *ofile, *words_opt;
extern const char   *VERSION;

void show_help(const char *bin_name);
void show_version(void);
void parse_args(int argc, char *argv[]);
void load_file(const char *path);
void load_string(const char *word);
void flush_outbuf(FILE *fp);
void create_wordlist(FILE *out, size_t max_depth);
int  prompt_user(void);

#endif