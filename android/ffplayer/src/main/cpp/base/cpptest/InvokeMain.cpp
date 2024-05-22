//
// Created by PeterX on 2023/10/27 0027.
//

#include "InvokeMain.h"


#include "InvokeMain.h"
#include <string>
#include "../const/common.h"


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
