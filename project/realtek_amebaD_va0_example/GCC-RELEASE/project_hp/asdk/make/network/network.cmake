cmake_minimum_required(VERSION 3.6)

project(network)

set(dir_lwip "${sdk_root}/component/common/network/lwip/lwip_v2.1.2")
set(dir_network "${sdk_root}/component/common/network")
set(dir_wlandrv "${sdk_root}/component/common/drivers/wlan/realtek")

list(
    APPEND ${list}

    # SRAM
    ${dir_lwip}/src/core/tcp_in.c
    ${dir_lwip}/src/core/tcp_out.c
    ${dir_lwip}/src/core/def.c
    ${dir_lwip}/src/core/ipv4/igmp.c
    ${dir_lwip}/src/core/pbuf.c
    ${dir_lwip}/src/core/inet_chksum.c
    ${dir_lwip}/src/api/api_msg.c
    ${dir_lwip}/src/api/netbuf.c
    ${dir_lwip}/port/realtek/freertos/sys_arch.c
    ${dir_lwip}/port/realtek/freertos/ethernetif.c
    ${dir_lwip}/port/realtek/freertos/br_rpt_handle.c
    ${dir_lwip}/port/realtek/freertos/bridgeif.c
    ${dir_lwip}/port/realtek/freertos/bridgeif_fdb.c
    ${dir_lwip}/port/realtek/hooks/lwip_default_hooks.c
    ${dir_lwip}/src/core/timeouts.c
    ${dir_lwip}/src/core/raw.c
    ${dir_lwip}/src/core/mem.c
    ${dir_lwip}/src/core/memp.c
    ${dir_lwip}/src/core/netif.c
    ${dir_lwip}/src/core/ip.c
    ${dir_lwip}/src/core/ipv4/ip4.c
    ${dir_lwip}/src/core/ipv4/ip4_addr.c
    ${dir_lwip}/src/core/ipv4/ip4_frag.c
    ${dir_lwip}/src/core/ipv6/ip6.c
    ${dir_lwip}/src/core/ipv6/ip6_addr.c
    ${dir_lwip}/src/core/ipv6/ip6_frag.c
    ${dir_lwip}/src/core/tcp.c
    ${dir_lwip}/src/core/udp.c
    ${dir_lwip}/src/api/sockets.c
    ${dir_lwip}/src/api/tcpip.c
    ${dir_lwip}/src/netif/ethernet.c
    ${dir_lwip}/src/core/ipv4/etharp.c
    ${dir_lwip}/src/core/ipv6/ethip6.c
    ${dir_lwip}/src/api/netdb.c
    ${dir_lwip}/src/api/api_lib.c
    ${dir_lwip}/src/api/netifapi.c
    ${dir_wlandrv}/src/osdep/lwip_intf.c

    # XIP
    ${dir_lwip}/src/core/ipv4/dhcp.c
    ${dir_lwip}/src/core/ipv6/dhcp6.c
    ${dir_lwip}/src/core/dns.c
    ${dir_lwip}/src/core/init.c
    ${dir_lwip}/src/core/ipv4/autoip.c
    ${dir_lwip}/src/core/ipv4/icmp.c
    ${dir_lwip}/src/core/ipv6/icmp6.c
    ${dir_lwip}/src/core/ipv6/inet6.c
    ${dir_lwip}/src/core/ipv6/mld6.c
    ${dir_lwip}/src/core/ipv6/nd6.c
    ${dir_lwip}/src/api/err.c
    ${dir_lwip}/src/core/stats.c
    ${dir_lwip}/src/core/sys.c
    ${dir_network}/sntp/sntp.c
    ${dir_network}/websocket/wsserver_tls.c
    ${dir_network}/websocket/wsclient_tls.c
    ${dir_network}/dhcp/dhcps.c
    ${dir_network}/httpc/httpc_tls.c
    ${dir_network}/httpd/httpd_tls.c
    ${dir_network}/mDNS/mDNSPlatform.c
    #${dir_network}/coap/sn_coap_ameba_port.c
    #${dir_network}/coap/sn_coap_builder.c
    #${dir_network}/coap/sn_coap_header_check.c
    #${dir_network}/coap/sn_coap_parser.c
    #${dir_network}/coap/sn_coap_protocol.c
)


#target_include_directories(
#    ${target}
#    PUBLIC
#    ${sdk_root}/component/common/network/libcoap/include
#)

