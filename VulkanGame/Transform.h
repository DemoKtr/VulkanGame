#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Transform
{
protected:
	//Local space information
	glm::vec3 m_pos = { 1.0f, 0.0f, 0.0f };
	glm::quat m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Quaternion for rotation
	glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };

	//Global space information concatenate in matrix
	glm::mat4 m_modelMatrix = glm::mat4(1.0f);

	//Dirty flag
	bool m_isDirty = true;

protected:
	glm::mat4 getLocalModelMatrix()
	{
		
		return glm::translate(glm::mat4(1.0f), m_pos) * glm::mat4(m_rotation) * glm::scale(glm::mat4(1.0f), m_scale);
	}
public:

	void computeModelMatrix()
	{

		m_modelMatrix = getLocalModelMatrix();
		m_isDirty = false;
	}

	void computeModelMatrix(const glm::mat4& parentGlobalModelMatrix)
	{
		m_modelMatrix = parentGlobalModelMatrix * getLocalModelMatrix();
		m_isDirty = false;
	}

	void setLocalPosition(const glm::vec3& newPosition)
	{
		m_pos = newPosition;
		m_isDirty = true;
	}

	void setLocalRotationX(float angle)
	{
		m_rotation = glm::quat(angle, 1, 0, 0) * m_rotation;
		m_isDirty = true;
	}
	void setLocalRotationY(float angle)
	{
		m_rotation = glm::quat(angle, 0, 1, 0) * m_rotation;
		m_isDirty = true;
	}
	void setLocalRotationZ(float angle)
	{
		m_rotation = glm::quat(angle, 0, 0, 1) * m_rotation;
		m_isDirty = true;
	}

	void setLocalScale(const glm::vec3& newScale)
	{
		m_scale = newScale;
		m_isDirty = true;
	}

	const glm::vec3& getGlobalPosition() const
	{
		return m_modelMatrix[3];
	}

	const glm::vec3& getLocalPosition() const
	{
		return m_pos;
	}

	const glm::quat& getLocalRotation() const
	{
		return m_rotation;
	}

	const glm::vec3& getLocalScale() const
	{
		return m_scale;
	}

	const glm::mat4& getModelMatrix() const
	{
		return m_modelMatrix;
	}

	glm::vec3 getRight() const
	{
		return m_modelMatrix[0];
	}


	glm::vec3 getUp() const
	{
		return m_modelMatrix[1];
	}

	glm::vec3 getBackward() const
	{
		return m_modelMatrix[2];
	}

	glm::vec3 getForward() const
	{
		return -m_modelMatrix[2];
	}

	glm::vec3 getGlobalScale() const
	{
		return { glm::length(getRight()), glm::length(getUp()), glm::length(getBackward()) };
	}

	bool isDirty() const
	{
		return m_isDirty;
	}
};


