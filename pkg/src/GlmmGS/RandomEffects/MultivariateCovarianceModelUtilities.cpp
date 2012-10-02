#include "../Standard.h"
#include "MultivariateCovarianceModelUtilities.h"

namespace GlmmGS
{
	namespace RandomEffects
	{
		// Functions for trace implementation
		namespace Functions
		{
			// Trace functions
			inline double Trace(double x)
			{
				return x;
			}

			inline double Trace(const ImmutableVector<double> & x)
			{
				return LinearAlgebra::Sum(x);
			}

			// SquareTrace functions
			inline double SquareTrace(double x, double y)
			{
				return x * y;
			}

			inline double SquareTrace(const ImmutableVector<double> & x, const ImmutableVector<double> & y)
			{
				return LinearAlgebra::ScalarProduct(x, y);
			}

			// Ordered triangular index
			inline static int Oti(int i, int j)
			{
				return ((i * (i + 1)) >> 1) + j;
			}

			// Triangular index
			inline static int Ti(int i, int j)
			{
				return j <= i ? Oti(i, j) : Oti(j, i);
			}

			// FSquareTrace
			double FSquareTrace(const ImmutableTriangularMatrix<double> & x, int i1, int j1, int i2, int j2)
			{
				if (i1 == j1 && i2 == j2)
				{
					return x(Oti(i1, i2), Oti(i1, i2));
				}
				else if (i1 == j1 || j2 == i2)
				{
					return 2.0 * x(Oti(i1, i2), Ti(j1, j2));
				}
				else
				{
					return 2.0 * (x(Oti(i1, i2), Ti(j1, j2)) +
								  x(Oti(i1, j2), Ti(j1, i2)));
				}
			}
		}

		// From precision vector to square variance matrix
		ImmutableTriangularMatrix<double> ThetaToVariance(const ImmutableVector<double> & theta, int nvars)
		{
			TriangularMatrix<double> prec(nvars);
			for (int ij = 0, i = 0; i < nvars; ++i)
				for (int j = 0; j <= i; ++j, ++ij)
					prec(i, j) = theta(ij);
			return CholeskyDecomposition(prec).Inverse();
		}

		// FProduct
		FProduct::FProduct(const ImmutableVector<Vector<double> > & x)
			: fproducts((x.Size() * (x.Size() + 1)) >> 1)
		{
			for (int ij = 0, i = 0; i < x.Size(); ++i)
			{
				for (int j = 0; j < i; ++j, ++ij)
					this->fproducts(ij) = 2.0 * LinearAlgebra::ScalarProduct(x(i), x(j));
				this->fproducts(ij) = LinearAlgebra::Square(x(i));
				++ij;
			}
		}

		// class Trace
		template <class TYPE>
		FTrace<TYPE>::FTrace(const ImmutableTriangularMatrix<TYPE> & x)
			: ftraces((x.NumberOfRows() * (x.NumberOfRows() + 1)) >> 1)
		{
			for (int ij = 0, i = 0; i < x.NumberOfRows(); ++i)
			{
				for (int j = 0; j < i; ++j, ++ij)
					this->ftraces(ij) = 2.0 * Functions::Trace(x(i, j));
				this->ftraces(ij) = Functions::Trace(x(i, i));
				++ij;
			}
		}

		// class SquareTrace
		template <class TYPE>
		FSquareTrace<TYPE>::FSquareTrace(const ImmutableTriangularMatrix<TYPE> & x)
			: fsquare_traces((x.NumberOfRows() * (x.NumberOfRows() + 1)) >> 1)
		{
			TriangularMatrix<double> square_traces(fsquare_traces.NumberOfRows());
			for (int ij1 = 0, i1 = 0; i1 < x.NumberOfRows(); ++i1)
				for (int j1 = 0; j1 <= i1; ++j1, ++ij1)
					for (int ij2 = 0, i2 = 0; i2 <= i1; ++i2)
						for (int j2 = 0; j2 <= i2 && ij2 <= ij1; ++j2, ++ij2)
							square_traces(ij1, ij2) = Functions::SquareTrace(x(i1, j1), x(i2, j2));

			for (int ij1 = 0, i1 = 0; i1 < x.NumberOfRows(); ++i1)
				for (int j1 = 0; j1 <= i1; ++j1, ++ij1)
					for (int ij2 = 0, i2 = 0; i2 <= i1; ++i2)
						for (int j2 = 0; j2 <= i2 && ij2 <= ij1; ++j2, ++ij2)
							this->fsquare_traces(ij1, ij2) = Functions::FSquareTrace(square_traces, i1, j1, i2, j2);

		}

		// Explicit instantiation
		template class FTrace<double>;
		template class FTrace<Vector<double> >;
		template class FSquareTrace<double>;
		template class FSquareTrace<Vector<double> >;
	}
}
