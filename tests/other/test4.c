int i = 1, b = 6;

i = -i- -b---i+i+-i- -i;

if (i == 5) {
  printf("yes");
}

if (b == 5) {
  printf("yes");
}

for (int b = 3; b < 4; b++)
  for (;;)
    break;

if (b == 5) {
  printf("yes");
}

for (;;) {
  if (i == 5) {
    if(b == 5) {
        int i = 0;
	while(i < 3) {
	   i++,b;
	   break;
	}
    }
    
    if(i == 5) {
	printf("ffff");
	break;
    }

    printf("fff");
  }
}
