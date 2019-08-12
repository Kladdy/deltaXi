#pragma once

class sv_SE
{
	public:
		static std::map<std::string, std::string> strings()
		{
			std::map<std::string, std::string> s;

			// Meta
			s["updateLocale_success"] = "Språket har ändrats till svenska.";

			// Simulation names
			s["sim_particles"] = "Partiklar";

			return s;
		}
};
