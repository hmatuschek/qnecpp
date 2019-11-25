#include "nec_context.h"
#include "c_geometry.h"
#include <iostream>
#include "api.h"
#include "optimize.h"


double fitness(Eigen::Ref<const Eigen::VectorXd> param) {
  double Ri = 1.2e-3/2; // 1.0mm²
  double Ra = 2.3e-3/2;
  double l1=param(0), l2=param(1), l3=param(2), l4=param(3), l5=param(4), l6=param(5), l7=param(6),
      l8=param(7);
  double t=3e-2;
  double R=1e6;
  double L1= 148e-9, C1=68e-12;
  double L2= 321e-9, C2=100e-12;
  double L3= 408e-9, C3=100e-12;
  double L4= 568e-9, C4=100e-12;
  double L5= 770e-9, C5=100e-12;
  double L6= 579e-9, C6=220e-12;
  double L7=1128e-9, C7=220e-12;

  Model model;

  Eigen::Vector3d z, dir; z << 0, 0, 0; dir << 0, 0, 1;
  model.setVoltageExcitation(
        model.addInsulatedWire(z, z+dir*l1, Ri, Ra, 4.0), 1); z += dir*l1;
  model.addTrap(z, z+dir*t, Ri, L1, C1, R); z += dir*t;
  model.addInsulatedWire(z, z+dir*l2, Ri, Ra, 4.0); z += dir*l2;
  model.addTrap(z, z+dir*t, Ri, L2, C2, R); z += dir*t;
  model.addInsulatedWire(z, z+dir*l3, Ri, Ra, 4.0); z += dir*l3;
  model.addTrap(z, z+dir*t, Ri, L3, C3, R); z += dir*t;
  model.addInsulatedWire(z, z+dir*l4, Ri, Ra, 4.0); z += dir*l4;
  model.addTrap(z, z+dir*t, Ri, L4, C4, R); z += dir*t;
  model.addInsulatedWire(z, z+dir*l5, Ri, Ra, 4.0); z += dir*l5;
  model.addTrap(z, z+dir*t, Ri, L5, C5, R); z += dir*t;
  model.addInsulatedWire(z, z+dir*l6, Ri, Ra, 4.0); z += dir*l6;
  model.addTrap(z, z+dir*t, Ri, L6, C6, R); z += dir*t;
  model.addInsulatedWire(z, z+dir*l7, Ri, Ra, 4.0); z += dir*l7;
  model.addTrap(z, z+dir*t, Ri, L7, C7, R); z += dir*t;
  model.addInsulatedWire(z, z+dir*l8, Ri, Ra, 4.0); z += dir*l8;

  model.setGoodGround();

  model.addFrequency({50.1, 28.1, 24.9, 21.1, 18.1, 14.1, 10.12, 7.1});

  model.setMeasurement(ImpedanceMeasurement());
  std::list<Result> result = model.run();
  std::cout << "At p=" << param.transpose() << std::endl;
  for (std::list<Result>::iterator r=result.begin(); r!=result.end(); r++) {
    std::cout << "@ F=" << r->frequency() << "MHz => Z=" << r->impedance() << ", |Z|=" << std::abs(r->impedance())
              << " => VSWR@" << 64*50 << "=" << r->vswr(64*50)
              << " => VSWR@" << 50 << "=" << r->vswr(50) << std::endl;
  }

  double totVSWR = 0;
  for (std::list<Result>::iterator r=result.begin(); r!=result.end(); r++)
    totVSWR += r->vswr(64*50);
  return 1./totVSWR;
}


int main(int argc, char *argv[]) {

  double l1=2.87, l2=1.95, l3=0.54, l4=0.74, l5=0.8, l6=1.54, l7=3.24, l8=5.40;
  Eigen::VectorXd param(8); param << l1, l2, l3, l4, l5, l6, l7, l8;
  CMA_ES::run(param, 0.2, fitness);

  return 0;
}
