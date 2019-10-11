#include <stdio.h>
#include <stdlib.h>

#include "ui.h"

int main( int argc, char** argv )
{
    initUI();
    runApp( argc, argv );
    return 0;
}
