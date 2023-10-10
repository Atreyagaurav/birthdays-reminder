#include <stdio.h>

#include "convertADBS.h"



int main(int argc, char *argv[])
{
  if (argc < 2){
    printf("Usage: %s YYYY-MM-DD {A|B}\n\n"
	   "YYYY-MM-DD is date in that format.\n"
	   "{A|B} AD or BS, automatically assumes conversion to another."
	   " Defaults to AD.\n\n"
	   "Pass - as argument if you want current date in BS.\n", argv[0]);
    return 0;
  }

  Date input_date, converted_date;
  enum calendar_system convert_to = BS;
  if (argv[1][0] == '-'){
    input_date = getCurrentDate();
  }else{
    sscanf(argv[1], "%d-%d-%d", &input_date.year, &input_date.month, &input_date.day);
    if (argc < 3 || argv[2][0] == 'A'){
      input_date.type = AD;
      convert_to = BS;
    }else if (argv[2][0] == 'B'){
      input_date.type = BS;
      convert_to = AD;
    }else{
      printf("Wrong Calender system.\n");
      return 1;
    }
  }
  
  if (! validDate(&input_date)){
    fprintf(stderr, "Sorry the Date is Invalid, cannot perform conversion.\n");
    return 1;
  }

  converted_date = convertADBS(&input_date, convert_to);
  printDate(&converted_date, 1);
  printf("\n");
  return 0;
}
