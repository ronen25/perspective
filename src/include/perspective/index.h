/******************************************************************************
 *
 * Copyright (c) 2017, the Perspective Authors.
 *
 * This file is part of the Perspective library, distributed under the terms of
 * the Apache License 2.0.  The full license can be found in the LICENSE file.
 *
 */

#pragma once
#include <perspective/base.h>
#include <perspective/scalar.h>
#include <boost/unordered_map.hpp>

namespace perspective
{

enum t_index_mode
{
    INDEX_MODE_IMPLICIT,
    INDEX_MODE_BITMAP
};

class a_index
{
public:
    virtual t_uindex lookup(t_tscalar v) = 0;
    virtual void upsert(t_tscalar v, t_uindex pos) = 0;
    virtual void remove(t_tscalar v) = 0;
};

class t_index : public a_index
{
    typedef boost::unordered_map<t_tscalar, t_uindex> t_mapping;

public:
    void
    upsert(t_tscalar v, t_uindex pos) override
    {
        m_mapping[v] = pos;
    }

    void
    remove(t_tscalar v) override
    {
        m_mapping.erase(v);
    }

    t_uindex lookup(t_tscalar v) override
    {
        auto iter = m_mapping.find(v);
        PSP_VERBOSE_ASSERT(iter != m_mapping.end(), "Missing key");
        return iter->second;
    }

private:
    t_mapping m_mapping;
};

} // end namespace perspective
