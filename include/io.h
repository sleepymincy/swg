// io.h

#ifndef IO_H
#define IO_H
#include "swg.h"

void load_file(const char *path);
void load_string(const char *word);
int  prompt_user(void);
void flush_outbuf(FILE *fp);

#endif