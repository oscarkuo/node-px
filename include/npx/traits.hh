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

#ifndef INCLUDED_NPX_TRAITS_HH
#define INCLUDED_NPX_TRAITS_HH

#include <node.h>
#include <string>
#include <npx/alias.hh>

namespace npx {
  template <typename V8T>
  struct node_type_trait { };

  template <>
  struct node_type_trait<v8::String> {
    typedef string_l return_type;
    static std::string name() { return "string"; }
    static bool is_type(value_h v) { return v->IsString(); }
    static string_l to_type(value_h v) { return v->ToString(); }
  };

  template <>
  struct node_type_trait<v8::Integer> {
    typedef integer_l return_type;
    static std::string name() { return "integer"; }
    static bool is_type(value_h v) { return v->IsNumber(); }
    static integer_l to_type(value_h v) { return v->ToInteger(); }
  };
}

#endif // INCLUDED_NPX_TRAITS_HH
