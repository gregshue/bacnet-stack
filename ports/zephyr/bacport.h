/**************************************************************************
*
* Copyright (c) 2020 Legrand North America, LLC.
*
* SPDX-License-Identifier: MIT
*
*********************************************************************/

#ifndef NET_H
#define NET_H

#include <zephyr/net/net_ip.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>
/* BACnet Stack defines - first */
#include "bacnet/bacdef.h"
/* BACnet Stack API */
#include "bacnet/basic/object/device.h"

static inline char *inet_ntoa(struct in_addr in) {
    return "(inet_ntoa() to be implemented)";
}

/* Local helper functions for this port */
extern int bip_get_local_netmask(
    struct in_addr *netmask);

#ifdef CONFIG_BACNET_USE_SECTION_ITERABLE_OBJECT_TABLE

#define BACNET_OBJECT_TABLE(table_name, _type, _init, _count, \
                            _index_to_instance, _valid_instance, _object_name, \
                            _read_property, _write_property, _RPM_list, \
                            _RR_info, _iterator, _value_list, _COV, \
                            _COV_clear, _intrinsic_reporting) \
STRUCT_SECTION_ITERABLE(object_functions, table_name) = {   \
    .Object_Type = _type,                                   \
    .Object_Init = _init,                                   \
    .Object_Count = _count,                                 \
    .Object_Index_To_Instance = _index_to_instance,         \
    .Object_Valid_Instance = _valid_instance,               \
    .Object_Name = _object_name,                            \
    .Object_Read_Property = _read_property,                 \
    .Object_Write_Property = _write_property,               \
    .Object_RPM_List = _RPM_list,                           \
    .Object_RR_Info = _RR_info,                             \
    .Object_Iterator = _iterator,                           \
    .Object_Value_List = _value_list,                       \
    .Object_COV = _COV,                                     \
    .Object_COV_Clear = _COV_clear,                         \
    .Object_Intrinsic_Reporting = _intrinsic_reporting      \
}

#else

#define BACNET_OBJECT_TABLE(table_name, _type, _init, _count, \
                            _index_to_instance, _valid_instance, _object_name, \
                            _read_property, _write_property, _RPM_list, \
                            _RR_info, _iterator, _value_list, _COV, \
                            _COV_clear, _intrinsic_reporting) \
    while{}(0)

#endif

#endif
