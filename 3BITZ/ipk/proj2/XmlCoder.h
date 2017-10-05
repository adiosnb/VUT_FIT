#ifndef XML_CODER
#define XML_CODER

#include <iostream>
#include <vector>

namespace XmlCoder {
    typedef struct {
        std::string mac_addr = "";
        std::string ipv4_addr = "";
        std::vector<std::string> ipv6_addrs;
    } host_t;

    typedef struct {
        std::string mac_addr = "";
        std::string ipv4_addr = "";
        std::string ipv6_addr = "";
    } signle_host_t;


    class Devices {

    private:
        std::vector<host_t> hosts;

    public:
        void add_host(signle_host_t host);

        host_t *find_host(std::string mac);

        std::string xml_encode();
    };

}


#endif //XML_CODER