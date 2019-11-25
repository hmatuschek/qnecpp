#include "geometry.h"
#include "nec_context.h"
#include <iostream>


/* ********************************************************************************************* *
 * Implementation of GeometryObj
 * ********************************************************************************************* */
GeometryObj::GeometryObj()
  : Object(), _elements()
{
  // pass...
}

GeometryObj::~GeometryObj() {
  // pass...
}

void
GeometryObj::mark() {
  if (isMarked())
    return;
  Object::mark();
  _lock.lock();
  for (size_t i=0; i<_elements.size(); i++)
    _elements[i]->mark();
  _lock.unlock();
}

int
GeometryObj::id(const GeoElementObj *obj) {
  for (size_t i=0; i<_elements.size(); i++)
	 if (obj == _elements[i]) {
      return i+1;
	 }
  return -1;
}

void
GeometryObj::add(GeoElementObj *obj) {
  for (size_t i=0; i<_elements.size(); i++)
    if (obj == _elements[i])
      return;
  _lock.lock();
  _elements.push_back(obj);
  _lock.unlock();
}

void
GeometryObj::constructGeometry(nec_context &ctx) {
  for (size_t i=0; i<_elements.size(); i++)
    _elements[i]->constructGeometry(ctx);
}

void
GeometryObj::constructEnvironment(nec_context &ctx) {
  for (size_t i=0; i<_elements.size(); i++)
    _elements[i]->constructEnvironment(ctx);
}


/* ********************************************************************************************* *
 * Implementation of GeoElementObj
 * ********************************************************************************************* */
GeoElementObj::GeoElementObj(GeometryObj *geo)
  : Object(), _geometry(geo)
{
  _geometry->add(this);
}

GeoElementObj::~GeoElementObj() {
  // pass...
}

void
GeoElementObj::mark() {
  if (isMarked())
    return;
  Object::mark();
  if (_geometry)
	  _geometry->mark();
}

int
GeoElementObj::id() const {
  return _geometry->id(this);
}


/* ********************************************************************************************* *
 * Implementation of WireObj
 * ********************************************************************************************* */
WireObj::WireObj(GeometryObj *geo, Eigen::Ref<const Eigen::Vector3d> start,
                 Eigen::Ref<const Eigen::Vector3d> end, double radius, size_t segments)
  : GeoElementObj(geo), _start(start), _end(end), _radius(radius), _segments(segments)
{
  // pass...
}

WireObj::~WireObj() {
  // pass...
}

Eigen::Ref<const Eigen::Vector3d>
WireObj::start() const {
  return _start;
}

Eigen::Ref<const Eigen::Vector3d>
WireObj::end() const {
  return _end;
}

double
WireObj::radius() const {
  return _radius;
}

void
WireObj::constructGeometry(nec_context &ctx) {
  /*std::cerr << "Wire " << id() << " from " << _start.transpose() << " to " << _end.transpose()
            << " with radius=" << _radius << " useing " << _segments << " segments." << std::endl;*/
  ctx.wire(id(), _segments,
           _start[0], _start[1], _start[2],
           _end[0], _end[1], _end[2],
           _radius, 1.0, 1.0);
}

void
WireObj::constructEnvironment(nec_context &ctx) {
  // pass...
}


/* ********************************************************************************************* *
 * Implementation of InsulatedWireObj
 * ********************************************************************************************* */
InsulatedWireObj::InsulatedWireObj(GeometryObj *geo, Eigen::Ref<const Eigen::Vector3d> start,
                                   Eigen::Ref<const Eigen::Vector3d> end,
                                   double Ri, double Ro, double epsilon, size_t segments)
  : WireObj(geo, start, end, Ri, segments), _outerRadius(Ro), _epsilon(epsilon)
{
  // pass...
}

InsulatedWireObj::~InsulatedWireObj() {
  // pass...
}

double
InsulatedWireObj::yurkov(double epsilon, double Ra, double Ri, double k) {
  return 2e-7 * (1-1./(epsilon*k*k)) * std::log(Ra/Ri);
}

void
InsulatedWireObj::constructGeometry(nec_context &ctx) {
  WireObj::constructGeometry(ctx);
}

void
InsulatedWireObj::constructEnvironment(nec_context &ctx) {
  double dL = yurkov(_epsilon, _outerRadius, _radius);
  /*std::cerr << "Insulation for wire " << id() << " from segments 1 to " << _segments
            << " with add. Ind. " << dL << std::endl;*/
  ctx.ld_card(2, id(), 1, _segments, 0, dL, 0);
}


/* ********************************************************************************************* *
 * Implementation of TrapObj
 * ********************************************************************************************* */
TrapObj::TrapObj(GeometryObj *geo, Eigen::Ref<const Eigen::Vector3d> start,
                 Eigen::Ref<const Eigen::Vector3d> end,
                 double radius, double L, double C, double R)
  : WireObj(geo, start, end, radius, 1), _L(L), _C(C), _R(R)
{
  // pass...
}

TrapObj::~TrapObj() {
  // pass...
}

void
TrapObj::constructGeometry(nec_context &ctx) {
  WireObj::constructGeometry(ctx);
}

void
TrapObj::constructEnvironment(nec_context &ctx) {
  /*std::cerr << "Parallel LCR for trap " << id() << " from segments 1 to 2 of " << _segments
            << " segments with L=" << _L  << ", C=" << _C << " and R=" << _R << "." << std::endl;*/
  ctx.ld_card(1, id(), 1, 2, _R, _L, _C);
}


