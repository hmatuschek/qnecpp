#ifndef OPTIMIZE_H
#define OPTIMIZE_H

#include "api.h"
#include <eigen3/Eigen/Dense>

class CMA_ES
{
public:
  typedef double (*fFitness)(Eigen::Ref<const Eigen::VectorXd> param);

public:
  /** CMA-ES step. Updates the mean @c mu and covariance variance @c sig, @c C.
   * @param m Specifies the N-dimensional mean vector of the paramter estimates.
   * @param sig Specifies the std. dev. of the parameter estimates.
   * @param C Specifies the correlation of the parameter estimates.
   * @param fFitness The fitness function. */
  static void run(Eigen::Ref<Eigen::VectorXd> m, double sig,
                  fFitness fitness, size_t lam=10, double eps=1e-2);

  /** CMA-ES step. Updates the mean @c mu and covariance variance @c sig, @c C.
   * @param m Specifies the N-dimensional mean vector of the paramter estimates.
   * @param sig Specifies the std. dev. of the parameter estimates.
   * @param C Specifies the correlation of the parameter estimates.
   * @param ps Specifies the ...
   * @param pc Specifies the ...
   * @param lam Specifies the number of offsprings.
   * @param mu Specifies the number of samples taken into account for the update (with mu <= lam).
   * @param w A mu-dimensional vector of weights.
   * @param fFitness The fitness function. */
  static void step(Eigen::Ref<Eigen::VectorXd> m,
                   double &sig, Eigen::Ref<Eigen::MatrixXd> C,
                   Eigen::Ref<Eigen::VectorXd> ps,
                   Eigen::Ref<Eigen::VectorXd> pc,
                   size_t lam, size_t mu, Eigen::Ref<const Eigen::VectorXd> w,
                   fFitness fitness);
};

#endif // OPTIMIZE_H
