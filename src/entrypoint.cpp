//
// Created by Bellaedris on 13/01/2026.
//

#include "Sunlight.h"
#define SOL_ALL_SAFETIES_ON 1
#include <sol.hpp>

int main()
{
    sun::Sunlight sunlight(1920, 1080);
    sunlight.Run();

    return 0;
}