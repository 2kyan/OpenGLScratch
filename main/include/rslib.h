#ifndef _RSLIB_H
#define _RSLIB_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

class Config;

class RSLib {
public:
    struct args {
        std::string config;
        int n;
        int k;
        int verbose;
    };

public:
    virtual ~RSLib();
    static RSLib* instance();

    std::shared_ptr<Config> getConfig();
    std::shared_ptr<Config> initConfig(int argc, char** argv);
    std::string getConfigFileName(const char* fileName);
    std::string getModelFileName(const char* fileName);
    std::string getShaderFileName(const char* fileName);
    std::string getTextureFileName(const char* fileName);
    std::string loadFile(std::string filename);
    int execCmd(std::string & cmd);
    void* glslRead(const char* fileName, size_t& size);
    std::vector<uint32_t> glslCompile(const char* fileName, size_t& size, int shader_type);
    std::stringstream glslCompile(const char* fileName, int shader_type);

protected:
    int init();
    void updateResourcePath(std::string&& dirName, std::vector<std::string> resList);
    std::string getResourceFileName(const std::string& fileName, std::string resType);
    std::string getDirectoryName(const std::string& fileName);
    std::string getBaseName(const std::string& fileName) const;
    std::string getSpvFileName(const std::string& fileName);
    int glsl2spv(const std::string& glslFileName, const std::string& spvFileName);

    std::string unifyPath(const std::string fPath) const;

private :
    RSLib();
    RSLib(RSLib const&) = delete;
    RSLib& operator=(RSLib const &) = delete;
    RSLib(RSLib &&) = delete;
    RSLib& operator=(RSLib &&) = delete;
    
private:
    std::vector<std::string> resTypeStrings;
    std::unordered_map<std::string, std::vector<std::string>> resPaths;
    std::shared_ptr<Config> m_config;
    struct args m_arg;
    bool m_enableSPVDump;
};


#endif //_RSLIB_H