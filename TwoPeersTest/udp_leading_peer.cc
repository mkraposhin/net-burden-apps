#include "udp_leading_peer.h"

LeadingPeer::LeadingPeer
(
    UdpPeer::SizeType N,
    UdpPeer::SizeType my_port_min,
    UdpPeer::SizeType nb_port_min,
    const std::string &my_ip_address
)
: UdpPeer(N, my_port_min, nb_port_min, my_ip_address)
{
}

void LeadingPeer::StartSend(const std::string& nb_ip_address)
{
    UdpPeer::SendAllToAll(nb_ip_address);
}

//
//END-OF-FILE
//

