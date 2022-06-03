/*
 * Copyright (c) 2017, 2021 ADLINK Technology Inc.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0, or the Apache License, Version 2.0
 * which is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0
 *
 * Contributors:
 *   ADLINK zenoh team, <zenoh@adlink-labs.tech>
 */

#include <string.h>
#include "zenoh-pico/config.h"
#include "zenoh-pico/link/manager.h"
#include "zenoh-pico/link/config/udp.h"
#include "zenoh-pico/system/link/udp.h"

#if Z_LINK_UDP_MULTICAST == 1

char *_z_parse_port_segment_udp_multicast(const char *address)
{
    const char *p_start = strrchr(address, ':');
    if (p_start == NULL)
        return NULL;
    p_start++;

    const char *p_end = &address[strlen(address)];

    int len = p_end - p_start;
    char *port = (char *)malloc((len + 1) * sizeof(char));
    strncpy(port, p_start, len);
    port[len] = '\0';

    return port;
}

char *_z_parse_address_segment_udp_multicast(const char *address)
{
    const char *p_start = &address[0];
    const char *p_end = strrchr(address, ':');

    if (*p_start == '[' && *(p_end - 1) == ']')
    {
        p_start++;
        p_end--;
        int len = p_end - p_start;
        char *ip6_addr = (char *)malloc((len + 1) * sizeof(char));
        strncpy(ip6_addr, p_start, len);
        ip6_addr[len] = '\0';

        return ip6_addr;
    }
    else
    {
        int len = p_end - p_start;
        char *ip4_addr_or_domain = (char *)malloc((len + 1) * sizeof(char));
        strncpy(ip4_addr_or_domain, p_start, len);
        ip4_addr_or_domain[len] = '\0';

        return ip4_addr_or_domain;
    }

    return NULL;
}

int _z_f_link_open_udp_multicast(void *arg)
{
    _z_link_t *self = (_z_link_t *)arg;

    const char *iface = _z_str_intmap_get(&self->_endpoint._config, UDP_CONFIG_IFACE_KEY);
    if (iface == NULL)
        goto ERR;

    clock_t timeout = Z_CONFIG_SOCKET_TIMEOUT_DEFAULT;
    char *tout = _z_str_intmap_get(&self->_endpoint._config, UDP_CONFIG_TOUT_KEY);
    if (tout != NULL)
        timeout = strtof(tout, NULL);

    if (_z_open_udp_multicast(self->_socket._udp._raddr, &self->_socket._udp._laddr, timeout, iface) < 0)
        goto ERR;

    return 0;

ERR:
    return -1;
}

int _z_f_link_listen_udp_multicast(void *arg)
{
    _z_link_t *self = (_z_link_t *)arg;

    const char *iface = _z_str_intmap_get(&self->_endpoint._config, UDP_CONFIG_IFACE_KEY);
    if (iface == NULL)
        goto ERR_1;

    self->_socket._udp._sock = _z_listen_udp_multicast(self->_socket._udp._raddr, Z_CONFIG_SOCKET_TIMEOUT_DEFAULT, iface);
    if (self->_socket._udp._sock < 0)
        goto ERR_1;

    self->_socket._udp._msock = _z_open_udp_multicast(self->_socket._udp._raddr, &self->_socket._udp._laddr, Z_CONFIG_SOCKET_TIMEOUT_DEFAULT, iface);
    if (self->_socket._udp._msock < 0)
        goto ERR_2;

    return 0;

ERR_2:
    _z_close_udp_multicast(self->_socket._udp._sock, self->_socket._udp._msock, self->_socket._udp._raddr);

ERR_1:
    return -1;
}

void _z_f_link_close_udp_multicast(void *arg)
{
    _z_link_t *self = (_z_link_t *)arg;

    _z_close_udp_multicast(self->_socket._udp._sock, self->_socket._udp._msock, self->_socket._udp._raddr);
}

void _z_f_link_free_udp_multicast(void *arg)
{
    _z_link_t *self = (_z_link_t *)arg;

    _z_free_endpoint_udp(self->_socket._udp._laddr);
    _z_free_endpoint_udp(self->_socket._udp._raddr);
}

size_t _z_f_link_write_udp_multicast(const void *arg, const uint8_t *ptr, size_t len)
{
    const _z_link_t *self = (const _z_link_t *)arg;

    return _z_send_udp_multicast(self->_socket._udp._msock, ptr, len, self->_socket._udp._raddr);
}

size_t _z_f_link_write_all_udp_multicast(const void *arg, const uint8_t *ptr, size_t len)
{
    const _z_link_t *self = (const _z_link_t *)arg;

    return _z_send_udp_multicast(self->_socket._udp._msock, ptr, len, self->_socket._udp._raddr);
}

size_t _z_f_link_read_udp_multicast(const void *arg, uint8_t *ptr, size_t len, _z_bytes_t *addr)
{
    const _z_link_t *self = (const _z_link_t *)arg;

    return _z_read_udp_multicast(self->_socket._udp._sock, ptr, len, self->_socket._udp._laddr, addr);
}

size_t _z_f_link_read_exact_udp_multicast(const void *arg, uint8_t *ptr, size_t len, _z_bytes_t *addr)
{
    const _z_link_t *self = (const _z_link_t *)arg;

    return _z_read_exact_udp_multicast(self->_socket._udp._sock, ptr, len, self->_socket._udp._laddr, addr);
}

uint16_t _z_get_link_mtu_udp_multicast(void)
{
    // @TODO: the return value should change depending on the target platform.
    return 1450;
}

_z_link_t *_z_new_link_udp_multicast(_z_endpoint_t endpoint)
{
    _z_link_t *lt = (_z_link_t *)malloc(sizeof(_z_link_t));

    lt->_is_reliable = 0;
    lt->_is_streamed = 0;
    lt->_is_multicast = 1;
    lt->_mtu = _z_get_link_mtu_udp_multicast();

    lt->_endpoint = endpoint;

    lt->_socket._udp._sock = -1;
    lt->_socket._udp._msock = -1;
    char *s_addr = _z_parse_address_segment_udp_multicast(endpoint._locator._address);
    char *s_port = _z_parse_port_segment_udp_multicast(endpoint._locator._address);
    lt->_socket._udp._raddr = _z_create_endpoint_udp(s_addr, s_port);
    lt->_socket._udp._laddr = NULL;
    free(s_addr);
    free(s_port);

    lt->_open_f = _z_f_link_open_udp_multicast;
    lt->_listen_f = _z_f_link_listen_udp_multicast;
    lt->_close_f = _z_f_link_close_udp_multicast;
    lt->_free_f = _z_f_link_free_udp_multicast;

    lt->_write_f = _z_f_link_write_udp_multicast;
    lt->_write_all_f = _z_f_link_write_all_udp_multicast;
    lt->_read_f = _z_f_link_read_udp_multicast;
    lt->_read_exact_f = _z_f_link_read_exact_udp_multicast;

    return lt;
}

#endif
