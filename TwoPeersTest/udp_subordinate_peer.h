#ifndef UDP_SUBORDINATE_PEER_H
#define UDP_SUBORDINATE_PEER_H

#include "udp_peer.h"

class SubordinatePeer : public UdpPeer
{
private:

    /// Forbid the default ctor
    SubordinatePeer() = delete;

    /// Forbid the copy ctor
    SubordinatePeer(const SubordinatePeer&) = delete;

    /// Forbid the move ctor
    SubordinatePeer(SubordinatePeer&&) = delete;

public:

    /// Constructs a new subordinate peer
    SubordinatePeer
    (
        UdpPeer::SizeType N,
        UdpPeer::SizeType my_port_min,
        UdpPeer::SizeType nb_port_min,
        const std::string& my_ip_address
    );

    /// Receive all messages from the leading peer
    void Receive
    (
        const std::string& nb_ip_address
    );

};

#endif
//
//END-OF-FILE
//
