module;
#include <initializer_list>
export module Matrix;
import Vector;

export template<const size_t N = 3, class T = float>
class Matrix
{
	public:
	Matrix(std::initializer_list<std::initializer_list<T>> values)
	{
		for (size_t x = 0; x < N; x++)
		{
			const auto outer = values.begin()[x];
			for (size_t y = 0; y < N; y++)
			{
				const auto inner = outer.begin()[y];
				this->values[x][y] = inner;
			}
		}
	}

	Matrix() : values{}
	{}

	T values[N][N];

	public:
	static Matrix Identity()
	{
		Matrix<N, T> newMat = Matrix();
		for (size_t x = 0, y = 0; x < N; x++, y++)
		{
			newMat.values[x][y] = 1;
		}
		return newMat;
	}

	Matrix Add(const Matrix& other)
	{
		Matrix<N, T> newMat = Matrix();
		for (size_t x = 0; x < N; x++)
		{
			for (size_t y = 0; y < N; y++)
			{
				newMat[x][y] = values[x][y] + other.values[x][y];
			}
		}
		return newMat;
	}

	Matrix Sub(const Matrix& other)
	{
		Matrix<N, T> newMat = Matrix();
		for (size_t x = 0; x < N; x++)
		{
			for (size_t y = 0; y < N; y++)
			{
				newMat[x][y] = values[x][y] - other.values[x][y];
			}
		}
		return newMat;
	}

	Matrix Dot(const Matrix& other)
	{
		Matrix<N, T> newMat = Matrix();
		for (size_t row = 0; row < N; row++)
		{
			for (size_t col = 0; col < N; col++)
			{
				for (size_t i = 0; i < N; i++)
				{
					const auto a = values[row][i];
					const auto b = other.values[i][col];
					newMat.values[row][col] += a * b;
				}
			}
		}
		return newMat;
	}

	Matrix Cross(const Matrix& other)
	{
		Matrix<N, T> newMat = Matrix();
		for (size_t i = 0; i < N; i++)
		{
			for (size_t x = 0, y = 0; x < N; x++, y++)
			{
				newMat.values[x][y + i] = values[x][y + i] * other.values[x][y + i];
			}
		}
		return newMat;
	}

	T Det()
	{
		T mainDiag = 1;
		for (size_t x = 0, y = 0; x < N; x++, y++)
		{
			mainDiag *= values[x][y];
		}
		T oppDiag = 1;
		for (size_t x = 0, y = N - 1; x < N; x++, y--)
		{
			oppDiag *= values[x][y];
		}
		return mainDiag - oppDiag;
	}
};