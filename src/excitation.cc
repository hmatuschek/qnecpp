#include "excitation.h"
#include "geometry.h"
#include "nec_context.h"
#include <iostream>


ExcitationObj::ExcitationObj(Type type, GeoElementObj *elm, size_t segment, double real, double imag)
  : Object(), _type(type), _element(elm), _segment(segment), _real(real), _imag(imag)
{
  // pass..
}

ExcitationObj::~ExcitationObj() {
  // pass...
}

void
ExcitationObj::mark() {
  if (isMarked())
    return;
  Object::mark();
  _element->mark();
}

ExcitationObj *
ExcitationObj::voltage(GeoElementObj *elm, size_t segment, double real, double imag) {
  return new ExcitationObj(Voltage, elm, segment, real, imag);
}

ExcitationObj *
ExcitationObj::current(GeoElementObj *elm, size_t segment, double real, double imag) {
  return new ExcitationObj(Current, elm, segment, real, imag);
}

void
ExcitationObj::construct(nec_context &ctx) {
  switch(_type) {
    case Voltage:
      ctx.ex_card(EXCITATION_VOLTAGE, _element->id(), _segment, 01, _real, _imag, 1.0, 0.0, 0.0, 0.0);
      /*std::cerr << "Add voltage excitation at wire " << _element->id() << " at segment " << _segment
                << " with amplitude (" << _real << ", " << _imag << ")." << std::endl;*/
      break;
    case Current:
      ctx.ex_card(EXCITATION_CURRENT, _element->id(), _segment, 01, _real, _imag, 1.0, 0.0, 0.0, 0.0);
      /*std::cerr << "Add current excitation at wire " << _element->id() << " at segment " << _segment
                << " with amplitude (" << _real << ", " << _imag << ")." << std::endl;*/
      break;
  }
}

