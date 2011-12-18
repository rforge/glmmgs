#include "../../../../Standard.h"
#include "../../../../Estimate.h"
#include "SparsePrecisionModel.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		namespace Working
		{
			namespace Stratified
			{
				namespace CovarianceModels
				{
					// Helpers

					// Weighted square norm of vector
					double Square(const LDL::SparseMatrix<double> & R, const Vector<double> & x)
					{
						_ASSERT_ARGUMENT(R.NumberOfColumns() == x.Size());
						const int ncols = R.NumberOfColumns();
						double sum = 0.0;
						for (int j = 0; j < ncols; ++j)
						{
							const int p2 = R.Count(j + 1);
							double tmp = 0;
							for (int p = R.Count(j); p < p2; ++p)
							{
								const int i = R.Index(p);
								const double value = R.Value(p);
								tmp += x(i) * value;
							}
							sum += x(j) * tmp;
						}
						return sum;
					}

					// Trace of diagonal blocks
					double BlockTrace(int row, const TriangularMatrix<double> & v, const LDL::SparseMatrix<double> & R)
					{
						const int ncols = R.NumberOfColumns();
						const int offset = row * ncols;
						double sum = 0.0;
						for (int j = 0; j < ncols; ++j)
						{
							const int p2 = R.Count(j + 1);
							for (int p = R.Count(j); p < p2; ++p)
							{
								const int i = R.Index(p);
								const double value = R.Value(p);
								if (j <= i)
								{
									// Lower/diagonal elements
									sum += v(offset + i, offset + j) * value;
								}
								else // i > j
								{
									// Upper elements
									sum += v(offset + j, offset + i) * value;
								}
							}
						}
						return sum;
					}

					// Diagonal block product
					double BlockProduct(int row, int col, const TriangularMatrix<double> & v, int offset, const LDL::SparseMatrix<double> & R)
					{
						double sum = 0.0;
						const int p2 = R.Count(col + 1);
						for (int p = R.Count(col); p < p2; ++p)
						{
							const int i = R.Index(p);
							const int value = R.Value(p);
							if (i <= row)
							{
								// Lower/diagonal elements
								sum += v(offset + row, offset + i) * value;
							}
							else
							{
								// Upper elements
								sum += v(offset + i, offset + row) * value;
							}
						}
						return sum;
					}

					// Off-diagonal block product
					double BlockProduct(int row, int col, const TriangularMatrix<double> & v, int offset_row, int offset_col, const LDL::SparseMatrix<double> & R)
					{
						_ASSERT_ARGUMENT(offset_col < offset_row);
						double sum = 0.0;
						const int p2 = R.Count(col + 1);
						for (int p = R.Count(col); p < p2; ++p)
						{
							const int i = R.Index(p);
							const int value = R.Value(p);
							sum += v(offset_row + row, offset_col + i) * value;
						}
						return sum;
					}

					// Trace of diagonal blocks product
					double BlockSquareTrace(int row, const TriangularMatrix<double> & v, const LDL::SparseMatrix<double> & R)
					{
						// TODO: optimize by saving the block products into a matrix A and then computing tr(A^2)
						const int ncols = R.NumberOfColumns();
						const int offset = row * ncols;
						double sum = 0.0;
						for (int i = 0; i < ncols; ++i)
							for (int j = 0; j < ncols; ++j)
								sum += BlockProduct(i, j, v, offset, R) * BlockProduct(j, i, v, offset, R);
						return sum;
					}

					// Trace of off-diagonal blocks product
					double BlockSquareTrace(int row, int col, const TriangularMatrix<double> & v, const LDL::SparseMatrix<double> & R)
					{
						_ASSERT_ARGUMENT(col < row);
						const int ncols = R.NumberOfColumns();
						const int offset_row = row * ncols;
						const int offset_col = col * ncols;
						double sum = 0.0;
						for (int i = 0; i < ncols; ++i)
							for (int j = 0; j < ncols; ++j)
								sum += BlockProduct(i, j, v, offset_row, offset_col, R) * BlockProduct(j, i, v, offset_row, offset_col, R);
						return sum;
					}

					// Transposed-matrix product
					double TMatrixProduct(int k, const LDL::SparseMatrix<double> & R, const Vector<double> & x)
					{
						double sum = 0.0;
						const int p2 = R.Count(k + 1);
						for (int p = R.Count(k); p < p2; ++p)
						{
							const int i = R.Index(p);
							const double value = R.Value(p);
							sum += value * x(i);
						}
						return sum;
					}

					// Construction
					SparsePrecisionModel::SparsePrecisionModel(int nvars, const LDL::SparseMatrix<double> R)
						: nvars(nvars), R(R), tau(nvars)
					{
						this->tau = 1.0;
					}

					SparsePrecisionModel::~SparsePrecisionModel()
					{
					}

					// Properties
					Vector<Estimate> SparsePrecisionModel::Estimates() const
					{
						Vector<Estimate> estimates(nvars);
						for (int j = 0; j < nvars; ++j)
							estimates(j) = Estimate(1.0 / sqrt(this->tau(j)), -1); // TODO: calculate variance
						return estimates;
					}

					// Methods
					void SparsePrecisionModel::Decompose(const TriangularMatrix<Vector<double> > & design_precision)
					{
						// Build an upper diagonal sparse-matrix equal to the design-precision plus the random-effects-precision

						// Number of levels of the stratified random effect
						const int nlevels = this->R.NumberOfColumns();

						// Number of non-zero entries of the sparse matrix
						const int count = this->R.Count(nlevels) * this->nvars + nlevels * (this->nvars * (this->nvars - 1));
						const int ncols = nlevels * this->nvars;

						// Data structures of the sparse matrix
						NewTypes::Vector<double> values(count);
						NewTypes::Vector<int> indices(count);
						NewTypes::Vector<int> counts(ncols + 1);

						// Set vector of counts
						counts(0) = 0;
						for (int index = 0, i = 0; i < this->nvars; ++i)
						{
							for (int k = 0; k < nlevels; ++k, ++index)
							{
								const int nelements = (this->nvars - 1) + (this->R.Count(k + 1) - this->R.Count(k));
								counts(index + 1) = counts(index) + nelements;
							}
						}

						// Set values and indices
						for (int index = 0, i = 0; i < this->nvars; ++i)
						{
							const double tau_i = this->tau(i);
							const Vector<double> & design_precision_i = design_precision(i, i);
							const int offset_i = i * nlevels;

							for (int k = 0; k < nlevels; ++k)
							{
								// Upper-diagonal blocks
								for (int j = 0; j < i; ++j)
								{
									values(index) = design_precision(i, j)(k);
									indices(index) = j * nlevels + k;
									++index;
								}

								// Diagonal block
								const int p2 = this->R.Count(k + 1);
								for (int p = this->R.Count(k); p < p2; ++p)
								{
									const int l = this->R.Index(p);
									if (l != k)
									{
										// Off-diagonal elements
										values(index) = tau_i * this->R.Value(p);
										indices(index) = offset_i + l;
										++index;
									}
									else
									{
										// Diagonal elements
										values(index) = design_precision_i(k) + tau_i * this->R.Value(p);
										indices(index) = offset_i + l;
										++index;
									}
								}

								// Lower-diagonal blocks
								for (int j = i + 1; j < this->nvars; ++j)
								{
									values(index) = design_precision(j, i)(k);
									indices(index) = j * nlevels + k;
									++index;
								}
							}
						}

						// Build sparse precision matrix
						LDL::SparseMatrix<double> upper(values, indices, counts);

						// Decompose sparse precision matrix
						this->chol.Decompose(upper);
					}

					int SparsePrecisionModel::Update(const Vector<Vector<double> > & beta, Comparer comparer)
					{
						// Calculate variance
						const TriangularMatrix<double> variance = this->chol.Inverse();

						// Calculate jacobian and minus the hessian
						Vector<double> jac(this->nvars);
						TriangularMatrix<double> minus_hessian(this->nvars);
						const int nlevels = this->R.NumberOfColumns();
						for (int i = 0; i < this->nvars; ++i)
						{
							const double bsquare = Square(this->R, beta(i));
							const double trace = BlockTrace(i, variance, this->R);
							jac(i) = nlevels / this->tau(i) - bsquare - trace;
							minus_hessian(i, i) = nlevels / Math::Square(this->tau(i)) - BlockSquareTrace(i, variance, this->R);
							for (int j = 0; j < i; ++j)
								minus_hessian(i, j) = -BlockSquareTrace(i, j, variance, this->R);
						}

						// Calculate update
						CholeskyDecomposition chol(minus_hessian);
						Vector<double> h = chol.Solve(jac);

						const int update = comparer.IsZero(h, this->tau) ? 0 : 1;

						// Update sigma_square
						this->tau += h;
						while (Min(this->tau) <= 0.0)
						{
							// Back-track
							h *= 0.5;
							this->tau -= h;
						}

						return update;
					}

					Vector<Vector<double> > SparsePrecisionModel::CoefficientsUpdate(const Vector<Vector<double> > & design_jacobian, const Vector<Vector<double> > & beta) const
					{
						// Add diagonal terms
						const int nlevels = this->R.NumberOfColumns();
						NewTypes::Vector<double> jac(nlevels * this->nvars);
						for (int index = 0, i = 0; i < this->nvars; ++i)
						{
							const Vector<double> & design_jacobian_i = design_jacobian(i);
							const Vector<double> & beta_i = beta(i);
							const double tau_i = this->tau(i);
							for (int k = 0; k < nlevels; ++k, ++index)
							{
								// Notice that since R = R^T, we can use a transpose matrix product
								// optimized by the column-sparse structure of R
								jac(index) = design_jacobian_i(k) - tau_i * TMatrixProduct(k, this->R, beta_i);
							}
						}

						// Solve
						NewTypes::Vector<double> h_tmp = this->chol.Solve(jac);

						// Copy update. TODO: Change return type to NewType::Vector
						Vector<Vector<double> > h(this->nvars);
						for (int index = 0, i = 0; i < this->nvars; ++i)
						{
							h(i).Size(nlevels);
							Vector<double> & hi = h(i);
							for (int k = 0; k < nlevels; ++k, ++index)
								hi(k) = h_tmp(index);
						}

						return h;
					}
				}
			}
		}
	}
}
