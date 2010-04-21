#include "server.hpp"

int main()
{
    std::cout << "CREATING MULTICAST SERVER" << std::endl;
    libcow::multicast::server server("/home/rednax/downloads/big_buck_bunny_480p_h264.mov",
                                     1,
                                     "127.0.0.1", 1234,
                                     1024*256,
                                     0,
                                     1);
    std::cout << "STARTING MULTICAST SERVER" << std::endl;
    server.start();
    std::cout << "GOOBYE!" << std::endl;
    return 0;
}
