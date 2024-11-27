#include "sokoban_base_game/base_context_factory.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
    try{
        auto context = sokoban::sbg::createContext();
    } catch (const std::exception& e){
        std::cout << "Exception: " << e.what() << '\n';
    }

   return 0;
}