#include "optimize.h"
#include <eigen3/Eigen/Cholesky>
#include <chrono>
#include <random>

unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator(seed1);
std::normal_distribution<double> distribution(0,1.0);

inline Eigen::VectorXd rand_normal(int N, double m, double s) {
  Eigen::VectorXd x(N);
  for (int i=0; i<N; i++)
    x(i) = m+s*distribution(generator);
  return x;
}


void
CMA_ES::run(Eigen::Ref<Eigen::VectorXd> m, double sig, fFitness fitness, size_t lam, double eps) {
  int N = m.rows();
  size_t mu = (2*lam)/3;
  Eigen::VectorXd w(lam); w.setZero();
  for (size_t i=0; i<lam; i++)
    w(i) = lam-i+1;
  w = w/w.sum();
  Eigen::MatrixXd C = Eigen::MatrixXd::Identity(N,N);
  Eigen::VectorXd pc(N); pc.setZero();
  Eigen::VectorXd ps(N); ps.setZero();
  while (sig>eps) {
    step(m, sig, C, ps, pc, lam, mu, w, fitness);
  }
}


void
CMA_ES::step(Eigen::Ref<Eigen::VectorXd> m, double &sig, Eigen::Ref<Eigen::MatrixXd> C,
             Eigen::Ref<Eigen::VectorXd> ps, Eigen::Ref<Eigen::VectorXd> pc,
             size_t lam, size_t mu, Eigen::Ref<const Eigen::VectorXd> w, fFitness fitness)
{
  int N = m.rows();
  double cs = 3./N;
  double cc = 1./std::sqrt(double(N));
  double c1 = 2./(N*N);
  double mu_eff = 1./w.head(mu).squaredNorm();
  double c_mu = std::min(mu_eff/N/N, 1-c1);
  double ds = 1.1;

  Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> VDV(C);
  Eigen::MatrixXd X(N,lam);
  Eigen::VectorXd fit(lam);

  Eigen::MatrixXd V(VDV.eigenvectors());
  Eigen::DiagonalMatrix<double, Eigen::Dynamic> D2(VDV.eigenvalues().array().sqrt().matrix());
  Eigen::DiagonalMatrix<double, Eigen::Dynamic> Di2((1./VDV.eigenvalues().array().sqrt()).matrix());

//#pragma omp parallel for
  for (size_t i=0; i<lam; i++) {
    X.col(i).noalias() = ( m + V*D2*rand_normal(N,0,sig) );
    for (int j=0; j<N; j++)
      X(j,i) = std::max(0.03, std::min(X(j,i),100.0));
    fit(i) = fitness(X.col(i));
  }

  Eigen::VectorXi perm(lam);
  for (size_t i=0; i<lam;i++) perm(i)=i;
  std::sort(perm.data(), perm.data()+perm.size(),
            [&fit](int i1, int i2) {return fit[i1] > fit[i2];});
  Eigen::MatrixXd Xp(N,lam);
  Eigen::VectorXd fitp(lam);
  for (size_t i=0;i<lam;i++) {
    Xp.col(i) = X.col(perm(i));
    fitp(i)   = fit(perm(i));
  }
  // update mean
  Eigen::VectorXd m2 = m;
  for (size_t i=0; i<mu; i++)
    m += w(i)*(Xp.col(i)-m2);

  // Update Ps
  ps = (1-cs)*ps + std::sqrt(cs*(2-cs)*mu_eff)*(V*Di2*V.transpose()*(m-m2)/sig);

  // Update Pc
  pc = (1-cc)*pc + std::sqrt(cc*(2-cc)*mu_eff)*(m-m2)/sig;

  C = (1-c1-c_mu*w.sum())*C + c1*pc*pc.transpose();
  for (size_t i=0; i<lam; i++) {
    Eigen::VectorXd y = (Xp.col(i)-m2)/sig;
    C += c_mu*w(i)*(y*y.transpose());
  }

  double pn = ps.norm()/(std::sqrt(double(N))*(1-1./(4*N)+1./(21*N*N)));
  sig = sig*std::exp(cs*(pn-1)/ds);
}
