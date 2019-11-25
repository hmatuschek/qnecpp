#ifndef __MODEL_H__
#define __MODEL_H__

#include <string>
#include <list>
#include "object.h"
#include "nec_context.h"
#include "geometry.h"
#include "ground.h"
#include "excitation.h"
#include "measurement.h"


class ModelObj: public Object
{
public:
  ModelObj(const std::wstring &name=L"");
  virtual ~ModelObj();

  void mark();

  GeometryObj *geometry() const;

  void setGround(GroundObj *ground);
  void setExcitation(ExcitationObj *excitation);

  void addFrequency(double F_in_MHz);

  void setMeasurement(MeasurementObj *measurement);

  virtual std::list<ResultObj*> run();

protected:
  std::wstring    _name;
  GeometryObj   *_geometry;
  GroundObj     *_ground;
  ExcitationObj *_excitation;
  std::vector<double> _frequencies;
  MeasurementObj *_measurement;
};

#endif // __MODEL_H__
