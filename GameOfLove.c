#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<stdbool.h>

int * getFirstUnoccupiedSpace(void *u, int h, int w, int guyH, int guyW);
bool checkForMeeting(void *u, int h, int w);

#define for_x for (int x = 0; x < w; x++)
#define for_y for (int y = 0; y < h; y++)
#define for_xy for_x for_y

//Globals for keeping array sizes handy
int MAX_H;
int MAX_W;
int DELAY;

void show(void *u, int w, int h)
{
	int (*univ)[w] = u;
	printf("\033[H");
	for_y {
		for_x {
            if(univ[y][x] == 2){
                printf("\033[41m  \033[m");
            }else if(univ[y][x] == 3){
                printf("\033[44m  \033[m");//\u001b[34m
            }else
                printf(univ[y][x] ? "\033[07m  \033[m" : "  ");
        }
		printf("\033[E");
	}
	fflush(stdout);
}

void evolve(void *u, int w, int h)
{
	unsigned (*univ)[w] = u;
	unsigned new[h][w];
  unsigned specialHRed;
  unsigned specialWRed;
  unsigned specialHBlue;
  unsigned specialWBlue;

	for_y for_x
  {
		int n = 0;
		for (int y1 = y - 1; y1 <= y + 1; y1++)
			for (int x1 = x - 1; x1 <= x + 1; x1++)
				if (univ[(y1 + h) % h][(x1 + w) % w])
					n++;

		if (univ[y][x]) n--;
		new[y][x] = (n == 3 || (n == 2 && univ[y][x]));

    if (univ[y][x] == 2)
    {
        specialHRed = y;
        specialWRed = x;
    }else if(univ[y][x] == 3){
        specialHBlue = y;
        specialWBlue = x;
    }

	}
	for_y for_x univ[y][x] = new[y][x];

    //Make the lonley guy do stuff
    //Get the number of neighbors
    int neighbors = 0;
    for (int y1 = specialHRed - 1; y1 <= specialHRed + 1; y1++)
        for (int x1 = specialWRed - 1; x1 <= specialWRed + 1; x1++)
            if (univ[(y1 + h) % h][(x1 + w) % w]){
                neighbors++;
            }


    //If he is lonley or has more then 1 neighbor
    //move a random direction
    //if(neighbors == 0 || neighbors >= 2)
    //{
        int *newLocation = getFirstUnoccupiedSpace(univ, h, w, specialHRed, specialWRed);
        int newspecialHRed = *(newLocation + 0);
        int newspecialWRed = *(newLocation + 1);
        //printf("Red Guy Location: %d, %d\n", newspecialHRed, newspecialWRed);
        univ[newspecialHRed][newspecialWRed] = 2;
    //}
    //If he has 1 buddy eat him
    //else if(neighbors == 1)
    //{
    //    bool GuyEaten = false;
    //    while(!GuyEaten)
    //    {

//        }
    //}
    //If he has or more buddies run away

    //Blue Guy
    neighbors = 0;
    for (int y1 = specialHBlue - 1; y1 <= specialHBlue + 1; y1++)
        for (int x1 = specialWBlue - 1; x1 <= specialWBlue + 1; x1++)
            if (univ[(y1 + h) % h][(x1 + w) % w]){
                neighbors++;
            }
    //if(neighbors == 0 || neighbors >= 2)
    //{
        newLocation = getFirstUnoccupiedSpace(univ, h, w, specialHBlue, specialWBlue);
        int newspecialHBlue = *(newLocation + 0);
        int newspecialWBlue = *(newLocation + 1);
        //printf("Blue Guy Location: %d, %d\n", newspecialHBlue, newspecialWBlue);
        univ[newspecialHBlue][newspecialWBlue] = 3;
    //}

}

bool checkForMeeting(void *u, int h, int w)
{
    unsigned (*univ)[w] = u;
    unsigned new[h][w];
    int redW;
    int redH;
    for_y for_x
    {
      int n = 0;
      for (int y1 = y - 1; y1 <= y + 1; y1++)
        for (int x1 = x - 1; x1 <= x + 1; x1++)
          if (univ[(y1 + h) % h][(x1 + w) % w])
            n++;

      if (univ[y][x]) n--;
      new[y][x] = (n == 3 || (n == 2 && univ[y][x]));

      if (univ[y][x] == 2)
      {
          redH = y;
          redW = x;
      }
    }

    int HMinus = redH-1;
    if(HMinus < 0)
      HMinus = 0;

    int HPlus = redH +1;
    if(HPlus > MAX_H)
      HPlus = MAX_H;

    int WMinus = redW-1;
    if(WMinus < 0)
      WMinus = 0;

    int WPlus = redW +1;
    if(WPlus > MAX_H)
      WPlus = MAX_H;
    //check all 8 directions for the other guyH
    if(  univ[HMinus][WMinus] == 3
      || univ[HMinus][redW] == 3
      || univ[HMinus][WPlus] == 3
      || univ[redH][WMinus] == 3
      || univ[redH][WPlus] == 3
      || univ[HPlus][WMinus] == 3
      || univ[HPlus][redW] == 3
      || univ[HPlus][WPlus] == 3)
    {
      printf("RedGuy [%d, %d]\n", redH, redW);
      printf("[%d, %d] == %d\n", HMinus, WMinus, univ[HMinus][WMinus]);
      printf("[%d, %d] == %d\n", HMinus, redW, univ[HMinus][redW]);
      printf("[%d, %d] == %d\n", HMinus, WPlus, univ[HMinus][WPlus]);
      printf("[%d, %d] == %d\n", redH, WMinus, univ[redH][WMinus]);
      printf("[%d, %d] == %d\n", redH, WPlus, univ[redH][WPlus]);
      printf("[%d, %d] == %d\n", HPlus, WMinus, univ[HPlus][WMinus]);
      printf("[%d, %d] == %d\n", HPlus, redW, univ[HPlus][redW]);
      printf("[%d, %d] == %d\n", HPlus, WPlus, univ[HPlus][WPlus]);
      return true;
    }else
      return false;

}

//Takes the universe and the location of the guy
//returns a 2d array of the first unoccupied spot it finds
int * getFirstUnoccupiedSpace(void *u, int h, int w, int guyH, int guyW)
{
    static int unoccupiedSpace[2];
    unsigned (*univ)[w] = u;
    //printf("Cool Guy Current Location: %d, %d\n", guyH, guyW);
    int dir = rand() % 8;
    //printf("Cool Guy Trying direction: %d\n", dir);
    unoccupiedSpace[0] = guyH;
    unoccupiedSpace[1] = guyW;
    int tries = 1;
    //0 = y-1 x-1
    //1 = y-1 x
    //2 = y-1 x+1
    //3 = y x-1
    //4 = y x+1
    //5 = y+1 x-1
    //6 = y+1 x
    //7 = y+1 x+1
    bool MovedToOpenSpace = false;
    while(!MovedToOpenSpace && tries < 16)
    {
        //calc Y
        if(dir < 3)
        {
            if((guyH - 1) >= 0)
                unoccupiedSpace[0] = guyH - 1;

        }else if( dir > 4 )
        {
            if((guyH + 1) < MAX_H)
                unoccupiedSpace[0] = guyH + 1;
        }

        //calc X
        if(dir == 0 || dir == 3 || dir == 5)
        {
            if((guyW - 1) >= 0)
                unoccupiedSpace[1] = guyW - 1;
        }else if( dir == 2 || dir == 4 || dir == 7 )
        {
            if((guyW + 1) < MAX_W)
                unoccupiedSpace[1] = guyW + 1;
        }
        //Make sure the space he is moving to is open
        //printf("Is [%d, %d] open? %d \n", unoccupiedSpace[0], unoccupiedSpace[1], (univ[unoccupiedSpace[0]][unoccupiedSpace[1]] == 0));
        if(univ[unoccupiedSpace[0]][unoccupiedSpace[1]] == 0)
            MovedToOpenSpace = true;
        else{
          dir = rand() % 8; //try a new direction
          tries ++;
        }
    }//while(...)

    return unoccupiedSpace;
}

void game(int w, int h)
{
	unsigned univ[h][w];
	for_xy univ[y][x] = rand() < RAND_MAX / 10 ? 1 : 0;
    int specialX = rand() % w;
    int specialY = rand() % h;

    univ[specialY][specialX] = 2;

    specialX = rand() % w;
    specialY = rand() % h;

    univ[specialY][specialX] = 3;

	while (1) {
		show(univ, w, h);
    if(checkForMeeting(univ, h, w)){
      printf("\n********** True Love! <3 ************\n");
      break;
    };
		evolve(univ, w, h);
		usleep(DELAY);
	}
}

int main(int c, char **v)
{
	int w = 0, h = 0;
	if (c > 1) w = atoi(v[1]);
	if (c > 2) h = atoi(v[2]);
  if (c > 3) DELAY = atoi(v[3]);
	if (w <= 0) w = 30;
	if (h <= 0) h = 30;
    MAX_H = h;
    MAX_W = w;
	game(w, h);
}
