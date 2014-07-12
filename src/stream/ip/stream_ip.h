/****************************************************************************
 *
** Copyright (C) 2014 Cisco and/or its affiliates. All rights reserved.
 * Copyright (C) 2011-2013 Sourcefire, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License Version 2 as
 * published by the Free Software Foundation.  You may not use, modify or
 * distribute this program under any other version of the GNU General
 * Public License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 ****************************************************************************/
 
/*
 * @file    stream_ip.h
 * @author  Russ Combs <rcombs@sourcefire.com>
 *
 */

#ifndef STREAM5_IP_H
#define STREAM5_IP_H

#include "flow/flow.h"

struct FragEngine
{
    uint32_t max_frags;
    uint32_t max_overlaps;
    uint32_t min_fragment_length;

    uint32_t frag_timeout; /* timeout for frags in this policy */
    uint16_t frag_policy;  /* policy to use for engine-based reassembly */

    uint8_t min_ttl;       /* Minimum TTL to accept */

    FragEngine();
};

struct StreamIpConfig
{
    FragEngine frag_engine;
    uint32_t session_timeout;

    StreamIpConfig();
};

void ip_sum();
void ip_stats();
void ip_reset();

StreamIpConfig* get_ip_cfg(Inspector*);
class Defrag* get_defrag(Inspector*);

#endif
