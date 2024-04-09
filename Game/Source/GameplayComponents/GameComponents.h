#pragma once
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Framework.h"
#include "Meshes/VertexFormats.h"

class fw::GameObject;
class Controller;

// Interface class to help clean up a bit
class IIsControlable
{
public:

	Controller* GetController() { return m_pController; }
	void SetController(Controller* pController) { m_pController = pController; }

protected:

	Controller* m_pController = nullptr;

};


class PlayerLogicComponent : public fw::Component, public IIsControlable
{
	friend class fw::ComponentManager;
public:

	PlayerLogicComponent(fw::GameObject* pGameObject);
	virtual ~PlayerLogicComponent();

	static const char* GetStaticType() { return "PlayerLogicComponent"; }
	virtual const char* GetType() override { return GetStaticType(); }

	void SetCanJump(bool canJump) { m_bCanJump = canJump; }

	bool GetCanJump() { return m_bCanJump; }

	//UpdateActivePlayers. This requires the GameObject to have a PhysicsComponent.
	void UpdatePlayer();


protected:

	bool m_bCanJump = true;
};


class KartLogicComponent : public fw::Component, public IIsControlable
{
	friend class fw::ComponentManager;
public:

	KartLogicComponent(fw::GameObject* pGameObject);
	virtual ~KartLogicComponent();

	static const char* GetStaticType() { return "KartLogicComponent"; }
	virtual const char* GetType() override { return GetStaticType(); }

	void SetSpeed(float speed) { m_Speed = speed; }
	void SetNumCoins(int numCoins) { m_NumCoins = numCoins; }
	void SetHitOilSlick(bool bHitOilSlick) { m_HitOilSlick = bHitOilSlick; }
	void SetHitSpeedPad(bool bHitSpeedPad, b2Vec2 direction);
	void SetHitCoin(bool bHitCoin) { m_HitCoin = bHitCoin; }

	float GetSpeed() { return m_Speed; }
	int GetNumCoins() { return m_NumCoins; }
	bool GetHitCoin() { return m_HitCoin; }

	//UpdateActivePlayers. This requires the GameObject to have a PhysicsComponent.
	void UpdateKart();

	virtual void AddToInspector() override;


protected:

	float m_TimeStep = 1 / 60.0f;

	float m_Speed = 15.0f;
	float m_MaxSpeed = 20.0f;
	float m_TurnSpeed = 60.0f;
	b2Vec2 m_Velocity = { 0,0 };
	b2Vec2 m_Force = { 0,0 };
	b2Vec2 m_Direction = { 0,0 };

	//Gimmicks
	int m_NumCoins = 0;
	bool m_HitCoin = false;
	float m_CoinTimer = 0.0f;
	float m_MaxCoinTime = 1.0f;


	bool m_HitOilSlick = false;
	float m_SlickTimer = 0.0f;
	float m_MaxSlickTime = 1.0f;

	bool m_HitSpeedPad = false;
	float m_PadTimer = 0.0f;
	float m_MaxPadTime = 1.0f;
	float m_PadSpeed = 25.0f;
	b2Vec2 m_SpeedPadDirection = { 0,0 };
};


class SensorComponent : public fw::Component
{
	friend class fw::ComponentManager;
public:

	enum class SensorType
	{
		Coin,
		OilSlick,
		SpeedPad
	};

	SensorComponent(fw::GameObject* pGameObject, bool isCoin = false, bool isOilSlick = false, bool isSpeedPad = false);
	virtual ~SensorComponent();

	static const char* GetStaticType() { return "SensorComponent"; }
	virtual const char* GetType() override { return GetStaticType(); }

	void SetIsCoin(bool isCoin) { m_IsCoin = isCoin; }
	void SetIsOilSlick(bool isOilSlick) { m_IsOilSlick = isOilSlick; }
	void SetIsSpeedPad(bool isSpeedPad) { m_IsSpeedPad = isSpeedPad; }

	bool GetIsCoin() { return m_IsCoin; }
	bool GetIsOilSlick() { return m_IsOilSlick; }
	bool GetIsSpeedPad() { return m_IsSpeedPad; }

	virtual void AddToInspector() override;


protected:

	bool m_IsCoin = false;
	bool m_IsOilSlick = false;
	bool m_IsSpeedPad = false;

};


class HeightMapComponent : public fw::RenderComponent
{
public:

	HeightMapComponent(fw::GameObject* pGameObject, fw::Mesh* pMesh, fw::Material* pMaterial, fw::vec3 worldSize);
	HeightMapComponent(fw::GameObject* pGameObject, fw::Mesh* pMesh, fw::Material* pMaterial, fw::vec3 worldSize, const char* filename, vec2 tileCount = vec2(1.0f,1.0f));
	virtual ~HeightMapComponent();

	virtual void Create();

	void SetFileName(const char* filename) { m_FileName = filename; }

	float GetHeightAtXZ(float x, float z);

	void Dig(vec3 playerPosition, float heightChange);

	void CalcNormals();

	virtual void AddToInspector() override;

	virtual void SaveToJSON(nlohmann::json& json) override;
	virtual void LoadFromJSON(nlohmann::json& json) override;

protected:

	fw::ivec2 m_NumVerts = { 0,0 };
	fw::vec3 m_WorldSize = { 0,0,0 };
	fw::Mesh* m_pNewMesh = nullptr;
	std::vector<VertexFormat_PosUVNormal> m_Verts;
	std::vector<uint16> m_Indicies;
	fw::vec2 m_TileCount = { 1.0f,1.0f };
	float m_DigRadius = 5.0f;

private:

	const char* m_FileName = nullptr;

};


class WaterComponent : public HeightMapComponent
{
public:

	WaterComponent(fw::GameObject* pGameObject, fw::Mesh* pMesh, fw::Material* pMaterial, fw::vec3 worldSize, fw::ivec2 numVerts);
	virtual ~WaterComponent();

	virtual void Create() override;

	virtual void AddToInspector() override;

	virtual void SaveToJSON(nlohmann::json& json) override;
	virtual void LoadFromJSON(nlohmann::json& json) override;

protected:

	fw::ivec2 m_NumVerts = { 0,0 };
};


class OrbitCameraComponent : public fw::Component, public IIsControlable
{
public:

	OrbitCameraComponent(fw::GameObject* pGameObject, vec3 rotation);
	virtual ~OrbitCameraComponent();

	static const char* GetStaticType() { return "OrbitCameraComponent"; }
	virtual const char* GetType() override { return GetStaticType(); }

	//Getters and setters
	vec3 GetRelativePosition() { return m_RelativePosition; }
	vec2 GetProjectionScale() { return m_ProjectionScale; }
	float32 GetAspectRatio() { return m_AspectRatio; }
	vec3 GetRelativeRotation() { return m_Rotation; }


	void SetRelativePosition(vec3 position) { m_RelativePosition = position; }
	void SetProjectionScale(vec2 scale) { m_ProjectionScale = scale; }
	void SetAspectRatio(float32 aspectRatio) { m_AspectRatio = aspectRatio; }
	void SetGameObject(fw::GameObject* pGameObject) { m_pGameObject = pGameObject; }
	void SetRelativeRotation(vec3 rotation) { m_Rotation = rotation; }

	virtual void AddToInspector() override;

	virtual void SaveToJSON(nlohmann::json& json) override;
	virtual void LoadFromJSON(nlohmann::json& json) override;

	void UpdateOrbitCamera();

protected:

	vec3 m_RelativePosition = { 0,0,0 };
	vec2 m_ProjectionScale = { 0.5f, 0.5f };
	float32 m_AspectRatio = 1.0f;
	vec3 m_Rotation = { 0,0,0 };
	float m_Distance = 30.0f;


};


class Player3DLogicComponent : public fw::Component, public IIsControlable
{
	friend class fw::ComponentManager;
public:

	Player3DLogicComponent(fw::GameObject* pGameObject);
	virtual ~Player3DLogicComponent();

	static const char* GetStaticType() { return "Player3DLogicComponent"; }
	virtual const char* GetType() override { return GetStaticType(); }

	void SetSpeed(float newSpeed) { m_Speed = newSpeed; }
	void SetHeightMap(HeightMapComponent* heightMap) { m_pHeightMap = heightMap; }

	float GetSpeed() { return m_Speed; }

	//UpdateActivePlayers. Requires an OrbitCameraComponent and a Transform
	void UpdatePlayer(float deltaTime);

	// Place Torch
	void PlaceTorch(vec3 position, vec3 direction);


	virtual void AddToInspector() override;

	virtual void SaveToJSON(nlohmann::json& json) override;
	virtual void LoadFromJSON(nlohmann::json& json) override;

protected:

	HeightMapComponent* m_pHeightMap = nullptr;
	float m_Speed = 20.0f;
	int m_TorchIndex = 2;
	float m_HeightChange = 2.0f;

};


class OrbitObjectComponent : public fw::Component
{
public:

	OrbitObjectComponent(fw::GameObject* pGameObject, float distance, float orbitSpeed);
	virtual ~OrbitObjectComponent();

	static const char* GetStaticType() { return "OrbitObjectComponent"; }
	virtual const char* GetType() override { return GetStaticType(); }

	//Getters and setters
	vec3 GetRelativeRotation() { return m_Rotation; }

	void SetGameObject(fw::GameObject* pGameObject) { m_pGameObject = pGameObject; }
	void SetRelativeRotation(vec3 rotation) { m_Rotation = rotation; }

	virtual void AddToInspector() override;

	virtual void SaveToJSON(nlohmann::json& json) override;
	virtual void LoadFromJSON(nlohmann::json& json) override;

	void UpdateOrbit(float deltaTime);

protected:

	vec3 m_Rotation = { 0,0,0 };
	float m_Distance = 0.0f;
	vec3 m_ObjectRotationPoint = { 0,0,0 };
	float m_OrbitSpeed = 1.0f;

};