#include <iostream>
#include "../headers/common.h"
#include "../headers/utils.h"

void ShowJoined(Array& a, string sep)
{
    if (a.size == 0)
        cout << "-> (empty)" << endl;
    else
        cout << "-> " << JoinArray(a, sep) << endl;
}
