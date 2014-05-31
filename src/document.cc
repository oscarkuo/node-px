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
#include <npx/paradox.hh>
#include <npx/document.hh>

#include <cstdlib>
#include <cstring>

namespace npx {
  function_template_p document::constructor;

  document::document() { 
  }

  document::~document() {
    release_if_opened(d_);
  }

  void document::node_initialise(object_h exports) {
    constructor = new_class("NpxDocument", node_new);
    attach_property("fields", constructor->PrototypeTemplate(), getter_fields);
    attach_method("close", constructor->PrototypeTemplate(), method_close);
    attach_method("count", constructor->PrototypeTemplate(), method_count);
    attach_method("put", constructor->PrototypeTemplate(), method_put);
    attach_method("get", constructor->PrototypeTemplate(), method_get);
    exports->Set(v8::String::NewSymbol("NpxDocument"), constructor->GetFunction());
  }

  value_h document::node_new(const v8::Arguments& args) {
    v8::HandleScope scope;
    (new npx::document())->Wrap(args.This());
    return args.This();
  }

  value_h document::node_new_instance(const v8::Arguments& args) {
    v8::HandleScope scope;
    return scope.Close(constructor->GetFunction()->NewInstance());
  }

  value_h document::method_close(const v8::Arguments& args) {
    document* d = node::ObjectWrap::Unwrap<document>(args.This());
    if (d) {
      release_if_opened(d->d_);
    }
    return v8::Undefined();
  }

  value_h document::method_get(const v8::Arguments& args) {
    v8::HandleScope scope;
    document* d = node::ObjectWrap::Unwrap<document>(args.This());

    pxval_t** dptr = 0;
    uint32_t fields_count = 0;

    try {
      if (!d->d_) throw not_opened();
      if (args.Length() != 1) throw bad_argument("px.document.get(index)");
      if (!args[0]->IsInt32()) throw type_error(to_cpp_string(args[0]), "integer");
      fields_count = PX_get_num_fields(d->d_);
      int32_t i = args[0]->Int32Value();

      dptr = PX_retrieve_record(d->d_, i);
      object_l o = v8::Object::New();

      for (uint32_t i=0; i<fields_count; ++i) {
        pxfield_t* f = PX_get_field(d->d_, i); // don't free
        string_h k = v8::String::New(f->px_fname);
        value_h v = to_v8_value(d->d_, *f, *dptr[i]);
        o->Set(k, v);
      }
      
      return scope.Close(o);
    } catch (const std::exception& e) {
      die(e.what());
    }

    free_pxvals(dptr, fields_count); 

    return v8::Undefined();
  }

  value_h document::method_put(const v8::Arguments& args) {
    document* d = node::ObjectWrap::Unwrap<document>(args.This());

    pxval_t** dptr = 0;
    uint32_t fields_count = 0;

    try {
      if (!d->d_) throw not_opened();

      fields_count = PX_get_num_fields(d->d_);

      if (args.Length() != 1) throw bad_argument("px.document.put(values)");
      if (!args[0]->IsArray()) throw type_error(to_cpp_string(args[0]), "array");

      array_l a = array_l::Cast(args[0]);

      if (a->Length() < fields_count) {
        throw field_size_mismatch(
          "px.document.insert(values)", fields_count, a->Length());
      }

      dptr = static_cast<pxval_t**>(std::malloc(fields_count*sizeof(pxval_t *)));
      std::memset(dptr, 0, fields_count*sizeof(pxval_t*));

      for(uint32_t i=0; i<fields_count; ++i) {
        pxfield_t* f = PX_get_field(d->d_, i); // don't free
        npx::value_l v = a->Get(i);
        MAKE_PXVAL(d->d_, dptr[i]);
        dptr[i] = to_px_value(d->d_, *f, v);
      }

      PX_insert_record(d->d_, dptr);
    } catch (const std::exception& e) {
      die(e.what());
    }

    free_pxvals(dptr, fields_count);   

    return v8::Undefined();
  }
 
  value_h document::method_count(const v8::Arguments& args) {
    v8::HandleScope scope;
    document* d = node::ObjectWrap::Unwrap<document>(args.This());

    try {
      if (!d->d_) throw not_opened();
      integer_l c = v8::Integer::New(PX_get_num_records(d->d_));
      return scope.Close(c);
    } catch (const std::exception& e) {
      die(e.what());
    }

    return v8::Undefined();
  }

  value_h document::getter_fields(string_l s, const v8::AccessorInfo& i) {
    v8::HandleScope scope;
    document* d = node::ObjectWrap::Unwrap<document>(i.This());

    try {
      if (!d->d_) throw not_opened();

      int c = PX_get_num_fields(d->d_);
      array_l a = v8::Array::New(c);

      for (int i=0; i<c; ++i) {
        // returns a pointer to field spec in pxdoc, don't free
        pxfield_t* f = PX_get_field(d->d_, i); 
        object_h o = to_v8_field_spec(*f);
        a->Set(i, o);
      }

      return scope.Close(a);
    } catch (const std::exception& e) {
      die(e.what());
    }
 
    return v8::Undefined();
  }
}
