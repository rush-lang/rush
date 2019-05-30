#include "nlohmann/json.hpp"
#include "inja/inja.hpp"
#include "options.hpp"

#include <iostream>
#include <string>
#include <string_view>


std::string remove_extension(std::string const& filename) {
	size_t lastindex = filename.find_last_of(".");
	return filename.substr(0, lastindex);
}

void print_render(inja::Environment& env, std::string file, inja::Template const& temp, inja::json data) {
	std::cout << file << " ======" << std::endl;
	std::cout << env.render(temp, data) << std::endl;
	std::cout << "===========" << std::endl;
}

void write_render(inja::Environment& env, std::string file, inja::Template const& temp, inja::json data, std::string_view outdir) {
	env.write(temp, data, remove_extension(file));
}

void render(options::options& opts) {
	inja::Environment env;
	auto data = env.load_json(opts.json().data());

	for (auto const& file : opts.files()) {
		auto temp = env.parse_template(file);

		if (opts.has_print()) {
			print_render(env, file, temp, data);
			continue;
		}

		write_render(env, file, temp, data, "");
	}
}

int main(int argc, char const** argv) {

	auto opts = options::parse(argc, argv);

	if (opts.has_help()) {
		opts.display_help();
		return 0;
	}

	render(opts);
}

