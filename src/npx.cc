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
#include <v8.h>

#include <npx/alias.hh>
#include <npx/global.hh>
#include <npx/document.hh>

using namespace v8;
using namespace npx;
using namespace std;

void cleanup_module(void* arg) {
  PX_shutdown();
}

void initialise_module(object_h target) {
  PX_boot();
  node::AtExit(cleanup_module, NULL);

  global::node_initialise(target);
  document::node_initialise(target);
}

NODE_MODULE(px, initialise_module)
