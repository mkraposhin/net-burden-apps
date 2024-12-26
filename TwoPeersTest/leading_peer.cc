#include <iostream>
#include "udp_leading_peer.h"

int main(int argc, char *argv[])
{
    const std::string my_ip_address = "127.0.0.1";
    const std::string nb_ip_address = "127.0.0.1";
    const UdpPeer::SizeType N = 10;
    const UdpPeer::SizeType my_port_min = 20000;
    const UdpPeer::SizeType nb_port_min = 30000;

    LeadingPeer leading_peer(N, my_port_min, nb_port_min, my_ip_address);
    leading_peer.StartSend(nb_ip_address);
    LeadingPeer::io_service_.run();
    return 0;
}

