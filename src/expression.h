#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "object.h"


class ExprObj: public Object
{
protected:
  ExprObj();

public:
  virtual ~ExprObj();
};


class ValueObj: public ExprObj
{

};


#endif // EXPRESSION_H
