#ifndef EXCITATION_H
#define EXCITATION_H

#include "object.h"

class GeoElementObj;
class nec_context;


class ExcitationObj: public Object
{
public:
  typedef enum {
    Voltage, Current
  } Type;

  typedef enum {
    Start, End, Center
  } Location;

protected:
  ExcitationObj(Type type, GeoElementObj *elm, size_t segment, double real=1.0, double imag=0.0);

public:
  virtual ~ExcitationObj();
  void mark();

  static ExcitationObj *voltage(GeoElementObj *elm, size_t segment, double real=1.0, double imag=0.0);
  static ExcitationObj *current(GeoElementObj *elm, size_t segment, double real=1.0, double imag=0.0);

  void construct(nec_context &ctx);

protected:
  Type _type;
  GeoElementObj *_element;
  size_t _segment;
  double _real;
  double _imag;
};

#endif // EXCITATION_H
