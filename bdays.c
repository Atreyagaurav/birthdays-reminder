#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "convertADBS.h"


#ifndef BIRTHDAYS_FILE
#define BIRTHDAYS_FILE "birthdays.txt"
#endif

#define COMMENT_CHAR '#'
#define DEFAULT_YEAR getCurrentDate().year
#define DEBUG 0
#define Err_msg(message)                                                       \
  printf("[%s] L:%d FUN:%s ERROR %s", __FILE__, __LINE__, __FUNCTION__, message)
#define Debug_msg(message)                                                     \
  ({                                                                           \
    if (DEBUG) {                                                               \
      printf("[%s] L:%d FUN:%s DEBUG %s", __FILE__, __LINE__, __FUNCTION__,        \
             message);                                                         \
    }                                                                          \
  })

#define DEBUG_BLK(code) ({if (DEBUG) {code}})

typedef struct{
  int days;
  char name[PERSON_NAME_LENGTH];
  Date birthDate;
} Person;

void printPerson(Person *p){
  if (p->days == 0){
    printf(" -TODAY-");
  }else{
    printf("%3d days",p->days);
  }
  printf(" %-18s (",p->name);
  printDate(&p->birthDate,0);
  printf(")\n");
}


void skipThisLine(FILE* fp){
  while(fgetc(fp)!='\n');
}

void skipWhiteSpace(FILE* fp){
  char c = ' ';
  while(1){
    if(c==' '||c=='\t'){
      c=fgetc(fp);
    }else{
      break;
    }
  }
  ungetc(c, fp);
}

int nextLine(FILE* fp){
  char c;
  while (1){
    c=fgetc(fp);
    switch (c){
    case EOF:
      return 0;
    case '\n':
    case '\r':
    case ' ':
      continue;
    case COMMENT_CHAR:
      skipThisLine(fp);
      break;
    default:
      ungetc(c, fp);
      return 1;
    }
  }
}


Date readSingleDate(FILE* fp,char* name){
  Date d;
  int i;
  int flag = 0;
  char c = ' ';
  char year[5];
  if (nextLine(fp)==0){
    Debug_msg("End of File Reached\n");
    d.year =  0;
    return d;
  }
  for(i=0;i<4;i++){
    year[i] = fgetc(fp);
    if (year[i]=='X'){
      d.year = DEFAULT_YEAR;
      DEBUG_BLK(printf("Year not given using default Year:%d\n",d.year););
      break;
    }
  }
  if (i==4){
    year[4]='\0';
    d.year = atoi(year);
  }
  while(fgetc(fp)!='-');
  fscanf(fp,"%d-%d",&d.month,&d.day);
  while(c!=':'){
    c=fgetc(fp);
    switch (c){
    case ' ':
      break;
    case 'A':
      flag = 1;
      break;
    case 'D':
      if (flag==1){
	d.type = AD;
	break;
      }
    case 'B':
      flag = 2;
      break;
    case 'S':
      if (flag==2){
	d.type = BS;
	break;
      }
    }
  }
  skipWhiteSpace(fp);
  if (name != NULL){
    c = ' ';
    flag = 0;
    while (c!='\n'){
      c = fgetc(fp);
      if (c==COMMENT_CHAR){
	Debug_msg("SKIPPING Comment\n");
	skipThisLine(fp);
	break;
      }
      if (flag>=PERSON_NAME_LENGTH-2){
	Debug_msg("SKIPPING Long Name\n");
	skipThisLine(fp);
	break;
      }
      *(name+flag) = c;
      flag++;
    }
    flag-=2;
    while(*(name+flag)==' ' || *(name+flag)=='\t'){
      flag--;
    }
    *(name+flag+1) = '\0';
    DEBUG_BLK(printf("Entry Read:");printDate(&d, 1);printf("\tNAME: %s\n",name););
  }else{
    DEBUG_BLK(printf("Entry Read:");printDate(&d, 1);printf("\tName Not read\n"););
    skipThisLine(fp);
  }
  return d;
}


int daysTillNextBDay(Date bday){
  Debug_msg("Calculating Days till next birthday\n");
  Date today = getCurrentDate();

  today = convertADBS(&today,bday.type);
  bday.year = today.year;
  int diff = dateDiff(&bday,&today);
  if (diff < 0){
    Debug_msg("Birthday for this year already gone, looking for next year\n");
    bday.year = today.year+1;
    diff = dateDiff(&bday,&today);
  }
  return diff;
}

Person** insertPerson(Person** p, Person pin, int num){
  Debug_msg("Reallocating Memory for new Person\n");
  *p = realloc(*p,(num+1)*sizeof(Person));
  int i;
  for (i=0;i<num;i++){
    if ((*p+i)->days > pin.days){
      break;
    }
  }
  Debug_msg("Person's position found, shifting others to make space\n");
  for(;num>i;num--){
    *(*p+num) = *(*p+num-1);
  }
  Debug_msg("Inserting person\n");
  *(*p+i)=pin;
  DEBUG_BLK(printf("Insertion successful for entry\n");printPerson(&pin););
  return p;
}

Person* readFromFile(char* filename, int* num, int low, int high){
  FILE *fp;
  Date d;
  Person* p;
  p = NULL;
  Person ptemp;
  int n = 0;
  char name[PERSON_NAME_LENGTH];
  Debug_msg("Opening file\n");
  fp = fopen(filename,"r");
  Debug_msg("Entering Infinite loop to read entries\n");
  while(1){
    d = readSingleDate(fp, name);
    if (d.year == 0){
      break;
    }else if(validDate(&d)==0){
      Err_msg("<Skipping>Invalid Date: ");
      printDate(&d,1);
      printf(" for %s\n",name);
      continue;
    }
    ptemp.birthDate = d;
    strcpy(ptemp.name,name);
    ptemp.days = daysTillNextBDay(d);
    
    if ((ptemp.days < low) || (ptemp.days >high)){
      Debug_msg("Read entry is outside the limit\n");
      continue;
    }
    insertPerson(&p, ptemp, n);
    n++;
  }
  fclose(fp);
  *num = n;
  return p;
}


int main(int argc, char **argv){
  Debug_msg("Starting Program\n");
  Person *p;
  int num,opt,limit_high,limit_low;
  limit_high = 366;
  limit_low = 0;
  Date d;
  
  Debug_msg("Starting parsing commandline arguments\n");
  while((opt = getopt(argc,argv,":u:l:thd")) != -1){
    switch (opt){
    case 'd':
      DEBUG_BLK(printf("OPTION Current Date:\n"););
      d = getCurrentDate();
      d = convertADBS(&d, BS);
      printDate(&d, 1);
      return 0;
    case 'u':
      limit_high = atoi(optarg);
      DEBUG_BLK(printf("OPTION Upper Limit: %d\n", limit_high););
      break;
    case 'l':
      limit_low = atoi(optarg);
      DEBUG_BLK(printf("OPTION Lower Limit: %d\n", limit_low););
      break;
    case 't':
      limit_low = 0;
      limit_high = 0;
      DEBUG_BLK(printf("OPTION Today's birthdays only\n"););
      break;
    case 'h':
      Debug_msg("OPTION Help\n");
    default:
      if (opt!='h'){
      Err_msg("Invalid Option\n");
      }
      printf("Birthday reminders for Constant display\nOptions:\n"
	     "\t-u val\t upper limit of days, val=(0-366)\n"
	     "\t-l val\t lower limit of days, val=(0-366)\n"
	     "\t-t\t Show today's birthdays only\n"
	     "\t-d\t Show today's date in BS\n");
      return 0;
    }
  }
  
  fflush(stdout);
  Debug_msg("Reading the birthdays File: " BIRTHDAYS_FILE "\n");
  
  p = readFromFile(BIRTHDAYS_FILE, &num, limit_low, limit_high);
  Debug_msg("Printing the birthdays in given limits\n");
  int i;
  for(i=0;i<num;i++){
    printPerson(p+i);
  }
  Debug_msg("Freeing memory\n");
  free(p);
  Debug_msg("Exiting program\n");
  return 0;
}

