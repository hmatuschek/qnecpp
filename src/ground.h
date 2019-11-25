#ifndef GROUND_H
#define GROUND_H

#include "object.h"

class nec_context;


class GroundObj: public Object
{
public:
  typedef enum {
    FREE_SPACE,
    PERFECT_GROUND,
    PARAM_GROUND
  } Type;

protected:
  GroundObj(Type type, double dielectric=0.0, double conductivity=0.0);

public:
  virtual ~GroundObj();

  static GroundObj *freeSpace();
  static GroundObj *perfectGround();
  static GroundObj *ground(double dielectric, double conductivity);
  static inline GroundObj *goodGround() { return ground(20.0, 0.0303); }
  static inline GroundObj *averageGround() { return ground(13.0, 0.005); }
  static inline GroundObj *poorGround() { return ground(3.0, 0.001); }
  static inline GroundObj *freshWaterGround() { return ground(80.0, 0.001); }
  static inline GroundObj *saltWaterGround() { return ground(81.0, 5.0); }

  void construct(nec_context &ctx);

protected:
  Type _type;
  double _dielectric;
  double _conductivity;
};

#endif // GROUND_H
