#pragma once

#include <cmath>
#include <concepts>
#include <functional>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace linearAlgebra {

    using Scalar = double;

    using Angle = double;

    // Concept for the Scalar type of the Vector
    template<typename T>
    concept ScalarType = std::is_arithmetic<T>::value;      // Arithmetic operations have to be valid

    template<ScalarType T, size_t Dim>
    class Vector {
        public:
            template <ScalarType ...Xn>
            requires(sizeof...(Xn) == Dim)      // Only activate if number of elements in list == Dim of the Vector
            Vector(const Xn& ... xn) {
                vec = std::initializer_list<T>{xn...};
            }

            auto operator+(const Vector& other) const -> Vector<T, Dim> {        // Vector addition
                std::vector<T> ret;
                std::transform(vec.begin(), vec.end(),
                    other.vec.begin(),
                    std::back_inserter(ret),
                    std::plus<T>());
                return ret;
            };

            auto operator-(const Vector& other) const -> Vector<T, Dim> {        // Vector subtraction
                std::vector<T> ret;
                std::transform(vec.begin(), vec.end(),
                    other.vec.begin(),
                    std::back_inserter(ret),
                    std::minus<T>());
                return ret;
            };

            auto operator*(const Scalar& s) const -> Vector<T, Dim> {            // Multiplication with Scalar on Vector
                std::vector<T> ret;
                std::transform(vec.begin(), vec.end(),
                    std::back_inserter(ret),
                    [&s](Scalar elem){ return s*elem; });
                return ret;
            }

            auto operator*(const Vector& other) const -> Scalar {                // Dotproduct
                Scalar ret = 0.0;
                for(size_t i=0; i<vec.size(); i++)
                    ret += vec[i]*other.vec[i];
                return ret;
            }

            auto length() const -> Scalar {
                Scalar ret = 0.0;
                for(size_t i=0; i<vec.size(); i++)
                    ret += vec[i]*vec[i];
                return std::sqrt(ret);
            }

            auto toString() const -> std::string {                        // Convert to string to output
                std::string str = "[ ";
                for(size_t i=0; i<vec.size()-1; i++) {
                    str += std::to_string(vec[i]) + ", ";
                }
                str += std::to_string(vec[vec.size()-1]) + " ]";
                return str;
            }

            // Enable Crossproduct only when Dim == 3
            auto x(const Vector<Scalar, 3>& other) const -> typename std::enable_if<Dim == 3, Vector<Scalar, 3>>::type {
                return {
                    vec[1]*other.vec[2] - vec[2]*other.vec[1],
                    vec[2]*other.vec[0] - vec[0]*other.vec[2],
                    vec[0]*other.vec[1] - vec[1]*other.vec[0]
                };
            }

        private:
            Vector(std::vector<T> v) {
                vec = v;
            }

            std::vector<T> vec;
    };

    using Vector3d = Vector<Scalar, 3>;

    template<ScalarType S, size_t Dim>
    inline auto operator<<(std::ostream& os, const Vector<S, Dim>& v) -> std::ostream&
    {
        os << v.toString();
        return os;
    }

}