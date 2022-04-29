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
 *     ADLINK zenoh team, <zenoh@adlink-labs.tech>
 */

#include <assert.h>
#include <string.h>
#include "zenoh-pico/collections/vec.h"

/*-------- vec --------*/
inline _z_vec_t _z_vec_make(size_t capacity)
{
    _z_vec_t v;
    v._capacity = capacity;
    v._len = 0;
    v._val = (void **)malloc(sizeof(void *) * capacity);
    return v;
}

void _z_vec_copy(_z_vec_t *dst, const _z_vec_t *src, z_element_clone_f d_f)
{
    dst->_capacity = src->_capacity;
    dst->_len = src->_len;
    dst->_val = (void **)malloc(sizeof(void *) * src->_capacity);
    for (size_t i = 0; i < src->_len; i++)
        _z_vec_append(dst, d_f(src->_val[i]));
}

void _z_vec_reset(_z_vec_t *v, z_element_free_f free_f)
{
    for (size_t i = 0; i < v->_len; i++)
        free_f(&v->_val[i]);

    v->_len = 0;
}

void _z_vec_clear(_z_vec_t *v, z_element_free_f free_f)
{
    for (size_t i = 0; i < v->_len; i++)
        free_f(&v->_val[i]);

    free(v->_val);
    v->_val = NULL;

    v->_capacity = 0;
    v->_len = 0;
}

void _z_vec_free(_z_vec_t **v, z_element_free_f free_f)
{
    _z_vec_t *ptr = (_z_vec_t *)*v;
    _z_vec_clear(ptr, free_f);

    free(ptr);
    *v = NULL;
}

size_t _z_vec_len(const _z_vec_t *v)
{
    return v->_len;
}

int _z_vec_is_empty(const _z_vec_t *v)
{
    return v->_len == 0;
}

void _z_vec_append(_z_vec_t *v, void *e)
{
    if (v->_len == v->_capacity)
    {
        // Allocate a new vector
        size_t _capacity = (v->_capacity << 1) | 0x01;
        void **_val = (void **)malloc(_capacity * sizeof(void *));
        memcpy(_val, v->_val, v->_capacity * sizeof(void *));

        // Free the old vector
        free(v->_val);

        // Update the current vector
        v->_val = _val;
        v->_capacity = _capacity;
    }

    v->_val[v->_len] = e;
    v->_len++;
}

void *_z_vec_get(const _z_vec_t *v, size_t i)
{
    assert(i < v->_len);

    return v->_val[i];
}

void _z_vec_set(_z_vec_t *v, size_t i, void *e, z_element_free_f free_f)
{
    assert(i < v->_len);

    if (v->_val[i] != NULL)
        free_f(&v->_val[i]);
    v->_val[i] = e;
}

void _z_vec_remove(_z_vec_t *v, size_t pos, z_element_free_f free_f)
{
    free_f(&v->_val[pos]);
    for (size_t i = pos; i < v->_len; i++)
        v->_val[pos] = v->_val[pos + 1];

    v->_val[v->_len] = NULL;
    v->_len--;
}
