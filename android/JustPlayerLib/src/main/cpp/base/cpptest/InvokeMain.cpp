//
// Created by Peter Xi on 2022/5/24.
//

#include "InvokeMain.h"
#include <string>


InvokeMain::InvokeMain()
:data(nullptr){

}


InvokeMain::~InvokeMain() {
    SafeDelete(data);
}


void InvokeMain::createData(const std::string &name, const std::string &desc) {
    std:ostringstream logOs;
    logOs<<" InvokeMain::createData: "<<name<<" desc: "<<desc;
    if (data == nullptr) {
        data = new TestCaseData();
        data->name = name;
        data->desc = desc;
        logOs<<" append: "<<data->name<<" desc: "<<data->desc;
    }
    LOGD(" %s ",logOs.str().c_str());
}