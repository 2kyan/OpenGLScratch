#include "config.h"
#include <fstream>
#include <sstream>
#include <string>
#include "rapidjson/document.h"
#include "spdlog/spdlog.h"


template <class Container>
Container split(const std::string& str, char delim =' ')
{
    Container cont;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        cont.push_back(token);
    }
    return cont;
}

Config::Config(std::string name)
{
    m_cfg_name = name;
    std::ifstream ifs(m_cfg_name);
    std::ostringstream contents;
    contents << ifs.rdbuf();

    if (m_doc.Parse(contents.str().c_str()).HasParseError()) {
        spdlog::error("Error in parsing {0}", m_cfg_name);
        return;
    }
    
    
    if (m_doc.HasMember("application")) {
        m_app = get_string("application");
        set_app(m_app);
    } else {
        spdlog::error("Error in finding application in config file {0}.", m_cfg_name);
    }
}

void Config::set_app(std::string app)
{
    set_current(app);

    m_width = get_uint("screen_width");
    m_height = get_uint("screen_height");

    spdlog::info("Reading {3} for {0} with screen {1}x{2}.", m_app, m_width, m_height, app);
}

bool Config::check_model(std::string attrib)
{
    return false;
}

Config::~Config()
{
    //m_doc.Clear();
}

rapidjson::Document::ValueType* Config::get_obj(std::string key)
{
    using namespace rapidjson;

    Document::ValueType* obj = &m_doc;
    if (obj->HasMember(key.c_str())) {
        obj = &(*obj)[key.c_str()];
        return obj;
    }

    std::string f_key = m_current + key;
    std::vector<std::string> r = split<std::vector<std::string>>(f_key, '/');

    std::string cur;
    for (auto& ak : r) {
        cur = cur + "/" + ak;
        auto k = ak.c_str();

        if (obj->HasMember(k)) {
            obj = &(*obj)[k];
        } else {
            obj = nullptr;
            spdlog::error("{0} doesn't exist in {1}", k, cur);
        }
    }
    return obj;

}

bool Config::get_bool(std::string key)
{
    auto v = get_obj(key);
    if (v) {
        return v->GetBool();
    } else {
        spdlog::warn("{0} doesn't exist, return false instead", key);
        return false;
    }
}

int Config::get_int(std::string key)
{
    auto v = get_obj(key);
    if (v) {
        return v->GetInt();
    } else {
        spdlog::warn("{0} doesn't exist, return 0 instead", key);
        return 0;
    }
}

int Config::get_uint(std::string key)
{
    auto v = get_obj(key);
    if (v) {
        return v->GetUint();
    } else {
        spdlog::warn("{0} doesn't exist, return 0 instead", key);
        return 0;
    }
}

std::string Config::get_string(std::string key) 
{
    auto v = get_obj(key);
    return v->GetString();

}

std::vector<std::string> Config::get_object_names(std::string key) 
{
    std::vector<std::string> result;
    auto v = get_obj(key);
    if (v->IsObject()) {
        for (auto o = v->MemberBegin(); o != v->MemberEnd(); o++) {
            result.push_back(std::string(o->name.GetString()));
        }
    }
    
    return result;

}
