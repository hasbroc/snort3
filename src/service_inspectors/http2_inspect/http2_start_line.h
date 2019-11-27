//--------------------------------------------------------------------------
// Copyright (C) 2019-2019 Cisco and/or its affiliates. All rights reserved.
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License Version 2 as published
// by the Free Software Foundation.  You may not use, modify or distribute
// this program under any other version of the GNU General Public License.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//--------------------------------------------------------------------------
// http2_start_line.h author Katura Harvey <katharve@cisco.com>

#ifndef HTTP2_START_LINE_H
#define HTTP2_START_LINE_H

#include "service_inspectors/http_inspect/http_common.h"
#include "service_inspectors/http_inspect/http_field.h"
#include "utils/event_gen.h"
#include "utils/infractions.h"

#include "http2_enum.h"

using Http2Infractions = Infractions<Http2Enums::INF__MAX_VALUE, Http2Enums::INF__NONE>;
using Http2EventGen = EventGen<Http2Enums::EVENT__MAX_VALUE, Http2Enums::EVENT__NONE,
    Http2Enums::HTTP2_GID>;

class Http2FlowData;

class Http2StartLine
{
public:
    static Http2StartLine* new_start_line_generator(HttpCommon::SourceId source_id,
        Http2EventGen* events, Http2Infractions* infractions);

    virtual ~Http2StartLine();

    friend class Http2Hpack;

    const Field* get_start_line();
    virtual void process_pseudo_header_name(const uint8_t* const& name, uint32_t length) = 0;
    virtual void process_pseudo_header_value(const uint8_t* const& value, const uint32_t length) = 0;
    bool finalize();
    bool is_finalized() { return finalized; }
    bool is_pseudo_value() { return value_coming != Http2Enums::HEADER__NONE; }
    bool is_pseudo_name(const uint8_t* const& name) { return name[0] == ':'; }

protected:
    Http2StartLine(Http2EventGen* events, Http2Infractions* infractions)   : events(events),
        infractions(infractions) { }

    void process_pseudo_header_precheck();
    virtual bool generate_start_line() = 0;

    Http2EventGen* events;
    Http2Infractions* infractions;
    bool finalized = false;
    uint32_t start_line_length = 0;
    uint8_t *start_line_buffer = nullptr;
    Http2Enums::PseudoHeaders value_coming = Http2Enums::HEADER__NONE;
    uint32_t pseudo_header_fragment_size = 0;

    // Version string is HTTP/1.1
    static const char* http_version_string;
    static const uint8_t http_version_length = 8;
};

#endif
