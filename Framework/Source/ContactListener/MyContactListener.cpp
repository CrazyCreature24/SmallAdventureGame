#include "MyContactListener.h"
#include "Scenes/Scene.h"

namespace fw {

    MyContactListener::MyContactListener(fw::EventManager* eventManager) :
        m_pEventManager(eventManager)
    {

    }

    MyContactListener::~MyContactListener()
    {

    }


    void MyContactListener::BeginContact(b2Contact* contact)
    {
        fw::GameObject* pObjectA = reinterpret_cast<fw::GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
        fw::GameObject* pObjectB = reinterpret_cast<fw::GameObject*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

        m_pEventManager->AddEvent(new fw::CollisionEvent(pObjectA, pObjectB));

        //pObjectA->SetCanJump(true);

        //Scene* pScene = pObjectA->GetScene();

        /*if (pObjectB->GetComponent<PhysicsComponent>()->GetBody()->GetFixtureList()->IsSensor() == true)
        {
            pScene->HandleSensorCollision(pObjectB, pObjectA);
        }*/
    }

}