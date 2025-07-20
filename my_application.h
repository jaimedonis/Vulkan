#pragma once

#include "application/application.h"

class MyApplication : public Application
{
public:
    MyApplication();

public:

    void OnSetup() override;

    void OnUpdate() override;

    void OnTeardown() override;

private:

};


