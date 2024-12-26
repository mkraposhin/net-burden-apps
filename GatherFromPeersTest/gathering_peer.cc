#include <iostream>
#include <fstream>
#include "udp_gathering_peer.h"

int main(int argc, char *argv[])
{
    std::ifstream json_file ("settings.json");
    nlohmann::json json_settings;
    json_file >> json_settings;
    auto it_settings = json_settings.find("gathering_peer");
    if (it_settings == json_settings.end())
    {
        std::cout << "gathering_peer section has not been found"
                  << ", exiting" << std::endl;
        return -1;
    }
    nlohmann::json peer_settings (*it_settings);
    std::cout << "Input settings: " << '\n'
              << std::setw(4) << peer_settings << '\n';
    const UdpPeer::SizeType M = peer_settings["no_remote_sockets"];
    const std::vector<std::string> nb_ip_addresses
    {
        peer_settings["nb_ip_addresses"].template get<std::vector<std::string>>()
    };

    GatheringPeer gathering_peer(peer_settings);
    gathering_peer.Receive(nb_ip_addresses, M);
    GatheringPeer::io_service_.run();
    return 0;
}

