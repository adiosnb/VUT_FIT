#ifndef NET_STRUCT
#define NET_STRUCT

#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <iostream>
#include <vector>
#include <netinet/icmp6.h>
#include <netinet/ip6.h>

#define MAC_ADDR_LEN 6
#define MY_MAX_MTU 1530

// this is copied from krenel headers
#define MY_ARP_REQUEST 1
#define MY_ARP_REPLY 2

#define ETH_IPV6_TYPE 0xdd86


typedef union {
    uint8_t val8[16];
    uint16_t val16[8];
    uint32_t val32[4];
    uint64_t val64[2];
} ipv6_int_t;

typedef struct {
    ipv6_int_t addr;
    ipv6_int_t mask;
} ipv6_addr_t;

typedef union{
    uint32_t val32;
    uint8_t  val8[4];
} ipv4_int_t;

typedef struct {
    ipv4_int_t ipv4_addr;
    ipv4_int_t ipv4_mask;
} ipv4_addr_t;

typedef struct {

    uint8_t mac_addr[6];
    ipv4_addr_t ipv4;
    std::vector<ipv6_addr_t> ipv6;

    struct sockaddr_ll device;
}interface_t;

typedef struct {
    uint16_t htype;
    uint16_t ptype;
    uint8_t hlen;
    uint8_t plen;
    uint16_t opcode;
    uint8_t sender_mac[6];
    uint8_t sender_ip[4];
    uint8_t target_mac[6];
    uint8_t target_ip[4];
} arp_request_header_t;

typedef struct {
    ethhdr frame_hdr;
    arp_request_header_t arp_hdr;
} arp_frame_t;

typedef struct {
    uint8_t type;
    uint8_t len;
    uint8_t mac[6];
} ns_options_t;

typedef struct {
    struct nd_neighbor_solicit ns;
    ns_options_t options;
}my_nd_neighbor_solicit_wtih_options_filed_t;

typedef struct  __attribute__ ((__packed__)){
    ethhdr frame_hdr;
    struct ip6_hdr my_ip6_hdr;
    my_nd_neighbor_solicit_wtih_options_filed_t ns;
} ns_ipv6_frame_t;

typedef struct  __attribute__ ((__packed__)) {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t identifier;
    uint16_t sequence_number;
    uint8_t data[32];
} icmpv6_echo_request_t;

typedef struct __attribute__ ((__packed__)){
    ethhdr frame_hdr;
    struct ip6_hdr my_ip6_hdr;
    icmpv6_echo_request_t icmpv6_echo;
} icmpv6_frame_t;

std::string mac_2_str(uint8_t mac[6]);
std::string ip_2_str(uint8_t ip[4]);
std::string ip6_2_str(uint8_t ip6[16]);
void checksum_ns_ipv6_frame(ns_ipv6_frame_t *fr_ns);
void checksum_icmp_frame(icmpv6_frame_t *icmp_frame);
uint16_t checksum (uint16_t *addr, int len);

#endif //NET_STRUCT