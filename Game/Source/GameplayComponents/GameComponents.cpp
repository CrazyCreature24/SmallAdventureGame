#include "Framework.h"
#include "GameplayComponents/GameComponents.h"
#include "Objects/Controller.h"
#include "Scenes/Mariokart.h"
#include "Meshes/VertexFormats.h"



PlayerLogicComponent::PlayerLogicComponent(fw::GameObject* pGameObject) : Component(pGameObject)
{

}

PlayerLogicComponent::~PlayerLogicComponent()
{

}

void PlayerLogicComponent::UpdatePlayer()
{
	if (m_pController)
	{
		Controller* controller = m_pController;

		fw::GameObject* pGameObject = GetOwner();
		fw::PhysicsComponent* pPhysicsComp = pGameObject->GetComponent<fw::PhysicsComponent>();
		assert(pPhysicsComp != nullptr);

		float timestep = 1 / 60.0f;
		float mass = pPhysicsComp->GetBody()->GetMass();
		b2Vec2 currentVelocity = pPhysicsComp->GetBody()->GetLinearVelocity();
		b2Vec2 targetVelocity = { currentVelocity.x, currentVelocity.y };
		float forceXCap = 10.0f;

		if (controller->WasPressed(Controller::Mask::Up) && m_bCanJump == true)
		{

			pPhysicsComp->GetBody()->ApplyLinearImpulse(b2Vec2(0, 10), b2Vec2(pPhysicsComp->GetBody()->GetPosition().x, pPhysicsComp->GetBody()->GetPosition().y), true);

		}
		else if (controller->WasPressed(Controller::Mask::Down))
		{

			pPhysicsComp->GetBody()->ApplyLinearImpulse(b2Vec2(0, -1), b2Vec2(pPhysicsComp->GetBody()->GetPosition().x, pPhysicsComp->GetBody()->GetPosition().y), true);

		}

		if (controller->IsHeld(Controller::Mask::Left))
		{

			targetVelocity = { -5.0f, currentVelocity.y };
		}

		else if (controller->IsHeld(Controller::Mask::Right))
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

	}
}

/////////////////////////////////////////////////////////

KartLogicComponent::KartLogicComponent(fw::GameObject* pGameObject) : Component(pGameObject)
{
}

KartLogicComponent::~KartLogicComponent()
{
}

void KartLogicComponent::SetHitSpeedPad(bool bHitSpeedPad, b2Vec2 direction)
{
	m_HitSpeedPad = bHitSpeedPad;

	m_SpeedPadDirection = direction;

}

void KartLogicComponent::UpdateKart()
{
	if (m_pController)
	{
		if (m_HitCoin) // If a coin is hit
		{
			m_CoinTimer += m_TimeStep;

			if (m_CoinTimer >= m_MaxCoinTime)
			{
				m_HitCoin = false;
				m_CoinTimer = 0.0f;

				//Removes Coin Render above Kart from the m_ComponentManager
				Mariokart* pScene = static_cast<Mariokart*>(m_pGameObject->GetScene());

				fw::RenderComponent* pCoinRenderComponent = pScene->GetCoinObject()->GetComponent<fw::RenderComponent>();

				pScene->GetComponentManager()->RemoveComponent(pCoinRenderComponent);

			}
		}



		Controller* controller = m_pController;

		fw::GameObject* pGameObject = GetOwner();
		fw::PhysicsComponent* pPhysicsComp = pGameObject->GetComponent<fw::PhysicsComponent>();
		assert(pPhysicsComp != nullptr);

		float mass = pPhysicsComp->GetBody()->GetMass();
		float forceCap = 10.0f;
		m_Force = { 0,0 };

		if (controller->IsHeld(Controller::Mask::Left))
		{
			pPhysicsComp->GetBody()->SetAngularVelocity(m_TurnSpeed * m_TimeStep);
		}
		else if (controller->IsHeld(Controller::Mask::Right))
		{
			pPhysicsComp->GetBody()->SetAngularVelocity(-m_TurnSpeed * m_TimeStep);
		}
		else
		{
			pPhysicsComp->GetBody()->SetAngularVelocity(0);
		}

		float AngleY = pPhysicsComp->GetBody()->GetAngle();

		if (m_HitOilSlick) //If the Oilslick is hit
		{
			m_SlickTimer += m_TimeStep;

			if (m_SlickTimer >= m_MaxSlickTime)
			{
				m_HitOilSlick = false;
				m_SlickTimer = 0.0f;
			}
		}
		else
		{
			b2Vec2 acceleration = { 0,0 };
			b2Vec2 currentVelocity = { 0,0 };
			b2Vec2 targetVelocity = { 0,0 };
			b2Vec2 velocity = { 0,0 };

			if (m_HitSpeedPad) // If the Speedpad is hit
			{
				m_PadTimer += m_TimeStep;

				if (m_PadTimer >= m_MaxPadTime)
				{
					m_HitSpeedPad = false;
					m_PadTimer = 0.0f;
				}

				float dotProduct = m_Direction.x * m_SpeedPadDirection.x + m_Direction.y * m_SpeedPadDirection.y;

				if (dotProduct > 0)
				{
					m_Direction = m_SpeedPadDirection;

					forceCap = 20.0f;

					float speed = m_PadSpeed + m_NumCoins;

					targetVelocity = { m_Direction.x * m_PadSpeed, m_Direction.y * m_PadSpeed };

					velocity = targetVelocity;
				}
				else
				{
					m_HitSpeedPad = false;
					m_PadTimer = 0.0f;
				}
			}
			else // Normal Driving conditions
			{
				//Adding 90 to account for direction the mesh is facing
				m_Direction = { cos(AngleY + fw::degreesToRads(90)), sin(AngleY + fw::degreesToRads(90)) };
				targetVelocity = { 0,0 };

				acceleration = { 0,0 };

				currentVelocity = pPhysicsComp->GetBody()->GetLinearVelocity();

				float speed = m_Speed + m_NumCoins;

				if (speed > m_MaxSpeed)
				{
					speed = m_MaxSpeed;
				}

				if (controller->IsHeld(Controller::Mask::Up))
				{
					targetVelocity = { m_Direction.x * speed, m_Direction.y * speed };
				}
				else if (controller->IsHeld(Controller::Mask::Down))
				{
					targetVelocity = { m_Direction.x * -speed, m_Direction.y * -speed };
				}
				velocity = targetVelocity - currentVelocity;
			}

			acceleration = { velocity.x / m_TimeStep, velocity.y / m_TimeStep };

			m_Force = b2Vec2((mass * acceleration.x), (mass * acceleration.y));

			float force = m_Force.Length();

			if (force > forceCap)
			{
				force = forceCap;
			}

			if (velocity.Length() != 0)
			{
				b2Vec2 direction = b2Vec2(velocity.x / velocity.Length(), velocity.y / velocity.Length());
				b2Vec2 forceVec = force * direction;

				pPhysicsComp->GetBody()->ApplyForceToCenter(forceVec, true);
			}
		}

		//For the inspector
		m_Velocity = pPhysicsComp->GetBody()->GetLinearVelocity();
	}
}

void KartLogicComponent::AddToInspector()
{

	ImGui::DragFloat2("Velocity", &m_Velocity.x);
	ImGui::DragFloat2("Force", &m_Force.x);
	ImGui::DragInt("Num Coins", &m_NumCoins);
}

///////////////////////////////////////////

SensorComponent::SensorComponent(fw::GameObject* pGameObject, bool isCoin, bool isOilSlick, bool isSpeedPad) :
	Component(pGameObject),
	m_IsCoin(isCoin),
	m_IsOilSlick(isOilSlick),
	m_IsSpeedPad(isSpeedPad)
{
}

SensorComponent::~SensorComponent()
{
}

void SensorComponent::AddToInspector()
{
}

/////////////////////////////////////////////////

HeightMapComponent::HeightMapComponent(fw::GameObject* pGameObject, fw::Mesh* pMesh, fw::Material* pMaterial, fw::vec3 worldSize) :
	RenderComponent(pGameObject, pMesh, pMaterial),
	m_WorldSize(worldSize)
{


}

HeightMapComponent::HeightMapComponent(fw::GameObject* pGameObject, fw::Mesh* pMesh, fw::Material* pMaterial, fw::vec3 worldSize, const char* filename, vec2 tileCount) :
	RenderComponent(pGameObject, pMesh, pMaterial),
	m_WorldSize(worldSize),
	m_FileName(filename),
	m_TileCount(tileCount)
{
	Create();

}

HeightMapComponent::~HeightMapComponent()
{
	delete m_pNewMesh;


}

void HeightMapComponent::Create()
{
	assert(m_FileName && "No filename given to load");


	// Load the file contents.
	uint32 length;
	char* fileContents = fw::LoadCompleteFile(m_FileName, &length);

	// Have stb_image decompress png from memory into a raw color array.
	int width;
	int height;
	int channels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load_from_memory((unsigned char*)fileContents, length, &width, &height, &channels, 1);
	assert(pixels != nullptr);
	m_NumVerts = { width, height };

	const int numVerts = m_NumVerts.x * m_NumVerts.y;
	m_Verts.reserve(numVerts);

	vec2 stepSize = vec2(m_WorldSize.x / (m_NumVerts.x - 1.0f), m_WorldSize.z / (m_NumVerts.y - 1.0f));

	for (int i = 0; i < m_NumVerts.y; i++)
	{
		for (int j = 0; j < m_NumVerts.x; j++)
		{
			int index = m_NumVerts.x * i + j;

			float heightOffset = (m_WorldSize.y / 255) * pixels[index];

			fw::vec3 offset = fw::vec3(stepSize.x * j, heightOffset, stepSize.y * i);
			fw::vec2 uvCoords = fw::vec2((offset.x / m_WorldSize.x) * m_TileCount.x, (offset.z / m_WorldSize.z) * m_TileCount.y);
			m_Verts.push_back({ offset, uvCoords });
		}
	}


	int numIndicies = ((m_NumVerts.x - 1) * (m_NumVerts.y - 1)) * 6;

	m_Indicies.reserve(numIndicies);

	for (int i = 0; i < numVerts; i++)
	{
		if ((i % m_NumVerts.x != m_NumVerts.x - 1 && i + m_NumVerts.x < numVerts))
		{
			m_Indicies.push_back(i);
			m_Indicies.push_back(i + m_NumVerts.x + 1);
			m_Indicies.push_back(i + 1);

			m_Indicies.push_back(i);
			m_Indicies.push_back(i + m_NumVerts.x);
			m_Indicies.push_back(i + m_NumVerts.x + 1);
		}
	}

	// Calculate normals.
	CalcNormals();


	m_pNewMesh = new fw::Mesh(VertexFormat_PosUVNormal::format, m_Verts.data(), sizeof(VertexFormat_PosUVNormal) * numVerts, m_Indicies.data(), sizeof(uint16) * (int)m_Indicies.size());

	m_pMesh = m_pNewMesh;

	delete[] fileContents;
	stbi_image_free(pixels);
}

float HeightMapComponent::GetHeightAtXZ(float x, float z)
{
	float height = 0;

	// Convert x,z to heightmap indices.
	float xTileSize = m_WorldSize.x / (m_NumVerts.x - 1);
	float zTileSize = m_WorldSize.z / (m_NumVerts.y - 1);
	float xIndexFloat = x / xTileSize;
	float zIndexFloat = z / zTileSize;
	int xIndex = (int)xIndexFloat;
	int zIndex = (int)zIndexFloat;
	float xPercentInQuad = xIndexFloat - xIndex;
	float zPercentInQuad = zIndexFloat - zIndex;

	// Get the 4 vertices that make up the quad.
	int stride = m_NumVerts.x;
	int index = zIndex * stride + xIndex;

	if (index + stride >= m_Verts.size() || index < 0)
	{
		height = 0;
		return height;
	}

	vec3 bottomLeft = m_Verts[index].pos;
	vec3 bottomRight = m_Verts[index + 1].pos;
	vec3 topLeft = m_Verts[index + stride].pos;
	vec3 topRight = m_Verts[index + stride + 1].pos;

	// Get the height at x,z.
	vec3 v1, v2, v3;
	if (xPercentInQuad < zPercentInQuad)
	{
		// Top left triangle.
		v1 = vec3(0, bottomLeft.y, 0);
		v2 = vec3(0, topLeft.y, 1);
		v3 = vec3(1, topRight.y, 1);
	}
	else
	{
		// Bottom right triangle.
		v1 = vec3(0, bottomLeft.y, 0);
		v2 = vec3(1, topRight.y, 1);
		v3 = vec3(1, bottomRight.y, 0);
	}

	// Barycentric interpolation.
	vec2 v4 = vec2(xPercentInQuad, zPercentInQuad);
	float areaTriangle = (v2.XZ() - v1.XZ()).Cross(v3.XZ() - v1.XZ());
	float area1 = (v4 - v3.XZ()).Cross(v2.XZ() - v3.XZ());
	float area2 = (v4 - v1.XZ()).Cross(v3.XZ() - v1.XZ());

	float perc1 = area1 / areaTriangle;
	float perc2 = area2 / areaTriangle;
	float perc3 = 1.0f - perc1 - perc2;

	height = perc1 * v1.y + perc2 * v2.y + perc3 * v3.y;

	return height;
}

void HeightMapComponent::Dig(vec3 playerPosition, float heightChange)
{

	int playerIndex;

	int stride = m_NumVerts.x;

	// Convert x,z to heightmap indices.
	float xTileSize = m_WorldSize.x / (m_NumVerts.x - 1);
	float zTileSize = m_WorldSize.z / (m_NumVerts.y - 1);
	float xIndexFloat = playerPosition.x / xTileSize;
	float zIndexFloat = playerPosition.z / zTileSize;
	int xIndex = (int)xIndexFloat;
	int zIndex = (int)zIndexFloat;
	playerIndex = zIndex * m_NumVerts.x + xIndex;


	int bLIndex = playerIndex - stride * (int)m_DigRadius - (int)m_DigRadius;
	int bRIndex = playerIndex - stride * (int)m_DigRadius + (int)m_DigRadius;
	int tLIndex = playerIndex + stride * (int)m_DigRadius - (int)m_DigRadius;
	int tRIndex = playerIndex + stride * (int)m_DigRadius + (int)m_DigRadius;

	int xLength = bRIndex - bLIndex;
	int yLength = (tLIndex - bLIndex) / stride;

	vec3 playerLocation = m_Verts[playerIndex].pos;

	for (int y = 0; y < yLength; y++)
	{
		for (int x = 0; x < xLength; x++)
		{
			int index = (bLIndex + (stride * y)) + x;

			if (index <= m_Verts.size() - 1 && index > 0)
			{
				vec3 vertPos = m_Verts[index].pos;

				float distance = abs((playerLocation - vertPos).Length());

				float height = (cos((distance / (m_DigRadius * 0.5f)) * ((3.14159f / 2))) / 2) * heightChange;

				if (heightChange > 0)
				{
					fw::MyClamp(height, 0.0f, heightChange);
				}
				else
				{
					fw::MyClamp(height, heightChange, 0.0f);
				}

				m_Verts[index].pos.y += height;
			}
		}
	}

	CalcNormals();


	{
		// Previous attempts that I know are wrong but do cool things

		//m_Verts[playerIndex - 1].pos.y += heightChange;                 // Right
		//m_Verts[playerIndex + 1].pos.y += heightChange;                 // Left
		//m_Verts[playerIndex - m_NumVerts.x].pos.y += heightChange;      // Up
		//m_Verts[playerIndex + m_NumVerts.x].pos.y += heightChange;      // Down
		//m_Verts[playerIndex + m_NumVerts.x + 1].pos.y += heightChange;  // Top Right
		//m_Verts[playerIndex + m_NumVerts.x - 1].pos.y += heightChange;  // Top Left
		//m_Verts[playerIndex - m_NumVerts.x + 1].pos.y += heightChange;  // Bottom Right
		//m_Verts[playerIndex - m_NumVerts.x - 1].pos.y += heightChange;  // Bottom Left

		// Use this loop for an art project
		//for (int i = 1; i <= 5; i++)
		//{
		//	m_Verts[playerIndex - i].pos.y += heightChange;						// Right
		//	m_Verts[playerIndex + i].pos.y += heightChange;						// Left
		//	m_Verts[playerIndex - m_NumVerts.x * i].pos.y += heightChange;      // Up
		//	m_Verts[playerIndex + m_NumVerts.x * i].pos.y += heightChange;      // Down
		//	m_Verts[playerIndex + m_NumVerts.x * i + i].pos.y += heightChange;  // Top Right
		//	m_Verts[playerIndex + m_NumVerts.x * i - i].pos.y += heightChange;  // Top Left
		//	m_Verts[playerIndex - m_NumVerts.x * i + i].pos.y += heightChange;  // Bottom Right
		//	m_Verts[playerIndex - m_NumVerts.x * i - i].pos.y += heightChange;  // Bottom Left
		//}

		//m_Verts[playerIndex - m_DigRadius].pos.y += heightChange;                 // Right
		//m_Verts[playerIndex + m_DigRadius].pos.y += heightChange;                 // Left
		//m_Verts[playerIndex - m_NumVerts.x * m_DigRadius].pos.y += heightChange;  // Up
		//m_Verts[playerIndex + m_NumVerts.x * m_DigRadius].pos.y += heightChange;  // Down
		//m_Verts[playerIndex + m_NumVerts.x + m_DigRadius].pos.y += heightChange;  // Top Right
		//m_Verts[playerIndex + m_NumVerts.x - m_DigRadius].pos.y += heightChange;  // Top Left
		//m_Verts[playerIndex - m_NumVerts.x + m_DigRadius].pos.y += heightChange;  // Bottom Right
		//m_Verts[playerIndex - m_NumVerts.x - m_DigRadius].pos.y += heightChange;  // Bottom Left
	}

	m_pMesh->Create(VertexFormat_PosUVNormal::format, m_Verts.data(), sizeof(VertexFormat_PosUVNormal) * m_Verts.size(), m_Indicies.data(), sizeof(uint16) * (int)m_Indicies.size());

}

void HeightMapComponent::CalcNormals()
{
	for (int y = 0; y < m_NumVerts.y; y++)
	{
		for (int x = 0; x < m_NumVerts.x; x++)
		{
			int index = y * m_NumVerts.x + x;

			vec3 normal = vec3(0, 1, 0);

			if (x > 0 && x < m_NumVerts.x - 1 && y > 0 && y < m_NumVerts.y - 1)
			{
				vec3 v0 = m_Verts[index].pos;
				vec3 v1 = m_Verts[index - 1].pos;
				vec3 v2 = m_Verts[index + 1].pos;
				vec3 v3 = m_Verts[index - m_NumVerts.x].pos;
				vec3 v4 = m_Verts[index + m_NumVerts.x].pos;

				vec3 left = v1 - v0;
				vec3 right = v2 - v0;
				vec3 down = v3 - v0;
				vec3 up = v4 - v0;

				vec3 tr = up.Cross(right).GetNormalized();
				vec3 br = right.Cross(down).GetNormalized();
				vec3 bl = down.Cross(left).GetNormalized();
				vec3 tl = left.Cross(up).GetNormalized();

				normal = (tr + br + bl + tl).GetNormalized();
			}

			//normal.y = 0;
			m_Verts[index].normal = normal;
		}
	}
}


void HeightMapComponent::AddToInspector()
{
	ImGui::DragFloat("Radius", &m_DigRadius);
}

void HeightMapComponent::SaveToJSON(nlohmann::json& json)
{
}

void HeightMapComponent::LoadFromJSON(nlohmann::json& json)
{
}


///////////////////////////////////////

WaterComponent::WaterComponent(fw::GameObject* pGameObject, fw::Mesh* pMesh, fw::Material* pMaterial, fw::vec3 worldSize, fw::ivec2 numVerts) :
	HeightMapComponent(pGameObject, pMesh, pMaterial, worldSize),
	m_NumVerts(numVerts)
{
	Create();

}

WaterComponent::~WaterComponent()
{

}

void WaterComponent::Create()
{
	const int numVerts = m_NumVerts.x * m_NumVerts.y;
	m_Verts.reserve(numVerts);

	vec2 stepSize = vec2(m_WorldSize.x / (m_NumVerts.x - 1.0f), m_WorldSize.z / (m_NumVerts.y - 1.0f));

	for (int i = 0; i < m_NumVerts.y; i++)
	{
		for (int j = 0; j < m_NumVerts.x; j++)
		{
			fw::vec3 offset = fw::vec3(stepSize.x * j, 0, stepSize.y * i);
			fw::vec2 uvCoords = fw::vec2((offset.x / m_WorldSize.x) * m_TileCount.x, (offset.z / m_WorldSize.z) * m_TileCount.y);
			m_Verts.push_back({ offset, uvCoords });
		}
	}


	int numIndicies = ((m_NumVerts.x - 1) * (m_NumVerts.y - 1)) * 6;

	m_Indicies.reserve(numIndicies);

	for (int i = 0; i < numVerts; i++)
	{
		if ((i % m_NumVerts.x != m_NumVerts.x && i + m_NumVerts.x < numVerts) || i == 0)
		{
			m_Indicies.push_back(i);
			m_Indicies.push_back(i + m_NumVerts.x + 1);
			m_Indicies.push_back(i + 1);

			m_Indicies.push_back(i);
			m_Indicies.push_back(i + m_NumVerts.x);
			m_Indicies.push_back(i + m_NumVerts.x + 1);
		}
	}


	// Calculate normals.
	for (int y = 0; y < m_NumVerts.y; y++)
	{
		for (int x = 0; x < m_NumVerts.x; x++)
		{
			int index = y * m_NumVerts.x + x;

			vec3 normal = vec3(0, 1, 0);

			m_Verts[index].normal = normal;
		}
	}


	m_pNewMesh = new fw::Mesh(VertexFormat_PosUVNormal::format, m_Verts.data(), sizeof(VertexFormat_PosUVNormal) * numVerts, m_Indicies.data(), sizeof(uint16) * numIndicies);

	m_pMesh = m_pNewMesh;

}

void WaterComponent::AddToInspector()
{

}

void WaterComponent::SaveToJSON(nlohmann::json& json)
{

}

void WaterComponent::LoadFromJSON(nlohmann::json& json)
{

}

/////////////////////////////////////////////////////

OrbitCameraComponent::OrbitCameraComponent(fw::GameObject* pGameObject, vec3 rotation) :
	Component(pGameObject),
	m_Rotation(rotation)
{
	fw::FWCore* pFW = m_pGameObject->GetScene()->GetGameCore()->GetFramework();
	SetAspectRatio((float)pFW->GetWindowClientWidth() / pFW->GetWindowClientHeight());
}

OrbitCameraComponent::~OrbitCameraComponent()
{
}

void OrbitCameraComponent::UpdateOrbitCamera()
{

	if (m_pController)
	{
		if (m_pController->IsHeld(Controller::Mask::CamLeft)) // Left
		{
			m_Rotation.y += 1;
		}
		else if (m_pController->IsHeld(Controller::Mask::CamRight)) // Right
		{
			m_Rotation.y -= 1;
		}

		if (m_pController->IsHeld(Controller::Mask::CamUp)) // Up
		{
			m_Rotation.x += 1;
		}
		else if (m_pController->IsHeld(Controller::Mask::CamDown)) // Down
		{
			m_Rotation.x -= 1;
		}
	}



	fw::TransformComponent* pTransform = m_pGameObject->GetComponent<fw::TransformComponent>();
	assert(pTransform != nullptr);

	fw::MyClamp(m_Rotation.x, -89.9f, 89.9f);

	//float distance = 30;
	float angleY = -fw::degreesToRads(m_Rotation.y);
	float angleX = fw::degreesToRads(m_Rotation.x);

	float sx = sin(angleX);
	float sy = sin(angleY);
	float cx = cos(angleX);
	float cy = cos(angleY);

	float locX = -cx * sy * m_Distance;
	float locY = sx * m_Distance;
	float locZ = -cx * cy * m_Distance;

	vec3 ObjectPosition = pTransform->GetPosition();
	vec3 CameraOffset = vec3(locX, locY, locZ);
	vec3 CameraPosition = ObjectPosition + CameraOffset;

	mat4 viewMatrix;
	vec3 eye(CameraPosition);
	vec3 up(0, 1, 0);
	vec3 at(ObjectPosition);
	viewMatrix.CreateLookAtView(eye, up, at);

	mat4 projMatrix;
	projMatrix.CreatePerspectiveVFoV(45, GetAspectRatio(), 0.01f, 200.0f);
	vec4 camPos = vec4(CameraPosition, 1);

	// Setup uniforms.
	fw::Uniforms* pUniforms = m_pGameObject->GetScene()->GetGameCore()->GetUniforms();
	bgfx::setUniform(pUniforms->GetUniform("u_ViewMatrix"), &viewMatrix);
	bgfx::setUniform(pUniforms->GetUniform("u_ProjMatrix"), &projMatrix);
	bgfx::setUniform(pUniforms->GetUniform("u_CameraPosition"), &camPos);

}

void OrbitCameraComponent::AddToInspector()
{
	ImGui::DragFloat3("RelPosition", &m_RelativePosition.x);
	ImGui::DragFloat3("RelRotation", &m_Rotation.x);
	ImGui::DragFloat("Distance", &m_Distance);
}

void OrbitCameraComponent::SaveToJSON(nlohmann::json& json)
{
	Component::SaveToJSON(json);

	json["RelPosition"] = { m_RelativePosition.x, m_RelativePosition.y, m_RelativePosition.z };
	json["AspectRatio"] = { m_AspectRatio };
}

void OrbitCameraComponent::LoadFromJSON(nlohmann::json& json)
{
	m_RelativePosition = vec3(json["RelPosition"][0], json["RelPosition"][1], json["RelPosition"][2]);
	m_AspectRatio = json["AspectRatio"];
}

/////////////////////////////////////////////////////////

Player3DLogicComponent::Player3DLogicComponent(fw::GameObject* pGameObject) :
	Component(pGameObject)
{

}

Player3DLogicComponent::~Player3DLogicComponent()
{

}

void Player3DLogicComponent::UpdatePlayer(float deltaTime)
{
	if (m_pController)
	{
		fw::TransformComponent* pTransform = m_pGameObject->GetComponent<fw::TransformComponent>();
		OrbitCameraComponent* pCamera = m_pGameObject->GetComponent<OrbitCameraComponent>();
		assert(pCamera && pTransform);

		float cameraAngle = pCamera->GetRelativeRotation().y;
		float cameraAngleRads = fw::degreesToRads(cameraAngle + 90);

		vec3 forwardDir = vec3(cos(cameraAngleRads), 0, sin(cameraAngleRads));
		vec3 rightDir = vec3(cos(cameraAngleRads - fw::degreesToRads(90)), 0, sin(cameraAngleRads - fw::degreesToRads(90)));


		if (m_pController->IsHeld(Controller::Mask::Up))
		{
			pTransform->SetPosition(pTransform->GetPosition() + forwardDir * deltaTime * m_Speed);
		}
		else if (m_pController->IsHeld(Controller::Mask::Down))
		{
			pTransform->SetPosition(pTransform->GetPosition() - forwardDir * deltaTime * m_Speed);
		}

		if (m_pController->IsHeld(Controller::Mask::Left))
		{
			pTransform->SetPosition(pTransform->GetPosition() - rightDir * deltaTime * m_Speed);
		}
		else if (m_pController->IsHeld(Controller::Mask::Right))
		{
			pTransform->SetPosition(pTransform->GetPosition() + rightDir * deltaTime * m_Speed);
		}

		pTransform->SetRotation(vec3(0, cameraAngle, 0));

		vec3 playerPos = pTransform->GetPosition();



		if (m_pController->WasPressed(Controller::Mask::Action)) // E
		{
			PlaceTorch(playerPos, forwardDir);
		}

		if (m_pHeightMap)
		{
			pTransform->SetPosition(vec3(playerPos.x, m_pHeightMap->GetHeightAtXZ(playerPos.x, playerPos.z) + 0.75f, playerPos.z));

			if (m_pController->WasPressed(Controller::Mask::DigDown)) // Z
			{
				m_pHeightMap->Dig(playerPos, -m_HeightChange);
			}
			else if (m_pController->WasPressed(Controller::Mask::DigUp)) // X
			{
				m_pHeightMap->Dig(playerPos, m_HeightChange);
			}
		}
	}
}

void Player3DLogicComponent::PlaceTorch(vec3 position, vec3 direction)
{
	std::vector<fw::Component*> lights = m_pGameObject->GetScene()->GetComponentManager()->GetComponentVector<fw::LightComponent>();

	if (lights.size() >= 3)
	{
		fw::LightComponent* avalableTorch = static_cast<fw::LightComponent*>(lights[m_TorchIndex]);

		fw::TransformComponent* pTransform = avalableTorch->GetOwner()->GetComponent<fw::TransformComponent>();

		vec3 torchPosition = position + direction * 2;
		torchPosition = vec3(torchPosition.x, m_pHeightMap->GetHeightAtXZ(torchPosition.x, torchPosition.z), torchPosition.z);
		pTransform->SetPosition(torchPosition + vec3(0, 1, 0));

		// Torches emit a yellow light
		avalableTorch->m_LightColor = vec4(1, 1, 0, 1);


		if (m_TorchIndex == lights.size() - 1)
		{
			// To account for the sun and the point light on player
			m_TorchIndex = 2;
		}
		else
		{
			m_TorchIndex++;
		}
	}
}

void Player3DLogicComponent::AddToInspector()
{
	ImGui::DragFloat("MovementSpeed", &m_Speed);
	ImGui::DragFloat("HeightChangeAmount", &m_HeightChange);
}

void Player3DLogicComponent::SaveToJSON(nlohmann::json& json)
{

}

void Player3DLogicComponent::LoadFromJSON(nlohmann::json& json)
{

}

/////////////////////////////////////////////////////

OrbitObjectComponent::OrbitObjectComponent(fw::GameObject* pGameObject, float distance, float orbitSpeed) :
	Component(pGameObject),
	m_Distance(distance),
	m_OrbitSpeed(orbitSpeed)
{
	fw::TransformComponent* pTransform = m_pGameObject->GetComponent<fw::TransformComponent>();
	assert(pTransform != nullptr);

	m_ObjectRotationPoint = pTransform->GetPosition();
}

OrbitObjectComponent::~OrbitObjectComponent()
{

}

void OrbitObjectComponent::AddToInspector()
{
	ImGui::DragFloat("OrbitDistance", &m_Distance);
	ImGui::DragFloat("OrbitSpeed", &m_OrbitSpeed);
	ImGui::DragFloat3("orbitCenterPosition", &m_ObjectRotationPoint.x);
}

void OrbitObjectComponent::SaveToJSON(nlohmann::json& json)
{

}

void OrbitObjectComponent::LoadFromJSON(nlohmann::json& json)
{

}

void OrbitObjectComponent::UpdateOrbit(float deltaTime)
{
	m_Rotation.x += deltaTime * m_OrbitSpeed;

	fw::TransformComponent* pTransform = m_pGameObject->GetComponent<fw::TransformComponent>();
	assert(pTransform != nullptr);

	float angleY = -fw::degreesToRads(m_Rotation.y);
	float angleX = fw::degreesToRads(m_Rotation.x);

	float sx = sin(angleX);
	float sy = sin(angleY);
	float cx = cos(angleX);
	float cy = cos(angleY);

	float locX = -cx * sy * m_Distance;
	float locY = sx * m_Distance;
	float locZ = -cx * cy * m_Distance;

	vec3 ObjectOffset = vec3(locX, locY, locZ);
	vec3 ObjectPosition = m_ObjectRotationPoint + ObjectOffset;
	pTransform->SetPosition(ObjectPosition);

}
