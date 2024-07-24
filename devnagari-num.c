#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
  char c;
  while ((c = getchar()) != EOF) {
    switch (c){
    case '0':
      printf("०");
      break;
    case '1':
      printf("१");
      break;
    case '2':
      printf("२");
      break;
    case '3':
      printf("३");
      break;
    case '4':
      printf("४");
      break;
    case '5':
      printf("५");
      break;
    case '6':
      printf("६");
      break;
    case '7':
      printf("७");
      break;
    case '8':
      printf("८");
      break;
    case '9':
      printf("९");
      break;
    default:
      printf("%c", c);
    }
  }
  return 0;
}

