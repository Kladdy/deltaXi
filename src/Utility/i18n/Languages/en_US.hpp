#pragma once

class en_US
{
	public:
		static std::map<std::string, std::wstring> strings()
		{
			std::map<std::string, std::wstring> s;

			// Meta
			s["updateLocale_success"] = L"The language has been switched to English.";

			// Simulation names
			s["sim_particles"] = L"Particles";
			s["sim_doppler"] = L"Doppler";
			s["sim_fluid"] = L"Fluids";
			s["sim_relativity"] = L"Relativity";

			return s;
		}
};
