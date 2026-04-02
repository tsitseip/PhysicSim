/* 
 * @file Math.tpp
 * @brief Provides the implementation for the Math classes.
 */
#ifndef MATH_TPP
#define MATH_TPP

namespace math {

    // ~~~~~~~~~~~~~~~~~ Vector Implementation ~~~~~~~~~~~~~~~~~

    template <typename T, core::size_type D>
    Vector<T, D>::Vector(const core::fixed_array<T, D>& arr) : data(arr) {}

    template <typename T, core::size_type D>
    Vector<T, D>::Vector() : data({}) {
        data.fill(T{});
    }

    template <typename T, core::size_type D>
    core::size_type Vector<T, D>::dimension() const noexcept {
        return D;
    }

    template <typename T, core::size_type D>
    typename Vector<T, D>::reference Vector<T, D>::operator[](core::size_type index) {
        return data[index];
    }

    template <typename T, core::size_type D>
    typename Vector<T, D>::const_reference Vector<T, D>::operator[](core::size_type index) const {
        return data[index];
    }

    template <typename T, core::size_type D>
    Vector<T, D>& Vector<T, D>::operator=(const Vector& other) {
        if (this != &other) data = other.data;
        return *this;
    }

    template <typename T, core::size_type D>
    Vector<T, D>& Vector<T, D>::operator=(Vector&& other) noexcept {
        if (this != &other) data = core::move(other.data);
        return *this;
    }

    template <typename T, core::size_type D>
    Vector<T, D>& Vector<T, D>::operator+=(const Vector& other) {
        for (core::size_type i = 0; i < D; ++i) data[i] += other.data[i];
        return *this;
    }

    template <typename T, core::size_type D>
    Vector<T, D>& Vector<T, D>::operator-=(const Vector& other) {
        for (core::size_type i = 0; i < D; ++i) data[i] -= other.data[i];
        return *this;
    }

    template <typename T, core::size_type D>
    Vector<T, D>& Vector<T, D>::operator*=(const T& scalar) {
        for (core::size_type i = 0; i < D; ++i) data[i] *= scalar;
        return *this;
    }

    template <typename T, core::size_type D>
    Vector<T, D>& Vector<T, D>::operator*=(const Matrix<T, D>& other) {
        Vector<T, D> result = *this;
        for (core::size_type i = 0; i < D; ++i) {
            data[i] = T{};
            for (core::size_type j = 0; j < D; ++j) { 
                data[i] += result.data[j] * other[j][i];
            }
        }
        return *this;
    }

    // ~~~~~~~~~~~~~~~~~ Matrix Implementation ~~~~~~~~~~~~~~~~~

    template <typename T, core::size_type D>
    Matrix<T, D>::Matrix(const core::fixed_array<core::fixed_array<T, D>, D>& arr) {
        for (core::size_type i = 0; i < D; ++i) data[i] = Vector<T, D>(arr[i]);
    }

    template <typename T, core::size_type D>
    core::size_type Matrix<T, D>::dimension() const noexcept {
        return D;
    }

    template <typename T, core::size_type D>
    typename Matrix<T, D>::reference Matrix<T, D>::operator[](core::size_type index) {
        return data[index];
    }

    template <typename T, core::size_type D>
    typename Matrix<T, D>::const_reference Matrix<T, D>::operator[](core::size_type index) const {
        return data[index];
    }

    template <typename T, core::size_type D>
    Matrix<T, D>& Matrix<T, D>::operator*=(const Matrix& other) {
        Matrix<T, D> result = *this;
        for (core::size_type i = 0; i < D; ++i) {
            for (core::size_type j = 0; j < D; ++j) {
                data[i][j] = T{};
                for (core::size_type k = 0; k < D; ++k) {
                    data[i][j] += result[i][k] * other[k][j];
                }
            }
        }
        return *this;
    }

    // ~~~~~~~~~~~~~~~~~ Global Operators & Functions ~~~~~~~~~~~~~~~~~

    template <typename T, core::size_type D>
    Vector<T, D> operator+(Vector<T, D> lhs, const Vector<T, D>& rhs) {
        return lhs += rhs;
    }

    template <typename T, core::size_type D>
    Vector<T, D> operator-(Vector<T, D> lhs, const Vector<T, D>& rhs) {
        return lhs -= rhs;
    }

    template <typename T, core::size_type D>
    Vector<T, D> operator*(Vector<T, D> v, const T& scalar) {
        return v *= scalar;
    }

    template <typename T, core::size_type D>
    T dot(const Vector<T, D>& a, const Vector<T, D>& b) {
        T result = T{};
        for (core::size_type i = 0; i < D; ++i) result += a[i] * b[i];
        return result;
    }

    template <typename T, core::size_type D>
    Vector<T, D> operator*(Vector<T, D> v, Matrix<T, D> m) {
        return v *= m;
    }

} // namespace math

#endif // MATH_TPP
