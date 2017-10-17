#ifndef SCANNER
#define SCANNER

#include <iostream>
#include <netinet/in.h>
#include <thread>
#include "XmlCoder.h"
#include "NetStruct.h"

class Scanner {
private:
    std::string interface_name;
    std::string out_file;

    interface_t local_int;
    int sock_des;

    XmlCoder::Devices found_devices;

    short listener_stay_in_cycle;

public:
    Scanner();

    void set_interface_name(const std::string &interface_name);

    void set_out_file(const std::string &out_file);

    void write_xml_to_file();

    void write_xml_to_cout();

    void get_ip_interface_data();

    void scan();

    void scan_ipv4();

    void scan_ipv6();

    void scan_ipv6_net_sequentially(ipv6_addr_t addr);

    void scan_ipv6_net_know_ports(ipv6_addr_t addr);

    void send_ipv4_arp_request(ipv4_int_t search_add);

    void send_ipv6_ns_request(ipv6_int_t search_ard, ipv6_int_t interface_addr);

    void send_ipv6_multicast_ping();

    void send_ipv6_multicast_ping_with_addr(ipv6_int_t addr);

    void listen_network_adresses();

    void open_socket();

    void close_socket();
};

#endif //SCANNER