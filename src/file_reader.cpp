#include "file_reader.hpp"
#include <iostream>

namespace libcow
{
    namespace multicast
    {
        file_reader::file_reader(std::size_t piece_size,
                                 std::size_t packet_size,
                                 const char *file_uri) :
                            piece_size_(piece_size),
                            packet_size_(packet_size),
                            current_piece_(0),
                            space_left_(piece_size)
        {
            file_.open(file_uri,std::ifstream::in | std::ifstream::binary);
            exists_ = file_.good();
        }

        utils::buffer file_reader::next(const utils::buffer& buf)
        {
            if(!eof())
            {
                int read_bytes;
                int bytes_to_read = packet_size_ - 1;
                if((space_left_ - bytes_to_read) > 0) {
                    file_.read(buf.data()+1,bytes_to_read);
                    read_bytes = file_.gcount();

                    space_left_ -= bytes_to_read;
                } else {
                    file_.read(buf.data()+1,space_left_);
                    read_bytes = file_.gcount();

                    space_left_ = piece_size_;
                    ++current_piece_;
                }
                return utils::buffer(buf.data(), read_bytes+1);
            }
            // throw exception?
        }

        std::size_t file_reader::current_piece() const
        {
            return current_piece_;
        }
    }

}
