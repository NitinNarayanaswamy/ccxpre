#pragma once

#define PRINT(message) std::cout << message << std::endl
#ifndef NDEBUG
    #define PRINT_DEBUG(message) std::cout << "[ DEBUG ] " << message << std::endl
#else
    #define PRINT_DEBUG(message) 
#endif
#define PRINT_INFO(message) std::cout << "[ INFO ] " << message << std::endl
#define PRINT_WARNING(message) std::cout << "[ WARNING ] " << message << std::endl
#define PRINT_ERROR(message) std::cout << "[ ERROR ] " << message << std::endl
#define PRINT_NEW_LINE std::cout << std::endl