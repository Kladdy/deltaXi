#pragma once

class sv_SE
{
	public:
		static std::map<std::string, std::wstring> strings()
		{
			std::map<std::string, std::wstring> s;

			// Meta
			s["updateLocale_success"] = L"Språket har ändrats till svenska.";

			// Simulation names
			s["sim_particles"] = L"PartiklarÅÄÖåäö";

			return s;
		}
};
