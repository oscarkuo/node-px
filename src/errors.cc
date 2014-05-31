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

#include <sstream>
#include <npx/errors.hh>

namespace {
  std::string make_bad_argument(const std::string& signature) {
    std::ostringstream oss;
    oss << "Bad argument for " << signature;
    return oss.str();	
  }

  std::string make_property_missing (const std::string& o, const std::string& p) {
    std::ostringstream oss;
    oss << "Property [" << p << "] is not defined in object [" << o << "]";
    return oss.str();	
  }

  std::string make_bad_field_type(const std::string& type) {
    std::ostringstream oss;
    oss << "Unknown pxfield type [" << type << "]";
    return oss.str();	
  }

  std::string make_type_error(const std::string& name, const std::string& type) {
    std::ostringstream oss;
    oss << "[" << name << "] is not of type [" << type << "]";
    return oss.str();	
  }

  std::string make_pxlib_error(const std::string& what, int type) {
    std::ostringstream oss;
    oss << "pxlib error [" << what << "] type [" << type << "]";
    return oss.str();	
  }

  std::string make_field_size_mismatch_error(
    const std::string& name, int required, int provided) 
  {
    std::ostringstream oss;
    oss << "[" << name << "] requires [" 
        << required << "] fields but ["
        << provided << "] provided";
    return oss.str();	
  }
}

namespace npx {
  bad_argument::bad_argument(const std::string& signature)
    : error(make_bad_argument(signature))
  { }

  type_error::type_error(const std::string& name, const std::string& type) 
    : error(make_type_error(name, type)) 
  { }

  property_missing::property_missing(const std::string& o, const std::string& p)
    : error(make_property_missing(o, p))
  { }

  bad_field_type::bad_field_type(const std::string& type) 
    : error(make_bad_field_type(type))
  { }

  pxlib_error::pxlib_error(const std::string& what, int type) 
    : error(make_pxlib_error(what, type))
  { }

  field_size_mismatch::field_size_mismatch(
    const std::string& name, int required, int provided)
    : error(make_field_size_mismatch_error(name, required, provided))
  { }

  not_opened::not_opened()
    : error("document not opened")
  { }

  not_implemented::not_implemented(const std::string& feature)
    : error(feature + " not implemented")
  { }
}
