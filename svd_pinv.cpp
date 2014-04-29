/**
 * Moore-Penrose pseudo-inverse with Eigen3
 * 
 * 1. Download eigen (from http://eigen.tuxfamily.org/index.php?title=Main_Page)
 * 2. Extract to current directory eigen/3.2.1
 * 3. Compile the code with 
 *      g++ svd_pinv.cpp -I./eigen/3.2.1
 *
 * Ref: 
 *  0. http://eigen.tuxfamily.org/dox/AsciiQuickReference.txt
 *  1. http://eigen.tuxfamily.org/dox-devel/group__QuickRefPage.html
 *  2. http://eigen.tuxfamily.org/dox/classEigen_1_1JacobiSVD.html
 */
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/SVD>
#include <Eigen/QR>

#include <cstdio>
#include <cstdlib>
#include "mmio.h"

using namespace Eigen;
//using Eigen::MatrixXd;
//using Eigen::ArrayXf;
//using Eigen::JacobiSVD;
using namespace std;

int load_matrix_market(MatrixXd &A, const char* fname)
{
  FILE *fin;
  if ( ( fin = fopen(fname, "rb") ) == NULL )
    {
      fprintf(stderr, "Error: cannot load file %s\n", fname);
      exit(1);
    }

  MM_typecode matcode;
  if ( mm_read_banner(fin, &matcode) != 0 )
    {
      fprintf(stderr, "Error: could not process Matrix Market banner.\n");
      exit(1);
    }

  /*  This is how one can screen matrix types if their application */
  /*  only supports a subset of the Matrix Market data types.      */

  // if (mm_is_complex(matcode) && mm_is_matrix(matcode) && 
  //     mm_is_sparse(matcode) )
  //   {
  //     printf("Sorry, this application does not support ");
  //     printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
  //     exit(1);
  //   }

  int n, m, nnz;
  int ret_code;
  if ( (ret_code = mm_read_mtx_crd_size(fin, &n, &m, &nnz)) != 0 )
    {
      fprintf(stderr, "Error: cannot load matrix metadata\n");
      exit(1);
    }

  
  // int *I = (int *) malloc(nnz * sizeof(int));
  // int *J = (int *) malloc(nnz * sizeof(int));
  // double *val = (double *) malloc(nnz * sizeof(double));
  //MatrixXd A(n, m);
  A.resize(n, m);

  int i, j;
  double val;

  if ( mm_is_pattern(matcode) )
    {
      for (int k=0; k < nnz; ++k)
	{
	  fscanf(fin, "%d %d\n", &i, &j);
	  --i; --j; // adjust to 0-based indexing	  
	  A(i, j) = 1.0;
	}      
    }
  else // now there will be some value for each non-zero entry
    {
      for (int k=0; k < nnz; ++k)
	{
	  fscanf(fin, "%d %d %lg\n", &i, &j, &val);
	  --i; --j; // adjust to 0-based indexing
	  A(i, j) = val;
	}      
    }
  
  if ( fin != stdin )
    fclose(fin);

  //cout << A << endl;
  return 0;
}

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

  MatrixXd A;
  load_matrix_market(A, "rbs480a.mtx");
  //load_matrix_market(A, "chesapeake.mtx");
  //cout << "Size of A: " << A.size() << endl;
  n = A.rows();
  m = A.cols();
  //return 0;
  
  //Array44i a = Array44i::Random();
  //MatrixXd A = MatrixXd::Random(n, m);
  //VectorXd b = ArrayXd::Random(n);
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
