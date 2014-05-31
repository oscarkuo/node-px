/*
 *    (c) Copyright 2003-2005  Uwe Steinmann.
 *    All rights reserved.
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 2 of the License, or (at your option) any later version.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not, write to the
 *    Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 *    Boston, MA 02111-1307, USA.
 */

#ifndef INCLUDE_NPX_PARADOX_HH
#define INCLUDE_NPX_PARADOX_HH

extern "C" {
#include <paradox.h>
}

#include <npx/node.hh>
#include <npx/alias.hh>

namespace npx {
  // converts a v8 javascript object to pxfield_t
  pxfield_t to_px_field_spec(value_h v);

  // converts a pxfield_t to a V8 javascript object
  object_h to_v8_field_spec(pxfield_t& f);

  // close and delete 'd' only if it is opened
  void release_if_opened(pxdoc_t*& d);

  // converts a pxval_t to v8::Value
  value_h to_v8_value(pxdoc_t* d, const pxfield_t& f, const pxval_t& from);

  // converts a v8::Value to pxval_t
  pxval_t* to_px_value(pxdoc_t* d, const pxfield_t& f, npx::value_h from);

  // release all resource in the pxval_t pointer 
  void free_pxval(pxval_t* p);

  // release resources in each pxval and then delete array
  void free_pxvals(pxval_t** p, uint32_t c);
}
 
#endif // INCLUDE_NPX_PARADOX_HH
