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

#ifndef INCLUDED_NPX_ERROR_HH
#define INCLUDED_NPX_ERROR_HH

#include <stdexcept>

namespace npx {
  struct error : public std::runtime_error { 
    explicit error(const std::string& what_arg) 
      : std::runtime_error(what_arg) { } 
  };

  struct bad_argument : public error { 
    bad_argument(const std::string& signature);
  };

  struct type_error : public error { 
    type_error(const std::string& name, const std::string& type); 
  }; 

  struct property_missing : public error { 
    property_missing (const std::string& obj, const std::string& prop); 
  }; 

  struct bad_field_type : public error { 
    bad_field_type (const std::string& type);
  }; 

  struct pxlib_error : public error {
    pxlib_error (const std::string& what, int type);
  };

  struct not_opened : public error {
    not_opened ();
  };

  struct not_implemented : public error {
    not_implemented (const std::string& feature);
  };

  struct field_size_mismatch : public error {
    field_size_mismatch (const std::string& name, int required, int provided);
  };
}

#endif // INCLUDED_NPX_ERROR_HH
