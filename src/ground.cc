#include "ground.h"
#include "nec_context.h"


GroundObj::GroundObj(Type type, double dielectic, double conductivity)
  : Object(), _type(type), _dielectric(dielectic), _conductivity(conductivity)
{
  // pass...
}

GroundObj::~GroundObj() {
  // pass...
}

GroundObj *
GroundObj::freeSpace() {
  return new GroundObj(FREE_SPACE);
}

GroundObj *
GroundObj::perfectGround() {
  return new GroundObj(PERFECT_GROUND);
}

GroundObj *
GroundObj::ground(double dielectric, double conductivity) {
  return new GroundObj(PARAM_GROUND, dielectric, conductivity);
}

void
GroundObj::construct(nec_context &ctx) {
  switch(_type) {
    case PERFECT_GROUND:
      ctx.gn_card(1, 0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0);
      //std::cerr << "Set perfect ground." << std::endl;
      break;
    case PARAM_GROUND:
      ctx.gn_card(0, 0, _dielectric, _conductivity, 0.0, 0.0, 0.0, 0.0);
      /*std::cerr << "Set parameterized ground with eps=" << _dielectric
                << " and cond=" << _conductivity << "." << std::endl;*/
      break;
    case FREE_SPACE:
    default:
      //std::cerr << "Set free-space model." << std::endl;
      ctx.gn_card(-1, 0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0);
      break;
  }
}
