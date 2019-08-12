#pragma once

class template_Language
{
	public:
		static std::map<std::string, std::wstring> strings()
		{
			std::map<std::string, std::wstring> s;

			// Meta
			s["updateLocale_success"] = L"updateLocale_success";

			// Simulation names
			s["sim_particles"] = L"sim_particles";

			return s;
		}
};