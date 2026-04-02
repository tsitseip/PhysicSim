/**
 * @file Shader.h
 * @brief Defines the Shader class for compiling and managing OpenGL shaders.
 */
#ifndef SHADER_H
#define SHADER_H

#include "core/Globals.h"
#include <glad/glad.h>

/**
 * @namespace graphics
 * @brief Graphics rendering components and utilities.
 */
namespace graphics {
    /**
     * @brief A class for managing OpenGL shaders, including loading, compiling, and linking.
     */
    class Shader {
    public:
        /**
         * @brief OpenGL ID of the shader program.
         */
        unsigned int ID;

        /**
         * @brief Constructs a new Shader object.
         * @param vSource The source code for the vertex shader.
         * @param fSource The source code for the fragment shader.
         */
        Shader(const char* vSource, const char* fSource) {
            unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vSource, NULL);
            glCompileShader(vertex);
            checkErrors(vertex, "VERTEX");

            unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fSource, NULL);
            glCompileShader(fragment);
            checkErrors(fragment, "FRAGMENT");

            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);
            checkErrors(ID, "PROGRAM");

            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        /**
         * @brief Deletes the OpenGL shader program on destruction.
         */
        ~Shader() {
            if (ID != 0) {
                glDeleteProgram(ID);
            }
        }

        /**
         * @brief Activates the shader program for use in OpenGL.
         */
        void use() { glUseProgram(ID); }

    private:
        /**
         * @brief Checks for errors in shader compilation or program linking.
         * @param shader The shader or program to check.
         * @param type The type of the shader or program.
         */
        void checkErrors(unsigned int shader, core::string type) {
            int success;
            char infoLog[1024];
            if (type != "PROGRAM") {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success) {
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    core::cerr << "SHADER_COMPILATION_ERROR: " << type << "\n" << infoLog << core::nl;
                }
            } else {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success) {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    core::cerr << "PROGRAM_LINKING_ERROR: " << type << "\n" << infoLog << core::nl;
                }
            }
        }
    };
} // namespace graphics


#endif