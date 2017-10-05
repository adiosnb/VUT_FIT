#include "Scanner.h"
#include "NetStruct.h"

#include <fstream>
#include <ifaddrs.h>
#include <net/if.h>
#include <cstring>
#include <unistd.h>
#include <cmath>

#include <sys/ioctl.h>


Scanner::Scanner() {
    interface_name = "";
    out_file = "";
    sock_des = -1;
    listener_stay_in_cycle = 1;
}

void Scanner::set_interface_name(const std::string &interface_name) {
    this->interface_name = interface_name;
}

void Scanner::set_out_file(const std::string &out_file) {
    this->out_file = out_file;
}

void Scanner::write_xml_to_file() {
    std::ofstream out_file_stream;
    out_file_stream.open(out_file, std::ofstream::out);
    out_file_stream << found_devices.xml_encode();
    out_file_stream.close();
}

void Scanner::write_xml_to_cout() {
    std::cout << found_devices.xml_encode();
}

void Scanner::get_ip_interface_data() {
    struct ifaddrs *list_local_addrs, *item_addrs;
    struct ifreq ifr;

    int sd;
    sockaddr *tmpAddrPtr;

    //getting device id
    memset(&this->local_int.device, 0, sizeof(this->local_int.device));
    if ((this->local_int.device.sll_ifindex = if_nametoindex(this->interface_name.c_str())) < 0){
        throw "Error : if_nametoindex() : <func> get_ip_interface_data";
    }

    //getting list of interface in system
    getifaddrs(&list_local_addrs);

    // iterating through linked list of interfaces
    for (item_addrs = list_local_addrs; item_addrs != NULL; item_addrs = item_addrs->ifa_next) {
        if (item_addrs->ifa_name == interface_name) {
            if (item_addrs->ifa_addr->sa_family == AF_INET){

                for (int i = 0 ; i < 4 ; i++) {
                    local_int.ipv4.ipv4_addr.val8[i] = item_addrs->ifa_addr->sa_data[2+i];
                }
                for (int i = 0 ; i < 4 ; i++) {
                    local_int.ipv4.ipv4_mask.val8[i] = item_addrs->ifa_netmask->sa_data[2+i];
                }
            }
#ifdef IPK_IPV6
            else if (item_addrs->ifa_addr->sa_family == AF_INET6){
                ipv6_addr_t new_addr;

                for (int i = 0 ; i < 16 ; i++){
                    new_addr.addr.val8[i] = item_addrs->ifa_addr->sa_data[6+i];
                }

                for (int i = 0 ; i < 16 ; i++){
                    new_addr.mask.val8[i] = item_addrs->ifa_netmask->sa_data[6+i];
                }

                this->local_int.ipv6.push_back(new_addr);
            }
#endif
        }
    }

    //getting mac addr of interface

    // Submit request for a socket descriptor to look up interface.
    if ((sd = socket (AF_INET, SOCK_DGRAM, IPPROTO_IP)) < 0) {
        throw "Socket error";
    }

    // Use ioctl() to look up interface name and get its MAC address.
    memset (&ifr, 0, sizeof (ifr));
    snprintf (ifr.ifr_name, sizeof (ifr.ifr_name), "%s", this->interface_name.c_str());
    if (ioctl (sd, SIOCGIFHWADDR, &ifr) < 0) throw "ioctl error";
    close (sd);

    for (int i = 0 ; i < 6 ; i++) this->local_int.mac_addr[i] = ifr.ifr_ifru.ifru_hwaddr.sa_data[i];

    freeifaddrs(list_local_addrs);
}

void Scanner::scan() {
    open_socket();

    std::thread ipv4_listener(&Scanner::listen_network_adresses, this);
    std::thread ipv4_requester(&Scanner::scan_ipv4, this);

#ifdef IPK_IPV6
    std::thread ipv6_requester(&Scanner::scan_ipv6, this);
    std::thread ipv6_ping_sweep(&Scanner::send_ipv6_multicast_ping, this);

    ipv6_ping_sweep.join();
    ipv6_requester.join();

#endif

    ipv4_requester.join();
    sleep(5);

    listener_stay_in_cycle = 0;

    sleep(3);
    if (ipv4_listener.joinable()) {
        ipv4_listener.join();
    }
    else {
        ipv4_listener.detach();
        delete(&ipv4_listener);
    }

    close_socket();
}

void Scanner::scan_ipv4() {
    //get prefix
    int prefix = 0;
    uint32_t net_mask = this->local_int.ipv4.ipv4_mask.val32;
    for (int i = 0 ; i < 32 ; i++){
        prefix += (net_mask >> i) & (uint32_t)1;
    }

    //find network address
    ipv4_int_t current_addr, net_addr;
    net_addr.val32 = htonl(this->local_int.ipv4.ipv4_addr.val32)
                     & htonl(this->local_int.ipv4.ipv4_mask.val32);

    //for cycle through all ip which might scanned
    for (int i = 1 ; i < exp2(32-prefix) - 1; i++){
        current_addr.val32 = net_addr.val32 + i;  // adding i to netwrok address
        current_addr.val32 = htonl(current_addr.val32);
        send_ipv4_arp_request(current_addr);

    } // end for cycle through ip addresses
}

#ifdef IPK_IPV6
void Scanner::scan_ipv6() {
    for (auto &&addr : local_int.ipv6) {
        //scan_ipv6_net_sequentially(addr);
 //       scan_ipv6_net_know_ports(addr);
    }
}

void Scanner::scan_ipv6_net_sequentially(ipv6_addr_t addr) {
    //get prefix
    int prefix = 0;
    for (int byte4 = 0 ; byte4 < 4 ; byte4++) {
        uint32_t net_mask = addr.mask.val32[byte4];
        for (int i = 0; i < 32; i++) {
            prefix += (net_mask >> i) & (uint32_t) 1;
        }
    }

    //finding network address
    ipv6_int_t current, net_addr;
    for (int i = 0 ; i < 4 ; i++){
        net_addr.val32[i] = addr.addr.val32[i] & addr.mask.val32[i];
    }


    //splitting ipv6 addr into 4 part A, B, C, D by 32 bits
    //iterating through whole ipv6 network
    uint64_t A,B,C,D;
    current.val32[0] = net_addr.val32[0];
    current.val32[1] = net_addr.val32[1];
    current.val32[2] = net_addr.val32[2];
    current.val32[3] = net_addr.val32[3];
    A = 0;
    while ((((current.val32[0] = net_addr.val32[0] + htonl(A))&addr.mask.val32[0])
            == net_addr.val32[0]) && (A <= UINT32_MAX)){
        current.val32[0] = net_addr.val32[0] + htonl(A);
        B = 0;
        while ((((current.val32[1] = net_addr.val32[1] + htonl(B))&addr.mask.val32[1])
                == net_addr.val32[1]) && (B <= UINT32_MAX)){
            current.val32[1] = net_addr.val32[1] + htonl(B);
            C = 0;
            while ((((current.val32[2] = net_addr.val32[2] + htonl(C))&addr.mask.val32[2])
                    == net_addr.val32[2]) && (C <= UINT32_MAX)) {
                current.val32[2] = net_addr.val32[2] + htonl(C);
                D = 0;
                while ((((current.val32[3] = net_addr.val32[3] + htonl(D))&addr.mask.val32[3])
                       == net_addr.val32[3]) && (D <= UINT32_MAX)) {

                    D++;
                }
                C++;
                std::cout << ip6_2_str(current.val8) << std::endl;
            }
            B++;
        }
        A++;
    }

    std::cout << "IPv6 net : " << ip6_2_str(net_addr.val8) << "/" << prefix << std::endl;

    std::string jj;
    std::cin >> jj;
}

void Scanner::scan_ipv6_net_know_ports(ipv6_addr_t addr) {
    //get prefix
    int prefix = 0;
    for (int byte4 = 0 ; byte4 < 4 ; byte4++) {
        uint32_t net_mask = addr.mask.val32[byte4];
        for (int i = 0; i < 32; i++) {
            prefix += (net_mask >> i) & (uint32_t) 1;
        }
    }

    //finding network address
    ipv6_int_t current, net_addr;
    for (int i = 0 ; i < 4 ; i++){
        net_addr.val32[i] = addr.addr.val32[i] & addr.mask.val32[i];
    }


    for (uint16_t i = 0; i < 2000; i ++){
        net_addr.val16[7] = htons(i);
        send_ipv6_ns_request(net_addr, addr.addr);
    }

    net_addr.val32[3] = htonl(2);
    send_ipv6_ns_request(net_addr, addr.addr);

}

void Scanner::send_ipv6_ns_request(ipv6_int_t search_ard, ipv6_int_t interface_addr) {
    ssize_t msg_len;
    ns_ipv6_frame_t ns_frame;
    ipv6_int_t mutlicast_ipv6_addr = search_ard;

    //setting mac addrs
    memcpy(ns_frame.frame_hdr.h_source, local_int.mac_addr, MAC_ADDR_LEN);

    ns_frame.frame_hdr.h_dest[0] = 0x33;
    ns_frame.frame_hdr.h_dest[1] = 0x33;
    ns_frame.frame_hdr.h_dest[2] = 0xff;
    ns_frame.frame_hdr.h_dest[3] = search_ard.val8[13];
    ns_frame.frame_hdr.h_dest[4] = search_ard.val8[14];
    ns_frame.frame_hdr.h_dest[5] = search_ard.val8[15];

    ns_frame.frame_hdr.h_proto = ETH_IPV6_TYPE;

    // cerating ns header
    ns_frame.ns.ns.nd_ns_hdr.icmp6_type = ND_NEIGHBOR_SOLICIT;
    ns_frame.ns.ns.nd_ns_hdr.icmp6_code = 0;
    ns_frame.ns.ns.nd_ns_hdr.icmp6_cksum = 0;
    ns_frame.ns.ns.nd_ns_hdr.icmp6_dataun.icmp6_un_data32[0] = 0;
    memcpy(&ns_frame.ns.ns.nd_ns_target, search_ard.val8, sizeof(search_ard));

    //setting ns options
    ns_frame.ns.options.type = 0x01;
    ns_frame.ns.options.len = 1;
    memcpy(&ns_frame.ns.options.mac, local_int.mac_addr, MAC_ADDR_LEN*sizeof(uint8_t));

    //creating muslti cast addr for ns
    mutlicast_ipv6_addr.val8[0] = 0xFF;
    mutlicast_ipv6_addr.val8[1] = 0x2;
    mutlicast_ipv6_addr.val8[2] = 0;
    mutlicast_ipv6_addr.val8[3] = 0;
    mutlicast_ipv6_addr.val32[1] = 0;
    mutlicast_ipv6_addr.val32[2] = htonl(1);
    mutlicast_ipv6_addr.val8[12] = 0xFF;

    memcpy(&ns_frame.my_ip6_hdr.ip6_dst, mutlicast_ipv6_addr.val8, sizeof(mutlicast_ipv6_addr));
    memcpy(&ns_frame.my_ip6_hdr.ip6_src, interface_addr.val8, sizeof(interface_addr));

    ns_frame.my_ip6_hdr.ip6_ctlun.ip6_un1.ip6_un1_flow = 0x60; //setting version to value 6
    ns_frame.my_ip6_hdr.ip6_ctlun.ip6_un1.ip6_un1_plen = 0x2000; //payload len
    ns_frame.my_ip6_hdr.ip6_ctlun.ip6_un1.ip6_un1_nxt = 0x3a;
    ns_frame.my_ip6_hdr.ip6_ctlun.ip6_un1.ip6_un1_hlim = 0xff; //  according to RFC

    checksum_ns_ipv6_frame(&ns_frame); // calculating checksum

    if ((msg_len = sendto(sock_des, &ns_frame, sizeof(ns_frame), 0,
                          (struct sockaddr *)&this->local_int.device,
                          sizeof(this->local_int.device))) < 0){
        throw "Error: sendto() : func<send_ipv6_ns_request>";
    }

}

void Scanner::send_ipv6_multicast_ping() {
    for (auto &&addr : local_int.ipv6) {
        send_ipv6_multicast_ping_with_addr(addr.addr);
    }
}

void Scanner::send_ipv6_multicast_ping_with_addr(ipv6_int_t addr) {
    icmpv6_frame_t echo_frame;

    //setting frame header
    memcpy(echo_frame.frame_hdr.h_source, local_int.mac_addr, MAC_ADDR_LEN);

    echo_frame.frame_hdr.h_dest[0] = 0x33;
    echo_frame.frame_hdr.h_dest[1] = 0x33;
    echo_frame.frame_hdr.h_dest[2] = 0x00;
    echo_frame.frame_hdr.h_dest[3] = 0x00;
    echo_frame.frame_hdr.h_dest[4] = 0x00;
    echo_frame.frame_hdr.h_dest[5] = 0x01;

    echo_frame.frame_hdr.h_proto = ETH_IPV6_TYPE;


    //setting ipv6 header
    memcpy(&echo_frame.my_ip6_hdr.ip6_src.__in6_u, addr.val8, sizeof(ipv6_int_t));

    for (int i = 0 ; i < 4 ; i++) echo_frame.my_ip6_hdr.ip6_dst.__in6_u.__u6_addr32[i] = 0;
    echo_frame.my_ip6_hdr.ip6_dst.__in6_u.__u6_addr16[0] = htons(0xFF02);
    echo_frame.my_ip6_hdr.ip6_dst.__in6_u.__u6_addr16[7] = htons(0x0001);

    echo_frame.my_ip6_hdr.ip6_ctlun.ip6_un1.ip6_un1_flow = 0x60;
    echo_frame.my_ip6_hdr.ip6_ctlun.ip6_un1.ip6_un1_hlim = 1;
    echo_frame.my_ip6_hdr.ip6_ctlun.ip6_un1.ip6_un1_nxt = IPPROTO_ICMPV6;
    echo_frame.my_ip6_hdr.ip6_ctlun.ip6_un1.ip6_un1_plen = htons(sizeof(icmpv6_echo_request_t));

    echo_frame.icmpv6_echo.type = ICMP6_ECHO_REQUEST;
    echo_frame.icmpv6_echo.code = 0;
    echo_frame.icmpv6_echo.identifier = 0;
    echo_frame.icmpv6_echo.sequence_number = 0;
    echo_frame.icmpv6_echo.checksum = 0;

    for (unsigned char i = 0 ; i < 32 ; i++)
        echo_frame.icmpv6_echo.data[i] = 'a' +i;

    checksum_icmp_frame(&echo_frame);

    if ((sendto(sock_des, &echo_frame, sizeof(echo_frame), 0,
                (struct sockaddr *)&this->local_int.device,
                sizeof(this->local_int.device))) < 0){
        throw "Error: sendto() : func<send_ipv6_ns_request>";
    }
}
#endif

void Scanner::send_ipv4_arp_request(ipv4_int_t search_add) {

    int socket_descriptor = sock_des;
    ssize_t msg_len;

    // creating arp request header
    arp_request_header_t arp_request;
    arp_request.htype = htons(1);
    arp_request.ptype = htons(ETH_P_IP);
    arp_request.hlen = 6 ;
    arp_request.plen = 4;
    arp_request.opcode = htons(MY_ARP_REQUEST);

    memcpy(&arp_request.sender_mac, this->local_int.mac_addr,6*sizeof(uint8_t));
    memset(&arp_request.target_mac,0,6*sizeof(uint8_t));

    memcpy(&arp_request.sender_ip, this->local_int.ipv4.ipv4_addr.val8, 4*sizeof(uint8_t));
    memcpy(&arp_request.target_ip, search_add.val8, 4*sizeof(uint8_t));

    //creating frame header
    ethhdr frame_header;
    memset(frame_header.h_dest, 0xff, 6*sizeof(unsigned char));
    memcpy(frame_header.h_source, this->local_int.mac_addr, 6*sizeof(unsigned char));
    frame_header.h_proto = htons(ETH_P_ARP);

    arp_frame_t frame;

    frame.frame_hdr = frame_header;
    frame.arp_hdr = arp_request;

    if ((msg_len = sendto(socket_descriptor, &frame, sizeof(frame), 0,
                          (struct sockaddr *)&this->local_int.device,
                          sizeof(this->local_int.device))) < 0){
        throw "Error: sendto() : func<send_ipv4_arp_request>";
    }

}

void Scanner::listen_network_adresses() {
    uint8_t buffer[MY_MAX_MTU];
    arp_frame_t *recieved_frame;
    ns_ipv6_frame_t *ipv6_frame;
    socklen_t addr_len = sizeof(this->local_int.device);

    //never ending story
    while(listener_stay_in_cycle){
        if (recvfrom(sock_des, &buffer, MY_MAX_MTU, 0,
                     (struct sockaddr *)&this->local_int.device, &addr_len) < 0){
            if (errno == EINTR) continue;
            else throw "Error while receiving message <func>listen_network_adresses";
        }

        recieved_frame = reinterpret_cast<arp_frame_t*>(buffer);

        if (recieved_frame->arp_hdr.opcode == htons(MY_ARP_REPLY) and
                recieved_frame->frame_hdr.h_proto == htons(ETH_P_ARP)){
            XmlCoder::signle_host_t new_host;

            new_host.mac_addr = mac_2_str(recieved_frame->arp_hdr.sender_mac);
            new_host.ipv4_addr = ip_2_str(recieved_frame->arp_hdr.sender_ip);

            found_devices.add_host(new_host);
        }
#ifdef IPK_IPV6
        if (recieved_frame->frame_hdr.h_proto == ETH_IPV6_TYPE){

            if (
                recieved_frame->frame_hdr.h_source[0] == 0 and
                recieved_frame->frame_hdr.h_source[1] == 0 and
                recieved_frame->frame_hdr.h_source[2] == 0 and
                recieved_frame->frame_hdr.h_source[3] == 0 and
                recieved_frame->frame_hdr.h_source[4] == 0 and
                recieved_frame->frame_hdr.h_source[5] == 0
                    )
                continue;

            XmlCoder::signle_host_t new_host;

            ipv6_frame = reinterpret_cast<ns_ipv6_frame_t*>(buffer);

            new_host.mac_addr = mac_2_str(ipv6_frame->frame_hdr.h_source);
            new_host.ipv6_addr = ip6_2_str(ipv6_frame->my_ip6_hdr.ip6_src.__in6_u.__u6_addr8);

            found_devices.add_host(new_host);
        }
#endif
    }
}

void Scanner::open_socket() {
    if((sock_des = socket(PF_PACKET, SOCK_RAW,htons(ETH_P_ALL))) < 0 ) {
        throw "error open socket";
    }
}

void Scanner::close_socket() {
    close(sock_des);
    sock_des = -1;
}



