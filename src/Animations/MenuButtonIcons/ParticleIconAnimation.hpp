#ifndef PARTICLEICONANIMATION_HPP
#define PARTICLEICONANIMATION_HPP

class ParticleIconAnimation
{
private:
	void addParticle(int index);

public:
	void updateAnimation(sf::Vector2f pos);

	ParticleIconAnimation(int particleAmount);
	ParticleIconAnimation();
	~ParticleIconAnimation();
};

#endif // !PARTICLEICONANIMATION_HPP
