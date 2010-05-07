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

#ifndef ___multicast_server_hpp___
#define ___multicast_server_hpp___

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"

#include "packetizer.hpp"
#include "file_reader.hpp"
#include "cow/utils/buffer.hpp"

namespace libcow
{
    namespace multicast
    {
        class server
        {
        public:
            /**
             * Constructos a multicast server which broadcast movie data
             * in realtime from a specified file.
             *
             * @param multicast_ip Where the data will be broadcasted to
             * @param multicast_port The port for the multicast connection
             * @param listen_ip ? 
             * @param packet_size The size of a multicast packet (in KB)
             * @param movie_id The unique id for the movie
             * @param bitrate The bitrate for the movie (in kbit/s)
             * @param file_uri The path to the file which should be broadcasted
             * @param piece_size The size of a BitTorrent for the given file (in KB)
             * @param protocol_version The protocol version which should used
             * @param sync_frequence The frequency for sending sync packets
             *
             * @author Erik Helin & Felix Rios
             * @version 1.0
             */
            server(std::string multicast_ip,
                           size_t multicast_port,
                           std::string listen_ip,
                           size_t packet_size,
                           size_t movie_id,
                           size_t bitrate,
                           const char *file_uri,
                           size_t piece_size,
                           size_t protocol_version,
                           size_t sync_frequencey);

            /**
             * Frees the allocated data
             */
            ~server();

            /**
             * Starts the multicast server. The first package being sent is a sync package,
             * and then a sync package is being sent every 128th packet.
             */
            void start();

            /**
             * Stops the multicast server.
             */
            void stop();

        private:
            void handle_send(const boost::system::error_code&);
            void handle_timeout(const boost::system::error_code&);
            void send_sync_packet();
            void send_movie_packet();
            void send_buffer(const utils::buffer& buf);

            boost::asio::ip::udp::endpoint endpoint_;
            boost::asio::io_service io_service_;
            boost::asio::ip::udp::socket socket_;
            boost::asio::deadline_timer timer_;

            file_reader reader_;
            packetizer pack_;
            char *sync_data_;
            char *movie_data_;
            const std::size_t movie_id_;

            std::size_t bitrate_;
            std::size_t protocol_id_;
            std::size_t wait_msec_;
        };
    }
}

#endif // ___multicast_server_hpp___
