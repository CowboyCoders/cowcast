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

#ifndef ___file_reader_hpp___
#define ___file_reader_hpp___

#include <string>
#include <fstream>

#include "cow/utils/buffer.hpp"

namespace libcow
{
    namespace multicast
    {
       /**
        * This class is used by the libcow::multicast::server in order to read pieces
        * from disk and send them via multicast. The file reader will automatically
        * create multicast packets that the libcow::multicast::server can use.
        */
        class file_reader
        {
        public:
           /**
            * Creates a new libcow::multicast::file_reader that will read data
            * from the specified file.
            * @param piece_size The size of each BitTorrent piece for the file.
            * @param packet_size The size of each packet to send via multicast.
            * @param file_uri A pointer to the file to read from.
            */
            file_reader(std::size_t piece_size,
                        std::size_t packet_size,
                        const char *file_uri);
           /**
            * Returns the next packet to send via multicast.
            * Since we need 1 byte of free space in the buffer to
            * store protocal data, a new buffer is returned.
            * @param buf The buffer to fill with the next packet.
            * @return The next packet in a buffer.
            */
            utils::buffer next(const utils::buffer& buf);

           /**
            * Returns the current BitTorrent piece that the file reader is
            * reading from.
            * @return The current piece id.
            */
            std::size_t current_piece() const;

           /**
            * Returns whether or not we have reached the end of the file.
            * @return True if we have reached the end of the file, otherwise false.
            */
            bool eof() const {
                return file_.eof();
            }

           /**
            * Returns whether or not the file reader could open the file.
            * @return True if the file could be opened, otherwise false.
            */
            bool exists() const {
                return exists_;
            }

           /**
            * Returns the packet size that this file reader uses.
            * @return The packet size.
            */
            int packet_size() const {
                return packet_size_;
            }
            
           /**
            * Returns the piece size that this file reader uses.
            * @return The piece size.
            */
            size_t piece_size() const {
                return piece_size_;
            }
        private:
            std::size_t piece_size_;
            int packet_size_;
            std::size_t current_piece_;
            int space_left_;
            std::ifstream file_;
            bool exists_;
        };
    }
}

#endif // ___file_reader_hpp___
