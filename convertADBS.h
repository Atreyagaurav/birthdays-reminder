#ifndef _CONVERT_ADBS

#define _CONVERT_ADBS

#define PERSON_NAME_LENGTH 50

enum calendar_system {AD, BS};

typedef struct{
  int year;
  int month;
  int day;
  enum calendar_system type;
} Date;

int isLeapYear(int y);

int validDate(Date* d);

int daysSinceADEpoch(Date *d);

int daysSinceBSEpoch(Date *d);

Date nthDayOfBSEpoch(int n);

Date nthDayOfADEpoch(int n);

int daysSinceEpoch(Date *d);

Date nthDayOfEpoch(int n, enum calendar_system to);

Date convertADBS(Date *date,enum calendar_system to);

char* strADBS(enum calendar_system c);

void printDate(Date *d, int year);

Date getCurrentDate();

int dateDiff(Date* date1,Date* date2);

#endif
