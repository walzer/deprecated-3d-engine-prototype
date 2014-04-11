#include "Base.h"
#include "C3DEffect.h"
#include "C3DStream.h"
#include "C3DTexture.h"
#include "C3DSampler.h"
#include "C3DRenderState.h"
#include "C3DElementNode.h"
#include "C3DSamplerCube.h"
#include "Base.h"

#define OPENGL_ES_DEFINE  "#define OPENGL_ES"

namespace cocos3d
{
static C3DEffect* __currentEffect = NULL;

C3DEffect::C3DEffect(const std::string& name) : C3DResource(name),_program(0)
{
	LOG_TRACE_VARG("+Effect: %s", name.c_str());
}

C3DEffect::~C3DEffect()
{
	LOG_TRACE_VARG("-Effect: %s with ID:%d", getID().c_str(), _program);

    // Free uniforms.
    for (std::map<std::string, Uniform*>::iterator itr = _uniforms.begin(); itr != _uniforms.end(); itr++)
    {
        SAFE_DELETE(itr->second);
    }

    if (_program)
    {
        // If our program object is currently bound, unbind it before we're destroyed.
        if (__currentEffect == this)
        {
            GL_ASSERT( glUseProgram(0) );
            __currentEffect = NULL;
        }

        GL_ASSERT( glDeleteProgram(_program) );
        _program = 0;
    }
}

void replaceIncludes(const std::string& source, std::string& out)
{
    // Replace the #include "xxxx.xxx" with the sourced file contents of "filepath/xxxx.xxx"
    std::string str = source;
    size_t lastPos = 0;
    size_t headPos = 0;
    size_t fileLen = str.length();
    size_t tailPos = fileLen;
    while (headPos < fileLen)
    {
        lastPos = headPos;
        if (headPos == 0)
        {
            // find the first "#include"
            headPos = str.find("#include");
        }
        else
        {
            // find the next "#include"
            headPos = str.find("#include", headPos + 1);
        }

        // If "#include" is found
        if (headPos != std::string::npos)
        {
            // append from our last position for the legth (head - last position)
            out.append(str.substr(lastPos,  headPos - lastPos));

            // find the start quote "
            size_t startQuote = str.find("\"", headPos) + 1;
            if (startQuote == std::string::npos)
            {
                // We have started an "#include" but missing the leading quote "
                //LOG_ERROR_VARG("Compile failed for shader '%s' missing leading \".");
                return;
            }
            // find the end quote "
            size_t endQuote = str.find("\"", startQuote);
            if (endQuote == std::string::npos)
            {
                // We have a start quote but missing the trailing quote "
                //LOG_ERROR_VARG("Compile failed for shader '%s' missing trailing \".");
                return;
            }

            // jump the head position past the end quote
            headPos = endQuote + 1;

            // File path to include and 'stitch' in the value in the quotes to the file path and source it.
            std::string directoryPath = "shaders/";

            size_t len = endQuote - (startQuote);
            std::string includeStr = str.substr(startQuote, len);
            directoryPath.append(includeStr);

            char* includedSource = C3DStreamManager::readAll(directoryPath);
            if (includedSource == NULL)
            {
                //LOG_ERROR_VARG("Compile failed for shader '%s' invalid filepath.", directoryPath.c_str());
                return;
            }
            else
            {
                // Valid file so lets attempt to see if we need to append anything to it too (recurse...)
                replaceIncludes(includedSource, out);
                SAFE_DELETE_ARRAY(includedSource);
            }
        }
        else
        {
            // Append the remaining
            out.append(str.c_str(), lastPos, tailPos);
        }
    }
}

void C3DEffect::reload()
{
	setCurrentEffect(NULL);

	// Free uniforms.
    for (std::map<std::string, Uniform*>::iterator itr = _uniforms.begin(); itr != _uniforms.end(); ++itr)
    {
        SAFE_DELETE(itr->second);
    }

	_uniforms.clear();
	_vertexAttributes.clear();

	_program = 0;

	C3DElementNode* elementNode = C3DElementNode::createEmptyNode("test", "effect");
	if(elementNode != NULL)
	{
		elementNode->setElement("vertexShader", _vshPath);
		elementNode->setElement("fragmentShader", _fshPath);
		if(!_defines.empty())
			elementNode->setElement("defines", _defines);
	}

	load(elementNode);

	SAFE_DELETE(elementNode);
}

bool C3DEffect::load(C3DElementNode* node)
{
	C3DResource::load(node);

	const std::string& vshPath = node->getElement("vertexShader");
	const std::string& fshPath = node->getElement("fragmentShader");
	const std::string& defines = node->getElement("defines");

	_vshPath = vshPath;
	_fshPath = fshPath;
	_defines = defines;

    // Read source from file.
	char* vshSource = C3DStreamManager::readAll(vshPath);
    if (vshSource == NULL)
    {
		return false;
    }
	char* fshSource = C3DStreamManager::readAll(fshPath);
    if (fshSource == NULL)
    {
        SAFE_DELETE_ARRAY(vshSource);
		return false;
    }

    if(this->load(vshSource, fshSource, defines) == false)
	{
		LOG_ERROR_VARG("Failed to create effect from shaders: %s, %s", _vshPath.c_str(), _fshPath.c_str());
	}
    SAFE_DELETE_ARRAY(vshSource);
    SAFE_DELETE_ARRAY(fshSource);
    return true;
}

bool C3DEffect::load(const std::string& vshSource, const std::string& fshSource, const std::string& defines)
{
    const unsigned int SHADER_SOURCE_LENGTH = 3;
    const GLchar* shaderSource[SHADER_SOURCE_LENGTH];
    char* infoLog = NULL;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint program;
    GLint length;
    GLint success;

    // Compile vertex shader.
    std::string definesStr = defines.empty()? "" : defines;
#ifdef OPENGL_ES
    if (defines.length() != 0)
        definesStr += "\n";
    definesStr+= OPENGL_ES_DEFINE;
#endif
    shaderSource[0] = definesStr.c_str();
    shaderSource[1] = "\n";

	std::string vshSourceStr = "";
    {
        // Replace the #include "xxxxx.xxx" with the sources that come from file paths
        replaceIncludes(vshSource, vshSourceStr);
        if (!vshSource.empty())
            vshSourceStr += "\n";
    }

	shaderSource[2] = vshSourceStr.c_str();
    GL_ASSERT( vertexShader = glCreateShader(GL_VERTEX_SHADER) );
    GL_ASSERT( glShaderSource(vertexShader, SHADER_SOURCE_LENGTH, shaderSource, NULL) );
    GL_ASSERT( glCompileShader(vertexShader) );
    GL_ASSERT( glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success) );
    if (success != GL_TRUE)
    {
        GL_ASSERT( glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length) );
        if (length > 0)
        {
            infoLog = new char[length];
            GL_ASSERT( glGetShaderInfoLog(vertexShader, length, NULL, infoLog) );
            infoLog[length-1] = '\0';
        }
        LOG_ERROR_VARG("Compile failed for vertex shader (%s): %s", _vshPath.c_str() == NULL ? "NULL" : _vshPath.c_str(), infoLog == NULL ? "" : infoLog);
        SAFE_DELETE_ARRAY(infoLog);

        // Clean up.
        GL_ASSERT( glDeleteShader(vertexShader) );

        return false;
    }

    // Compile the fragment shader.
	std::string fshSourceStr = "";
    {
        // Replace the #include "xxxxx.xxx" with the sources that come from file paths
        replaceIncludes(fshSource, fshSourceStr);
        if (!fshSource.empty())
            fshSourceStr += "\n";
    }
	shaderSource[2] = fshSourceStr.c_str();

    GL_ASSERT( fragmentShader = glCreateShader(GL_FRAGMENT_SHADER) );
    GL_ASSERT( glShaderSource(fragmentShader, SHADER_SOURCE_LENGTH, shaderSource, NULL) );
    GL_ASSERT( glCompileShader(fragmentShader) );
    GL_ASSERT( glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success) );
    if (success != GL_TRUE)
    {
        GL_ASSERT( glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length) );
        if (length > 0)
        {
            infoLog = new char[length];
            GL_ASSERT( glGetShaderInfoLog(fragmentShader, length, NULL, infoLog) );
            infoLog[length-1] = '\0';
        }
        LOG_ERROR_VARG("Compile failed for fragment shader (%s): %s", _fshPath.c_str() == NULL ? "NULL" : _fshPath.c_str(), infoLog == NULL ? "" : infoLog);
        SAFE_DELETE_ARRAY(infoLog);

        // Clean up.
        GL_ASSERT( glDeleteShader(vertexShader) );
        GL_ASSERT( glDeleteShader(fragmentShader) );

        return false;
    }

    // Link program.
    GL_ASSERT( program = glCreateProgram() );
    GL_ASSERT( glAttachShader(program, vertexShader) );
    GL_ASSERT( glAttachShader(program, fragmentShader) );
    GL_ASSERT( glLinkProgram(program) );
    GL_ASSERT( glGetProgramiv(program, GL_LINK_STATUS, &success) );

    // Delete shaders after linking.
    GL_ASSERT( glDeleteShader(vertexShader) );
    GL_ASSERT( glDeleteShader(fragmentShader) );

    // Check link status.
    if (success != GL_TRUE)
    {
        GL_ASSERT( glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length) );
        if (length > 0)
        {
            infoLog = new char[length];
            GL_ASSERT( glGetProgramInfoLog(program, length, NULL, infoLog) );
            infoLog[length-1] = '\0';
        }
        LOG_ERROR_VARG("Linking program failed (%s,%s): %s", _vshPath.c_str() == NULL ? "NULL" : _vshPath.c_str(), _fshPath.c_str() == NULL ? "NULL" : _fshPath.c_str(), infoLog == NULL ? "" : infoLog);
        SAFE_DELETE_ARRAY(infoLog);

        // Clean up.
        GL_ASSERT( glDeleteProgram(program) );

        return false;
    }

    this->_program = program;

    // Query and store vertex attribute meta-data from the program.
    // NOTE: Rather than using glBindAttribLocation to explicitly specify our own
    // preferred attribute locations, we're going to query the locations that were
    // automatically bound by the GPU. While it can sometimes be convenient to use
    // glBindAttribLocation, some vendors actually reserve certain attribute indices
    // and thereore using this function can create compatibility issues between
    // different hardware vendors.
    GLint activeAttributes;
    GL_ASSERT( glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &activeAttributes) );
    if (activeAttributes > 0)
    {
        GL_ASSERT( glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &length) );
        if (length > 0)
        {
            GLchar* attribName = new GLchar[length + 1];
            GLint attribSize;
            GLenum attribType;
            GLint attribLocation;
            for (int i = 0; i < activeAttributes; ++i)
            {
                // Query attribute info.
                GL_ASSERT( glGetActiveAttrib(program, i, length, NULL, &attribSize, &attribType, attribName) );
                attribName[length] = '\0';

                // Query the pre-assigned attribute location.
                GL_ASSERT( attribLocation = glGetAttribLocation(program, attribName) );

                // Assign the vertex attribute mapping for the effect.
                this->_vertexAttributes[attribName] = attribLocation;
            }
            SAFE_DELETE_ARRAY(attribName);
        }
    }

    // Query and store uniforms from the program.
    GLint activeUniforms;
    GL_ASSERT( glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &activeUniforms) );
    if (activeUniforms > 0)
    {
        GL_ASSERT( glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length) );
        if (length > 0)
        {
            GLchar* uniformName = new GLchar[length + 1];
            GLint uniformSize;
            GLenum uniformType;
            GLint uniformLocation;
            unsigned int samplerIndex = 0;
            for (int i = 0; i < activeUniforms; ++i)
            {
                // Query uniform info.
                GL_ASSERT( glGetActiveUniform(program, i, length, NULL, &uniformSize, &uniformType, uniformName) );
                uniformName[length] = '\0';  // null terminate
                if (uniformSize > 1 && length > 3)
                {
                    // This is an array uniform. I'm stripping array indexers off it since GL does not
                    // seem to be consistent across different drivers/implementations in how it returns
                    // array uniforms. On some systems it will return "u_matrixArray", while on others
                    // it will return "u_matrixArray[0]".
                    char* c = strrchr(uniformName, '[');
                    if (c)
                    {
                        *c = '\0';
                    }
                }

                // Query the pre-assigned uniform location.
                GL_ASSERT( uniformLocation = glGetUniformLocation(program, uniformName) );

                Uniform* uniform = new Uniform();
                uniform->_effect = this;
                uniform->_name = uniformName;
                uniform->_location = uniformLocation;
                uniform->_type = uniformType;
				if (uniformType == GL_SAMPLER_2D || uniformType == GL_SAMPLER_CUBE)
					uniform->_index = samplerIndex++;
				else
					uniform->_index = /*uniformType == GL_SAMPLER_2D ? (samplerIndex++) : */0;

                this->_uniforms[uniformName] = uniform;
            }
            SAFE_DELETE_ARRAY(uniformName);
        }
    }
    return true;
}

//const std::string& C3DEffect::getId() const
//{
//    return _id.c_str();
//}

VertexAttribute C3DEffect::getVertexAttribute(const std::string& name) const
{
    std::map<std::string, VertexAttribute>::const_iterator itr = _vertexAttributes.find(name);
    return (itr == _vertexAttributes.end() ? -1 : itr->second);
}

Uniform* C3DEffect::getUniform(const std::string& name) const
{
    std::map<std::string, Uniform*>::const_iterator itr = _uniforms.find(name);
    return (itr == _uniforms.end() ? NULL : itr->second);
}

Uniform* C3DEffect::getUniform(unsigned int index) const
{
    unsigned int i = 0;
    for (std::map<std::string, Uniform*>::const_iterator itr = _uniforms.begin(); itr != _uniforms.end(); itr++, i++)
    {
        if (i == index)
        {
            return itr->second;
        }
    }
    return NULL;
}

unsigned int C3DEffect::getUniformCount() const
{
    return _uniforms.size();
}

void C3DEffect::bindValue(Uniform* uniform, float value)
{
    GL_ASSERT( glUniform1f(uniform->_location, value) );
}

void C3DEffect::bindValue(Uniform* uniform, const float* values, unsigned int count)
{
    GL_ASSERT( glUniform1fv(uniform->_location, count, values) );
}

void C3DEffect::bindValue(Uniform* uniform, int value)
{
    GL_ASSERT( glUniform1i(uniform->_location, value) );
}

void C3DEffect::bindValue(Uniform* uniform, const int* values, unsigned int count)
{
    GL_ASSERT( glUniform1iv(uniform->_location, count, values) );
}

void C3DEffect::bindValue(Uniform* uniform, const C3DMatrix& value)
{
    GL_ASSERT( glUniformMatrix4fv(uniform->_location, 1, GL_FALSE, value.m) );
}

void C3DEffect::bindValue(Uniform* uniform, const C3DMatrix* values, unsigned int count)
{
    GL_ASSERT( glUniformMatrix4fv(uniform->_location, count, GL_FALSE, (GLfloat*)values) );
}

void C3DEffect::bindValue(Uniform* uniform, const C3DVector2& value)
{
    GL_ASSERT( glUniform2f(uniform->_location, value.x, value.y) );
}

void C3DEffect::bindValue(Uniform* uniform, const C3DVector2* values, unsigned int count)
{
    GL_ASSERT( glUniform2fv(uniform->_location, count, (GLfloat*)values) );
}

void C3DEffect::bindValue(Uniform* uniform, const C3DVector3& value)
{
    GL_ASSERT( glUniform3f(uniform->_location, value.x, value.y, value.z) );
}

void C3DEffect::bindValue(Uniform* uniform, const C3DVector3* values, unsigned int count)
{
    GL_ASSERT( glUniform3fv(uniform->_location, count, (GLfloat*)values) );
}

void C3DEffect::bindValue(Uniform* uniform, const C3DVector4& value)
{
    GL_ASSERT( glUniform4f(uniform->_location, value.x, value.y, value.z, value.w) );
}

void C3DEffect::bindValue(Uniform* uniform, const C3DVector4* values, unsigned int count)
{
    GL_ASSERT( glUniform4fv(uniform->_location, count, (GLfloat*)values) );
}

void C3DEffect::bindValue(Uniform* uniform, const C3DSampler* sampler)
{
    assert(uniform->_type == GL_SAMPLER_2D);

    //GL_ASSERT( glActiveTexture(GL_TEXTURE0 + uniform->_index) );

    C3DRenderState::activeTexture(GL_TEXTURE0 + uniform->_index);

    // Bind the sampler - this binds the texture and applies sampler state
    const_cast<C3DSampler*>(sampler)->bind();

    GL_ASSERT( glUniform1i(uniform->_location, uniform->_index) );
}

void C3DEffect::bindValue(Uniform* uniform, const C3DTexture* texture)
{
	assert(uniform->_type == GL_SAMPLER_2D);

    //GL_ASSERT( glActiveTexture(GL_TEXTURE0 + uniform->_index) );

    C3DRenderState::activeTexture(GL_TEXTURE0 + uniform->_index);

    //binds the texture
	GL_ASSERT( glBindTexture(GL_TEXTURE_2D, texture->getHandle()) );

    GL_ASSERT( glUniform1i(uniform->_location, uniform->_index) );
}

void C3DEffect::bindValue(Uniform* uniform, const C3DSamplerCube* sampler)
{
	assert(uniform->_type == GL_SAMPLER_CUBE);
	C3DRenderState::activeTexture(GL_TEXTURE0 + uniform->_index);

	//binds the texture
	const_cast<C3DSamplerCube*>(sampler)->bind();

	GL_ASSERT( glUniform1i(uniform->_location, uniform->_index) );
}

void C3DEffect::bind()
{
    //GL_ASSERT( glUseProgram(_program) );

    //__currentEffect = this;
    setCurrentEffect(this);
}

void C3DEffect::setCurrentEffect(C3DEffect* effect)
{
    if (effect && __currentEffect != effect)
    {
        GL_ASSERT( glUseProgram(effect->_program) );

        __currentEffect = effect;
    }
    else if(effect == NULL)
        __currentEffect = NULL;
}

C3DEffect* C3DEffect::getCurrentEffect()
{
    return __currentEffect;
}

Uniform::Uniform() :
    _location(-1), _type(0), _index(0)
{
}

Uniform::Uniform(const Uniform& copy)
{
    // hidden
}

Uniform::~Uniform()
{
    // hidden
}

C3DEffect* Uniform::getEffect() const
{
    return _effect;
}

const std::string& Uniform::getName() const
{
    return _name;
}

const GLenum Uniform::getType() const
{
    return _type;
}
}