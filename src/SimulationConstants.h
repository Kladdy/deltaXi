// SimulationConstants.h
#ifndef SIMULATIONCONSTANTS_H
#define SIMULATIONCONSTANTS_H

#include "PCH.hpp"

class SimulationConstants
{
	public:
		// -----
		// Dials
		// -----

		// Slider
		static const int dials_SliderRoundedRectanglePointCount = 30;
		static const int dials_SliderRoundedRectangleRadius = 8.f;
		static const int dials_SliderRoundedRectangleOutlineThickness = -2;
		const sf::Color dials_SliderRoundedRectangleOutlineColor = sf::Color::White;
		const sf::Color dials_SliderFillColor = sf::Color::White;
		const sf::Color dials_SliderSmallFillColor = sf::Color::White;
		static const int dials_SliderSmallRadius = 2;
		static const int dials_SliderSmallPointCount = 15;
		static const int dials_SliderSelectionRadius = 12;
		static const int dials_SliderSelectionPointCount = 30;
		const sf::Color dials_SliderSelectionFillColor = sf::Color::Black;
		const sf::Color dials_SliderSelectionOutlineColor = sf::Color::White;
		static const int dials_SliderSelectionOutlineThickness = -2;
		static const auto dials_SliderSelectionAnimationDuration = 300; // Animation duration in milliseconds
		const sf::Color dials_SliderKnobHoldColor = sf::Color(99, 124, 135);



		// ------------------
		// Particle simulator
		// ------------------

		// Particle
		static const int ps_ParticleRadius = 15;
		const sf::Color ps_ParticleColor = sf::Color::White;
		static const int ps_circlePointCount = 30;
		static const int ps_maxRandomVelocity = 1000.f;
		static const int ps_extrapolationPrecision = 10;
		static const int ps_particleColorVariations = 5;
		static const int ps_particleContainerSizeVariations = 4;

		// Wall
		const sf::Color ps_wallColor = sf::Color::White;
		static const int ps_pixelsFromEdge = 20;


};

#endif