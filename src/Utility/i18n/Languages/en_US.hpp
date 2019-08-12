#pragma once

class en_US
{
	public:
		static std::map<std::string, std::string> strings()
		{
			std::map<std::string, std::string> s;

			// Meta
			s["updateLocale_success"] = "The language has been switched to English.";

			// Simulation names
			s["sim_particles"] = "Particles";

			return s;
		}
};
