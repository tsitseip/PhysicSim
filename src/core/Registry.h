/**
 * @file Registry.h
 * @brief Handles registration and creation of objects based on JSON configuration.
 */
#ifndef REGISTRY_H
#define REGISTRY_H

#include "core/Globals.h"

/**
 * @namespace core
 * @brief Core utilities and classes.
 */
namespace core {

    /**
     * @brief A generic Singleton Registry for any interface type (Base).
     * 
     * Allows for dynamic object creation by mapping string names to creator functions.
     * 
     * @tparam Base The interface type to be managed by the registry.
     */
    template <typename Base>
    class Registry {
    public:
        /**
         * @brief Function type that creates a unique_ptr of Base from JSON configuration.
         */
        using CreatorFunc = core::func<core::unique_ptr<Base>(const json&)>;

        /**
         * @brief Returns the singleton instance of the Registry for the given Base type.
         * @return A reference to the Singleton Registry instance.
         */
        static Registry& get() {
            static Registry instance;
            return instance;
        }

        /**
         * @brief Registers a new creator function associated with a specific name.
         * @param name The name to associate with the creator function.
         * @param creator The function that creates an instance of Base from JSON.
         */
        void add(const core::string& name, CreatorFunc creator) {
            m_creators[name] = creator;
        }

        /**
         * @brief Creates an instance of Base using the registered creator function for the given name.
         * @param name The name of the object type to create.
         * @param config The JSON configuration to pass to the creator function.
         * @return A unique_ptr to the newly created object, or nullptr if not found.
         */
        core::unique_ptr<Base> create(const core::string& name, const json& config) {
            if (m_creators.contains(name)) {
                return m_creators[name](config);
            }
            core::cerr << core::format(core::constants::errors::REGISTRY_ERROR, name) << core::nl;
            return nullptr;
        }

    private:
        /**
         * @brief Private constructor for Singleton pattern.
         */
        Registry() = default;

        /**
         * @brief Map of names to creator functions.
         */
        core::map<core::string, CreatorFunc> m_creators;
    };

    /**
     * @brief Utility struct used to automatically register a creator function in a Registry upon initialization.
     * 
     * @tparam Base The interface type associated with the Registry.
     */
    template<typename Base>
    struct Registrar {
        /**
         * @brief Registers a creator function with the Registry when the Registrar is instantiated.
         * @param name The name to associate with the creator function.
         * @param creator The function that creates an instance of Base from JSON.
         */
        Registrar(const core::string& name, typename Registry<Base>::CreatorFunc creator){
            Registry<Base>::get().add(name, creator);
        }
    };

} // namespace core

#endif
