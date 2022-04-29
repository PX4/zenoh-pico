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

#include "zenoh-pico/transport/link/task/lease.h"

int _z_send_keep_alive(_z_transport_t *zt)
{   
    if (zt->_type == _Z_TRANSPORT_UNICAST_TYPE)
        return _zp_unicast_send_keep_alive(&zt->_transport._unicast);
    else if (zt->_type == _Z_TRANSPORT_MULTICAST_TYPE)
        return _zp_multicast_send_keep_alive(&zt->_transport._multicast);
    else
        return -1;
}

void *_zp_lease_task(void *arg)
{
    _z_transport_t *zt = (_z_transport_t *)arg;

    if (zt->_type == _Z_TRANSPORT_UNICAST_TYPE)
        return _zp_unicast_lease_task(&zt->_transport._unicast);
    else if (zt->_type == _Z_TRANSPORT_MULTICAST_TYPE)
        return _zp_multicast_lease_task(&zt->_transport._multicast);
    else
        return NULL;
}
