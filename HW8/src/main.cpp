#include "duplicate_finder/digester.h"
#include "duplicate_finder/finder_task.h"
#include "duplicate_finder/string_vector_hasher.h"
#include "duplicate_finder/duplicate_finder.h"
#include "duplicate_finder/file_finder.h"

#include <boost/program_options.hpp>
#include <boost/version.hpp>

#include <cryptopp/cryptlib.h>
#include <cryptopp/sha3.h>
#include <cryptopp/hex.h>

#include <iostream>

int main(int argc, char* argv[])
{
    FinderTask t;
    
    using Group = std::vector<std::filesystem::path>;
    std::unordered_map<StringVector, Group, StringVectorHasher> map;

    DuplicateFinder df;
    FileFinder ff;

    return 0;
}
