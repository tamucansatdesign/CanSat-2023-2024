#pragma once
#ifndef __STATES_H__
#define __STATES_H__

// State logic
namespace States
{
  extern uint16_t EE_STATE;

  // ordered by state diagram
  // Bootup 0
  void Initialization(Common::Payload_States &pay_states);  // 1
  void Standby(Common::Payload_States &pay_states);         // 2 
  void Deployment(Common::Payload_States &pay_states);      // 3
  void Landing(Common::Payload_States &pay_states);         // 4
}
#endif  