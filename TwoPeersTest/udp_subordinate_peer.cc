#include "udp_subordinate_peer.h"

SubordinatePeer::SubordinatePeer
(
    UdpPeer::SizeType N,
    UdpPeer::SizeType my_port_min,
    UdpPeer::SizeType nb_port_min,
    const std::string &my_ip_address
)
: UdpPeer(N, my_port_min, nb_port_min, my_ip_address)
{
}

void SubordinatePeer::Receive(const std::string& nb_ip_address)
{
    UdpPeer::ReceiveFromAll(nb_ip_address);
}

//
//END-OF-FILE
//

