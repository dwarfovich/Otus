#include "bulker_sl/async.h"
#include "bulker_net/server.h"


#include <iostream>
#include <functional>
#include <charconv>
#include <sstream>
#include <cstring>

int main(int argc, char* argv[])
{
    std::size_t staticBlockSize = 1;
    if (argc == 2) {
        auto [ptr, errorCode] = std::from_chars(argv[1], argv[1] + std::strlen(argv[1]), staticBlockSize);
        if (errorCode != std::errc {}) {
            std::cout << "Wrong arguments\n";
            return -1;
        }
    }

    async::ContextDispatcher cd;

    auto h  = async::connect(staticBlockSize, cd);
    auto h2 = async::connect(staticBlockSize, cd);
    async::receive(h, "cmd1\ncmd2\ncmd3", 1, cd);
    async::receive(h, "1", 1, cd);
    async::receive(h2, "1\n", 2, cd);
    async::receive(h, "2\n3\n4\n5\n6\n{\na\n", 15, cd);
    async::receive(h, "b\nc\nd\n}\n89\n", 11, cd);
    async::disconnect(h, cd);
    async::disconnect(h2, cd);

    return 0;
}
