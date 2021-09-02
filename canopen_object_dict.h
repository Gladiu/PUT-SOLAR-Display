#ifndef CANOPEN_OBJECT_DICT_H
#define CANOPEN_OBJECT_DICT_H

#endif // CANOPEN_OBJECT_DICT_H
#include <cstdint>
#include <string>
//#include <QObject>



/*******************************************************************************
 DEVICE INFO:
 VendorName:     STMicroelectronics
 ProductName:    STM32F407VGT6
 *******************************************************************************/


typedef struct {
  std::string frameId;
} CanopenNode;

/*******************************************************************************
 OBJECT DICTIONARY
 *******************************************************************************/
#define CO_OD_NoOfElements 9
extern CanopenNode dashboard;  // ECM
extern CanopenNode inverter; // ECM
extern CanopenNode ecm;        // MPPTs, Inverters, Dashboard, Lights controller
extern CanopenNode lightsController; // ECM
extern CanopenNode bms;               // ECM

void CanopenObjectDictInit();
