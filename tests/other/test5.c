int a = 99, b=99, c; c = 44;

//a = 99; b = 99; c = 44;


if (c == 1) b = 1;
else b = 99;

/*
b = 99;
*/

if (c == 2) b = 1;
else if(c == 3) b = 2;
else if(c == 4) b = 3;
else if(c != 5) b = 99;
/*
b=99;
 */

while(c < 49) c++;
while(c++ < 54);
/*
c = 55;
*/

do {
  ;
  ;
  ;
  for(int a = 1; a < 2; a++) {
    for(int b = 1; b < 3; b++);
  }

  for(int a = 1; a < 2; a++) {
    for(int b = 1; b < 3; b++)for(int c = 1; c < 8; c++);
    break;
  }

  if(a == 2)
    break;
}while(c++ < 65);;

/*
c = 66;
*/

do for(;;)break;
while(c++ < 66);

/*
c = 67;
*/

if (c == 67) //fffff
  printf("finish");
