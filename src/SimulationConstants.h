// SimulationConstants.h
#ifndef SIMULATIONCONSTANTS_H
#define SIMULATIONCONSTANTS_H

#include "PCH.hpp"

class SimulationConstants
{
	public:
		// ------------------
		// Particle simulator
		// ------------------

		// Particle
		static const int ps_ParticleRadius = 15;
		const sf::Color ps_ParticleColor = sf::Color::White;
		static const int ps_circlePointCount = 30;
		static const int ps_maxRandomVelocity = 3000.f;

		// Wall
		const sf::Color ps_wallColor = sf::Color::White;
		static const int ps_pixelsFromEdge = 20;


};

#endif