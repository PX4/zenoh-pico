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
#include <stdio.h>
#include <string.h>

#include "zenoh-pico.h"

int main(int argc, char **argv)
{
    z_init_logger();

    z_str_t expr = "/demo/example/**";
    if (argc > 1)
    {
        expr = argv[1];
    }
    z_owned_config_t config = z_config_default();
    if (argc > 2)
    {
        z_config_insert(z_loan(&config), Z_CONFIG_PEER_KEY, z_string_make(argv[2]));
    }

    z_config_insert(z_loan(&config), Z_CONFIG_USER_KEY, z_string_make("user"));
    z_config_insert(z_loan(&config), Z_CONFIG_PASSWORD_KEY, z_string_make("password"));

    printf("Openning session...\n");
    z_owned_session_t s = z_open(z_move(&config));
    if (!z_check(&s))
    {
        printf("Unable to open session!\n");
        exit(-1);
    }

    // Start the receive and the session lease loop for zenoh-pico
    _zp_start_read_task(z_loan(&s));
    _zp_start_lease_task(z_loan(&s));

    printf("Sending Query '%s'...\n", expr);
    z_query_target_t target = z_query_target_default();
    target._target = Z_TARGET_ALL;
    z_owned_reply_data_array_t replies = z_get_collect(z_loan(&s), z_expr_new(expr), "", target, z_query_consolidation_default());

    for (unsigned int i = 0; i < z_loan(&replies)->_len; ++i)
    {
        printf(">> Received ('%s': '%.*s')\n",
               z_loan(&replies)->_val[i]._sample._key._rname,
               (int)z_loan(&replies)->_val[i]._sample._value.len, z_loan(&replies)->_val[i]._sample._value.val);
    }
    z_reply_data_array_clear(z_move(&replies));

    _zp_stop_read_task(z_loan(&s));
    _zp_stop_lease_task(z_loan(&s));
    z_close(z_move(&s));
    return 0;
}
