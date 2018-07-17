#include "nlohmann/json.hpp"
#include "inja/inja.hpp"
#include "cxxopts/cxxopts.hpp"

#include <iostream>
#include <string>

namespace opts = cxxopts;

void render(opts::ParseResult results);
std::string remove_extension(std::string const& filename);

int main(int argc, char const** argv) {
	auto options = opts::Options { "injen", "Source code generation tool" };
	options.add_options()
		("h,help", "Print usage information and exit.")
		("p,print", "Print the results to stdout.")
		("o,outdir", "Print the results to stdout.")
		("f,files", "A list of files to render.", opts::value<std::vector<std::string>>())
		("j,json", "Path to the data json data.", opts::value<std::string>())
	;
	options.parse_positional({ "files" });
	auto results = options.parse(argc, argv);

	if (!results["help"].as<bool>()) {
		render(results);
	}
}


std::string remove_extension(std::string const& filename) {
	size_t lastindex = filename.find_last_of(".");
	return filename.substr(0, lastindex);
}

void render(opts::ParseResult results) {
	inja::Environment env;

	auto jsonPath = results["json"].as<std::string>();
	auto filePaths = results["files"].as<std::vector<std::string>>();

	auto json = env.load_json(jsonPath);
	for (auto const& file : filePaths) {
		auto temp = env.parse_template(file);
		if (results["print"].as<bool>()) {
			std::cout << file << " ======" << std::endl;
			std::cout << env.render_template(temp, json) << std::endl;
			std::cout << "===========" << std::endl;
		} else {
			env.write(temp, json, remove_extension(file));
		}
	}
}