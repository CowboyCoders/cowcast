/*
Copyright 2010 CowboyCoders. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list
      of conditions and the following disclaimer in the documentation and/or other materials
      provided with the distribution.

THIS SOFTWARE IS PROVIDED BY COWBOYCODERS ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL COWBOYCODERS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those of the
authors and should not be interpreted as representing official policies, either expressed
or implied, of CowboyCoders.
*/

#include <iostream>
#include <boost/log/trivial.hpp>
#include "server.hpp"

void fail_with_error(const std::string& msg, int status) {
    std::cerr << msg << "\n";
    exit(status);
}

/***
 * Multicast server entry point.
 */
int main(int argc, char* argv[]) {
    if(argc < 9) {
        std::string usage = "Usage: multicast_server "
                            "<multicast_ip_address> "
                            "<multicast_port> "
                            "<multicast_listen_address> "
                            "<multicast_packet_size> "
                            "<movie_id> "
                            "<movie_bitrate> "
                            "<file_path> "
                            "<bittorrent_piece_size> ";
        fail_with_error(usage,1);
    }

    std::string ip(argv[1]);
    int port = atoi(argv[2]);
    std::string listen_ip(argv[3]);
    int packet_size(atoi(argv[4]));
    int id = atoi(argv[5]);
    int bitrate = atoi(argv[6]);
    std::string file_path(argv[7]);
    int piece_size = atoi(argv[8]);
    size_t protocol_version = 1;

    if(id < 0) {
        fail_with_error("Error: Invalid, negative id.", 1);
    }

    if(packet_size <= 0) {
        fail_with_error("Error: multicast packet size must be larger than zero",1);
    }

    if(piece_size <= 0) {
        fail_with_error("Error: bittorrent piece size must be larger than zero",1);
    }

    if(bitrate <= 0) {
        fail_with_error("Error: bitrate must be non-zero and positive", 1);
    }
    
    if(port <= 0) {
        fail_with_error("Invalid port", 1);
    }

    libcow::multicast::server s(ip,
                                static_cast<size_t>(port),
                                listen_ip,
                                static_cast<size_t>(packet_size),
                                static_cast<size_t>(id),
                                static_cast<size_t>(bitrate),
                                file_path.c_str(),
                                static_cast<size_t>(piece_size),
                                protocol_version);

    BOOST_LOG_TRIVIAL(info) << "Starting transmission of " << file_path << " [" << id << "]";
    s.start();
    BOOST_LOG_TRIVIAL(info) << "Ended transmission of " << file_path << " [" << id << "]";

    return 0;
}
