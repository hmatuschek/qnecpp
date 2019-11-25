#include "model.h"
#include <iostream>


ModelObj::ModelObj(const std::wstring &name)
  : Object(), _name(name), _geometry(nullptr), _ground(nullptr), _excitation(nullptr),
    _frequencies(), _measurement(nullptr)
{
  GeometryObj *obj = new GeometryObj();
  _lock.lock();
  _geometry = obj;
  _lock.unlock();
  GC::get()->unref(obj);
}

ModelObj::~ModelObj() {
  // pass...
}

void
ModelObj::mark() {
  if (isMarked())
    return;
  Object::mark();
  _lock.lock();
  if (_geometry)
	  _geometry->mark();
  if (_ground)
    _ground->mark();
  if (_excitation)
    _excitation->mark();
  if (_measurement)
    _measurement->mark();
  _lock.unlock();
}

GeometryObj *
ModelObj::geometry() const {
  return _geometry;
}

void
ModelObj::setGround(GroundObj *ground) {
  _lock.lock();
  _ground = ground;
  _lock.unlock();
}

void
ModelObj::setExcitation(ExcitationObj *excitation) {
  _lock.lock();
  _excitation = excitation;
  _lock.unlock();
}

void
ModelObj::addFrequency(double F_in_MHz) {
  _frequencies.push_back(F_in_MHz);
}

void
ModelObj::setMeasurement(MeasurementObj *measurement) {
  _lock.lock();
  _measurement = measurement;
  _lock.unlock();
}

std::list<ResultObj *>
ModelObj::run() {
  nec_context ctx;
  ctx.initialize();

  _geometry->constructGeometry(ctx);
  //std::cerr << "Completed geometry." << std::endl;

  ctx.geometry_complete(1);

  if (_ground)
    _ground->construct(ctx);
  else
    ctx.gn_card(-1, 0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0);

  _geometry->constructEnvironment(ctx);

  if (_excitation)
    _excitation->construct(ctx);

  std::list<ResultObj *> results;
  if (nullptr == _measurement)
    return results;

  for (size_t i=0; i<_frequencies.size(); i++) {
    ctx.fr_card(0, 1, _frequencies[i], 1);
    //std::cerr << "Add frquency " << _frequencies[i] << std::endl;
    ctx.xq_card(0);
    //std::cerr << "Simulate." << std::endl;
    ResultObj *result = new ResultObj(_frequencies[i]);
    _measurement->measure(i, _frequencies[i], ctx, result);
    results.push_back(result);
  }

  return results;
}
