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

#pragma once

// See: http://stackoverflow.com/questions/11002641/dynamic-casting-for-unique-ptr

#include <memory>
#include <typeinfo>
#include <boost/variant.hpp>

namespace ibmisc {

// http://ficksworkshop.com/blog/14-coding/86-how-to-static-cast-std-unique-ptr 
template<typename D, typename B>
std::unique_ptr<D> static_cast_unique_ptr(std::unique_ptr<B>& base)
{
    return std::unique_ptr<D>(static_cast<D*>(base.release()));
}
  
template<typename D, typename B>
std::unique_ptr<D> static_cast_unique_ptr(std::unique_ptr<B>&& base)
{
    return std::unique_ptr<D>(static_cast<D*>(base.release()));
}


template<typename D, typename B>
std::unique_ptr<D> dynamic_cast_unique_ptr(std::unique_ptr<B>& base)
{
    return std::unique_ptr<D>(dynamic_cast<D*>(base.release()));
}
  
template<typename D, typename B>
std::unique_ptr<D> dynamic_cast_unique_ptr(std::unique_ptr<B>&& base)
{
    return std::unique_ptr<D>(dynamic_cast<D*>(base.release()));
}



template <class T_DEST, class T_SRC>
inline std::shared_ptr<T_DEST> dynamic_shared_cast(std::unique_ptr<T_SRC> &&src)
{
    return std::shared_ptr<T_DEST>(dynamic_cast_unique_ptr<T_DEST, T_SRC>(std::move(src)));
}

// ---------------------------------------------------------------

/** Stores a value that is lazily evaluted.  The evaluation function
may return either a object (and "owned" reference), or a C-pointer (a
"borrowed" reference). */
template<class TypeT>
class LazyPtr {
    TypeT *_ptr;                        // Borrowed reference
    std::unique_ptr<TypeT> _uptr;   // Owned reference

    boost::variant<
        std::function<TypeT *()>,       // Borrowed
        std::function<std::unique_ptr<TypeT> ()>    // Owned
    > _compute;


public:

    /** Constructs with an already-evaluted borrowed reference. */
    LazyPtr(TypeT *ptr) : _ptr(ptr) {}

    /** Constructs with an already-evaluated owned refernece. */
    LazyPtr(std::unique_ptr<TypeT> &&uptr) : _uptr(std::move(uptr)) {
        _ptr = _uptr.get();
    }

    /** Constructs with a function to produce an owned reference. */
    LazyPtr(std::function<std::unique_ptr<TypeT> ()> const &compute_owned)
        : _ptr(0), _compute(compute_owned) {}

    /** Constructs with a function to produce a borrowed referene. */
    LazyPtr(std::function<TypeT *()> const &compute_borrowed)
        : _ptr(0), _compute(compute_borrowed) {}




    /** Constructs with a function to produce an owned reference. */
    LazyPtr(std::function<std::unique_ptr<TypeT> ()> &&compute_owned)
        : _ptr(0), _compute(std::move(compute_owned)) {}

    /** Constructs with a function to produce a borrowed referene. */
    LazyPtr(std::function<TypeT *()> &&compute_borrowed)
        : _ptr(0), _compute(std::move(compute_borrowed)) {}

    // -----------------------------------------------------
    // Used by operator*()
    class eval_visitor : public boost::static_visitor<> {
        LazyPtr<TypeT> const *lptr;
    public:
        eval_visitor(LazyPtr<TypeT> const *_lptr) : lptr(_lptr) {}

        void operator()(std::function<TypeT *()> const &borrowed_fn) const
        {
            const_cast<TypeT *&>(lptr->_ptr) = borrowed_fn(); }
        void operator()(std::function<std::unique_ptr<TypeT> ()> const &owned_fn) const {
            const_cast<std::unique_ptr<TypeT>&>(lptr->_uptr) = owned_fn();
            const_cast<TypeT *&>(lptr->_ptr) = &*lptr->_uptr;
        }
    };
    friend class eval_visitor;


    /** Dereference our "pointer."  Evaluates the function, if it has
    not already been evaluated. */
    TypeT &operator*() const {
        if (!_ptr) {
            boost::apply_visitor(eval_visitor(this), _compute);
        }
        return *_ptr;
    }
    TypeT *operator->() const
        { return &operator*(); }
};
// --------------------------------------------------------------

}   // namespace ibmisc





#if 0
template <typename T_SRC, typename T_DEST, typename T_DELETER>
bool dynamic_pointer_move(std::unique_ptr<T_DEST, T_DELETER> & dest,
                          std::unique_ptr<T_SRC, T_DELETER> & src)
{
    if (!src) {
        dest.reset();
        return true;
    }

    T_DEST * dest_ptr = dynamic_cast<T_DEST *>(src.get());
    if (!dest_ptr)
        return false;

    std::unique_ptr<T_DEST, T_DELETER> dest_temp(dest_ptr, src.get_deleter());
    src.release();
    dest.swap(dest_temp);
    return true;
}

template <typename T_SRC, typename T_DEST>
bool dynamic_pointer_move(std::unique_ptr<T_DEST> & dest,
                          std::unique_ptr<T_SRC> & src)
{
    if (!src) {
        dest.reset();
        return true;
    }

    T_DEST * dest_ptr = dynamic_cast<T_DEST *>(src.get());
    if (!dest_ptr)
        return false;

    src.release();
    dest.reset(dest_ptr);
    return true;
}
#endif

