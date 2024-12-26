#ifndef UDP_SENDING_PEER_H
#define UDP_SENDING_PEER_H

#include "udp_peer.h"

class SendingPeer : public UdpPeer
{
private:

    /// Forbid the default ctor
    SendingPeer() = delete;

    /// Forbid the copy ctor
    SendingPeer(const SendingPeer&) = delete;

    /// Forbid the move ctor
    SendingPeer(SendingPeer&&) = delete;

protected:

    /// Restarts the sending procedure
    void AfterSending() override;

    /// The previous IP address at which we had sent messages
    std::string prev_nb_ip_address_;

    /// The previous number of sockets of the remote peer
    UdpPeer::SizeType prev_M_;

    /// The maximum number of messages to send per 1 iteration
    UdpPeer::SizeType max_messages_count_;

    /// The amount of milliseconds to sleep before sending next bunch of messages
    UdpPeer::SizeType sleep_ms_;

public:

    /// Constructs new SendingPeer
    SendingPeer
    (
        UdpPeer::SizeType N,
        UdpPeer::SizeType my_port_min,
        UdpPeer::SizeType nb_port_min,
        const std::string& my_ip_address,
        UdpPeer::SizeType sleep_ms = 0
    );

    /// Constructs new SendingPeer from a JSON object
    SendingPeer
    (
        const nlohmann::json& json
    );

    /// Sends messages from all sockets to the all ports of the neighbour
    void StartSend(const std::string& nb_ip_address, UdpPeer::SizeType M);
};

#endif
//
//END-OF-FILE
//
