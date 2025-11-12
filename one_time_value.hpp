#ifndef ONE_TIME_VALUE_HPP
#define ONE_TIME_VALUE_HPP

#include <optional>
#include <utility>

/**
 * @brief A wrapper around a value that can only be read once.
 *
 * This class stores a single optional value of type `T`. Once the value
 * is accessed via `consume()` or `take_or()`, it is automatically cleared
 * (set to `std::nullopt`). This makes it useful for representing events,
 * tokens, or transient states that should only be read once.
 *
 * @tparam T Type of the stored value.
 */
template <typename T> class OneTimeValue {
  public:
    /**
     * @brief Constructs an empty OneTimeValue.
     */
    OneTimeValue() = default;

    /**
     * @brief Constructs the object with a copy of the given value.
     * @param value The value to store.
     */
    OneTimeValue(const T &value) : value_(value) {}

    /**
     * @brief Constructs the object by moving in the given value.
     * @param value The value to move into storage.
     */
    OneTimeValue(T &&value) : value_(std::move(value)) {}

    /**
     * @brief Sets a new value by copy.
     * @param value The value to store.
     */
    void set(const T &value) { value_ = value; }

    /**
     * @brief Sets a new value by move.
     * @param value The value to move into storage.
     */
    void set(T &&value) { value_ = std::move(value); }

    /**
     * @brief Checks if a value is currently stored.
     * @return `true` if a value exists, `false` otherwise.
     */
    bool has_value() const noexcept { return value_.has_value(); }

    /**
     * @brief Retrieves and clears the stored value.
     *
     * If a value is present, it is returned and the internal storage
     * is reset to `std::nullopt`. Otherwise, `std::nullopt` is returned.
     *
     * @return The stored value wrapped in `std::optional`, or `std::nullopt` if none.
     */
    std::optional<T> consume() {
        if (!value_)
            return std::nullopt;
        auto tmp = std::move(value_);
        value_.reset();
        return tmp;
    }

    /**
     * @brief Retrieves and clears the stored value, or returns a default.
     *
     * If a value is present, it is returned and cleared. If not, the
     * provided default value is returned instead.
     *
     * @param default_value The value to return if no stored value exists.
     * @return The stored or default value.
     */
    T take_or(const T &default_value) {
        if (value_) {
            T tmp = std::move(*value_);
            value_.reset();
            return tmp;
        }
        return default_value;
    }

    /**
     * @brief Clears the stored value manually.
     */
    void reset() noexcept { value_.reset(); }

  private:
    /// The internally stored optional value.
    std::optional<T> value_;
};

#endif // ONE_TIME_VALUE_HPP
