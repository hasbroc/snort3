/*
** Copyright (C) 2014 Cisco and/or its affiliates. All rights reserved.
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License Version 2 as
** published by the Free Software Foundation.  You may not use, modify or
** distribute this program under any other version of the GNU General
** Public License.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
// rule_metadata.cc author Josh Rosenbaum <jrosenba@cisco.com>

#include <sstream>
#include <vector>

#include "conversion_state.h"
#include "utils/converter.h"
#include "rule_states/rule_api.h"
#include "utils/s2l_util.h"

namespace rules
{

namespace {


class Metadata : public ConversionState
{
public:
    Metadata(Converter& c) : ConversionState(c) {};
    virtual ~Metadata() {};
    virtual bool convert(std::istringstream& data);
};

} // namespace

bool Metadata::convert(std::istringstream& data_stream)
{
    std::string keyword;
    std::string tmp;
    std::string value;
    std::string soid_val = std::string();
    bool retval = true;

    retval = rule_api.add_rule_option("metadata");
    rule_api.select_option("metadata");

    tmp = util::get_rule_option_args(data_stream);
    std::istringstream metadata_stream(util::trim(tmp));


    while(metadata_stream >> keyword)
    {
        bool tmpval = true;
        value = std::string();

        while (metadata_stream >> tmp &&
                tmp.back() != ',')
        {
            if (!value.empty())
                value += ' ';
            value += tmp;
            tmp = std::string();
        }

        // tmp can be empty if we hit the end of stream
        if (!value.empty() && !tmp.empty())
            value += ' ';
        value += tmp;

        // won't end with comma if end of metadata string
        if (value.back() == ',')
            value.pop_back();

        if (!keyword.compare("rule-flushing"))
            rule_api.add_comment_to_rule("metadata: rule-flushing - deprecated");

        else if (!keyword.compare("soid"))
            soid_val = value;  // add this after metadata to keep ordering

        else if (!keyword.compare("engine"))
        {
            rule_api.make_rule_a_comment();
            rule_api.add_comment_to_rule("metadata: engine - deprecated");
        }

        else
        {
            tmpval = rule_api.add_suboption(keyword, value);
        }

        if (retval)
            retval = tmpval;

    }

    if (!soid_val.empty())
        retval = rule_api.add_rule_option("soid", soid_val);

    rule_api.unselect_option();
    return set_next_rule_state(data_stream) && retval;
}

/**************************
 *******  A P I ***********
 **************************/


static ConversionState* ctor(Converter& c)
{
    return new Metadata(c);
}

static const std::string metadata = "metadata";
static const ConvertMap metadata_api =
{
    metadata,
    ctor,
};

const ConvertMap* metadata_map = &metadata_api;

} // namespace rules
