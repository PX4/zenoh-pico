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

#ifndef ZENOH_PICO_SESSION_TYPES_H
#define ZENOH_PICO_SESSION_TYPES_H

#include "zenoh-pico/protocol/core.h"
#include "zenoh-pico/transport/manager.h"
#include "zenoh-pico/collections/list.h"
#include "zenoh-pico/collections/string.h"

#define _Z_RESOURCE_REMOTE 0
#define _Z_RESOURCE_IS_LOCAL 1

/**
 * An reply to a :c:func:`z_query` (or :c:func:`_z_query_collect`).
 *
 * Members:
 *   _z_sample_t data: a :c:type:`_z_sample_t` containing the key and value of the reply.
 *   unsigned int replier_kind: The kind of the replier that sent this reply.
 *   _z_bytes_t replier_id: The id of the replier that sent this reply.
 *
 */
typedef struct
{
    _z_sample_t _sample;
    unsigned int _replier_kind;
    _z_bytes_t _replier_id;
} _z_reply_data_t;

_Z_ELEM_DEFINE(_z_reply_data, _z_reply_data_t, _z_noop_size, _z_noop_clear, _z_noop_copy)
_Z_LIST_DEFINE(_z_reply_data, _z_reply_data_t)
_Z_ARRAY_DEFINE(_z_reply_data, _z_reply_data_t)

/**
 * An reply to a :c:func:`z_query`.
 *
 * Members:
 *   _z_reply_t_Tag tag: Indicates if the reply contains data or if it's a FINAL reply.
 *   _z_reply_data_t data: The reply data if :c:member:`_z_reply_t.tag` equals :c:member:`_z_reply_t_Tag.Z_REPLY_TAG_DATA`.
 *
 */
typedef struct
{
    z_reply_tag_t _tag;
    _z_reply_data_t _data;
} _z_reply_t;

typedef struct
{
    _z_zint_t _id;
    _z_keyexpr_t _key;
} _z_resource_t;

int _z_resource_eq(const _z_resource_t *one, const _z_resource_t *two);
void _z_resource_clear(_z_resource_t *res);

_Z_ELEM_DEFINE(_z_resource, _z_resource_t, _z_noop_size, _z_resource_clear, _z_noop_copy)
_Z_LIST_DEFINE(_z_resource, _z_resource_t)

/**
 * The callback signature of the functions handling data messages.
 */
typedef void (*_z_data_handler_t)(const _z_sample_t *sample, const void *arg);

typedef struct
{
    _z_zint_t _id;
    _z_str_t _rname;
    _z_keyexpr_t _key;
    _z_subinfo_t _info;
    _z_data_handler_t _callback;
    void *_arg;
} _z_subscription_t;

int _z_subscription_eq(const _z_subscription_t *one, const _z_subscription_t *two);
void _z_subscription_clear(_z_subscription_t *sub);

_Z_ELEM_DEFINE(_z_subscriber, _z_subscription_t, _z_noop_size, _z_subscription_clear, _z_noop_copy)
_Z_LIST_DEFINE(_z_subscriber, _z_subscription_t)

typedef struct
{
    _z_zint_t _id;
    _z_keyexpr_t _key;
} _z_publication_t;

typedef struct
{
    _z_reply_t *_reply;
    _z_timestamp_t _tstamp;
} _z_pending_reply_t;

int _z_pending_reply_eq(const _z_pending_reply_t *one, const _z_pending_reply_t *two);
void _z_pending_reply_clear(_z_pending_reply_t *res);

_Z_ELEM_DEFINE(_z_pending_reply, _z_pending_reply_t, _z_noop_size, _z_pending_reply_clear, _z_noop_copy)
_Z_LIST_DEFINE(_z_pending_reply, _z_pending_reply_t)

/**
 * The callback signature of the functions handling query replies.
 */
typedef void (*_z_query_handler_t)(_z_reply_t reply, const void *arg);

typedef struct
{
    _z_zint_t _id;
    _z_str_t _rname;
    _z_keyexpr_t _key;
    _z_str_t _predicate;
    _z_query_target_t _target;
    _z_consolidation_strategy_t _consolidation;
    _z_pending_reply_list_t *_pending_replies;
    _z_query_handler_t _callback;
    void *_arg;
} _z_pending_query_t;

int _z_pending_query_eq(const _z_pending_query_t *one, const _z_pending_query_t *two);
void _z_pending_query_clear(_z_pending_query_t *res);

_Z_ELEM_DEFINE(_z_pending_query, _z_pending_query_t, _z_noop_size, _z_pending_query_clear, _z_noop_copy)
_Z_LIST_DEFINE(_z_pending_query, _z_pending_query_t)

typedef struct
{
    _z_mutex_t _mutex;
    _z_condvar_t _cond_var;
    _z_reply_data_list_t *_replies;
} _z_pending_query_collect_t;

#endif /* ZENOH_PICO_SESSION_TYPES_H */
