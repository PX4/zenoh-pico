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

#ifndef ZENOH_PICO_UTILS_PROPERTY_H
#define ZENOH_PICO_UTILS_PROPERTY_H

#include <stdint.h>
#include "zenoh-pico/collections/intmap.h"
#include "zenoh-pico/collections/string.h"
#include "zenoh-pico/utils/result.h"

// Properties returned by _z_info()
#define Z_INFO_PID_KEY 0x00
#define Z_INFO_PEER_PID_KEY 0x01
#define Z_INFO_ROUTER_PID_KEY 0x02

/**
 * Zenoh-net properties are represented as int-string map.
 */
typedef _z_str_intmap_t _z_properties_t;

/**
 * Returns a new empty map of properties.
 */
_z_properties_t _z_properties_make(void);

/**
 * Initialize a new empty map of properties.
 */
int _z_properties_init(_z_properties_t *ps);

/**
 * Insert a property with a given key to a properties map.
 * If a property with the same key already exists in the properties map, it is replaced.
 *
 * Parameters:
 *   ps: A pointer to the properties map.
 *   key: The key of the property to add.
 *   value: The value of the property to add.
 */
int _z_properties_insert(_z_properties_t *ps, unsigned int key, _z_string_t value);

/**
 * Get the property with the given key from a properties map.
 *
 * Parameters:
 *     ps: A pointer to properties map.
 *     key: The key of the property.
 *
 * Returns:
 *     The value of the property with key ``key`` in properties map ``ps``.
 */
_z_str_t _z_properties_get(const _z_properties_t *ps, unsigned int key);

/**
 * Get the length of the given properties map.
 *
 * Parameters:
 *     ps: A pointer to the properties map.
 *
 * Returns:
 *     The length of the given properties map.
 */
#define _z_properties_len _z_str_intmap_len

/**
 * Get the length of the given properties map.
 *
 * Parameters:
 *     ps: A pointer to the properties map.
 *
 * Returns:
 *     A boolean to indicate if properties are present.
 */
#define _z_properties_is_empty _z_str_intmap_is_empty

/**
 * Clear a set of properties.
 *
 * Parameters:
 *   ps: A pointer to the properties map.
 */
#define _z_properties_clear _z_str_intmap_clear

/**
 * Free a set of properties.
 *
 * Parameters:
 *   ps: A pointer to a pointer of properties.
 *
 */
#define _z_properties_free _z_str_intmap_free

#endif /* ZENOH_PICO_UTILS_PROPERTY_H */
