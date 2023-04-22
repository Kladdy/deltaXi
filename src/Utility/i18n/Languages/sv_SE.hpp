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
			s["sim_particles"] = L"Partiklar";
			s["sim_doppler"] = L"Doppler";
			s["sim_fluid"] = L"Fluider";
			s["sim_relativity"] = L"Relativitet";

			// Particles
			s["sim_particles_infotext"] = L"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Quam adipiscing vitae proin sagittis nisl rhoncus mattis. Eget nunc scelerisque viverra mauris in aliquam sem fringilla ut. Nisi vitae suscipit tellus mauris a diam maecenas sed enim. Libero volutpat sed cras ornare. Potenti nullam ac tortor vitae. Odio ut enim blandit volutpat maecenas volutpat blandit aliquam etiam. Tortor aliquam nulla facilisi cras. Id eu nisl nunc mi ipsum. Egestas dui id ornare arcu odio. Viverra nam libero justo laoreet sit. Morbi tristique senectus et netus et malesuada. Urna nec tincidunt praesent semper.";
			s["sim_particle_dial_colors"] = L"Färger";
			s["sim_particle_dial_size"] = L"Storlekar";
			s["sim_particle_dial_containerSize"] = L"Behållarstorlek";
			s["sim_particle_statepicker_graphs"] = L"Grafer";
			s["sim_particle_statepicker_actions"] = L"Handlingar";
			s["sim_particle_statepicker_dials"] = L"Spakar";

			return s;
		}
};
