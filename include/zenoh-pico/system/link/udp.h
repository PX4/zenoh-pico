//
// Copyright (c) 2022 ZettaScale Technology
//
// This program and the accompanying materials are made available under the
// terms of the Eclipse Public License 2.0 which is available at
// http://www.eclipse.org/legal/epl-2.0, or the Apache License, Version 2.0
// which is available at https://www.apache.org/licenses/LICENSE-2.0.
//
// SPDX-License-Identifier: EPL-2.0 OR Apache-2.0
//
// Contributors:
//   ZettaScale Zenoh Team, <zenoh@zettascale.tech>
//

#ifndef ZENOH_PICO_SYSTEM_LINK_UDP_H
#define ZENOH_PICO_SYSTEM_LINK_UDP_H

#include <stdint.h>
#include "zenoh-pico/collections/string.h"

#if ZN_LINK_UDP_UNICAST == 1 || ZN_LINK_UDP_MULTICAST == 1

typedef struct
{
    int sock;
    int msock;
    void *raddr;
    void *laddr;
} _zn_udp_socket_t;

void *_zn_create_endpoint_udp(const z_str_t s_addr, const z_str_t port);
void _zn_free_endpoint_udp(void *arg);

// Unicast
int _zn_open_udp_unicast(void *arg, const clock_t tout);
int _zn_listen_udp_unicast(void *arg, const clock_t tout);
void _zn_close_udp_unicast(int sock);
size_t _zn_read_exact_udp_unicast(int sock, uint8_t *ptr, size_t len);
size_t _zn_read_udp_unicast(int sock, uint8_t *ptr, size_t len);
size_t _zn_send_udp_unicast(int sock, const uint8_t *ptr, size_t len, void *arg);

// Multicast
int _zn_open_udp_multicast(void *arg_1, void **arg_2, const clock_t tout, const z_str_t iface);
int _zn_listen_udp_multicast(void *arg, const clock_t tout, const z_str_t iface);
void _zn_close_udp_multicast(int sock_recv, int sock_send, void *arg);
size_t _zn_read_exact_udp_multicast(int sock, uint8_t *ptr, size_t len, void *arg, z_bytes_t *addr);
size_t _zn_read_udp_multicast(int sock, uint8_t *ptr, size_t len, void *arg, z_bytes_t *addr);
size_t _zn_send_udp_multicast(int sock, const uint8_t *ptr, size_t len, void *arg);
#endif

#endif /* ZENOH_PICO_SYSTEM_LINK_UDP_H */
