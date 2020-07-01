#include <iostream>
#include <ctime>
#include <cstdlib>
#include "PlaneGame.h"

int main()
{
    srand(time(0)); //dodajemy losowosc

    PlaneGame gra;
    while (gra.ifWorks())
    {
        gra.update();
        gra.render();
    }
}

