#include <canopen_object_dict.h>

CanopenNode dashboard;         // ECM
CanopenNode inverter;        // ECM
CanopenNode ecm;               // MPPTs, Inverters, Dashboard, Lights controller
CanopenNode lights_controller; // ECM
CanopenNode bms;               // ECM

void CanopenObjectDictInit() {
  dashboard.frameId = "70";

  inverter.frameId = "290";

  ecm.frameId = "01";

  bms.frameId = "185";

  lights_controller.frameId = "581";
}
