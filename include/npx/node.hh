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

#ifndef INCLUDED_NPX_NODE_HH
#define INCLUDED_NPX_NODE_HH

#include <node.h>

#include <npx/alias.hh>
#include <npx/errors.hh>
#include <npx/traits.hh>

namespace npx {

  // convert node value "v" to string
  std::string to_cpp_string(value_h v);

  // throw node exception with message
  void die(const char* message);

  // initialize a node function (class) with name and factory method
  function_template_p new_class(
    const char* name,
    v8::InvocationCallback factoryMethod);

  // attach a method "name" to object "otl" implemented in "callback"
  void attach_method(
    const char* name,
    object_l o, 
    v8::InvocationCallback callback);

  // attach a method "name" to object template "otl" implemented in "callback"
  void attach_method(
    const char* name,
    object_template_l o, 
    v8::InvocationCallback callback);

  // attach a property "name" to object template "otl" with accessors
  void attach_property (
    const char* name,
    object_template_l otl, 
    v8::AccessorGetter getter,
    v8::AccessorSetter setter = 0);

  // generic template method for getting property "p" value from object "o"
  template <typename NodeT>
  typename node_type_trait<NodeT>::return_type 
  get_value(object_h o, string_h p) {
    v8::HandleScope scope;

    if (!o->Has(p)) {
      throw property_missing(
        to_cpp_string(o->ToDetailString()), to_cpp_string(p));
    }

    value_l v = o->Get(p);

    if (!node_type_trait<NodeT>::is_type(v)) {
      throw type_error(to_cpp_string(v), node_type_trait<NodeT>::name());
    }

    return node_type_trait<NodeT>::to_type(scope.Close(v));
  }
}

#endif // INCLUDED_NPX_NODE_HH
