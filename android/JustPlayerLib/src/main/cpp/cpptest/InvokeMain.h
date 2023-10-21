//
// Created by Peter Xi on 2022/5/24.
//

#ifndef ANDROIDTEST_INVOKEMAIN_H
#define ANDROIDTEST_INVOKEMAIN_H

#include "../util/Util.h"
#include <iostream>
#include "../util/LogUtil.h"
#include <sstream>

using namespace std;

struct TestCaseData {
    std::string name;
    std::string desc;
};


class InvokeMain {
private:
    TestCaseData* data;

public:
    InvokeMain();
    ~InvokeMain();

    void createData(const std::string& name,const  std::string& desc);

};


#endif //ANDROIDTEST_INVOKEMAIN_H
