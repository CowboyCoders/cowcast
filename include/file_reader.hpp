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
        class file_reader
        {
        public:
            file_reader(std::size_t piece_size,
                        std::size_t packet_size,
                        const char *file_uri);

            utils::buffer next(const utils::buffer& buf);

            std::size_t current_piece() const;

            bool eof() const {
                return file_.eof();
            }

            bool exists() const {
                return exists_;
            }

            int packet_size() const {
                return packet_size_;
            }
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
