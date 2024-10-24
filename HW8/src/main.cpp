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
#include <atomic>
#include <thread>
#include <chrono>

std::atomic_bool taskFinished {false};

void onTaskFinished(){
    std::cout << "task finished (thread id=" << std::this_thread::get_id() << ")\n";
    taskFinished.store(true);
}

int main(int argc, char* argv[])
{
    std::cout << "Starting (thread id=" << std::this_thread::get_id() << ")\n";

    FinderTask t;

    using Group = std::vector<std::filesystem::path>;
    std::unordered_map<StringVector, Group, StringVectorHasher> map;
    FileFinder      ff;

    DuplicateFinder df;
    df.taskFinished.connect(onTaskFinished);

    FinderTask      task;
    task.recursiveSearch = true;
    task.targets.push_back("C:\\Boo\\Temp");
    task.targets.push_back("C:\\Boo\\Wallpapers");
    task.targets.push_back("C:\\Boo\\Транспорт");
    df.findDuplicates(task); 

    int c = 0;
    while(!taskFinished){
        if (c>=10){
            std::cout << "Waiting (thread id=" << std::this_thread::get_id() << ")\n";
            c = 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ++c;

    }

    std::cout << "Exiting programm (thread id=" << std::this_thread::get_id() << ")\n";

    return 0;
}
