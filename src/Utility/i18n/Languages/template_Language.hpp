#pragma once

class template_Language
{
	public:
		static std::map<std::string, std::string> strings()
		{
			std::map<std::string, std::string> s;

			// Meta
			s["updateLocale_success"] = "updateLocale_success";

			// Simulation names
			s["sim_particles"] = "sim_particles";

			return s;
		}
};