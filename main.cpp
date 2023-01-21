#include <string.h>
#include <stdint.h>
#include <cstddef>


#include <vector>
#include <unordered_map>

#include "pool_allocator_cpp.h"
#include "pool_allocator.h"

#include "CppUTest/CommandLineTestRunner.h"

/*!
 * Realization of allocator in pool_allocator.c. See main docs in pool_allocator.h.
 */


void CppWrapperDebug()
{

    pa_init();

    std::vector<int, PoolAllocatorCPP<int>> vec;

    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);


    std::basic_string<char, std::char_traits<char>, PoolAllocatorCPP<char>> str = "assdferfgergtrgtrg";



    std::unordered_map<std::string, int, std::hash<std::string>, std::equal_to<std::string>, PoolAllocatorCPP<std::pair<const std::string, int>>> map;

    map.insert({"aaa", 1});
    map.insert({"vv", 1});
    map.insert({"aadda", 33});
    map.insert({"aarfea", 44});
    map.insert({"aaera", 1});

}


int main(int argc, char* argv[ ])
{
    CppWrapperDebug(); // example of usage allocator with cpp standart library containers.
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
