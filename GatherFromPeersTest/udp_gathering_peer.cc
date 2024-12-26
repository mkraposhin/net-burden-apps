#include "debug_print.h"
#include "udp_gathering_peer.h"

void GatheringPeer::AfterReceiving()
{
    DebugPrint::stream << "Messages received: " << messages_received() << '\n';

    if (messages_received() >= max_messages_count_)
    {
        reset_messages_received();
        Receive(prev_nb_ip_addresses_, prev_M_);
    }
}

GatheringPeer::GatheringPeer
(
    UdpPeer::SizeType N,
    UdpPeer::SizeType my_port_min,
    UdpPeer::SizeType nb_port_min,
    const std::string &my_ip_address
)
:   UdpPeer(N, my_port_min, nb_port_min, my_ip_address),
    prev_nb_ip_addresses_(),
    prev_M_(0),
    max_messages_count_(0)
{
}

GatheringPeer::GatheringPeer
(
    const nlohmann::json &json
)
:   UdpPeer(json),
    prev_nb_ip_addresses_(),
    prev_M_(0),
    max_messages_count_(0)
{
}

void GatheringPeer::Receive
(
    const std::vector<std::string>& nb_ip_addresses,
    UdpPeer::SizeType M
)
{
    prev_nb_ip_addresses_ = nb_ip_addresses;
    prev_M_ = M;
    max_messages_count_ = nb_ip_addresses.size() *
        (M == 0 ? n_local_sockets() : M) * n_local_sockets();
    DebugPrint::stream << "max_messages_count_ = " << max_messages_count_ << '\n';
    for (const auto& nb_ip_address : nb_ip_addresses)
    {
        UdpPeer::ReceiveFromAll(nb_ip_address, M);
    }
}

//
//END-OF-FILE
//

