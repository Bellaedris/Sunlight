//
// Created by Bellaedris on 13/01/2026.
//

#include "Sunlight.h"
#define SOL_ALL_SAFETIES_ON 1
#include <sol.hpp>

#include "Lumiere/EngineCfg.h"

int main(int argc, char** argv)
{
    std::filesystem::path exePath = std::filesystem::weakly_canonical(argv[0]);
    std::filesystem::path exeDir = exePath.parent_path();

    lum::cfg::Init(exeDir);
    sun::Sunlight sunlight(1920, 1080);
    sunlight.Run();

    return 0;
}
