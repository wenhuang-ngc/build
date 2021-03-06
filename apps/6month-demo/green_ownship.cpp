#include "ownship.h"
#include "sensors.h"
#include "rpc.h"

#include "hal_xdcomms.h"
#include "hal_gma.h"

// Depending on whether the subject is same or diff color, update may be local or xd
void OwnShip::update(Subject *s) {
}

void OwnShipShadow::update(Subject *s) {
  GpsSensor *gps = dynamic_cast<GpsSensor *>(s);
  if (!gps) {
    return;
  }
  Position position  = gps->getPosition();
  position_datatype pos;
  pos.x = position._x;
  pos.y = position._y;
  pos.z = position._z;
  pos.trailer.seq = seq;
  pos.trailer.rqr = rqr;
  pos.trailer.oid = oid;
  pos.trailer.mid = mid;
  pos.trailer.crc = crc;

  gaps_tag  t_tag, r_tag;
  uint32_t  t_mux = 1, t_sec = 1, type = DATA_TYP_POSITION;

  tag_write(&t_tag, t_mux, t_sec, type);
  xdc_asyn_send(&pos, t_tag);
}
