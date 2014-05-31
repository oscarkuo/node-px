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

#include <string>
#include <cstring>
#include <sstream>

#include <npx/node.hh>
#include <npx/errors.hh>
#include <npx/paradox.hh>

#include <ctime>
#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <iostream>

#define NPX_FTYPE(t) { #t, t }

namespace {
  char* clone_cstr(const char* s, int n) {
    int l = n+1; // for null char
    char* c = static_cast<char*>(std::malloc(l));
    if (!c) return NULL;
    std::memset(c, 0, l);
    std::memcpy(c, s, n);
    return c;
  }

  template <typename ValueT, typename LimitT>
  bool is_within_limits(ValueT value) {
    return  (value <= std::numeric_limits<LimitT>::max()) &&
            (value >= std::numeric_limits<LimitT>::min()) ;
  }

  const int pxft_count = 17;
  
  typedef struct pxft_ {
    std::string name;
    char type;
  } pxft_t;

  pxft_t known_pxfts[pxft_count] = {
    NPX_FTYPE(pxfAlpha),
    NPX_FTYPE(pxfDate),
    NPX_FTYPE(pxfShort),
    NPX_FTYPE(pxfLong),
    NPX_FTYPE(pxfCurrency),
    NPX_FTYPE(pxfNumber),
    NPX_FTYPE(pxfLogical),
    NPX_FTYPE(pxfMemoBLOb),
    NPX_FTYPE(pxfBLOb),
    NPX_FTYPE(pxfFmtMemoBLOb),
    NPX_FTYPE(pxfOLE),
    NPX_FTYPE(pxfGraphic),
    NPX_FTYPE(pxfTime),
    NPX_FTYPE(pxfTimestamp),
    NPX_FTYPE(pxfAutoInc),
    NPX_FTYPE(pxfBCD),
    NPX_FTYPE(pxfBytes),
  };

  const char* cstr_from_px_type(char v) {
    for (int i=0; i<pxft_count; ++i) {
      if (known_pxfts[i].type == v)
        return known_pxfts[i].name.c_str();
    }
    return NULL;
  }

  bool is_value_valid_for_field_spec(const pxfield_t& f, npx::value_h v) {
    switch (f.px_ftype) {
    case pxfShort:
      return is_within_limits<int32_t, short>(v->Int32Value());
    case pxfLong:
      return is_within_limits<int32_t, long>(v->Int32Value());
    case pxfLogical:
      return true;
    case pxfAlpha:
      return v->ToString()->Length() <= f.px_flen;
    case pxfTimestamp:
      return true;
    case pxfDate:
      return true;
    }

    throw npx::not_implemented(cstr_from_px_type(f.px_ftype));
  }

  bool is_value_type_valid_for_field_spec(const pxfield_t& f, npx::value_h v) {
    switch (f.px_ftype) {
    case pxfLong:
    case pxfShort:
      return v->IsInt32();
    case pxfLogical:
      return v->IsBoolean();
    case pxfAlpha:
      return v->IsString();
    case pxfTimestamp:
      return v->IsDate();
    case pxfDate:
      return v->IsDate();
    }

    throw npx::not_implemented(cstr_from_px_type(f.px_ftype));
  }

  pxval_t* new_pxval(pxdoc_t* d) {
    pxval_t* to;
    MAKE_PXVAL(d, to);
    std::memset(to, 0, sizeof(pxval_t));
    return to;
  }

  pxval_t* to_px_date(pxdoc_t* d, npx::value_h from) {
    time_t dt = NODE_V8_UNIXTIME(from);
    std::tm* lt = std::localtime(&dt);
    return PX_make_date(
      d, lt->tm_year, lt->tm_mon+1, lt->tm_mday); 
  }

  pxval_t* to_px_timestamp(pxdoc_t* d, npx::value_h from) {
    time_t dt = NODE_V8_UNIXTIME(from);
    std::tm* lt = std::localtime(&dt);
    return PX_make_timestamp(
      d,
      lt->tm_year, lt->tm_mon+1, lt->tm_mday, 
      lt->tm_hour, lt->tm_min, lt->tm_sec); 
  }

  pxval_t* to_px_boolean(pxdoc_t* d, bool from) {
    pxval_t* to = new_pxval(d);
    to->value.lval = from;
    return to;
  }

  pxval_t* to_px_integer(pxdoc_t* d, int32_t from) {
    pxval_t* to = new_pxval(d);
    to->value.lval = from;
    return to;
  }

  pxval_t* to_px_numeric(pxdoc_t* d, double from) {
    pxval_t* to = new_pxval(d);
    to->value.dval = from;
    return to;
  }

  pxval_t* to_px_string(pxdoc_t* d, const std::string& from) {
    pxval_t* to = new_pxval(d);
    to->value.str.len = from.length();
    to->value.str.val = clone_cstr(from.c_str(), from.length());
    return to;
  }
 
  npx::string_h to_v8_field_type(char v) {
    v8::HandleScope scope;

    const char* name = cstr_from_px_type(v);
    if (name == NULL) {
      std::ostringstream oss;
      oss << v;
      throw npx::bad_field_type(oss.str());
    }

    return v8::String::New(name);
  }

  char to_px_field_type(npx::string_h v) {
    v8::HandleScope scope;
    v8::String::Utf8Value s(v);

    for (int i=0; i<pxft_count; ++i) {
      if (!known_pxfts[i].name.compare(*s))
        return known_pxfts[i].type;
    }

    throw npx::bad_field_type(*s);
  }
}

namespace npx {
  npx::value_h to_v8_date(double value) {
    struct tm ta;
    int secs, days;

    // 86400 - number of seconds per day
    // 1721425 - number of days since begining of julian calendar 4714 BC

    value = value / 1000.0;
    days = (int) (value / 86400);
    secs = (int) std::fmod(value, 86400);
    PX_SdnToGregorian(days+1721425, &ta.tm_year, &ta.tm_mon, &ta.tm_mday);
    ta.tm_mon--;
    ta.tm_hour = secs/3600;
    ta.tm_min = secs/60%60;
    ta.tm_sec = secs%60;

    time_t tt = mktime(&ta);
    return NODE_UNIXTIME_V8(tt);
  }
 
  pxfield_t to_px_field_spec(value_h from) {
    v8::HandleScope scope;

    if (!from->IsObject()) {
      throw npx::type_error(npx::to_cpp_string(from->ToDetailString()), "object");
    }
    
    npx::object_l o = from->ToObject();
    npx::string_l name = npx::get_value<v8::String>(o, v8::String::New("name"));
    npx::string_l type = npx::get_value<v8::String>(o, v8::String::New("type"));
    npx::integer_l bytes = v8::Integer::New(0);  
    npx::integer_l digits = v8::Integer::New(0);

    char ft = ::to_px_field_type(type);

    switch (ft) {
    case pxfShort:
      bytes = v8::Integer::New(2);
      break;
    case pxfLong:
    case pxfDate:
    case pxfTime:
    case pxfAutoInc:
      bytes = v8::Integer::New(4);
      break;
    case pxfNumber:
    case pxfCurrency:
    case pxfTimestamp:
      bytes = v8::Integer::New(8);
      break;
    case pxfLogical:
      bytes = v8::Integer::New(1);
      break;
    case pxfBCD:
      bytes = npx::get_value<v8::Integer>(o, v8::String::New("size")); 
      digits = npx::get_value<v8::Integer>(o, v8::String::New("digits"));
      break;
    default:
      bytes = npx::get_value<v8::Integer>(o, v8::String::New("size"));
      break;
    }

    pxfield_t to;
    std::string s = npx::to_cpp_string(name);
    to.px_fname = clone_cstr(s.c_str(), s.length());
    to.px_ftype = ft;
    to.px_flen = bytes->Value();
    to.px_fdc = digits->Value();

    return to;
  }

  void release_if_opened(pxdoc_t*& d) {
    if (d) {
      if (d->px_stream && d->px_stream->close) {
        PX_close(d);
      }
      PX_delete(d);
      d = NULL;
    }
  }

  npx::value_h to_v8_value(pxdoc_t* d, const pxfield_t& f, const pxval_t& from) {
    if (from.isnull) return v8::Null();

    switch (f.px_ftype) {
    case pxfLong:
    case pxfShort:
      return v8::Integer::New(from.value.lval);
    case pxfLogical:
      return v8::Boolean::New(from.value.lval != 0);
    case pxfAlpha:
      return v8::String::New(from.value.str.val);
    case pxfTimestamp:
      return to_v8_date(from.value.dval);
    case pxfDate:
      return to_v8_date(from.value.lval*86400.0*1000.0);
    }

    throw npx::not_implemented(cstr_from_px_type(f.px_ftype));
  }

  pxval_t* to_px_value(pxdoc_t* d, const pxfield_t& f, npx::value_h from) {
    if (!is_value_type_valid_for_field_spec(f, from)) {
      throw npx::type_error(
        to_cpp_string(from), 
        cstr_from_px_type(f.px_ftype));
    }

    if (!is_value_valid_for_field_spec(f, from)) {
      std::ostringstream oss;
      oss << "[" << npx::to_cpp_string(from)
          << "] is out of range for type [";

      switch (f.px_ftype) {
      case pxfAlpha:
          oss << cstr_from_px_type(f.px_ftype) 
              << "(" << f.px_flen << ")";
          break;
      default:
          oss << cstr_from_px_type(f.px_ftype);
          break;
      }
      oss << "]";
      throw std::overflow_error(oss.str());
    }

    //TODO: handle NULL for each type?

    pxval_t* to = NULL;

    switch (f.px_ftype) {
      case pxfLong:
      case pxfShort:
        to = to_px_integer(d, from->Int32Value());
        break;
      case pxfLogical:
        to = to_px_boolean(d, from->BooleanValue() == true);
        break;
      case pxfAlpha:
        to = to_px_string(d, to_cpp_string(from));
        break;
      case pxfTimestamp:
        to = to_px_timestamp(d, from);
        break;
      case pxfDate:
        to = to_px_date(d, from);
        break;
      default:
        throw npx::not_implemented(cstr_from_px_type(f.px_ftype));
    }

    return to;
  }

  object_h to_v8_field_spec(pxfield_t& f) {
    v8::HandleScope scope;
    object_h o = v8::Object::New();
    o->Set(v8::String::New("name"), v8::String::New(f.px_fname));
    o->Set(v8::String::New("type"), string_l::New(to_v8_field_type(f.px_ftype)));
    o->Set(v8::String::New("size"), v8::Integer::New(f.px_flen));
    o->Set(v8::String::New("digits"), v8::Integer::New(f.px_fdc));
    return scope.Close(o);
  }

  void free_pxval(pxval_t* p) {
    if (!p) return;
    switch (p->type) {
    case pxfAlpha:
      if (!p->isnull) {
        std::free(p->value.str.val);
      }
      break;
    }

    std::free(p);
  }

  void free_pxvals(pxval_t** p, uint32_t c) {
    if (!p) return;
    for (uint32_t i=0; i<c; ++i) free_pxval(p[i]);
    std::free(p);
  }
}
