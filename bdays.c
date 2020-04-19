#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "convertADBS.h"

#define BIRTHDAYS_FILE "birthdays_dummy.txt"
#define COMMENT_CHAR '#'

typedef struct{
  int days;
  char name[PERSON_NAME_LENGTH];
  Date birthDate;
} Person;

void printPerson(Person *p){
  if (p->days ==0){
    printf("---TODAY---");
  }else{
    printf("in %3d days",p->days);
  }
  printf("\t%-12s (",p->name);
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
  fseek(fp, -1, SEEK_CUR);
}

int nextLine(FILE* fp){
  char c;
  int flag = 1;
  while (flag){
    c=fgetc(fp);
    switch (c){
    case EOF:
      return 0;
    case '\n':
    case ' ':
      continue;
    case COMMENT_CHAR:
      skipThisLine(fp);
      break;
    default:
      flag=0;
    }
  }
  fseek(fp, -1, SEEK_CUR);
  return 1;
}


Date readSingleDate(FILE* fp,char* name){
  Date d;
  int flag = 0;
  char c = ' ';
  if (nextLine(fp)==0){
    d.year =  0;
    return d;
  }
  fscanf(fp,"%d-%d-%d",&d.year,&d.month,&d.day);
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
	skipThisLine(fp);
	break;
      }
      if (flag>=PERSON_NAME_LENGTH-2){
	skipThisLine(fp);
	break;
      }
      *(name+flag) = c;
      flag++;
    }
    *(name+flag-1) = '\0';
  }else{
    skipThisLine(fp);
  }
  return d;
}


int daysTillNextBDay(Date bday){
  Date today = getCurrentDate();

  today = convertADBS(&today,bday.type);
  bday.year = today.year;
  int diff = dateDiff(&bday,&today);
  if (diff < 0){
    bday.year = today.year+1;
    diff = dateDiff(&bday,&today);
  }
  return diff;
}

Person* insertPerson(Person* p, Person pin, int num){
  p = realloc(p,(num+1)*sizeof(Person));
  int i;
  for (i=0;i<num;i++){
    if ((p+i)->days > pin.days){
      break;
    }
  }
  for(;num>i;num--){
    *(p+num) = *(p+num-1);
  }
  *(p+i)=pin;
  return p;
}

Person* readFromFile(char* filename, int* num, int low, int high){
  FILE *fp;
  Date d;
  Person* p;
  Person ptemp;
  int n = 0;
  char name[PERSON_NAME_LENGTH];
  fp = fopen(filename,"r");
  while(1){
    d = readSingleDate(fp, name);
    if (d.year == 0){
      break;
    }else if(validDate(&d)==0){
      printf("<Skipping>Invalid Date: ");
      printDate(&d,1);
      printf(" for %s\n",name);
      continue;
    }
    ptemp.birthDate = d;
    strcpy(ptemp.name,name);
    ptemp.days = daysTillNextBDay(d);
    
    if ((ptemp.days < low) || (ptemp.days >high)){
      continue;
    }
    p = insertPerson(p, ptemp, n);
    n++;
  }
  fclose(fp);
  *num = n;
  return p;
}


int main(int argc, char **argv){
  Person *p;
  int num,opt,limit_high,limit_low;
  limit_high = 366;
  limit_low = 0;
  Date d;

  while((opt = getopt(argc,argv,":u:l:thd")) != -1){
    switch (opt){
    case 'd':
      d = getCurrentDate();
      d = convertADBS(&d, BS);
      printDate(&d, 1);
      return 0;
    case 'u':
      limit_high = atoi(optarg);
      break;
    case 'l':
      limit_low = atoi(optarg);
      break;
    case 't':
      limit_low = 0;
      limit_high = 0;
      break;
    case 'h':
    default:
      printf("Birthday reminders for Constant display\nOptions:\n");
      printf("\t-u val\t upper limit of days, val=(0-366)\n");
      printf("\t-l val\t lower limit of days, val=(0-366)\n");
      printf("\t-t\t Show today's birthdays only\n");
      printf("\t-d\t Show today's date in BS\n");
      return 0;
    }
  }
  fflush(stdout);
  
  p = readFromFile(BIRTHDAYS_FILE, &num, limit_low, limit_high);

  int i;
  for(i=0;i<num;i++){
    printPerson(p+i);
  }
  /* printf("%s-%d",__FILE__,__LINE__); */
  return 0;
}

