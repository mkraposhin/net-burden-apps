#include <iostream>
#include <fstream>
#include "udp_sending_peer.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Local IP address has not been specified" << '\n'
                  << "Usage: ./sending_peer <local IP>" << std::endl;
        return -1;
    }
    const std::string peer_ip (argv[1]);
    std::ifstream json_file ("settings.json");
    nlohmann::json json_settings;
    json_file >> json_settings;
    auto it_settings = json_settings.find("sending_peers");
    if (it_settings == json_settings.end())
    {
        std::cout << "sending_peers section has not been found"
                  << ", exiting" << std::endl;
        return -1;
    }
    auto it_peer_settings = it_settings->find(peer_ip);
    if (it_peer_settings == it_settings->end())
    {
        std::cout << "can\'t find section for peer "<< peer_ip
                  << ", exiting" << std::endl;
        return -1;
    }
    nlohmann::json peer_settings (*it_peer_settings);
    std::cout << "Input settings: " << '\n'
              << std::setw(4) << peer_settings << '\n';

    const UdpPeer::SizeType M = peer_settings["no_remote_sockets"];
    const std::string nb_ip_address
    {
        peer_settings["nb_ip_addresses"].template get<std::string>()
    };

    SendingPeer sending_peer
    (
        peer_settings
    );
    sending_peer.StartSend(nb_ip_address, M);
    SendingPeer::io_service_.run();
    return 0;
}


