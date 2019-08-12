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
			s["sim_doppler"] = L"sim_doppler";
			s["sim_fluid"] = L"sim_fluid";
			s["sim_relativity"] = L"sim_relativity";

			return s;
		}
};