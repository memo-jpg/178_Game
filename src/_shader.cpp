#include "_shader.h"

_shader::_shader()
{
    //ctor
    vs = 0;
    fs = 0;
    program = 0;
}

_shader::~_shader()
{
    //dtor
    cleanUp();
}
void _shader::loadFile(char* fileName, string& str)
{
    char tmp[1024];
    ifstream in(fileName);

    if(!in.is_open()){
        cout << "ERROR: File can't be opened\n";
        return;
    }

    while(in.getline(tmp, 1024)){
        str += tmp;
        str += '\n';
    }

    str += '\n';
    in.close();
}

unsigned int _shader::loadShader(string& src, unsigned int mode)
{
    unsigned int id;            // id for shader program

    const char* cSource;        // to convert string to char array
    char error[1024];

    id = glCreateShader(mode);
    cSource = src.c_str();      // convert string to chars

    glShaderSource(id, 1, &cSource, NULL);
    glCompileShader(id);

    glGetShaderInfoLog(id, 1024, NULL, error);      // check for shader errors

    cout << "Compile Status: " << error << endl;

    return id;
}

void _shader::initShader(char* vFileName, char* fFileName)
{
    string source;

    loadFile(vFileName, source);
    vs = loadShader(source, GL_VERTEX_SHADER);

    source = "";

    loadFile(fFileName, source);
    fs = loadShader(source, GL_FRAGMENT_SHADER);

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
}

void _shader::cleanUp()
{
    if (program != 0 && vs != 0)
    {
        glDetachShader(program, vs);
    }

    if (program != 0 && fs != 0)
    {
        glDetachShader(program, fs);
    }

    if (vs != 0)
    {
        glDeleteShader(vs);
        vs = 0;
    }

    if (fs != 0)
    {
        glDeleteShader(fs);
        fs = 0;
    }

    if (program != 0)
    {
        glDeleteProgram(program);
        program = 0;
    }
}
