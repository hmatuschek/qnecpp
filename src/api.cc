#include "api.h"
#include "object.h"
#include "geometry.h"
#include "measurement.h"
#include "model.h"


/* ********************************************************************************************* *
 * Implementation of Container
 * ********************************************************************************************* */
Container::Container()
  : _object(nullptr)
{
  // pass...
}

Container::Container(Object *obj)
  : _object(obj)
{
  if (_object)
    GC::get()->ref(_object);
}

Container::Container(const Container &other)
  : _object(other._object)
{
  if (_object)
    GC::get()->ref(_object);
}

Container::~Container()
{
  if (_object)
    GC::get()->unref(_object);
}

Container &
Container::operator =(const Container &other) {
  if (_object)
    GC::get()->unref(_object);
  _object = other._object;
  if (_object)
    GC::get()->ref(_object);
  return *this;
}

Object *
Container::operator ->() const {
  return _object;
}

Object *
Container::operator *() const {
  return _object;
}


/* ********************************************************************************************* *
 * Implementation of GeoElement
 * ********************************************************************************************* */
GeoElement::GeoElement(GeoElementObj *obj)
  : Container(obj), _element(obj)
{
  // pass...
}

GeoElement::GeoElement(const GeoElement &other)
  : Container(other), _element(other._element)
{
  // pass...
}

GeoElementObj *
GeoElement::operator ->() const {
  return _element;
}

GeoElementObj *
GeoElement::operator *() const {
  return _element;
}

GeoElement &
GeoElement::operator =(const GeoElement &other) {
  Container::operator =(other);
  _element = other._element;
  return *this;
}


/* ********************************************************************************************* *
 * Implementation of Wire
 * ********************************************************************************************* */
Wire::Wire(WireObj *obj)
  : GeoElement(obj), _wire(obj)
{
  // pass...
}

Wire::Wire(const Wire &other)
  : GeoElement(other), _wire(other._wire)
{
  // pass...
}

WireObj *
Wire::operator ->() const {
  return _wire;
}

WireObj *
Wire::operator *() const {
  return _wire;
}

Wire &
Wire::operator =(const Wire &other) {
  GeoElement::operator =(other);
  _wire = other._wire;
  return *this;
}


/* ********************************************************************************************* *
 * Implementation of InsulatedWire
 * ********************************************************************************************* */
InsulatedWire::InsulatedWire(InsulatedWireObj *obj)
  : Wire(obj), _inswire(obj)
{
  // pass...
}

InsulatedWire::InsulatedWire(const InsulatedWire &other)
  : Wire(other), _inswire(other._inswire)
{
  // pass...
}

InsulatedWireObj *
InsulatedWire::operator ->() const {
  return _inswire;
}

InsulatedWireObj *
InsulatedWire::operator *() const {
  return _inswire;
}

InsulatedWire &
InsulatedWire::operator =(const InsulatedWire &other) {
  Wire::operator =(other);
  _inswire = other._inswire;
  return *this;
}

/* ********************************************************************************************* *
 * Implementation of Trap
 * ********************************************************************************************* */
Trap::Trap(TrapObj *obj)
  : Wire(obj), _trap(obj)
{
  // pass...
}

Trap::Trap(const Trap &other)
  : Wire(other), _trap(other._trap)
{
  // pass...
}

TrapObj *
Trap::operator ->() const {
  return _trap;
}

TrapObj *
Trap::operator *() const {
  return _trap;
}

Trap &
Trap::operator =(const Trap &other) {
  Wire::operator =(other);
  _trap = other._trap;
  return *this;
}


/* ********************************************************************************************* *
 * Implementation of Result
 * ********************************************************************************************* */
Result::Result(ResultObj *obj)
  : Container(obj), _result(obj)
{
  // pass...
}

Result::Result(const Result &other)
  : Container(other), _result(other._result)
{
  // pass...
}

Result::~Result() {
  // pass...
}

ResultObj *
Result::operator ->() const {
  return _result;
}

ResultObj *
Result::operator *() const {
  return _result;
}

Result &
Result::operator =(const Result &other) {
  Container::operator =(other);
  _result = other._result;
  return *this;
}

double
Result::frequency() const {
  return _result->frequency();
}

const std::complex<double> &
Result::impedance() const {
  return _result->impedance();
}

double
Result::vswr(double Z0) const {
  std::complex<double> g = (_result->impedance()-Z0)/(_result->impedance()+Z0);
  return (1.+std::abs(g))/(1.-std::abs(g));
}


/* ********************************************************************************************* *
 * Implementation of Measurement
 * ********************************************************************************************* */
Measurement::Measurement(MeasurementObj *obj)
  : Container(obj), _measurement(obj)
{
  // pass...
}

Measurement::Measurement(const Measurement &other)
  : Container(other), _measurement(other._measurement)
{
  // pass...
}

Measurement::~Measurement() {
  // pass...
}

MeasurementObj *
Measurement::operator ->() const {
  return _measurement;
}

MeasurementObj *
Measurement::operator *() const {
  return _measurement;
}

Measurement &
Measurement::operator =(const Measurement &other) {
  Container::operator =(other);
  _measurement = other._measurement;
  return *this;
}


/* ********************************************************************************************* *
 * Implementation of ImpedanceMeasurement
 * ********************************************************************************************* */
ImpedanceMeasurement::ImpedanceMeasurement()
  : Measurement(new ImpedanceMeasurementObj()),
    _impedance_measurement(static_cast<ImpedanceMeasurementObj *>(_measurement))
{
  GC::get()->unref(_impedance_measurement);
}

ImpedanceMeasurement::ImpedanceMeasurement(ImpedanceMeasurementObj *obj)
  : Measurement(obj), _impedance_measurement(obj)
{
  // pass...
}

ImpedanceMeasurement::ImpedanceMeasurement(const ImpedanceMeasurement &other)
  : Measurement(other), _impedance_measurement(other._impedance_measurement)
{
  // pass...
}

ImpedanceMeasurement::~ImpedanceMeasurement() {
  // pass...
}

ImpedanceMeasurementObj *
ImpedanceMeasurement::operator ->() const {
  return _impedance_measurement;
}

ImpedanceMeasurementObj *
ImpedanceMeasurement::operator *() const {
  return _impedance_measurement;
}

ImpedanceMeasurement &
ImpedanceMeasurement::operator =(const ImpedanceMeasurement &other) {
  Measurement::operator =(other);
  _impedance_measurement = other._impedance_measurement;
  return *this;
}


/* ********************************************************************************************* *
 * Implementation of Model
 * ********************************************************************************************* */
Model::Model(const std::wstring &name)
  : Container(new ModelObj(name)), _model(static_cast<ModelObj *>(_object))
{
  // pass...
}

Model::Model(const Model &other)
  : Container(other), _model(other._model)
{
  // pass...
}

Model &
Model::operator =(const Model &other) {
  Container::operator =(other);
  _model = other._model;
  return *this;
}

Wire
Model::addWire(Eigen::Ref<const Eigen::Vector3d> start, Eigen::Ref<const Eigen::Vector3d> end,
               double radius, size_t segments)
{
  Wire wire(new WireObj(_model->geometry(), start, end, radius, segments));
  _model->geometry()->add(*wire);
  GC::get()->unref(*wire);
  return wire;
}

Wire
Model::addWire(const Wire &from, Eigen::Ref<const Eigen::Vector3d> end,
               double radius, size_t segments)
{
  Wire wire(new WireObj(_model->geometry(), from->end(), end, radius, segments));
  _model->geometry()->add(*wire);
  GC::get()->unref(*wire);
  return wire;
}

InsulatedWire
Model::addInsulatedWire(Eigen::Ref<const Eigen::Vector3d> start,
                        Eigen::Ref<const Eigen::Vector3d> end,
                        double Ri, double Ro, double epsilon, size_t segments)
{
  InsulatedWire wire(new InsulatedWireObj(_model->geometry(), start, end, Ri, Ro, epsilon, segments));
  _model->geometry()->add(*wire);
  GC::get()->unref(*wire);
  return wire;
}

InsulatedWire
Model::addInsulatedWire(const Wire &from, Eigen::Ref<const Eigen::Vector3d> end,
                        double Ri, double Ro, double epsilon, size_t segments)
{
  InsulatedWire wire(new InsulatedWireObj(_model->geometry(), from->end(), end, Ri, Ro, epsilon, segments));
  _model->geometry()->add(*wire);
  GC::get()->unref(*wire);
  return wire;
}

Trap
Model::addTrap(Eigen::Ref<const Eigen::Vector3d> start,
               Eigen::Ref<const Eigen::Vector3d> end,
               double radius, double L, double C, double R)
{
  Trap trap(new TrapObj(_model->geometry(), start, end, radius, L, C, R));
  _model->geometry()->add(*trap);
  GC::get()->unref(*trap);
  return trap;
}

Trap
Model::addTrap(const Wire &from, Eigen::Ref<const Eigen::Vector3d> end,
               double radius, double L, double C, double R)
{
  Trap trap(new TrapObj(_model->geometry(), from->end(), end, radius, L, C, R));
  _model->geometry()->add(*trap);
  GC::get()->unref(*trap);
  return trap;
}

void
Model::setFreeSpace() {
  _model->setGround(GroundObj::freeSpace());
}

void
Model::setPerfectGround() {
  _model->setGround(GroundObj::perfectGround());
}

void
Model::setGoodGround(){
  _model->setGround(GroundObj::goodGround());
}

void
Model::setAverageGround() {
  _model->setGround(GroundObj::averageGround());
}

void
Model::setPoorGround() {
  _model->setGround(GroundObj::poorGround());
}

void
Model::setFreshWaterGround() {
  _model->setGround(GroundObj::freshWaterGround());
}

void
Model::setSaltWaterGround() {
  _model->setGround(GroundObj::saltWaterGround());
}

void
Model::setGround(double dielectic, double conductivity) {
  _model->setGround(GroundObj::ground(dielectic, conductivity));
}

void
Model::setVoltageExcitation(const GeoElement &element, size_t segment, double real, double imag) {
  _model->setExcitation(ExcitationObj::voltage(*element, segment, real, imag));
}

void
Model::setCurrentExcitation(const GeoElement &element, size_t segment, double real, double imag) {
  _model->setExcitation(ExcitationObj::current(*element, segment, real, imag));
}

void
Model::addFrequency(double F_in_MHz) {
  _model->addFrequency(F_in_MHz);
}

void
Model::addFrequency(const std::vector<double> &F_in_MHz) {
  for (size_t i=0; i<F_in_MHz.size(); i++)
    _model->addFrequency(F_in_MHz[i]);
}

void
Model::setMeasurement(const Measurement &measurement) {
  _model->setMeasurement(*measurement);
}

std::list<Result>
Model::run() {
  std::list<ResultObj *> robs = _model->run();
  std::list<Result> results;
  for (std::list<ResultObj *>::iterator r=robs.begin(); r!=robs.end(); r++) {
    results.push_back(*r); GC::get()->unref(*r);
  }
  return results;
}

