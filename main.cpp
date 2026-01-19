#include "UIManager.h"
#include <iostream>

int main()
{
    try
    {
        UIManager ui;
        ui.pokreni();
    }
    catch (const std::exception& e)
    {
        std::cout << "Fatalna greska: " << e.what() << "\n";
        return 1;
    }

    return 0;
}




