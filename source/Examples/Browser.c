/**
 * Netzhaut - Web Browser 
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../API/Header/Netzhaut.h"

#include <stdio.h>

void handleOutput(Nh_Output *Output_p) {
    printf(Nh_stringify(Output_p));
}

int main(int argc, char **argv_pp) 
{
#define CHECK(result) if (result != NH_SUCCESS) {return 1;}

    CHECK(Nh_init(argc, argv_pp, handleOutput))
    CHECK(Nh_start())

    while (Nh_keepRunning()) {CHECK(Nh_update())}

    CHECK(Nh_release(NH_NULL))
}

