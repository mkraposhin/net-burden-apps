#include "debug_print.h"
#include "udp_sending_peer.h"
#include <chrono>
#include <thread>

void SendingPeer::AfterSending()
{
    if (messages_sent() >= max_messages_count_)
    {
        reset_messages_sent();
        if (sleep_ms_ > 0)
        {
            std::chrono::duration<UdpPeer::SizeType,std::milli> sleep_time
            (
                sleep_ms_
            );
            DebugPrint::stream<< "sleeping for " 
                << sleep_ms_ << "ms before sending the next bunch" << '\n';
            std::this_thread::sleep_for(sleep_time);
            StartSend(prev_nb_ip_address_, prev_M_);
        }
    }
}

SendingPeer::SendingPeer
(
    UdpPeer::SizeType N,
    UdpPeer::SizeType my_port_min,
    UdpPeer::SizeType nb_port_min,
    const std::string &my_ip_address,
    UdpPeer::SizeType sleep_ms
)
:   UdpPeer(N, my_port_min, nb_port_min, my_ip_address),
    prev_nb_ip_address_(),
    prev_M_(0),
    max_messages_count_(0),
    sleep_ms_(sleep_ms)
{
}

SendingPeer::SendingPeer
(
    const nlohmann::json& json
)
:   UdpPeer(json),
    prev_nb_ip_address_(),
    prev_M_(0),
    max_messages_count_(0),
    sleep_ms_(json["sleep_ms"])
{
}

void SendingPeer::StartSend
(
    const std::string& nb_ip_address,
    UdpPeer::SizeType M
)
{
    prev_nb_ip_address_ = nb_ip_address;
    prev_M_ = M;
    max_messages_count_ = (M == 0 ? n_local_sockets() : M) * n_local_sockets();
    UdpPeer::SendAllToAll(nb_ip_address, M);
}

//
//END-OF-FILE
//

