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

#include <npx/node.hh>
#include <npx/alias.hh>
#include <npx/errors.hh>
#include <npx/global.hh>
#include <npx/paradox.hh>
#include <npx/document.hh>

using namespace v8;
using namespace node;

namespace {
  void _npx_error_handler(pxdoc_t* p, int type, const char* msg, void* data) {
    throw npx::pxlib_error(msg, type);
  }

  pxdoc_t* _npx_new() {
    return PX_new3(_npx_error_handler, NULL, NULL, NULL, NULL);
  }
}

namespace npx {
  void global::node_initialise(object_h target) {
    HandleScope scope;
    attach_method("make", object_l::New(target), make_document);
    attach_method("open", object_l::New(target), open_document);
  }

  value_h global::open_document(const Arguments& args) {
    HandleScope scope;
    pxdoc_t* x = NULL;

    try {
      if (args.Length() != 1) {
        throw bad_argument("px.open(path)");
      }

      std::string p = to_cpp_string(args[0]->ToString());

      x = _npx_new();
      PX_open_file(x, p.c_str());

      object_h i = document::node_new_instance(args)->ToObject();
      ObjectWrap::Unwrap<document>(i)->set(x);
      return scope.Close(i);
    } catch (const std::exception& ex) {
      die(ex.what());
    } catch (...) {
      die("Unknown error");
    }

    return scope.Close(Undefined());
  }

  value_h global::make_document(const Arguments& args) {
    HandleScope scope;
    pxdoc_t* x = NULL;
    pxfield_t* f = NULL;

    try {
      if (args.Length() != 2) {
        throw bad_argument("px.make(fields, path)");
      }
      if (!args[0]->IsArray()) {
        throw type_error(to_cpp_string(args[0]), "array");
      }
      if (!args[1]->IsString()) {
        throw type_error(to_cpp_string(args[1]), "string");
      }

      array_h a = array_l::Cast(args[0]);
      std::string p = to_cpp_string(args[1]->ToString());

      x = _npx_new();
      f = static_cast<pxfield_t*>(x->malloc(x, a->Length()*sizeof(pxfield_t), "to_pxfields"));

      for (unsigned int i=0; i<a->Length(); ++i) {
        f[i] = to_px_field_spec(a->Get(i));
      }
    
      PX_create_file(x, f, a->Length(), p.c_str(), pxfFileTypNonIndexDB);

      object_h i = document::node_new_instance(args)->ToObject();
      ObjectWrap::Unwrap<document>(i)->set(x);
      return scope.Close(i);
    } catch (const std::exception& ex) {
      ThrowException(Exception::Error(String::New(ex.what())));
    } catch (...) {
      ThrowException(Exception::Error(String::New("Unknown error")));
    }

    if (f) x->free(x, f);
    return scope.Close(Undefined());
  }

}
