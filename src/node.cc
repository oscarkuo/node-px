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

#include <node.h>
#include <npx/node.hh>

namespace npx {
  void die (const char* message) {
    v8::ThrowException(v8::Exception::Error(v8::String::New(message)));
  }

  std::string to_cpp_string (value_h v) {
    v8::HandleScope scope;
    v8::String::Utf8Value s(v->ToString());

    if (*s) {
      return std::string(*s);
    }

    throw npx::type_error("v", "string");
  }

  function_template_p new_class (
    const char* name,
    v8::InvocationCallback factoryMethod) 
  {
    function_template_l t = v8::FunctionTemplate::New(factoryMethod);

    t->SetClassName(v8::String::NewSymbol(name));
    t->InstanceTemplate()->SetInternalFieldCount(1);

    return function_template_p::New(t);
  }

  void attach_method (
    const char* name,
    object_l o, 
    v8::InvocationCallback fp)
  {
    o->Set(
      v8::String::NewSymbol(name),
      v8::FunctionTemplate::New(fp)->GetFunction());
  }

  void attach_method (
    const char* name,
    object_template_l o, 
    v8::InvocationCallback fp)
  {
    o->Set(
      v8::String::NewSymbol(name),
      v8::FunctionTemplate::New(fp)->GetFunction());
  }

  void attach_property (
    const char* name,
    object_template_l otl, 
    v8::AccessorGetter getter,
    v8::AccessorSetter setter)
  {  
    otl->SetAccessor(
      v8::String::NewSymbol(name), getter, setter);
  }
}
