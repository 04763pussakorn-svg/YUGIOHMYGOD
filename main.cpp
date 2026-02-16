#include <iostream>  
#include <Windows.h>  
#include <sstream>
#include <iomanip> 

#include "display.h"
#include "class.h"

int main() {
  
    updateScreen();
    Sleep(500);
    
    cout << Class_Card(1).name;
    
    return 0;  
}  