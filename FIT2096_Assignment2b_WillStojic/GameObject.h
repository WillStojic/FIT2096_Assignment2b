#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Direct3D.h"
#include "Mesh.h"
#include "Collisions.h"

class GameObject
{
protected:
	Vector3 m_position;
	float m_rotX, m_rotY, m_rotZ;
	float m_scaleX, m_scaleY, m_scaleZ;
	Vector3 m_targetPosition;

	Matrix m_world;
	Mesh* m_mesh;
	Texture* m_texture;
	Shader* m_shader;

	//game variables
	float m_health;
	int m_ammo;

	CBoundingBox* m_boundingBox;
	CBoundingSphere* m_boundingSphere;
public:
	GameObject();
	GameObject(Mesh* mesh, Shader* shader);
	GameObject(Mesh* mesh, Shader* shader, Texture* texture);
	GameObject(Mesh* mesh, Shader* shader, Vector3 position);
	GameObject(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position);
	virtual ~GameObject();

	// GameObject is now an abstract class as Update is pure virtual
	virtual void Update(float timestep) = 0;
	virtual void Render(Direct3D* renderer, Camera* cam);

	// Accessors
	Vector3 GetTargetPosition() { return m_targetPosition; }
	Vector3 GetPosition() { return m_position; }
	float GetXRotation() { return m_rotX; }
	float GetYRotation() { return m_rotY; }
	float GetZRotation() { return m_rotZ; }
	float GetXScale() { return m_scaleX; }
	float GetYScale() { return m_scaleY; }
	float GetZScale() { return m_scaleZ; }
	Mesh* GetMesh() { return m_mesh; }
	Texture* GetTexture() { return m_texture; }
	Shader* GetShader() { return m_shader; }
	CBoundingBox* GetBoundingBox() { return m_boundingBox; }
	CBoundingSphere* GetBoundingSphere() { return m_boundingSphere; }

	// Mutators
	void SetTargetPosition(Vector3 targetPos) { m_targetPosition = targetPos; }
	void SetTargetPosition(int x, int y, int z) { m_targetPosition = Vector3(x, y, z); }
	void SetPosition(Vector3 pos) { m_position = pos; }
	void SetXRotation(float xRot) { m_rotX = xRot; }
	void SetYRotation(float yRot) { m_rotY = yRot; }
	void SetZRotation(float zRot) { m_rotZ = zRot; }
	void SetXScale(float xScale) { m_scaleX = xScale; }
	void SetYScale(float yScale) { m_scaleY = yScale; }
	void SetZScale(float zScale) { m_scaleZ = zScale; }
	void SetUniformScale(float scale) { m_scaleX = m_scaleY = m_scaleZ = scale; }
	void SetMesh(Mesh* mesh) { m_mesh = mesh; }
	void SetTexture(Texture* texture) { m_texture = texture; }
	void SetShader(Shader* shader) { m_shader = shader; }
	void SetBoundingBox(CBoundingBox* bounds) { m_boundingBox = bounds; }

	//game functions
	float GetHealth() { return m_health; }
	void SetHealth(float health) { m_health = health; }

	int GetAmmo() { return m_ammo; }
	void SetAmmo(int newAmmo) { m_ammo = newAmmo; }
};

#endif

