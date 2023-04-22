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

			// Particles
			s["sim_particles_infotext"] = L"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Quam adipiscing vitae proin sagittis nisl rhoncus mattis. Eget nunc scelerisque viverra mauris in aliquam sem fringilla ut. Nisi vitae suscipit tellus mauris a diam maecenas sed enim. Libero volutpat sed cras ornare.";
			s["sim_particle_dial_colors"] = L"Colors";
			s["sim_particle_dial_size"] = L"Size";
			s["sim_particle_dial_containerSize"] = L"Container size";
			s["sim_particle_statepicker_graphs"] = L"Graphs";
			s["sim_particle_statepicker_actions"] = L"Actions";
			s["sim_particle_statepicker_dials"] = L"Dials";

			return s;
		}
};
