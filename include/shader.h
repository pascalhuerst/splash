/*
 * Copyright (C) 2013 Emmanuel Durand
 *
 * This file is part of Splash.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * blobserver is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with blobserver.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * @shader.h
 * The Shader class
 */

#ifndef SHADER_H
#define SHADER_H

#define GLFW_NO_GLU
#define GL_GLEXT_PROTOTYPES

#include "config.h"
#include "coretypes.h"

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <GLFW/glfw3.h>

#include "texture.h"

namespace Splash {

class Shader : public BaseObject
{
    public:
        enum ShaderType
        {
            vertex = 0,
            geometry,
            fragment
        };

        enum Sideness
        {
            doubleSided = 0,
            singleSided,
            inverted
        };

        /**
         * Constructor
         */
        Shader();

        /**
         * Destructor
         */
        ~Shader();

        /**
         * No copy constructor, but a move one
         */
        Shader(const Shader&) = delete;
        Shader(Shader&& s)
        {
            _shaders = s._shaders;
            _program = s._program;
            _isLinked = s._isLinked;
            _locationMVP = s._locationMVP;
            _locationNormalMatrix = s._locationNormalMatrix;
            _locationSide = s._locationSide;
            _locationTextureNbr = s._locationTextureNbr;

            _sideness = s._sideness;
            _textureNbr = s._textureNbr;
        }

        /**
         * Activate this shader
         */
        void activate();

        /**
         * Deactivate this shader
         */
        void deactivate();

        /**
         * Set the sideness of the object
         */
        void setSideness(const Sideness side);

        /**
         * Set a shader source
         */
        void setSource(const std::string& src, const ShaderType type);

        /**
         * Set a shader source from file
         */
        void setSourceFromFile(const std::string filename, const ShaderType type);

        /**
         * Add a new texture to use
         */
        void setTexture(const TexturePtr texture, const GLuint textureUnit, const std::string& name);

        /**
         * Set the view projection matrix
         */
        void setViewProjectionMatrix(const glm::mat4& mvp);

    private:
        std::map<ShaderType, GLuint> _shaders;
        GLuint _program {0};
        bool _isLinked = {false};
        GLint _locationMVP {0};
        GLint _locationNormalMatrix {0};
        GLint _locationSide {0};
        GLint _locationTextureNbr {0};

        Sideness _sideness {doubleSided};
        int _textureNbr {0};

        void compileProgram();
        bool linkProgram();

    public:
        /**
         * Default vertex shader
         */
        const std::string DEFAULT_VERTEX_SHADER {R"(
            #version 330 core

            layout(location = 0) in vec4 _vertex;
            layout(location = 1) in vec2 _texcoord;
            layout(location = 2) in vec3 _normal;
            uniform mat4 _viewProjectionMatrix;
            uniform mat4 _normalMatrix;
            smooth out vec2 texCoord;
            smooth out vec3 normal;

            void main(void)
            {
                gl_Position = _viewProjectionMatrix * _vertex;
                normal = (_normalMatrix * vec4(_normal, 0.0)).xyz;
                texCoord = _texcoord;
            }
        )"};

        /**
         * Default fragment shader
         */
        const std::string DEFAULT_FRAGMENT_SHADER {R"(
            #version 330 core

            uniform sampler2D _tex0;
            uniform sampler2D _tex1;
            uniform int _sideness;
            uniform int _textureNbr;
            in vec2 texCoord;
            in vec3 normal;
            out vec4 fragColor;

            void main(void)
            {
                if ((dot(normal, vec3(0.0, 0.0, 1.0)) >= 0.0 && _sideness == 1) || (dot(normal, vec3(0.0, 0.0, 1.0)) <= 0.0 && _sideness == 2))
                    discard;

                if (_textureNbr > 0)
                    fragColor = texture(_tex0, texCoord);
                if (_textureNbr > 1)
                {
                    vec4 color = texture(_tex1, texCoord);
                    if (color != vec4(0.0, 1.0, 1.0, 1.0))
                        fragColor = fragColor * 0.2 + color * 0.8;
                }
            }
        )"};

};

typedef std::shared_ptr<Shader> ShaderPtr;

} // end of namespace

#endif // SHADER_H
