#pragma once

#include <array>
#include <chrono>
#include <type_traits>


namespace hidonash::core
{
    template<typename T>
    class InterpolatingValue
    {
        static_assert(std::is_floating_point<T>::value, "T should be float or double");

    public:
        InterpolatingValue(T value, double sampleRate,
                    const std::chrono::milliseconds& duration = std::chrono::milliseconds(5));

        operator T() const;

        void resetTo(T value);

        T operator++(int);

        T& operator++();

        void operator=(T value);

        void operator+=(T value);

    private:
        InterpolatingValue() = default;

        T currentValue_;
        T goalValue_;
        size_t durationInTicks_ {0};
        T step_;
        size_t remainingTicks_ {0};
    };

    template<typename Ty, size_t N>
    void assign(std::array<Ty, N>& from, std::array<InterpolatingValue<Ty>, N>& to)
    {
        for (auto n = 0; n < from.size(); ++n)
            to[n] = from[n];
    }

    extern template class InterpolatingValue<float>;
    extern template class InterpolatingValue<double>;
}
