/**
 * Moore-Penrose pseudo-inverse with Eigen3
 * 
 * 1. Download eigen (from http://eigen.tuxfamily.org/index.php?title=Main_Page)
 * 2. Extract to current directory eigen-3.2.1
 * 3. Compile the code with 
 *      g++ svd_eigen.cpp -I./eigen-3.2.1
 *
 * Ref: 
 *  1. http://eigen.tuxfamily.org/dox-devel/group__QuickRefPage.html
 *  2. http://eigen.tuxfamily.org/dox/classEigen_1_1JacobiSVD.html
 */
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/SVD>
#include <Eigen/QR>

using namespace Eigen;
//using Eigen::MatrixXd;
//using Eigen::ArrayXf;
//using Eigen::JacobiSVD;
using namespace std;

int main(int argc, char **argv)
{
  // MatrixXd m(2,2);
  // for (int i=0; i<2; ++i)
  //   for (int j=0; j<2; ++j)
  //     m(i,j) = i + j;
  
  // cout << m << endl;

  unsigned int n, m = -1;
  switch ( argc )
    {
    case 3:
      m = atoi(argv[2]);
    case 2:
      n = atoi(argv[1]);
      break;
    default:
      m = n = 4;
      break;
    }
  if ( -1 == m ) 
    m = n;
  
  //Array44i a = Array44i::Random();
  MatrixXd A = MatrixXd::Random(n, m);
  VectorXd b = ArrayXd::Random(n);
  //cout << A << endl;
  //cout << a.leftCols(2) << endl;
  
  JacobiSVD<MatrixXd> svd(A, ComputeThinU | ComputeThinV);
  //cout << svd.singularValues() << endl;
  MatrixXd S = svd.singularValues().cwiseInverse().asDiagonal();  
  MatrixXd Ap = svd.matrixV() * S * svd.matrixU().transpose();

  if ( n < 7 && m < 7 )
    cout << Ap << endl;

  // Property 1: A * Ap * A = A
  cout << "Property 1: residual " << (A * Ap * A - A).cwiseAbs().maxCoeff() << endl;
  // Property 2: Ap * A * Ap = Ap
  cout << "Property 2: residual " << (Ap * A * Ap - Ap).cwiseAbs().maxCoeff() << endl;
  // Property 3: (A * Ap)' = A * Ap
  cout << "Property 3: residual " << ((A * Ap).transpose() - A * Ap).cwiseAbs().maxCoeff() << endl;
  // Property 4: (Ap * A)' = Ap * A
  cout << "Property 4: residual " << ((Ap * A).transpose() - Ap * A).cwiseAbs().maxCoeff() << endl;

  // VectorXd r = A * (Ap * b) - b;
  // cout << "residual norm: " << r.norm() << endl;
  // cout << "residual max element: " << r.cwiseAbs().maxCoeff() << endl;
  // cout << "b vector norm: " << b.norm() << endl;
  // cout << "b max element: " << b.cwiseAbs().maxCoeff() << endl;

  return 0;
}
