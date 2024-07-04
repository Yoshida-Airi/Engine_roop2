#include "GameObject.h"

void GameObject::Initialize(const std::vector<Model*>& models)
{
	models_ = models;
}

void GameObject::Update()
{
	for (Model* model : models_)
	{
		model->Update();
	}
}

void GameObject::Draw(Camera* camera)
{
	for (Model* model : models_)
	{
		model->Draw(camera);
	}
}
