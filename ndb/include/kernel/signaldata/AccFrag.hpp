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

#ifndef ACC_FRAG_HPP
#define ACC_FRAG_HPP

#include "SignalData.hpp"

class AccFragReq {
  /**
   * Sender(s)
   */
  friend class Dblqh;

  /**
   * Receiver(s)
   */
  friend class Dbacc;
public:
  STATIC_CONST( SignalLength = 12 );

private:
  Uint32 userPtr;
  Uint32 userRef;
  Uint32 tableId;
  Uint32 reqInfo;
  Uint32 fragId;
  Uint32 localKeyLen;
  Uint32 maxLoadFactor;
  Uint32 minLoadFactor;
  Uint32 kValue;
  Uint32 lhFragBits;
  Uint32 lhDirBits;
  Uint32 keyLength;
};

class AccFragConf {
  /**
   * Sender(s)
   */
  friend class Dbacc;

  /**
   * Receiver(s)
   */
  friend class Dblqh;
public:
  STATIC_CONST( SignalLength = 7 );

private:
  Uint32 userPtr;
  Uint32 rootFragPtr;
  Uint32 fragId[2];
  Uint32 fragPtr[2];
  Uint32 rootHashCheck;
};

class AccFragRef {
  /**
   * Sender(s)
   */
  friend class Dbacc;

  /**
   * Receiver(s)
   */
  friend class Dblqh;
public:
  STATIC_CONST( SignalLength = 2 );

private:
  Uint32 userPtr;
  Uint32 errorCode;
};

#endif
