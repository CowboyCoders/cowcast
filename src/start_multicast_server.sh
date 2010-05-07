#!/bin/bash

# multicast parameters
multicast_ip_address=239.255.0.1
multicast_port=12345
multicast_listen_address=0.0.0.0
multicast_packet_size=1024

# movie parameters
movie_id=1 # The id of the movie as specified in the program_table.xml
movie_bitrate=10000 # Must be at least 8*multicast_packet_size
file_path=../../on_demand_server/docs/
bittorrent_packet_size=65536 # This must be exactly the samee as in the torrent file

retcode=`./multicast_server $multicast_ip_address $multicast_port $multicast_listen_address $multicast_packet_size $movie_id $movie_bitrate $file_path $bittorrent_packet_size`
echo $retcode


