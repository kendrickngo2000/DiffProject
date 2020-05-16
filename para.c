//
//  para.c
//  diff
//
//  Created by William McCarthy on 5/9/19.
//  Copyright © 2019 William McCarthy. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "para.h"
#include "util.h"

#define BUFLEN 256
#define BUFLEN 256
#define MAXSTRINGS 1024
#define TOOMANYFILES_ERROR 2
#define CONFLICTING_OUTPUT_OPTIONS 3

static char buf[BUFLEN];
static char* strings1[MAXSTRINGS], * strings2[MAXSTRINGS];
static int showversion = 0, showbrief = 0, ignorecase = 0, report_identical = 0, showsidebyside = 0;
static int showleftcolumn = 0, showunified = 0, showcontext = 0, suppresscommon = 0, diffnormal = 0;

static int count1 = 0, count2 = 0;

para* para_make(char* base[], int filesize, int start, int stop) {
  para* p = (para*) malloc(sizeof(para));
  p->base = base;
  p->filesize = filesize;
  p->start = start;
  p->stop = stop;
  p->firstline = (p == NULL || start < 0) ? NULL : p->base[start];
  p->secondline = (p == NULL || start < 0 || filesize < 2) ? NULL : p->base[start + 1];
  
  return p;
}

para* para_first(char* base[], int size) {
  para* p = para_make(base, size, 0, -1);
  return para_next(p);
}

void para_destroy(para* p) { free(p); }

para* para_next(para* p) {
  if (p == NULL || p->stop == p->filesize) { return NULL; }
  
  int i;
  para* pnew = para_make(p->base, p->filesize, p->stop + 1, p->stop + 1);
  for (i = pnew->start; i < p->filesize && strcmp(p->base[i], "\n") != 0; ++i) { }
  pnew->stop = i;
  
  if (pnew->start >= p->filesize) {
    free(pnew);
    pnew = NULL;
  }
  return pnew;
}
size_t para_filesize(para* p) { 
    return p == NULL ? 0 : p->filesize; 
}

size_t para_size(para* p) { 
    return p == NULL || p->stop < p->start ? 0 : p->stop - p->start + 1; 
}

char** para_base(para* p) { return p->base; }

char* para_info(para* p) {
  static char buf[BUFLEN];   // static for a reason
  snprintf(buf, sizeof(buf), "base: %p, filesize: %d, start: %d, stop: %d\n",
           p->base, p->filesize, p->start, p->stop);
  return buf;  // buf MUST be static
}

void printline(void) {
    for (int i = 0; i < 10; ++i) {
        printf("==========");
    }
    printf("\n");
}

int para_equal(para* p, para* q) {
  if (p == NULL || q == NULL) { return 0; }
  if (para_size(p) != para_size(q)) { return 0; }
  if (p->start >= p->filesize || q->start >= q->filesize) { return 0; }
  int i = p->start, j = q->start, equal = 0;
  while ((equal = strcmp(p->base[i], q->base[i])) == 0) { ++i; ++j; }
  return 1;
}

void para_print(para* p, void (*fp)(const char*)) {
  if (p == NULL) { return; }
  for (int i = p->start; i <= p->stop && i != p->filesize; ++i) { fp(p->base[i]); }
}

void para_printfile(char* base[], int count, void (*fp)(const char*)) {
  para* p = para_first(base, count);
  while (p != NULL) {
    para_print(p, fp);
    p = para_next(p);
  }
  printline();

  char* yesorno(int condition) {
      return condition == 0 ? "no" : "YES";
  }

  FILE* openfile(const char* filename, const char* openflags) {
      FILE* f;
      if ((f = fopen(filename, openflags)) == NULL) {
          printf("can't open '%s'\n", filename);
          exit(1);
      }
      return f;
  }

  FILE* openfile(const char* filename, const char* openflags) {
      FILE* f;
      if ((f = fopen(filename, openflags)) == NULL) {
          printf("can't open '%s'\n", filename);
          exit(1);
      }
      return f;
  }
  void diff_output_conflict_error(void) {
      fprintf(stderr, "diff: conflicting output style options\n");
      fprintf(stderr, "diff: Try `diff --help' for more information.)\n");
      exit(CONFLICTING_OUTPUT_OPTIONS);
  }

 

  int main(int argc, const char* argv[]) {
      init_options_files(--argc, ++argv);

  

      para* p = para_first(strings1, count1);
      para* q = para_first(strings2, count2);

      normal(p, q);

      if (showbrief == 1) {
          brief(p, q);
      }
      if (report_identical == 1) {
          identical(p, q);
      }

      return 0;
  }