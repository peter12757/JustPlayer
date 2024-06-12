//
// Created by Administrator on 2023/10/27 0027.
//

#ifndef JUSTPLAYER_INVOKEMAIN_H
#define JUSTPLAYER_INVOKEMAIN_H

#include "../util/Util.h"
#include <iostream>
#include "../log/LogUtil.h"
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


#endif //JUSTPLAYER_INVOKEMAIN_H
