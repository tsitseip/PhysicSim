/**
 * @file Globals.h
 * @brief Global type aliases, constants, and utilities for the core namespace.
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include "StdIncludes.h"
#include "Constants.h"

#ifndef DIMENSION
#define DIMENSION 3
#endif

/**
 * @namespace core
 * @brief Core utilities and type aliases.
 */
namespace core {
    // Containers
    /**
     * @brief Type alias for std::vector.
     * @tparam T Type of the elements.
     */
    template <typename T>
    using array = std::vector<T>;

    /**
     * @brief Type alias for std::array.
     * @tparam T Type of the elements.
     * @tparam N Number of elements.
     */
    template <typename T, std::size_t N>
    using fixed_array = std::array<T, N>;

    /**
     * @brief Type alias for std::deque.
     * @tparam T Type of the elements.
     */
    template <typename T>
    using list = std::deque<T>;

    /**
     * @brief Type alias for std::map.
     * @tparam K Key type.
     * @tparam V Value type.
     */
    template <typename K, typename V>
    using map = std::map<K, V>;

    // Strings
    /**
     * @brief Type alias for std::string.
     */
    using string = std::string;
    /**
     * @brief Type alias for std::string_view.
     */
    using string_view = std::string_view;

    // Smart Pointers
    /**
     * @brief Type alias for std::unique_ptr.
     * @tparam T Type of the object.
     */
    template <typename T>
    using unique_ptr = std::unique_ptr<T>;

    /**
     * @brief Type alias for std::shared_ptr.
     * @tparam T Type of the object.
     */
    template <typename T>
    using shared_ptr = std::shared_ptr<T>;

    inline auto& nullopt = std::nullopt;

    /**
     * @brief Casts a value to an rvalue reference to enable moving.
     * @tparam T The type of the object.
     * @param arg The object to be moved.
     * @return rvalue reference to arg.
     */
    template <class T>
    constexpr std::remove_reference_t<T>&& move(T&& arg) noexcept {
        return std::move(arg);
    }

    /**
     * @brief Forwards lvalues as lvalues and rvalues as rvalues.
     * @tparam T The type of the object.
     * @param arg The object to be forwarded.
     * @return T&&.
     */
    template <class T>
    constexpr T&& forward(std::remove_reference_t<T>& arg) noexcept {
        return std::forward<T>(arg);
    }

    template <class T>
    constexpr T&& forward(std::remove_reference_t<T>&& arg) noexcept {
        return std::forward<T>(arg);
    }

    /**
     * @brief Creates a unique_ptr.
     * @tparam T Type of the object.
     * @tparam Args Variadic template arguments for the constructor.
     * @param args Arguments to pass to the constructor.
     * @return std::unique_ptr to the newly created object.
     */
    template <typename T, typename... Args>
    auto make_unique(Args&&... args) {
        return std::make_unique<T>(core::forward<Args>(args)...);
    }

    /**
     * @brief Creates a shared_ptr.
     * @tparam T Type of the object.
     * @tparam Args Variadic template arguments for the constructor.
     * @param args Arguments to pass to the constructor.
     * @return std::shared_ptr to the newly created object.
     */
    template <typename T, typename... Args>
    auto make_shared(Args&&... args) {
        return std::make_shared<T>(core::forward<Args>(args)...);
    }

    // JSON
    /**
     * @brief Type alias for nlohmann::json.
     */
    using json = nlohmann::json;

    // Concurrency
    /**
     * @brief Type alias for std::mutex.
     */
    using mutex = std::mutex;
    /**
     * @brief Type alias for std::thread.
     */
    using thread = std::thread;
    /**
     * @brief Type alias for std::atomic.
     * @tparam T Type of the atomic variable.
     */
    template <typename T>
    using atomic = std::atomic<T>;

    /**
     * @brief Type alias for std::lock_guard with std::mutex.
     */
    template <typename T>
    using lock_guard = std::lock_guard<std::mutex>;

    // I/O
    /**
     * @brief Type alias for std::ifstream.
     */
    using ifstream = std::ifstream;
    /**
     * @brief Type alias for std::ofstream.
     */
    using ofstream = std::ofstream;
    /**
     * @brief Type alias for std::iostream.
     */
    using iostream = std::iostream;
    
    /**
     * @brief Reference to std::cout.
     */
    inline auto& cout = std::cout;
    
    /**
     * @brief Reference to std::cerr.
     */
    inline auto& cerr = std::cerr;

    /**
     * @brief Alias for newline constant.
     */
    constexpr auto nl = constants::nl;
    
    /**
     * @brief Formats a string using std::format.
     * @tparam Args Variadic template arguments for the format string.
     * @param fmt The format string.
     * @param args The arguments to format.
     * @return The formatted string.
     */
    template <typename... Args>
    auto format(std::format_string<Args...> fmt, Args&&... args) {
        return std::format(fmt, core::forward<Args>(args)...);
    }

    // Others
    /**
     * @brief Type alias for std::exception.
     */
    using exception = std::exception;
    /**
     * @brief Type alias for std::optional.
     * @tparam T Type of the optional value.
     */
    template <typename T>
    using optional = std::optional<T>;

    /**
     * @brief Type alias for std::function.
     * @tparam R Return type of the function.
     * @tparam Args Argument types of the function.
     */
    template<typename Signature>
    using func = std::function<Signature>;


    /**
    * @brief Type alias for std::size_t.
    */
    using size_type = std::size_t;

}

#endif // GLOBALS_H
