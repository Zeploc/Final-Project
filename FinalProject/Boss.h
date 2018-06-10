#pragma once
#include "c:\Users\Jonty\Source\Repos\Final-Project\FinalProject\Engine\Entity.h"
class Boss :
	public Entity
{
public:
	Boss(Utils::Transform _Transform, Utils::EANCHOR _Anchor);
	~Boss();
	void Update();
	void OnBulletCollision();
	float BossHealth = 10.0f;
private:
	float m_fOscillatingHeight;
	float OccillationSpeed = 300;
	float BobbingSpeed = 5;
	


};

