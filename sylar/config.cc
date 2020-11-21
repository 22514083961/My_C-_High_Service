#include "sylar/config.h"
#include <iostream>
namespace sylar {

Config::ConfigVarMap Config::s_datas;
/**
     * @brief 查找配置参数,返回配置参数的基类
     * @param[in] name 配置参数名称
     */
ConfigVarBase::ptr Config::LookupBase(const std::string& name) {
    auto it = s_datas.find(name);  
    return it == s_datas.end() ? nullptr : it->second;
}

//"A.B", 10
//A:
//  B: 10
//  C: str

static void ListAllMember(const std::string& prefix,
                          const YAML::Node& node,
                          std::list<std::pair<std::string, const YAML::Node> >& output) {
    if(prefix.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._012345678")
            != std::string::npos) {
        SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "Config invalid name: " << prefix << " : " << node;
        return;
    }
    std::cout<<"******************************prefix="<<prefix<<std::endl;
    output.push_back(std::make_pair(prefix, node));
    if(node.IsMap()) {
        for(auto it = node.begin();
                it != node.end(); ++it) {
            //std::cout<<"it->first.Scalar="<<it->first.Scalar()<<std::endl;
            ListAllMember(prefix.empty() ? it->first.Scalar()
                    : prefix + "." + it->first.Scalar(), it->second, output);
        }
    }
}
/**
* @brief 使用YAML::Node初始化配置模块
*/
void Config::LoadFromYaml(const YAML::Node& root) {
    std::list<std::pair<std::string, const YAML::Node> > all_nodes;
    ListAllMember("", root, all_nodes);

    for(auto& i : all_nodes) {
        std::string key = i.first;
        //cou<<""
        if(key.empty()) {
            
            continue;
        }
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        ConfigVarBase::ptr var = LookupBase(key);
        if(var) {
            SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << key<<"in the config";
            if(i.second.IsScalar()) {
                SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "ss.ctr="<<i.second.Scalar();
                var->fromString(i.second.Scalar());
            } else {
               // SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "var is NULL Enter this";
                std::stringstream ss;
                ss << i.second;
                SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "ss.ctr="<<ss.str();
                var->fromString(ss.str());
            }
        }else{
            SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << key<<" not in the config";
        }
    }
}
}
