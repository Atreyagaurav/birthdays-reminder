#include <stdio.h>
#include <time.h>
#include "convertADBS.h"


// Number of days in each month of English calendar (for non-leap year)
int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// Information about number of days in every month since 2000 BS to 2089 BS
const int dataBS[][13] = {
    {30,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {30,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,29,30,30,29,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,29,30,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,29,30,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,31,32,31,31,30,29,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,30},
    {31,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,31,32,31,31,30,29,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {30,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,31,32,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {30,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {30,32,31,32,31,31,29,30,30,29,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,29,30,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,29,30,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,30,29,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,30},
    {31,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,31,32,31,31,30,29,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,30},
    {31,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,31,32,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {30,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,29,30,29,30,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,29,30,30,29,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,29,30,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,30,29,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,30},
    {31,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,31,32,31,31,30,29,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,30},
    {31,32,31,32,31,30,30,30,29,30,29,31},
    {30,32,31,32,31,30,30,30,29,30,30,30},
    {31,31,32,31,31,30,30,30,29,30,30,30},
    {31,31,32,31,31,30,30,30,29,30,30,30},
    {31,32,31,32,30,31,30,30,29,30,30,30},
    {30,32,31,32,31,30,30,30,29,30,30,30},
    {31,31,32,31,31,31,30,30,30,30,30,30},
    {30,31,32,32,30,31,30,30,29,30,30,30},
    {30,32,31,32,31,30,30,30,29,30,30,30}
};


int isLeapYear(int y){
  // returns true if y is a leap year
  return (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
}

int validDate(Date* d){
  // Returns 0 if the date is invalid
  if (d->month < 1 || d->month > 12)
    return 0;
  if (d->day > 32 || d->day < 1)
    return 0;
  if (d->type == AD) {
    return d->year > 0 ;
  } else {
    return d->year > 1999 && d->year < 2090;
  }
}

int daysSinceADEpoch(Date *d){
  // Return number of days since 0001/01/01 AD upto
  // this day
  // (e.g.  0001/01/02 is 2nd day)
  extern int daysInMonth[];
  int i;
  // Count number of days from 1 AD upto start of the given year
  int y = d->year - 1;
  int daysBeforeThisYear = y*365 + y/4 - y/100 + y/400;
    
  // Count number of days from start of year upto the give month
  int daysBeforeThisMonth = 0; 
  for (i=0; i< d->month - 1; i++){
    daysBeforeThisMonth += daysInMonth[i];
  }
  
  // If leap year and the month is after february, add leap day of february
  if (isLeapYear(d->year) && d->month > 2){
      daysBeforeThisMonth++;
  }
  return daysBeforeThisYear + daysBeforeThisMonth + d->day;
}

int daysSinceBSEpoch(Date *d){
  // Return number of days since 2000/01/01 BS upto
  // this day
  // (e.g.  2000/01/03 is 3rd day)
  int yr = d->year - 2000;
  int days = d->day, i =0 ,j =0;

  for (i=0; i<yr; i++){
    for (j = 0; j< 12; j++){
      days += dataBS[i][j];
    }
  }
  for (j = 0; j< d->month - 1; j++){
    days += dataBS[yr][j];
  }
  
  return days ;
}

Date nthDayOfBSEpoch(int n){
  int month,year, day = 1, days = 0, daysInThisMonth;
  month = 0;
  year = 0;
  while (days<n){
    daysInThisMonth = dataBS[year][month];
    if (days+daysInThisMonth >= n){
      day = n - days;
      days = n;
    } else {
      days += daysInThisMonth;
      month++;
      if (month == 12){
	year++;
	month = 0;
      }      
    }
  }
  // NOTE: if n <=  0 then the while loop is not entered and 2000/1/1 is returned
  // else processing is done normally and req. date is returned
  Date d = {year + 2000, month + 1, day, BS};
  return d;
}

Date nthDayOfADEpoch(int n){
  Date d;
  int daysInThisMonth;
  int y = n/365.2425;
  int daysBeforeThisYear = y*365 + y/4 - y/100 + y/400;
  int remDays = n - daysBeforeThisYear;
  if (remDays == 0){
    d.year = y;
    d.month = 12;
    d.day = 31;
    return d;
  }
  
  d.year = y + 1;
  d.month = 1;  
  while (remDays > 0){
    daysInThisMonth = daysInMonth[d.month-1];
    if (d.month == 2 && isLeapYear(d.year))
      daysInThisMonth++;

    if (remDays <= daysInThisMonth){
      d.day = remDays;
      remDays =0;
    } else {
      d.month++;
      if (d.month > 12){
	d.month = 1;
	d.year++;
      }
      remDays -= daysInThisMonth;
    }
  }
  d.type = AD;
  return d;
}

int daysSinceEpoch(Date *d){
  if (d->type==AD){
    return daysSinceADEpoch(d);
  }else{
    return daysSinceBSEpoch(d);
  }
}

Date nthDayOfEpoch(int n, enum calendar_system to){
  if (to==BS){
    return nthDayOfBSEpoch(n);
  }else{
    return nthDayOfADEpoch(n);
  }
}

Date convertADBS(Date *date,enum calendar_system to) {
  if (date->type==to){
    return *date;
  }
  static Date BSEpoch = {1943, 4, 14, AD};
  int sinceADEpochToBSEpoch = daysSinceADEpoch(&BSEpoch);
  int sinceEpochToNow = daysSinceEpoch(date);
  int sinceConvertedEpoch;
  if (to==BS){
    sinceConvertedEpoch = sinceEpochToNow - sinceADEpochToBSEpoch + 1;
  }else{
    sinceConvertedEpoch = sinceADEpochToBSEpoch + sinceEpochToNow - 1;
  }
  return nthDayOfEpoch(sinceConvertedEpoch, to);
}


char* strADBS(enum calendar_system c) {
  if (c == BS){
    return "BS";
  }else{
    return "AD";
  }
}

void printDate(Date *d, int year){
  if (year){
    printf("%d-",d->year);
  }
  printf("%02d-%02d %s",d->month,d->day, strADBS(d->type));
}


Date getCurrentDate(){
  // Gets current Month and year from system
  // using function of time.h
  time_t now;
  struct tm * timeinfo;
  Date d;
  time(&now);
  timeinfo = localtime(&now);
  d.year = timeinfo->tm_year + 1900;
  d.month = timeinfo->tm_mon + 1 ;
  d.day = timeinfo->tm_mday;
  d.type = AD;
  return d;
}

int dateDiff(Date* date1,Date* date2){
  Date d = convertADBS(date2, date1->type);
  return daysSinceEpoch(date1) - daysSinceEpoch(&d);
}


