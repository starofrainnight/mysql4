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

#ifndef COPY_ACTIVE_HPP
#define COPY_ACTIVE_HPP

#include "SignalData.hpp"

class CopyActiveReq {
  /**
   * Sender(s)
   */
  friend class Dbdih;

  /**
   * Receiver(s)
   */
  friend class Dblqh;
public:
  STATIC_CONST( SignalLength = 5 );

private:
  Uint32 userPtr;
  Uint32 userRef;
  Uint32 tableId;
  Uint32 fragId;
  Uint32 distributionKey;
};

class CopyActiveConf {
  /**
   * Sender(s)
   */
  friend class Dblqh;

  /**
   * Receiver(s)
   */
  friend class Dbdih;
public:
  STATIC_CONST( SignalLength = 5 );

private:
  Uint32 userPtr;
  Uint32 startingNodeId;
  Uint32 tableId;
  Uint32 fragId;
  Uint32 startGci;
};
class CopyActiveRef {
  /**
   * Sender(s)
   */
  friend class Dblqh;

  /**
   * Receiver(s)
   */
  friend class Dbdih;
public:
  STATIC_CONST( SignalLength = 5 );

private:
  Uint32 userPtr;
  Uint32 startingNodeId;
  Uint32 tableId;
  Uint32 fragId;
  Uint32 errorCode;
};

#endif
