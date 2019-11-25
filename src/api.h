#ifndef API_H
#define API_H

#include <string>
#include <list>
#include <vector>
#include <eigen3/Eigen/Dense>


class Object;
class GeoElementObj;
class WireObj;
class InsulatedWireObj;
class TrapObj;
class MeasurementObj;
class ImpedanceMeasurementObj;
class ResultObj;
class ModelObj;


class Container
{
protected:
  Container();

public:
  Container(Object *obj);
  Container(const Container &other);
  ~Container();

  Object* operator ->() const;
  Object* operator *() const;

  Container &operator=(const Container &other);

protected:
  Object *_object;
};


class GeoElement: public Container
{
public:
  GeoElement(GeoElementObj *obj);
  GeoElement(const GeoElement &other);

  GeoElementObj* operator ->() const;
  GeoElementObj* operator *() const;
  GeoElement &operator =(const GeoElement &other);

protected:
  GeoElementObj *_element;
};


class Wire: public GeoElement
{
public:
  Wire(WireObj *obj);
  Wire(const Wire &other);

  WireObj* operator ->() const;
  WireObj* operator *() const;
  Wire &operator =(const Wire &other);

protected:
  WireObj *_wire;
};


class InsulatedWire: public Wire
{
public:
  InsulatedWire(InsulatedWireObj *obj);
  InsulatedWire(const InsulatedWire &other);

  InsulatedWireObj *operator ->() const;
  InsulatedWireObj *operator *() const;
  InsulatedWire &operator =(const InsulatedWire &other);

protected:
  InsulatedWireObj *_inswire;
};


class Trap: public Wire
{
public:
  Trap(TrapObj *obj);
  Trap(const Trap &other);

  TrapObj* operator ->() const;
  TrapObj* operator *() const;
  Trap &operator =(const Trap &other);

protected:
  TrapObj *_trap;
};


class Measurement: public Container
{
protected:
  Measurement(MeasurementObj *obj);

public:
  Measurement(const Measurement &other);
  virtual ~Measurement();

  MeasurementObj *operator ->() const;
  MeasurementObj *operator *() const;

  Measurement &operator =(const Measurement &other);

protected:
  MeasurementObj *_measurement;
};


class ImpedanceMeasurement: public Measurement
{
public:
  ImpedanceMeasurement();
  ImpedanceMeasurement(ImpedanceMeasurementObj *obj);
  ImpedanceMeasurement(const ImpedanceMeasurement &other);
  virtual ~ImpedanceMeasurement();

  ImpedanceMeasurementObj *operator ->() const;
  ImpedanceMeasurementObj *operator *() const;

  ImpedanceMeasurement &operator =(const ImpedanceMeasurement &other);

protected:
  ImpedanceMeasurementObj *_impedance_measurement;
};


class Result: public Container
{
public:
  Result(ResultObj *obj);
  Result(const Result &other);
  virtual ~Result();

  ResultObj *operator ->() const;
  ResultObj *operator *() const;

  Result &operator =(const Result &other);

  double frequency() const;
  const std::complex<double> &impedance() const;
  double vswr(double Z0=50) const;

protected:
  ResultObj *_result;
};


class Model: public Container
{
public:
  Model(const std::wstring &name=L"");
  Model(const Model &other);

  Model &operator =(const Model &other);

  Wire addWire(Eigen::Ref<const Eigen::Vector3d> start, Eigen::Ref<const Eigen::Vector3d> end,
               double radius, size_t segments=20);
  Wire addWire(const Wire &from, Eigen::Ref<const Eigen::Vector3d> end,
               double radius, size_t segments=20);

  InsulatedWire addInsulatedWire(Eigen::Ref<const Eigen::Vector3d> start,
                                 Eigen::Ref<const Eigen::Vector3d> end,
                                 double Ri, double Ro, double epsilon, size_t segments=20);
  InsulatedWire addInsulatedWire(const Wire &from, Eigen::Ref<const Eigen::Vector3d> end,
                                 double Ri, double Ro, double epsilon, size_t segments=20);

  Trap addTrap(Eigen::Ref<const Eigen::Vector3d> start,
               Eigen::Ref<const Eigen::Vector3d> end,
               double radius, double L, double C, double R=1e6);
  Trap addTrap(const Wire &from, Eigen::Ref<const Eigen::Vector3d> end,
               double radius, double L, double C, double R=1e6);

  void setFreeSpace();
  void setPerfectGround();
  void setGoodGround();
  void setAverageGround();
  void setPoorGround();
  void setFreshWaterGround();
  void setSaltWaterGround();
  void setGround(double dielectic, double conductivity);

  void setVoltageExcitation(const GeoElement &element, size_t segment, double real=1.0, double imag=0);
  void setCurrentExcitation(const GeoElement &element, size_t segment, double real=1.0, double imag=0);

  void addFrequency(double F_in_MHz);
  void addFrequency(const std::vector<double> &F_in_MHz);
  void setMeasurement(const Measurement &measurement);

  std::list<Result> run();

protected:
  ModelObj *_model;
};


#endif // API_H
