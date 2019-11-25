#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include "object.h"
#include <vector>
#include <complex>

class nec_context;

class ResultObj: public Object
{
public:
  ResultObj(double F_in_MHz);
  virtual ~ResultObj();

  double frequency() const;
  const std::complex<double> &impedance() const;
  void setImpedance(const std::complex<double> &Z);

protected:
  double _F;
  std::complex<double> _Z;
};


class MeasurementObj: public Object
{
protected:
  MeasurementObj();

public:
  virtual ~MeasurementObj();

  virtual void measure(int idx, double F_in_MHz, nec_context &ctx, ResultObj *res) = 0;
};

class ImpedanceMeasurementObj: public MeasurementObj
{
public:
  ImpedanceMeasurementObj();
  virtual ~ImpedanceMeasurementObj();

  void measure(int idx, double F_in_MHz, nec_context &ctx, ResultObj *res);
};

#endif // MEASUREMENT_H
