
#include "XmlCoder.h"

void XmlCoder::Devices::add_host(signle_host_t host) {
    host_t *ptr_to_devices;
    if ((ptr_to_devices = find_host(host.mac_addr)) != nullptr){
        if (host.ipv4_addr != "") ptr_to_devices->ipv4_addr = host.ipv4_addr;
        // check duplicit ipv6
        if (host.ipv6_addr != ""){
            std::vector<std::string>::iterator iter = ptr_to_devices->ipv6_addrs.begin();
            for (auto &&ipv6_addr : ptr_to_devices->ipv6_addrs) {
                if (ipv6_addr == host.ipv6_addr) return;
            }
            ptr_to_devices->ipv6_addrs.push_back(host.ipv6_addr);
        }
    }
    else {
        host_t new_host;
        new_host.mac_addr = host.mac_addr;
        new_host.ipv4_addr = host.ipv4_addr;
        if (host.ipv6_addr != ""){
            new_host.ipv6_addrs.push_back(host.ipv6_addr);
        }
        hosts.push_back(new_host);
    }
}

std::string XmlCoder::Devices::xml_encode() {
    std::string final_xml_str;

    final_xml_str = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

    final_xml_str += "<devices>\n";

    for (auto &&item : hosts) {
        final_xml_str += "  <host mac=\"" + item.mac_addr + "\">\n";
        if (item.ipv4_addr != ""){
            final_xml_str += "    <ipv4>";
            final_xml_str += item.ipv4_addr;
            final_xml_str += "</ipv4>\n";
        }

        for (auto &&str : item.ipv6_addrs) {
            final_xml_str += "    <ipv6>";
            final_xml_str += str;
            final_xml_str += "</ipv6>\n";        }

        final_xml_str += "  </host>\n";
    }
    final_xml_str += "</devices>\n";
    return final_xml_str;
}

XmlCoder::host_t *XmlCoder::Devices::find_host(std::string mac) {
    std::vector<host_t>::iterator iter;

    iter = hosts.begin();
    while (iter != hosts.end()){
        if (iter.base()->mac_addr == mac){
            return iter.base();
        }
        iter++;
    }

    return nullptr;
}
