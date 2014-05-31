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

#ifndef INCLUDED_NPX_TYPES_HH
#define INCLUDED_NPX_TYPES_HH

#include <node.h>
#include <v8.h>

namespace npx {
  typedef v8::Local<v8::Date> date_l;
  typedef v8::Handle<v8::Date> date_h;
  typedef v8::Persistent<v8::Date> date_p;

  typedef v8::Local<v8::Object> object_l;
  typedef v8::Handle<v8::Object> object_h;
  typedef v8::Persistent<v8::Object> object_p;

  typedef v8::Local<v8::Value> value_l;
  typedef v8::Handle<v8::Value> value_h;
  typedef v8::Persistent<v8::Value> value_p;

  typedef v8::Local<v8::Array> array_l;
  typedef v8::Handle<v8::Array> array_h;
  typedef v8::Persistent<v8::Array> array_p;

  typedef v8::Local<v8::String> string_l;
  typedef v8::Handle<v8::String> string_h;
  typedef v8::Persistent<v8::String> string_p;

  typedef v8::Local<v8::Integer> integer_l;
  typedef v8::Handle<v8::Integer> integer_h;
  typedef v8::Persistent<v8::Integer> integer_p;

  typedef v8::Local<v8::FunctionTemplate> function_template_l;
  typedef v8::Handle<v8::FunctionTemplate> function_template_h;
  typedef v8::Persistent<v8::FunctionTemplate> function_template_p;

  typedef v8::Local<v8::ObjectTemplate> object_template_l;
  typedef v8::Handle<v8::ObjectTemplate> object_template_h;
  typedef v8::Persistent<v8::ObjectTemplate> object_template_p;

  typedef v8::Local<v8::Function> function_l;
  typedef v8::Handle<v8::Function> function_h;
  typedef v8::Persistent<v8::Function> function_p;

  typedef v8::Local<v8::Template> template_l;
  typedef v8::Handle<v8::Template> template_h;
  typedef v8::Persistent<v8::Template> template_p;
}

#endif // INCLUDED_NPX_TYPES_HH
