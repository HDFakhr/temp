#pragma once

#include <string>

namespace pooyan::board::frz
{
    struct ConnConfig {
        using ip_t = std::string;
        using port_t = unsigned short;

        ip_t ip;
        port_t port{ 0 };
    };

    template<typename T>
    static ConnConfig make_conn_config(T&& ip, ConnConfig::port_t port) {
        ConnConfig _;
        _.ip.assign(std::forward<T>(ip));
        _.port = port;
        return _;
    }
}