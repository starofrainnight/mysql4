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

#include <signaldata/FireTrigOrd.hpp>
#include <RefConvert.hpp>

static
const char *
trigEvent(Uint32 i){
  switch(i){
  case TriggerEvent::TE_INSERT:
    return "insert";
    break;
  case TriggerEvent::TE_UPDATE:
    return "update";
    break;
  case TriggerEvent::TE_DELETE:
    return "delete";
    break;
  }
  return "UNKNOWN";
}

bool 
printFIRE_TRIG_ORD(FILE * output, const Uint32 * theData, Uint32 len, 
		   Uint16 receiverBlockNo)
{
  const FireTrigOrd * const sig = (FireTrigOrd *) theData;

  fprintf(output, " TriggerId: %d TriggerEvent: %s\n",
	  sig->getTriggerId(),
	  trigEvent(sig->getTriggerEvent()));
  fprintf(output, " UserRef: (%d, %d) User data: %x\n",
	  refToNode(sig->getUserRef()),
	  refToBlock(sig->getUserRef()),
	  sig->getConnectionPtr());
  fprintf(output, " Signal: PK=%d BEFORE=%d AFTER=%d\n",
	  sig->getNoOfPrimaryKeyWords(),
	  sig->getNoOfBeforeValueWords(),
	  sig->getNoOfAfterValueWords());
  
  return true;
}
