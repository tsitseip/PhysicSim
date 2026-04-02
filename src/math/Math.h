/**
 * @file Math.h
 * @brief Mathematical utilities, including Vector and Matrix classes.
 */

#ifndef MATH_H
#define MATH_H

#include "core/Globals.h"

/**
 * @namespace math
 * @brief Namespace for mathematical operations and structures.
 */
namespace math {

    /**
     * @brief Pi constant.
     * @tparam T The numeric type.
     */
    template<typename T> constexpr T pi = std::numbers::pi_v<T>;

    /**
     * @brief Sine function wrapper.
     * @tparam T The numeric type.
     * @param angle The angle in radians.
     * @return The sine of the angle.
     */
    template<typename T> inline T sin(T angle) { return std::sin(angle); }

    /**
     * @brief Cosine function wrapper.
     * @tparam T The numeric type.
     * @param angle The angle in radians.
     * @return The cosine of the angle.
     */
    template<typename T> inline T cos(T angle) { return std::cos(angle); }

    /**
     * @brief Square root function wrapper.
     * @tparam T The numeric type.
     * @param value The value.
     * @return The square root of the value.
     */
    template<typename T> inline T sqrt(T value) { return std::sqrt(value); }

    /**
     * @brief Power function wrapper.
     * @tparam T The numeric type.
     * @param base The base.
     * @param exp The exponent.
     * @return The base raised to the power of exp.
     */
    template<typename T> inline T pow(T base, T exp) { return std::pow(base, exp); }

    /**
     * @brief Converts degrees to radians.
     * @tparam T The numeric type.
     * @param degrees The angle in degrees.
     * @return The angle in radians.
     */
    template<typename T> constexpr T to_radians(T degrees) { return degrees * (pi<T> / T(180)); }


    template <typename T, core::size_type D>
    class Matrix;

    /**
     * @brief A D-dimensional vector.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     */
    template <typename T, core::size_type D>
    class Vector {
        private:
            core::fixed_array<T, D> data;
        public:
            using value_type = T;
            using size_type = core::size_type;
            using reference = T&;
            using const_reference = const T&;

            /**
             * @brief Construct a new Vector object from a fixed array.
             * @param arr The array containing the vector elements.
             */
            Vector(const core::fixed_array<T, D>& arr);

            /**
             * @brief Construct a new Vector object (default constructor).
             */
            Vector();

            /**
             * @brief Copy constructor.
             */
            Vector(const Vector& other) = default;

            /**
             * @brief Default destructor.
             */
            ~Vector() = default;

            /**
             * @brief Returns the dimension of the vector.
             * @return The dimension.
             */
            core::size_type dimension() const noexcept;
            
            /**
             * @brief Access element at index.
             * @param index The index.
             * @return Reference to the element.
             */
            reference operator[](core::size_type index);

            /**
             * @brief Access element at index (const).
             * @param index The index.
             * @return Const reference to the element.
             */
            const_reference operator[](core::size_type index) const;

            /**
             * @brief Assignment operator.
             * @param other The other vector.
             * @return Reference to this vector.
             */
            Vector& operator=(const Vector& other);

            /**
             * @brief Move assignment operator.
             * @param other The other vector.
             * @return Reference to this vector.
             */
            Vector& operator=(Vector&& other) noexcept;
            
            /**
             * @brief Compound addition operator.
             * @param other The other vector.
             * @return Reference to this vector.
             */
            Vector& operator+=(const Vector& other);

            /**
             * @brief Compound subtraction operator.
             * @param other The other vector.
             * @return Reference to this vector.
             */
            Vector& operator-=(const Vector& other);

            /**
             * @brief Compound scalar multiplication operator.
             * @param scalar The scalar.
             * @return Reference to this vector.
             */
            Vector& operator*=(const T& scalar);

            /**
             * @brief Compound matrix multiplication operator.
             * @param other The matrix.
             * @return Reference to this vector.
             */
            Vector& operator*=(const Matrix<T, D>& other);
    };

    /**
     * @brief A DxD matrix.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     */
    template <typename T, core::size_type D>
    class Matrix {
        private:
            core::fixed_array<Vector<T, D>, D> data;
        public:
            using value_type = T;
            using sizecore_type = core::size_type;
            using reference = Vector<T, D>&;
            using const_reference = const Vector<T, D>&;

            /**
             * @brief Returns an identity matrix.
             * @return The identity matrix.
             */
            static Matrix identity() {
                Matrix id = Matrix(core::fixed_array<core::fixed_array<T, D>, D>{});
                for (core::size_type i = 0; i < D; ++i) {
                    for (core::size_type j = 0; j < D; ++j) {
                        id[i][j] = (i == j) ? T(1) : T(0);
                    }
                }
                return id;
            }

            /**
             * @brief Returns a rotation matrix for a given plane and angle.
             * @param i First index of the plane.
             * @param j Second index of the plane.
             * @param theta The angle in radians.
             * @return The rotation matrix.
             */
            static Matrix rotation(core::size_type i, core::size_type j, T theta) {
                Matrix res = Matrix::identity();
                res[i][i] = math::cos(theta);
                res[j][j] = math::cos(theta);
                res[i][j] = -math::sin(theta);
                res[j][i] = math::sin(theta);
                return res;
            }

            /**
             * @brief Construct a new Matrix object from a 2D fixed array.
             * @param arr The 2D array.
             */
            Matrix(const core::fixed_array<core::fixed_array<T, D>, D>& arr);

            /**
             * @brief Default destructor.
             */
            ~Matrix() = default;

            /**
             * @brief Returns the dimension of the matrix.
             * @return The dimension.
             */
            core::size_type dimension() const noexcept;
            
            /**
             * @brief Access row at index.
             * @param index The index.
             * @return Reference to the row vector.
             */
            reference operator[](core::size_type index);

            /**
             * @brief Access row at index (const).
             * @param index The index.
             * @return Const reference to the row vector.
             */
            const_reference operator[](core::size_type index) const;

            /**
             * @brief Compound matrix multiplication operator.
             * @param other The other matrix.
             * @return Reference to this matrix.
             */
            Matrix& operator*=(const Matrix& other);
    };

    /**
     * @brief Vector addition operator.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     * @param lhs The left-hand side vector.
     * @param rhs The right-hand side vector.
     * @return The resulting vector.
     */
    template <typename T, core::size_type D>
    Vector<T, D> operator+(Vector<T, D> lhs, const Vector<T, D>& rhs);

    /**
     * @brief Vector subtraction operator.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     * @param lhs The left-hand side vector.
     * @param rhs The right-hand side vector.
     * @return The resulting vector.
     */
    template <typename T, core::size_type D>
    Vector<T, D> operator-(Vector<T, D> lhs, const Vector<T, D>& rhs);

    /**
     * @brief Vector-scalar multiplication operator.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     * @param v The vector.
     * @param scalar The scalar.
     * @return The resulting vector.
     */
    template <typename T, core::size_type D>
    Vector<T, D> operator*(Vector<T, D> v, const T& scalar);

    /**
     * @brief Vector-matrix multiplication operator.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     * @param v The vector.
     * @param scalar The matrix.
     * @return The resulting vector.
     */
    template <typename T, core::size_type D>
    Vector<T, D> operator*(Vector<T, D> v, Matrix<T, D> scalar);

    /**
     * @brief Computes the dot product of two vectors.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     * @param a The first vector.
     * @param b The second vector.
     * @return The dot product.
     */
    template <typename T, core::size_type D>
    T dot(const Vector<T, D>& a, const Vector<T, D>& b);

} // namespace math

#include "Math.tpp"

#endif // MATH_H
