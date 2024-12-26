#ifndef UDP_PEER_H
#define UDP_PEER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <memory>
#include <vector>
#include <list>
#include <array>
#include <map>
#include "json.hpp"

///@brief Declares a UDP protocol peer which participates
///in a network communication session and exchanges data with
///it's neihbour
class UdpPeer
{

public:

    using SizeType = std::size_t;

    static constexpr SizeType BufferSize = 512;

    using BufferType = std::array<char, BufferSize>;

    using BufferList = std::list<BufferType>;

    using ErrorType = boost::system::error_code;

    using IpAddressType = boost::asio::ip::address_v4;

    using EndPointType = boost::asio::ip::udp::endpoint;

    using EndPointList = std::list<EndPointType>;

    using SocketType = boost::asio::ip::udp::socket;

    using SocketPtr = std::unique_ptr<SocketType>;

    using SocketVector = std::vector<SocketPtr>;

    struct IpAddressAndPort
    {
        ///
        std::string ip_address_;

        ///
        SizeType port_;

        ///
        IpAddressAndPort(const std::string& ip_address, SizeType port):
            ip_address_(ip_address), port_(port){}

        ///
        IpAddressAndPort(const IpAddressAndPort&) = default;

        ///
        IpAddressAndPort(IpAddressAndPort&&) = default;

        ///
        IpAddressAndPort &operator = (const IpAddressAndPort&) = default;
    };

public:

    static boost::asio::io_service io_service_;

private:

    /// forbid the default ctor
    UdpPeer() = delete;

    /// forbid the copy ctor
    UdpPeer(const UdpPeer&) = delete;

    /// forbid copy from universal reference ctor
    UdpPeer(UdpPeer &&) = delete;

    /// Opens listening sockets
    void OpenLocalSockets
    (
        const std::string& my_ip_address
    );

    /// Connects to the specified peer by sending data
    /// to it
    void SendToPeer
    (
        SizeType k,
        SizeType peer_port,
        const std::string& peer_ip_address
    );

    /// Executes when data has been sent to a peer
    void SendCompleted
    (
        SizeType msg_size,
        const ErrorType& ec,
        SizeType bytes_transferred
    );

    /// Receives data from a peer
    void ReceiveFromPeer
    (
        SizeType k,
        SizeType peer_port,
        const std::string& peer_ip_address
    );

    /// Processes the received data (via the specified socket)
    void ProcessReceive
    (
        SizeType k,
        boost::asio::const_buffer recv,
        const ErrorType& ec,
        SizeType bytes_transferred
    );

    /// Finds buffer using the given number of a local socket and
    /// the given peer port number
    char* FindReceiveBuffer
    (
        SizeType k,
        const IpAddressAndPort& remote_ep
    );

protected:

    /// Sends messages to ports within the specified range
    void SendToSpecifiedPeerPorts
    (
        SizeType k,
        const std::string peer_ip_address,
        SizeType start_port,
        SizeType n_remote_ports
    );

    /// Sends messages to all ports of the specified peer
    /// from the specified socket (socket number)
    void SendToAllPeerPorts
    (
        SizeType k,
        const std::string& peer_ip_address,
        SizeType n_remote_ports = 0
    );

    /// Sends messages to all ports of the specifed peer
    /// from all sockets
    void SendAllToAll
    (
        const std::string& peer_ip_address,
        SizeType n_remote_ports = 0
    );

    /// Receives messages from all ports of a peer
    void ReceiveFromAllPeerPorts
    (
        SizeType k,
        const std::string& peer_ip_address,
        SizeType n_remote_ports = 0
    );

    /// Receives messages from all ports of the specifed peer
    /// via all local sockets
    void ReceiveFromAll
    (
        const std::string& peer_ip_address,
        SizeType n_remote_ports = 0
    );

    /// Is called when a packet has been sent
    virtual void AfterSending(){}

    /// Is called when a packet has been received
    virtual void AfterReceiving(){}

    /// Returns the number of sent messages
    const SizeType& messages_sent() const
    {
        return messages_sent_;
    }

    /// Returns the number of received messages
    const SizeType& messages_received() const
    {
        return messages_received_;
    }

    /// Resets the number of messages sent
    void reset_messages_sent()
    {
        messages_sent_ = 0;
    }

    /// Resets the number of messages received
    void reset_messages_received()
    {
        messages_received_ = 0;
    }

    /// Returns number of local ports (sockets)
    SizeType n_local_sockets()
    {
        return N_;
    }

    /// Returns minimum port of the neighbour
    SizeType nb_port_min()
    {
        return nb_port_min_;
    }

public:

    /// Constructs a peer using:
    ///     - the given number of connections
    ///     - the given personal port range
    ///     - the given neighbour port range
    ///     - the given IP address
    UdpPeer
    (
        SizeType N,
        SizeType my_port_min,
        SizeType nb_port_min,
        const std::string& my_ip_address
    );

    /// Constructs a peer from the given JSON object
    UdpPeer
    (
        const nlohmann::json& json
    );

private:

    /// Number of ports (sockets) per peer
    const SizeType N_;

    /// Starting address of local ports range
    const SizeType my_port_min_;

    /// Starting address of a peer ports range
    const SizeType nb_port_min_;

    /// A list of listening (server) sockets
    SocketVector local_sockets_;

    /// A list of remote end points
    std::vector<std::map<IpAddressAndPort, EndPointType>> remote_endpoints_;

    /// A storage for incoming data
    std::vector<std::map<IpAddressAndPort, BufferType>> receive_buffers_;

    /// Counts the number of messages sent
    SizeType messages_sent_;

    /// Counts the number of messages received
    SizeType messages_received_;
};

#endif

//
//END-OF-FILE
//

