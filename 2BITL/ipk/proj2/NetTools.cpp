#include <sstream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include "NetStruct.h"

std::string ip_2_str(uint8_t ip[4]){
    char addressBuffer[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, ip, addressBuffer, INET_ADDRSTRLEN);
    return std::string(addressBuffer);
}

std::string mac_2_str(uint8_t mac[6]){
    std::stringstream mac_stream;
    mac_stream << std::hex;

    for (int i = 0; i < 6; i++){
        if (i > 0 and i % 2 == 0){
            mac_stream << ".";
        }

        if (mac[i] < 16){
            mac_stream << "0";
        }
        mac_stream << static_cast<int>(mac[i]);
    }

    return mac_stream.str();
}

std::string ip6_2_str(uint8_t ip6[16]){
    char addressBuffer[INET_ADDRSTRLEN];
    inet_ntop(AF_INET6, ip6, addressBuffer, INET6_ADDRSTRLEN);
    return std::string(addressBuffer);
}

void checksum_ns_ipv6_frame(ns_ipv6_frame_t *ns) {
    uint16_t fcs;
    uint8_t buffer[IP_MAXPACKET];
    memcpy(buffer, ns->my_ip6_hdr.ip6_src.__in6_u.__u6_addr8, sizeof(ipv6_int_t));
    memcpy(buffer+16, ns->my_ip6_hdr.ip6_dst.__in6_u.__u6_addr8, sizeof(ipv6_int_t));

    int ns_hdr_len = sizeof(my_nd_neighbor_solicit_wtih_options_filed_t);

    for (int i = 32; i < 40 ; i++) buffer[i] =0;

    buffer[34] = ns_hdr_len  / 256;  // Upper layer packet length
    buffer[35] = ns_hdr_len  % 256;  // Upper layer packet length
    buffer[39] = IPPROTO_ICMPV6;

    memcpy(buffer+40,&ns->ns, ns_hdr_len);

    fcs = checksum((uint16_t *)buffer,40 + ns_hdr_len);
    ns->ns.ns.nd_ns_hdr.icmp6_cksum = fcs;
}


void checksum_icmp_frame(icmpv6_frame_t *icmp_frame) {
    uint8_t buffer[IP_MAXPACKET];
    memcpy(buffer, icmp_frame->my_ip6_hdr.ip6_src.__in6_u.__u6_addr8, sizeof(ipv6_int_t));
    memcpy(buffer+16, icmp_frame->my_ip6_hdr.ip6_dst.__in6_u.__u6_addr8, sizeof(ipv6_int_t));

    int icmp_echo_len = sizeof(icmpv6_echo_request_t);

    for (int i = 32; i < 40 ; i++) buffer[i] =0;

    buffer[34] = icmp_echo_len  / 256;
    buffer[35] = icmp_echo_len  % 256;
    buffer[39] = IPPROTO_ICMPV6;

    memcpy(buffer+40,&icmp_frame->icmpv6_echo, icmp_echo_len);

    icmp_frame->icmpv6_echo.checksum = checksum((uint16_t *)buffer,40 + icmp_echo_len);
}


uint16_t checksum (uint16_t *addr, int len) {
    int count = len;
    register uint32_t sum = 0;
    uint16_t answer = 0;

    while (count > 1) {
        sum += *(addr++);
        count -= 2;
    }

    if (count > 0) sum += *(uint8_t *) addr;

    while (sum >> 16) sum = (sum & 0xffff) + (sum >> 16);

    answer = ~sum;
    return (answer);
}

