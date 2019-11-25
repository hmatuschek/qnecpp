#include "measurement.h"
#include "nec_context.h"


/* ********************************************************************************************* *
 * Implementation of ResultObj
 * ********************************************************************************************* */
ResultObj::ResultObj(double F_in_MHz)
  : Object(), _F(F_in_MHz), _Z(0,0)
{
  // pass...
}

ResultObj::~ResultObj() {
  // pass...
}

double
ResultObj::frequency() const {
  return _F;
}

const std::complex<double> &
ResultObj::impedance() const {
  return _Z;
}
void
ResultObj::setImpedance(const std::complex<double> &Z) {
  _Z = Z;
}


/* ********************************************************************************************* *
 * Implementation of MeasurementObj
 * ********************************************************************************************* */
MeasurementObj::MeasurementObj()
  : Object()
{
  // pass...
}

MeasurementObj::~MeasurementObj()
{
  // pass...
}


/* ********************************************************************************************* *
 * Implementation of ImpedanceMeasurementObj
 * ********************************************************************************************* */
ImpedanceMeasurementObj::ImpedanceMeasurementObj()
  : MeasurementObj()
{
  // pass...
}

ImpedanceMeasurementObj::~ImpedanceMeasurementObj()
{
  // pass...
}

void
ImpedanceMeasurementObj::measure(int idx, double F_in_MHz, nec_context &ctx, ResultObj *res) {
  res->setImpedance(std::complex<double>(ctx.get_impedance_real(idx), ctx.get_impedance_imag(idx)));
}
