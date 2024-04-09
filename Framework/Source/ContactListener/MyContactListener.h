#pragma once
#include "Framework.h"

namespace fw {



    class MyContactListener : public b2ContactListener
    {
    public:

        MyContactListener(fw::EventManager* eventManager);
        ~MyContactListener();

        virtual void BeginContact(b2Contact* contact) override;

    protected:

        fw::EventManager* m_pEventManager = nullptr;

    };

}