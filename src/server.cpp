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

#include "server.hpp"
#include <cassert>
#include <boost/log/trivial.hpp>

namespace libcow
{
    namespace multicast
    {
        server::server(std::string multicast_ip,
                       size_t multicast_port,
                       std::string listen_ip,
                       size_t packet_size,
                       size_t movie_id,
                       size_t bitrate,
                       const char *file_uri,
                       size_t piece_size,
                       size_t protocol_version) :
                endpoint_(boost::asio::ip::address::from_string(multicast_ip), multicast_port),
                io_service_(),
                socket_(io_service_, endpoint_.protocol()),
                timer_(io_service_),
                reader_(piece_size,packet_size,file_uri),                
                movie_id_(movie_id),
                bitrate_(bitrate),
                protocol_id_(protocol_version)
        {
            // A bitrate of 0 is nonsensical.
            assert(bitrate != 0);
            assert(bitrate >= packet_size*8);

            sync_data_ = new char[pack_.sync_packet_size()];
            movie_data_ = new char[reader_.packet_size()+1];

            wait_msec_ = 1000/(bitrate/(packet_size*8));
        }

        server::~server()
        {
            delete[] sync_data_;
            delete[] movie_data_;
        }

        void server::start()
        {
            if(!reader_.exists()) {
                BOOST_LOG_TRIVIAL(fatal) << "File not found, exiting\n";
                return;
            }

            BOOST_LOG_TRIVIAL(debug) << "Wait (msec): " << wait_msec_;

            send_sync_packet();
            io_service_.run();
        }

        void server::stop()
        {
            io_service_.stop();
        }
    
        void server::handle_send(const boost::system::error_code& error)
        {
            if (!error) {
                timer_.expires_from_now(boost::posix_time::millisec(wait_msec_));
                timer_.async_wait(
                        boost::bind(&server::handle_timeout,
                                    this,
                                    boost::asio::placeholders::error));
            } else {
                BOOST_LOG_TRIVIAL(error) << error.message();
            }
        }

        void server::send_sync_packet()
        {
            utils::buffer buf(sync_data_,pack_.sync_packet_size());
            pack_.create_sync_packet(protocol_id_,movie_id_,reader_.current_piece(),buf);
            send_buffer(buf);
        }

        void server::send_movie_packet()
        {
            utils::buffer data(movie_data_,reader_.packet_size()+1);
            utils::buffer buf = reader_.next(data);
            pack_.create_movie_packet(buf);
            send_buffer(buf);
        }

        void server::send_buffer(const utils::buffer& buf)
        {
            socket_.async_send_to(
                    boost::asio::buffer(buf.data(),buf.size()),
                    endpoint_,
                    boost::bind(&server::handle_send,
                                this,
                                boost::asio::placeholders::error));
        }

        void server::handle_timeout(const boost::system::error_code& error)
        {
            if(!error){
                if(!reader_.eof()) {
                    if(pack_.need_sync()) {
                        send_sync_packet();
                    }
                    send_movie_packet();
                } else {
                    BOOST_LOG_TRIVIAL(info) << "Transmission finished";
                }
            }
        }
}

    
}
