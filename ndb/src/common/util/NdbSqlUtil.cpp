/* Copyright (C) 2003 MySQL AB

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#include <NdbSqlUtil.hpp>

int
NdbSqlUtil::char_compare(const char* s1, unsigned n1,
                         const char* s2, unsigned n2, bool padded)
{
  int c1 = 0;
  int c2 = 0;
  unsigned i = 0;
  while (i < n1 || i < n2) {
    c1 = i < n1 ? s1[i] : padded ? 0x20 : 0;
    c2 = i < n2 ? s2[i] : padded ? 0x20 : 0;
    if (c1 != c2)
      break;
    i++;
  }
  return c1 - c2;
}

bool
NdbSqlUtil::char_like(const char* s1, unsigned n1,
                      const char* s2, unsigned n2, bool padded)
{
  int c1 = 0;
  int c2 = 0;
  unsigned i1 = 0;
  unsigned i2 = 0;
  while (i1 < n1 || i2 < n2) {
    c1 = i1 < n1 ? s1[i1] : padded ? 0x20 : 0;
    c2 = i2 < n2 ? s2[i2] : padded ? 0x20 : 0;
    if (c2 == '%') {
      while (i2 + 1 < n2 && s2[i2 + 1] == '%') {
        i2++;
      }
      unsigned m = 0;
      while (m <= n1 - i1) {
        if (char_like(s1 + i1 + m, n1 -i1 - m,
                      s2 + i2 + 1, n2 - i2 - 1, padded))
        return true;
        m++;
      }
      return false;
    }
    if (c2 == '_') {
      if (c1 == 0)
        return false;
    } else {
      if (c1 != c2)
        return false;
    }
    i1++;
    i2++;
  }
  return i1 == n2 && i2 == n2;
}

/**
 * Data types.
 */

const NdbSqlUtil::Type
NdbSqlUtil::m_typeList[] = {
  { // 0
    Type::Undefined,
    NULL
  },
  { // 1
    Type::Tinyint,
    cmpTinyint
  },
  { // 2
    Type::Tinyunsigned,
    cmpTinyunsigned
  },
  { // 3
    Type::Smallint,
    cmpSmallint
  },
  { // 4
    Type::Smallunsigned,
    cmpSmallunsigned
  },
  { // 5
    Type::Mediumint,
    cmpMediumint
  },
  { // 6
    Type::Mediumunsigned,
    cmpMediumunsigned
  },
  { // 7
    Type::Int,
    cmpInt
  },
  { // 8
    Type::Unsigned,
    cmpUnsigned
  },
  { // 9
    Type::Bigint,
    cmpBigint
  },
  { // 10
    Type::Bigunsigned,
    cmpBigunsigned
  },
  { // 11
    Type::Float,
    cmpFloat
  },
  { // 12
    Type::Double,
    cmpDouble
  },
  { // 13
    Type::Olddecimal,
    cmpOlddecimal
  },
  { // 14
    Type::Char,
    cmpChar
  },
  { // 15
    Type::Varchar,
    cmpVarchar
  },
  { // 16
    Type::Binary,
    cmpBinary
  },
  { // 17
    Type::Varbinary,
    cmpVarbinary
  },
  { // 18
    Type::Datetime,
    cmpDatetime
  },
  { // 19
    Type::Date,
    cmpDate
  },
  { // 20
    Type::Blob,
    cmpBlob
  },
  { // 21
    Type::Text,
    cmpText
  },
  { // 22
    Type::Undefined,    // 5.0 Bit
    NULL
  },
  { // 23
    Type::Undefined,    // 5.0 Longvarchar
    NULL
  },
  { // 24
    Type::Undefined,    // 5.0 Longvarbinary
    NULL
  },
  { // 25
    Type::Time,
    cmpTime
  },
  { // 26
    Type::Year,
    cmpYear
  },
  { // 27
    Type::Timestamp,
    cmpTimestamp
  },
  { // 28
    Type::Olddecimalunsigned,
    cmpOlddecimalunsigned
  }
};

const NdbSqlUtil::Type&
NdbSqlUtil::getType(Uint32 typeId)
{
  if (typeId < sizeof(m_typeList) / sizeof(m_typeList[0]) &&
      m_typeList[typeId].m_typeId != Type::Undefined) {
    return m_typeList[typeId];
  }
  return m_typeList[Type::Undefined];
}

const NdbSqlUtil::Type&
NdbSqlUtil::getTypeBinary(Uint32 typeId)
{
  switch (typeId) {
  case Type::Char:
    typeId = Type::Binary;
    break;
  case Type::Varchar:
    typeId = Type::Varbinary; 
    break;
  case Type::Text:
    typeId = Type::Blob;
    break;
  default:
    break;
  }
  return getType(typeId);
}

// compare

int
NdbSqlUtil::cmpTinyint(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  assert(full >= size && size > 0);
  union { Uint32 p[1]; Int8 v; } u1, u2;
  u1.p[0] = p1[0];
  u2.p[0] = p2[0];
  if (u1.v < u2.v)
    return -1;
  if (u1.v > u2.v)
    return +1;
  return 0;
}

int
NdbSqlUtil::cmpTinyunsigned(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  assert(full >= size && size > 0);
  union { Uint32 p[1]; Uint8 v; } u1, u2;
  u1.p[0] = p1[0];
  u2.p[0] = p2[0];
  if (u1.v < u2.v)
    return -1;
  if (u1.v > u2.v)
    return +1;
  return 0;
}

int
NdbSqlUtil::cmpSmallint(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  assert(full >= size && size > 0);
  union { Uint32 p[1]; Int16 v; } u1, u2;
  u1.p[0] = p1[0];
  u2.p[0] = p2[0];
  if (u1.v < u2.v)
    return -1;
  if (u1.v > u2.v)
    return +1;
  return 0;
}

int
NdbSqlUtil::cmpSmallunsigned(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  assert(full >= size && size > 0);
  union { Uint32 p[1]; Uint16 v; } u1, u2;
  u1.p[0] = p1[0];
  u2.p[0] = p2[0];
  if (u1.v < u2.v)
    return -1;
  if (u1.v > u2.v)
    return +1;
  return 0;
}

int
NdbSqlUtil::cmpMediumint(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  assert(full >= size && size > 0);
  union { const Uint32* p; const unsigned char* v; } u1, u2;
  u1.p = p1;
  u2.p = p2;
  Int32 v1 = sint3korr(u1.v);
  Int32 v2 = sint3korr(u2.v);
  if (v1 < v2)
    return -1;
  if (v1 > v2)
    return +1;
  return 0;
}

int
NdbSqlUtil::cmpMediumunsigned(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  assert(full >= size && size > 0);
  union { const Uint32* p; const unsigned char* v; } u1, u2;
  u1.p = p1;
  u2.p = p2;
  Uint32 v1 = uint3korr(u1.v);
  Uint32 v2 = uint3korr(u2.v);
  if (v1 < v2)
    return -1;
  if (v1 > v2)
    return +1;
  return 0;
}

int
NdbSqlUtil::cmpInt(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  assert(full >= size && size > 0);
  union { Uint32 p[1]; Int32 v; } u1, u2;
  u1.p[0] = p1[0];
  u2.p[0] = p2[0];
  if (u1.v < u2.v)
    return -1;
  if (u1.v > u2.v)
    return +1;
  return 0;
}

int
NdbSqlUtil::cmpUnsigned(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  assert(full >= size && size > 0);
  union { Uint32 p[1]; Uint32 v; } u1, u2;
  u1.v = p1[0];
  u2.v = p2[0];
  if (u1.v < u2.v)
    return -1;
  if (u1.v > u2.v)
    return +1;
  return 0;
}

int
NdbSqlUtil::cmpBigint(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  assert(full >= size && size > 0);
  if (size >= 2) {
    union { Uint32 p[2]; Int64 v; } u1, u2;
    u1.p[0] = p1[0];
    u1.p[1] = p1[1];
    u2.p[0] = p2[0];
    u2.p[1] = p2[1];
    if (u1.v < u2.v)
      return -1;
    if (u1.v > u2.v)
      return +1;
    return 0;
  }
  return CmpUnknown;
}

int
NdbSqlUtil::cmpBigunsigned(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  assert(full >= size && size > 0);
  if (size >= 2) {
    union { Uint32 p[2]; Uint64 v; } u1, u2;
    u1.p[0] = p1[0];
    u1.p[1] = p1[1];
    u2.p[0] = p2[0];
    u2.p[1] = p2[1];
    if (u1.v < u2.v)
      return -1;
    if (u1.v > u2.v)
      return +1;
    return 0;
  }
  return CmpUnknown;
}

int
NdbSqlUtil::cmpFloat(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  assert(full >= size && size > 0);
  union { Uint32 p[1]; float v; } u1, u2;
  u1.p[0] = p1[0];
  u2.p[0] = p2[0];
  // no format check
  if (u1.v < u2.v)
    return -1;
  if (u1.v > u2.v)
    return +1;
  return 0;
}

int
NdbSqlUtil::cmpDouble(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  assert(full >= size && size > 0);
  if (size >= 2) {
    union { Uint32 p[2]; double v; } u1, u2;
    u1.p[0] = p1[0];
    u1.p[1] = p1[1];
    u2.p[0] = p2[0];
    u2.p[1] = p2[1];
    // no format check
    if (u1.v < u2.v)
      return -1;
    if (u1.v > u2.v)
      return +1;
    return 0;
  }
  return CmpUnknown;
}

int
NdbSqlUtil::cmp_olddecimal(const uchar* s1, const uchar* s2, unsigned n)
{
  int sgn = +1;
  unsigned i = 0;
  while (i < n) {
    int c1 = s1[i];
    int c2 = s2[i];
    if (c1 == c2) {
      if (c1 == '-')
        sgn = -1;
    } else if (c1 == '-') {
      return -1;
    } else if (c2 == '-') {
      return +1;
    } else if (c1 < c2) {
      return -1 * sgn;
    } else {
      return +1 * sgn;
    }
    i++;
  }
  return 0;
}

int
NdbSqlUtil::cmpOlddecimal(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  assert(full >= size && size > 0);
  if (full == size) {
    union { const Uint32* p; const uchar* v; } u1, u2;
    u1.p = p1;
    u2.p = p2;
    return cmp_olddecimal(u1.v, u2.v, full << 2);
  }
  return CmpUnknown;
}

int
NdbSqlUtil::cmpOlddecimalunsigned(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  assert(full >= size && size > 0);
  if (full == size) {
    union { const Uint32* p; const uchar* v; } u1, u2;
    u1.p = p1;
    u2.p = p2;
    return cmp_olddecimal(u1.v, u2.v, full << 2);
  }
  return CmpUnknown;
}

int
NdbSqlUtil::cmpChar(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  // collation does not work on prefix for some charsets
  assert(full == size && size > 0);
  /*
   * Char is blank-padded to length and null-padded to word size.
   */
  union { const Uint32* p; const uchar* v; } u1, u2;
  u1.p = p1;
  u2.p = p2;
  // not const in MySQL
  CHARSET_INFO* cs = (CHARSET_INFO*)(info);
  // length in bytes including null padding to Uint32
  uint l1 = (full << 2);
  int k = (*cs->coll->strnncollsp)(cs, u1.v, l1, u2.v, l1);
  return k < 0 ? -1 : k > 0 ? +1 : 0;
}

int
NdbSqlUtil::cmpVarchar(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  assert(full >= size && size > 0);
  /*
   * Varchar is not allowed to contain a null byte and the value is
   * null-padded.  Therefore comparison does not need to use the length.
   *
   * Not used before MySQL 5.0.  Format is likely to change.  Handle
   * only binary collation for now.
   */
  union { const Uint32* p; const char* v; } u1, u2;
  u1.p = p1;
  u2.p = p2;
  // skip length in first 2 bytes
  int k = strncmp(u1.v + 2, u2.v + 2, (size << 2) - 2);
  return k < 0 ? -1 : k > 0 ? +1 : full == size ? 0 : CmpUnknown;
}

int
NdbSqlUtil::cmpBinary(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  assert(full >= size && size > 0);
  /*
   * Binary data of full length.  Compare bytewise.
   */
  union { const Uint32* p; const unsigned char* v; } u1, u2;
  u1.p = p1;
  u2.p = p2;
  int k = memcmp(u1.v, u2.v, size << 2);
  return k < 0 ? -1 : k > 0 ? +1 : full == size ? 0 : CmpUnknown;
}

int
NdbSqlUtil::cmpVarbinary(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  assert(full >= size && size > 0);
  /*
   * Binary data of variable length padded with nulls.  The comparison
   * does not need to use the length.
   *
   * Not used before MySQL 5.0.  Format is likely to change.
   */
  union { const Uint32* p; const unsigned char* v; } u1, u2;
  u1.p = p1;
  u2.p = p2;
  // skip length in first 2 bytes
  int k = memcmp(u1.v + 2, u2.v + 2, (size << 2) - 2);
  return k < 0 ? -1 : k > 0 ? +1 : full == size ? 0 : CmpUnknown;
}

int
NdbSqlUtil::cmpDatetime(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  assert(full >= size && size > 0);
  if (size >= 2) {
    union { Uint32 p[2]; Int64 v; } u1, u2;
    u1.p[0] = p1[0];
    u1.p[1] = p1[1];
    u2.p[0] = p2[0];
    u2.p[1] = p2[1];
    if (u1.v < u2.v)
      return -1;
    if (u1.v > u2.v)
      return +1;
    return 0;
  }
  return CmpUnknown;
}

int
NdbSqlUtil::cmpDate(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
#ifdef ndb_date_is_4_byte_native_int
  assert(full >= size && size > 0);
  union { Uint32 p[2]; Int32 v; } u1, u2;
  u1.p[0] = p1[0];
  u2.p[0] = p2[0];
  if (u1.v < u2.v)
    return -1;
  if (u1.v > u2.v)
    return +1;
  return 0;
#else
  assert(full >= size && size > 0);
  union { const Uint32* p; const unsigned char* v; } u1, u2;
  u1.p = p1;
  u2.p = p2;
#ifdef ndb_date_sol9x86_cc_xO3_madness
  // from Field_newdate::val_int
  Uint64 j1 = uint3korr(u1.v);
  Uint64 j2 = uint3korr(u2.v);
  j1 = (j1 % 32L)+(j1 / 32L % 16L)*100L + (j1/(16L*32L))*10000L;
  j2 = (j2 % 32L)+(j2 / 32L % 16L)*100L + (j2/(16L*32L))*10000L;
  if (j1 < j2)
    return -1;
  if (j1 > j2)
    return +1;
  return 0;
#else
  uint j1 = uint3korr(u1.v);
  uint j2 = uint3korr(u2.v);
  uint d1 = (j1 & 31);
  uint d2 = (j2 & 31);
  j1 = (j1 >> 5);
  j2 = (j2 >> 5);
  uint m1 = (j1 & 15);
  uint m2 = (j2 & 15);
  j1 = (j1 >> 4);
  j2 = (j2 >> 4);
  uint y1 = j1;
  uint y2 = j2;
  if (y1 < y2)
    return -1;
  if (y1 > y2)
    return +1;
  if (m1 < m2)
    return -1;
  if (m1 > m2)
    return +1;
  if (d1 < d2)
    return -1;
  if (d1 > d2)
    return +1;
  return 0;
#endif
#endif
}

int
NdbSqlUtil::cmpBlob(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  assert(full >= size && size > 0);
  /*
   * Blob comparison is on the inline bytes (null padded).
   */
  const unsigned head = NDB_BLOB_HEAD_SIZE;
  // skip blob head
  if (size >= head + 1) {
    union { const Uint32* p; const unsigned char* v; } u1, u2;
    u1.p = p1 + head;
    u2.p = p2 + head;
    int k = memcmp(u1.v, u2.v, (size - head) << 2);
    return k < 0 ? -1 : k > 0 ? +1 : full == size ? 0 : CmpUnknown;
  }
  return CmpUnknown;
}

int
NdbSqlUtil::cmpText(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  // collation does not work on prefix for some charsets
  assert(full == size && size > 0);
  /*
   * Text comparison is on the inline bytes (blank padded).  Currently
   * not supported for multi-byte charsets.
   */
  const unsigned head = NDB_BLOB_HEAD_SIZE;
  // skip blob head
  if (size >= head + 1) {
    union { const Uint32* p; const uchar* v; } u1, u2;
    u1.p = p1 + head;
    u2.p = p2 + head;
    // not const in MySQL
    CHARSET_INFO* cs = (CHARSET_INFO*)(info);
    // length in bytes including null padding to Uint32
    uint l1 = (full << 2);
    int k = (*cs->coll->strnncollsp)(cs, u1.v, l1, u2.v, l1);
    return k < 0 ? -1 : k > 0 ? +1 : 0;
  }
  return CmpUnknown;
}

int
NdbSqlUtil::cmpTime(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  assert(full >= size && size > 0);
  union { const Uint32* p; const unsigned char* v; } u1, u2;
  u1.p = p1;
  u2.p = p2;
  // from Field_time::val_int
  Int32 j1 = sint3korr(u1.v);
  Int32 j2 = sint3korr(u2.v);
  if (j1 < j2)
    return -1;
  if (j1 > j2)
    return +1;
  return 0;
}

int
NdbSqlUtil::cmpYear(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  assert(full >= size && size > 0);
  union { const Uint32* p; const unsigned char* v; } u1, u2;
  u1.p = p1;
  u2.p = p2;
  if (u1.v[0] < u2.v[0])
    return -1;
  if (u1.v[0] > u2.v[0])
    return +1;
  return 0;
}

int
NdbSqlUtil::cmpTimestamp(const void* info, const Uint32* p1, const Uint32* p2, Uint32 full, Uint32 size)
{
  assert(full >= size && size > 0);
  union { Uint32 p[1]; Uint32 v; } u1, u2;
  u1.v = p1[0];
  u2.v = p2[0];
  if (u1.v < u2.v)
    return -1;
  if (u1.v > u2.v)
    return +1;
  return 0;
}

// check charset

bool
NdbSqlUtil::usable_in_pk(Uint32 typeId, const void* info)
{
  const Type& type = getType(typeId);
  switch (type.m_typeId) {
  case Type::Undefined:
    break;
  case Type::Char:
    {
      const CHARSET_INFO *cs = (const CHARSET_INFO*)info;
      return
        cs != 0 &&
        cs->cset != 0 &&
        cs->coll != 0 &&
        cs->coll->strnxfrm != 0 &&
        cs->strxfrm_multiply <= 1; // current limitation
    }
    break;
  case Type::Varchar:
    return true; // Varchar not used via MySQL
  case Type::Blob:
  case Type::Text:
    break;
  default:
    return true;
  }
  return false;
}

bool
NdbSqlUtil::usable_in_hash_index(Uint32 typeId, const void* info)
{
  return usable_in_pk(typeId, info);
}

bool
NdbSqlUtil::usable_in_ordered_index(Uint32 typeId, const void* info)
{
  const Type& type = getType(typeId);
  switch (type.m_typeId) {
  case Type::Undefined:
    break;
  case Type::Char:
    {
      const CHARSET_INFO *cs = (const CHARSET_INFO*)info;
      return
        cs != 0 &&
        cs->cset != 0 &&
        cs->coll != 0 &&
        cs->coll->strnxfrm != 0 &&
        cs->coll->strnncollsp != 0 &&
        cs->strxfrm_multiply <= 1; // current limitation
    }
    break;
  case Type::Varchar:
    return true; // Varchar not used via MySQL
  case Type::Text:
    {
      const CHARSET_INFO *cs = (const CHARSET_INFO*)info;
      return
        cs != 0 &&
        cs->mbmaxlen == 1 && // extra limitation
        cs->cset != 0 &&
        cs->coll != 0 &&
        cs->coll->strnxfrm != 0 &&
        cs->coll->strnncollsp != 0 &&
        cs->strxfrm_multiply <= 1; // current limitation
    }
    break;
  default:
    return true;
  }
  return false;
}

#ifdef NDB_SQL_UTIL_TEST

#include <NdbTick.h>
#include <NdbOut.hpp>

struct Testcase {
  int op;       // 1=compare 2=like
  int res;
  const char* s1;
  const char* s2;
  int pad;
};
const Testcase testcase[] = {
  { 2, 1, "abc", "abc", 0 },
  { 2, 1, "abc", "abc%", 0 },
  { 2, 1, "abcdef", "abc%", 0 },
  { 2, 1, "abcdefabcdefabcdef", "abc%", 0 },
  { 2, 1, "abcdefabcdefabcdef", "abc%f", 0 },
  { 2, 0, "abcdefabcdefabcdef", "abc%z", 0 },
  { 2, 1, "abcdefabcdefabcdef", "%f", 0 },
  { 2, 1, "abcdef", "a%b%c%d%e%f", 0 },
  { 0, 0, 0, 0 }
};

int
main(int argc, char** argv)
{
  ndb_init(); // for charsets
  unsigned count = argc > 1 ? atoi(argv[1]) : 1000000;
  ndbout_c("count = %u", count);
  assert(count != 0);
  for (const Testcase* t = testcase; t->s1 != 0; t++) {
    ndbout_c("%d = '%s' %s '%s' pad=%d",
        t->res, t->s1, t->op == 1 ? "comp" : "like", t->s2);
    NDB_TICKS x1 = NdbTick_CurrentMillisecond();
    unsigned n1 = strlen(t->s1);
    unsigned n2 = strlen(t->s2);
    for (unsigned i = 0; i < count; i++) {
      if (t->op == 1) {
        int res = NdbSqlUtil::char_compare(t->s1, n1, t->s2, n2, t->pad);
        assert(res == t->res);
        continue;
      }
      if (t->op == 2) {
        int res = NdbSqlUtil::char_like(t->s1, n1, t->s2, n2, t->pad);
        assert(res == t->res);
        continue;
      }
      assert(false);
    }
    NDB_TICKS x2 = NdbTick_CurrentMillisecond();
    if (x2 < x1)
      x2 = x1;
    double usec = 1000000.0 * double(x2 - x1) / double(count);
    ndbout_c("time %.0f usec per call", usec);
  }
  // quick check
  for (unsigned i = 0; i < sizeof(m_typeList) / sizeof(m_typeList[0]); i++) {
    const NdbSqlUtil::Type& t = m_typeList[i];
    assert(t.m_typeId == i);
  }
  return 0;
}

#endif
