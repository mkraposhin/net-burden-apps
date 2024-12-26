#include "debug_print.h"
#include "udp_peer.h"
#include <iostream>

boost::asio::io_service UdpPeer::io_service_{};

bool operator < (const UdpPeer::IpAddressAndPort &lhs, const UdpPeer::IpAddressAndPort &rhs)
{
    return (lhs.ip_address_ < rhs.ip_address_) && (lhs.port_ < rhs.port_);
}

UdpPeer::UdpPeer
(
    SizeType N,
    SizeType my_port_min,
    SizeType nb_port_min,
    const std::string& peer_ip_address
)
:
    N_(N),
    my_port_min_(my_port_min),
    nb_port_min_(nb_port_min),
    messages_sent_(0),
    messages_received_(0)
{
    // allocate buffers
    receive_buffers_.resize(N_);
    remote_endpoints_.resize(N_);

    local_sockets_.resize(N_);
    OpenLocalSockets(peer_ip_address);
}

UdpPeer::UdpPeer
(
    const nlohmann::json &json
)
:   UdpPeer
    (
        json["n_local_sockets"],
        json["my_port_min"],
        json["nb_port_min"],
        json["my_ip_address"]
    )
{
}

void UdpPeer::OpenLocalSockets
(
    const std::string& ip_address
)
{
    SizeType port_no = my_port_min_;
    for (auto &sock_ptr : local_sockets_)
    {
        DebugPrint::stream << "Open port_no = " << port_no << '\n';
        sock_ptr.reset
        (
            new SocketType(io_service_, boost::asio::ip::udp::v4())
        );
        IpAddressType local_ip = IpAddressType::from_string(ip_address);
        EndPointType local_end_point(local_ip, port_no);
        sock_ptr->bind(local_end_point);
        port_no++;
    }
}

void UdpPeer::SendToPeer
(
    SizeType k,
    SizeType peer_port,
    const std::string& peer_ip_address
)
{
    if (k >= local_sockets_.size())
    {
        return;
    }
    IpAddressType peer_ip = IpAddressType::from_string(peer_ip_address);
    EndPointType peer_endpoint(peer_ip, peer_port);
    std::string hello_str = std::string("Hello, ") +
        peer_ip_address + ", this is " +
        local_sockets_[k]->local_endpoint().address().to_string();
    DebugPrint::stream << "socket " << k << ", sending data to "
        << peer_ip_address << ":" << peer_port << '\n';
    local_sockets_[k]->async_send_to
    (
        boost::asio::buffer(hello_str.c_str(), hello_str.size()),
        peer_endpoint,
        boost::bind(&UdpPeer::SendCompleted, this, hello_str.size(), _1, _2)
    );
}

void UdpPeer::SendCompleted
(
    SizeType msg_size,
    const ErrorType& ec,
    SizeType bytes_transferred
)
{
    if (msg_size != bytes_transferred)
    {
        DebugPrint::stream<< "An error during transmission of bytes has occured"
                 <<'\n';
        return;
    }
    messages_sent_++;
    AfterSending();
}

void UdpPeer::ReceiveFromPeer
(
    SizeType k,
    SizeType peer_port,
    const std::string& peer_ip_address
)
{
    if (k >= local_sockets_.size())
    {
        return;
    }

    IpAddressType peer_ip = IpAddressType::from_string(peer_ip_address);
    SizeType i_peer_port = peer_port - nb_port_min_;
    IpAddressAndPort ip_and_port (peer_ip_address, peer_port);

    std::pair<IpAddressAndPort, BufferType> buffer_pair =
        std::make_pair(ip_and_port, BufferType{});
    receive_buffers_[k].insert(buffer_pair);

    std::pair<IpAddressAndPort, EndPointType> endpoint_pair =
        std::make_pair(ip_and_port, EndPointType(peer_ip, peer_port));
    EndPointType &remote_ep =
        remote_endpoints_[k].insert(endpoint_pair).first->second;

    char *buf_ptr = FindReceiveBuffer(k, ip_and_port);
    if (buf_ptr == nullptr)
    {
        return;
    }
    boost::asio::mutable_buffer buffer (buf_ptr, BufferSize);
    DebugPrint::stream << "Receiving data from: "
              << peer_ip_address << ":" << peer_port << ", "
              << "socket " << k << '\n';
    local_sockets_[k]->async_receive_from
    (
        buffer,
        remote_ep,
        boost::bind
        (
            &UdpPeer::ProcessReceive,
            this,
            k,
            boost::asio::const_buffer(buffer),
           _1,
           _2
        )
    );
}

void UdpPeer::ProcessReceive
(
    SizeType k,
    boost::asio::const_buffer recv_buf,
    const ErrorType& ec,
    SizeType bytes_transferred
)
{
    DebugPrint::stream << "bytes_transferred = " << bytes_transferred << '\n';
    messages_received_++;
    AfterReceiving();
}

char* UdpPeer::FindReceiveBuffer
(
    SizeType k,
    const IpAddressAndPort& endpoint
)
{
    if (k >= N_)
    {
        return nullptr;
    }

    auto buf_it = receive_buffers_[k].find(endpoint);
    if (buf_it == receive_buffers_[k].end())
    {
        return nullptr;
    }

    return buf_it->second.data();
}

void UdpPeer::SendToSpecifiedPeerPorts
(
    SizeType k,
    const std::string& peer_ip_address,
    SizeType start_remote_port,
    SizeType n_remote_ports
)
{
    SizeType k_port = start_remote_port;
    SizeType last_remote_port = start_remote_port + n_remote_ports - 1;
    for(; k_port <= last_remote_port; k_port++)
    {
        SendToPeer(k, k_port, peer_ip_address);
    }
}

void UdpPeer::SendToAllPeerPorts
(
    SizeType k,
    const std::string& peer_ip_address,
    SizeType n_remote_ports
)
{
    SizeType M = n_remote_ports;
    if (M == 0)
    {
        M = N_;
    }

//    SizeType nb_port = nb_port_min_;
//    SizeType nb_port_max = nb_port_min_ + M;
//    for(; nb_port < nb_port_max; nb_port++)
//    {
//        SendToPeer(k, nb_port, peer_ip_address);
//    }
    SendToSpecifiedPeerPorts(k, peer_ip_address, nb_port_min_, M);
}

void UdpPeer::SendAllToAll
(
    const std::string& peer_ip_address,
    SizeType n_remote_ports
)
{
    for (SizeType k=0; k < N_; k++)
    {
        SendToAllPeerPorts(k, peer_ip_address, n_remote_ports);
    }
}

void UdpPeer::ReceiveFromAllPeerPorts
(
    SizeType k,
    const std::string& peer_ip_address,
    SizeType n_remote_ports
)
{
    SizeType M = n_remote_ports;
    if (M == 0)
    {
        M = N_;
    }
    SizeType nb_port = nb_port_min_;
    SizeType nb_port_max = nb_port_min_ + M;
    for(; nb_port < nb_port_max; nb_port++)
    {
        ReceiveFromPeer(k, nb_port, peer_ip_address);
    }
}

void UdpPeer::ReceiveFromAll
(
    const std::string& peer_ip_address,
    SizeType n_remote_ports
)
{
    SizeType M = n_remote_ports;
    if (M == 0)
    {
        M = N_;
    }
    for (SizeType k=0; k < N_; k++)
    {
        ReceiveFromAllPeerPorts(k, peer_ip_address, n_remote_ports);
    }
}

//
//END-OF-FILE
//
