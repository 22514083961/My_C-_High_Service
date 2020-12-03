#include "sylar/sylar.h"
#include <unistd.h>
#include <vector>
sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();
auto my_log=SYLAR_LOG_NAME("system2");
int count=0;
sylar::RWMutex s_mutex;
void fun1() {
    SYLAR_LOG_INFO(g_logger) << "name: " << sylar::Thread::GetName()
                             << " this.name: " << sylar::Thread::GetThis()->getName()
                             << " id: " << sylar::GetThreadId()
                             << " this.id: " << sylar::Thread::GetThis()->getId();
    for(int i = 0; i<50000;i++){
        sylar::RWMutex::WriteLock lock(s_mutex);
        count++;
        //sleep(1);
    }
}

void fun2() {
    while(1)
    {
        SYLAR_LOG_ERROR(my_log)<<"_______________________________fun2";
        sleep(1);
    }
}
void fun3(){
    while(1)
    {
        SYLAR_LOG_ERROR(my_log)<<"*******************************fun3";
        sleep(1);
    }
}
void configInit(){
    YAML::Node root = YAML::LoadFile("./conf/threadConfig.yml");
    sylar::Config::LoadFromYaml(root);
    std::cout << sylar::LoggerMgr::GetInstance()->toYamlString()<<std::endl;
}
int main(int argc, char** argv) {

    SYLAR_LOG_INFO(g_logger) << "thread test begin ";
    configInit();
   /* std::vector<sylar::Thread::ptr> thrs;
    for(int i = 0; i < 5; ++i) {
        sylar::Thread::ptr thr(new sylar::Thread(&fun1, "name_" + std::to_string(i)));
        thrs.push_back(thr);
    }

    for(int i = 0; i < 5; ++i) {
        thrs[i]->join();
    }
    SYLAR_LOG_INFO(g_logger)<<"count="<<count;*/
    std::vector<sylar::Thread::ptr> Thread_pool;
   
    //sylar::Thread::ptr thr1(new sylar::Thread(&fun2,"name"+std::to_string(1)));
   // sylar::Thread::ptr thr2(new sylar::Thread(&fun3,"name"+std::to_string(2)));
    //Thread_pool.push_back(thr1);
   // Thread_pool.push_back(thr2);
   // for(size_t i = 0; i < Thread_pool.size();i++){
    //    Thread_pool[i]->join();
   // }
        sylar::Config::Visit([](sylar::ConfigVarBase::ptr var){//visir all log information
    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) <<"name="<<var->getName()<<"  description"<<
    var->getDescription()<<"typename="<<var->getTypeName()<<
    " value=" << var->toString();
    });
    SYLAR_LOG_INFO(g_logger) << "thread test end mapsize="<<sylar::Config::GetDatas().size();;
    return 0;
}
