#pragma once
#include <iostream>

#ifdef _DEBUG
#define DEBUG_LOG(text) std::cout << "DEBUG: " << text << '\n'
#define DEBUG_ERR(text) std::cerr << "ERR: " << text << '\n'
#else
#define DEBUG_LOG(text)
#define DEBUG_ERR(text)
#endif //DEBUG