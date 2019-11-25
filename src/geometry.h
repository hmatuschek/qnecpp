#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "object.h"
#include <vector>
#include <eigen3/Eigen/Dense>


class nec_context;
class GeometryObj;

class GeoElementObj: public Object
{
protected:
  GeoElementObj(GeometryObj *geo);

public:
  virtual ~GeoElementObj();
  void mark();

  int id() const;

  virtual void constructGeometry(nec_context &ctx) = 0;
  virtual void constructEnvironment(nec_context &ctx) = 0;

protected:
  GeometryObj *_geometry;
};


class WireObj: public GeoElementObj
{
public:
  WireObj(GeometryObj *geo, Eigen::Ref<const Eigen::Vector3d> start, Eigen::Ref<const Eigen::Vector3d> end,
          double radius, size_t segments=20);

  virtual ~WireObj();

  Eigen::Ref<const Eigen::Vector3d> start() const;
  Eigen::Ref<const Eigen::Vector3d> end() const;
  double radius() const;

  void constructGeometry(nec_context &ctx);
  void constructEnvironment(nec_context &ctx);

protected:
  Eigen::Vector3d _start;
  Eigen::Vector3d _end;
  double _radius;
  size_t _segments;
};


class InsulatedWireObj: public WireObj
{
public:
  constexpr static double EPS_PVC = 4.0, EPS_PP = 2.1, EPS_PE = 2.4, EPS_PTFE = 2.0;

public:
  InsulatedWireObj(GeometryObj *geo, Eigen::Ref<const Eigen::Vector3d> start,
                   Eigen::Ref<const Eigen::Vector3d> end,
                   double Ri, double Ro, double epsilon, size_t segments=20);

  virtual ~InsulatedWireObj();

  void constructGeometry(nec_context &ctx);
  void constructEnvironment(nec_context &ctx);

  static double yurkov(double epsilon, double Ra, double Ri, double k=0.95);

protected:
  double _outerRadius;
  double _epsilon;
};


class TrapObj: public WireObj
{
public:
  TrapObj(GeometryObj *geo, Eigen::Ref<const Eigen::Vector3d> start,
          Eigen::Ref<const Eigen::Vector3d> end,
          double radius, double L, double C, double R=1e6);

  virtual ~TrapObj();

  void constructGeometry(nec_context &ctx);
  void constructEnvironment(nec_context &ctx);

protected:
  double _L, _C, _R;
};


class GeometryObj: public Object
{
public:
  GeometryObj();
  virtual ~GeometryObj();

  void mark();

  void constructGeometry(nec_context &ctx);
  void constructEnvironment(nec_context &ctx);

  void add(GeoElementObj *obj);
  int id(const GeoElementObj *obj);

protected:
  std::vector<GeoElementObj *> _elements;
};


#endif // GEOMETRY_H
