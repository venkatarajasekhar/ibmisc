/*
 * IBMisc: Misc. Routines for IceBin (and other code)
 * Copyright (c) 2013-2016 by Elizabeth Fischer
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

///////////////////////////////////////////////////////////////////////////////
// base.hpp: defines the enum_base type
//
// Copyright 2005 Frank Laub
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ENUM_BASE_HPP
#define BOOST_ENUM_BASE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <ostream>
#include <istream>
#include <boost/operators.hpp>
#include <boost/optional.hpp>

namespace boost {
namespace detail {

    template <typename Derived, typename ValueType = int>
    class enum_base 
        : private boost::totally_ordered<Derived>
    {
    public:
        typedef enum_base<Derived, ValueType> this_type;
        typedef size_t index_type;
        typedef ValueType value_type;
        typedef enum_iterator<Derived> const_iterator;
        typedef boost::optional<Derived> optional;

    public:
        enum_base() {}
        enum_base(index_type index) : m_index(index) {}

        static const_iterator begin()
        {
            return const_iterator(0);
        }

        static const_iterator end()
        {
            return const_iterator(Derived::size);
        }

        static optional get_by_value(value_type value)
        {
            for(index_type i = 0; i < Derived::size; ++i)
            {
                typedef boost::optional<value_type> optional_value;
                optional_value cur = Derived::values(enum_cast<Derived>(i));
                if(value == *cur)
                    return Derived(enum_cast<Derived>(i));
            }
            return optional();
        }

        static optional get_by_index(index_type index)
        {
            if(index >= Derived::size) return optional();
            return optional(enum_cast<Derived>(index));
        }

        const char* str() const
        {
            const char* ret = Derived::names(enum_cast<Derived>(m_index));
            BOOST_ASSERT(ret);
            return ret;
        }

        value_type value() const
        {
            typedef boost::optional<value_type> optional_value;
            optional_value ret = Derived::values(enum_cast<Derived>(this->m_index));
            BOOST_ASSERT(ret);
            return *ret;
        }

        index_type index() const
        {
            return m_index;
        }

        bool operator == (const this_type& rhs) const
        {
            return m_index == rhs.m_index;
        }

        bool operator < (const this_type& rhs) const
        {
            value_type lhs_value = value();
            value_type rhs_value = rhs.value();
            if(lhs_value == rhs_value)
                return m_index < rhs.m_index;
            return lhs_value < rhs_value;
        }

    protected:
        friend class enum_iterator<Derived>;
        index_type m_index;
    };

    template <typename D, typename V>
    std::ostream& operator << (std::ostream& os, const enum_base<D, V>& rhs)
    {
        return (os << rhs.str());
    }

    template <typename D, typename V>
    std::istream& operator >> (std::istream& is, enum_base<D, V>& rhs)
    {
        std::string str;
        is >> str;
        BOOST_DEDUCED_TYPENAME D::optional ret = D::get_by_name(str.c_str());
        if(ret)
            rhs = *ret;
        else
            is.setstate(std::ios::badbit);
        return is;
    }

} // detail
} // boost

#endif
