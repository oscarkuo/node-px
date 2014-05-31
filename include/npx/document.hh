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

#ifndef INCLUDED_NPX_DOCUMENT_HH
#define INCLUDED_NPX_DOCUMENT_HH

#include <node.h>

#include <npx/alias.hh>
#include <npx/paradox.hh>

namespace npx {
  class document : public node::ObjectWrap {
  private:
    pxdoc_t* d_;
    static function_template_p constructor;

  public:
    static void node_initialise(object_h exports);
    static value_h node_new(const v8::Arguments& args);
    static value_h node_new_instance(const v8::Arguments& args);

  private: // exported methods and accessors 
    static value_h getter_fields (string_l s, const v8::AccessorInfo& i);

    static value_h method_get(const v8::Arguments& args);
    static value_h method_put(const v8::Arguments& args);

    static value_h method_close(const v8::Arguments& args);
    static value_h method_count(const v8::Arguments& args);

  public:
    document();
    virtual ~document();

    void set(pxdoc_t* d) { d_ = d; }
    pxdoc_t* get() const { return d_; }
  };
}

#endif // INCLUDED_NPX_DOCUMENT_HH
