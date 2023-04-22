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

			// Particles
			s["sim_particles_infotext"] = L"sim_particles_infotext";
			s["sim_particle_dial_colors"] = L"sim_particle_dial_colors";
			s["sim_particle_dial_size"] = L"sim_particle_dial_size";
			s["sim_particle_dial_containerSize"] = L"sim_particle_dial_containerSize";
			s["sim_particle_statepicker_graphs"] = L"sim_particle_statepicker_graphs";
			s["sim_particle_statepicker_actions"] = L"sim_particle_statepicker_actions";
			s["sim_particle_statepicker_dials"] = L"sim_particle_statepicker_dials";

			return s;
		}
};