//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

#include "Game.h"
#include "GameCore.h"
#include "Objects/Player.h"
#include "Objects/Controller.h"


Player::Player(fw::Scene* pScene, Controller* pController, std::string name)
    : fw::GameObject(pScene, name )
    , m_pController( pController )
{
}

Player::~Player()
{

}

void Player::Update(float deltaTime)
{
    GameObject::Update( deltaTime );

    fw::PhysicsComponent* pPhysicsComp = GetComponent<fw::PhysicsComponent>();

    if (pPhysicsComp != nullptr)
    {
        float timestep = 1 / 60.0f;
        float mass = pPhysicsComp->GetBody()->GetMass();
        b2Vec2 currentVelocity = pPhysicsComp->GetBody()->GetLinearVelocity();
        b2Vec2 targetVelocity = { currentVelocity.x, currentVelocity.y };
        float forceXCap = 10.0f;


        //if (m_bLaunch)
        //{
        //    //pPhysicsComp->GetBody()->SetLinearVelocity(b2Vec2_zero);
        //    pPhysicsComp->GetBody()->ApplyLinearImpulse(b2Vec2(0, 15), b2Vec2(pPhysicsComp->GetBody()->GetPosition().x, pPhysicsComp->GetBody()->GetPosition().y), true);
        //    m_bLaunch = false;
        //}

        //if (m_pController->WasPressed(Controller::Mask::Up) && m_bCanJump == true)
        //{

        //    pPhysicsComp->GetBody()->ApplyLinearImpulse(b2Vec2(0, 10), b2Vec2(pPhysicsComp->GetBody()->GetPosition().x, pPhysicsComp->GetBody()->GetPosition().y), true);
        //    m_bCanJump = false;
        //}
        //else if (m_pController->WasPressed(Controller::Mask::Down))
        //{

        //    pPhysicsComp->GetBody()->ApplyLinearImpulse(b2Vec2(0, -1), b2Vec2(pPhysicsComp->GetBody()->GetPosition().x, pPhysicsComp->GetBody()->GetPosition().y), true);

        //}

        if (m_pController->IsHeld(Controller::Mask::Left))
        {

            targetVelocity = { -5.0f, currentVelocity.y };
        }

        else if (m_pController->IsHeld(Controller::Mask::Right))
        {

            targetVelocity = { 5.0f, currentVelocity.y };
        }


        b2Vec2 velocityDiff = targetVelocity - currentVelocity;
        b2Vec2 force = b2Vec2(mass * (velocityDiff.x / timestep), mass * (velocityDiff.y / timestep));

        if (force.x > forceXCap)
        {
            force.x = forceXCap;
        }
        else if (force.x < -forceXCap)
        {
            force.x = -forceXCap;
        }

        pPhysicsComp->GetBody()->ApplyForceToCenter(force, true);


        if (m_pController->WasPressed(Controller::Mask::Action))
        {
            //m_Position.x = fw::Random::Float( 0.0f, 10.0f );
            //m_Position.y = fw::Random::Float( 0.0f, 10.0f );
        }

    }
}

void Player::Launch()
{
    m_bLaunch = true;
}
