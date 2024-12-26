#ifndef UDP_GATHERING_PEER_H
#define UDP_GATHERING_PEER_H

#include "udp_peer.h"

class GatheringPeer : public UdpPeer
{
private:

    /// Forbid the default ctor
    GatheringPeer() = delete;

    /// Forbid the copy ctor
    GatheringPeer(const GatheringPeer&) = delete;

    /// Forbid the move ctor
    GatheringPeer(GatheringPeer&&) = delete;

protected:

    /// Is used to restart gathering loop
    void AfterReceiving() override;

    /// List of neighbours IP addresses from the previous iteration
    std::vector<std::string> prev_nb_ip_addresses_;

    /// Previous number of ports on the connected peers
    UdpPeer::SizeType prev_M_;

    /// Total number of messages per iteration
    UdpPeer::SizeType max_messages_count_;

public:

    /// Constructs new gathering peer
    GatheringPeer
    (
        UdpPeer::SizeType N,
        UdpPeer::SizeType my_port_min,
        UdpPeer::SizeType nb_port_min,
        const std::string& my_ip_address
    );

    /// Constructs new gathering peer from a JSON object
    GatheringPeer
    (
        const nlohmann::json& json
    );

    /// Sequentially receives all messages from the sending peers
    void Receive
    (
        const std::vector<std::string>& nb_ip_addresses,
        UdpPeer::SizeType M = 0
    );

};

#endif
//
//END-OF-FILE
//
