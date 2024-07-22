#include "convertADBS.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]) {
  char *fname = NULL;
  if (argc > 1) {
    fname = argv[1];
  } else {
    fname = "dates-sorted.txt";
  }
  FILE*  fp = fopen(fname, "r");
  if (fp == NULL) {
    printf("Dates file not present");
    return 1;
  }
  char *line = NULL;
  size_t n = 0;

  int ad_incorrect = 0;
  int bs_incorrect = 0;
  char ad_incorrect_msg[1024];
  char bs_incorrect_msg[1024];
  int linenum = 0;
  Date *d1, *d2, *d3;
  while (getline(&line, &n, fp) >= 0) {
    if (n == 0) {
      break;
    }
    linenum += 1;
    Date bs_date, ad_date, bs2ad_date, ad2bs_date;
    sscanf(line, "%d-%d-%d %d-%d-%d\n", &bs_date.year, &bs_date.month,
           &bs_date.day, &ad_date.year, &ad_date.month, &ad_date.day);
    bs_date.type = BS;
    ad_date.type = AD;
    if (!validDate(&bs_date)) {
      printf("./%s:%d: Invalid Date ", fname, linenum);
      printDate(&bs_date, 1);
      printf(" Cannot test.\n");
      return 1;
    }
    if (!validDate(&ad_date)) {
      printf("./%s:%d: Invalid Date ", fname, linenum);
      printDate(&ad_date, 1);
      printf(": Cannot test.\n");
      return 1;
    }
    bs2ad_date = convertADBS(&bs_date, AD);
    ad2bs_date = convertADBS(&ad_date, BS);
    if (bs_date.year != ad2bs_date.year || bs_date.month != ad2bs_date.month ||
        bs_date.day != ad2bs_date.day) {
      if (!bs_incorrect) {
	d1 = &ad_date;
	d2 = &bs_date;
	d3 = &ad2bs_date;
	sprintf(bs_incorrect_msg,
		"%04d-%02d-%02d %s != %04d-%02d-%02d"
		" %s (%04d-%02d-%02d %s)",
		d1->year, d1->month,d1->day, strADBS(d1->type),
		d2->year, d2->month,d2->day, strADBS(d2->type),
		d3->year, d3->month,d3->day, strADBS(d3->type)
		);
      }
      bs_incorrect += 1;
    } else {
      if (bs_incorrect) {
	printf("./%s:%d: %d Err %s\n",
		fname, linenum - bs_incorrect, bs_incorrect, bs_incorrect_msg);
      }
      bs_incorrect = 0;
    }
    if (ad_date.year != bs2ad_date.year || ad_date.month != bs2ad_date.month ||
        ad_date.day != bs2ad_date.day) {
      if (!ad_incorrect) {
	d1 = &bs_date;
	d2 = &ad_date;
	d3 = &bs2ad_date;
	sprintf(ad_incorrect_msg,
		"%04d-%02d-%02d %s != %04d-%02d-%02d"
		" %s (%04d-%02d-%02d %s)",
		d1->year, d1->month,d1->day, strADBS(d1->type),
		d2->year, d2->month,d2->day, strADBS(d2->type),
		d3->year, d3->month,d3->day, strADBS(d3->type)
		);
      }
      ad_incorrect += 1;
    } else {
      if (ad_incorrect) {
	printf("./%s:%d: %d Err %s\n",
		fname, linenum - ad_incorrect, ad_incorrect, ad_incorrect_msg);
      }
      ad_incorrect = 0;
    }
  }
  free(line);
  fclose(fp);
  return 0;
}
