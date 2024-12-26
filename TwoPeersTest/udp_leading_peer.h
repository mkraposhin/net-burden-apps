#ifndef UDP_LEADING_PEER_H
#define UDP_LEADING_PEER_H

#include "udp_peer.h"

class LeadingPeer : public UdpPeer
{
private:

    /// Forbid the default ctor
    LeadingPeer() = delete;

    /// Forbid the copy ctor
    LeadingPeer(const LeadingPeer&) = delete;

    /// Forbid the move ctor
    LeadingPeer(LeadingPeer&&) = delete;

public:

    /// Constructs a new leading peer
    LeadingPeer
    (
        UdpPeer::SizeType N,
        UdpPeer::SizeType my_port_min,
        UdpPeer::SizeType nb_port_min,
        const std::string& my_ip_address
    );

    /// Sends messages from all sockets to the all ports of the neighbour
    void StartSend(const std::string& nb_ip_address);
};

#endif
//
//END-OF-FILE
//
