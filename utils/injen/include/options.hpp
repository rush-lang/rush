#pragma once

#ifndef INJEN_OPTIONS_HPP
#define INJEN_OPTIONS_HPP

#include "cxxopts/cxxopts.hpp"
#include <string>

namespace options {

	class options {
		friend options parse(int&, char const**&);

	public:
		options(options const&) = delete;
		void operator = (options const&) = delete;

		bool has_help() const {
			return _result["help"].as<bool>();
		}

		bool has_print() const {
			return _result.count("print") && _result["print"].as<bool>();
		}

		std::string const& outdir() const {
			return _result["outdir"].as<std::string>();
		}

		std::string const& json() const {
			return _result["json"].as<std::string>();
		}

		std::vector<std::string> const& files() const {
			return _result["files"].as<std::vector<std::string>>();
		}

		void display_help() {

		}

	private:
		cxxopts::Options _options;
		cxxopts::ParseResult _result;

		options(int& argc, char const**& argv, cxxopts::Options options)
			: _options { std::move(options) }, _result { _options.parse(argc, argv) } { }
	};

	options parse(int& argc, char const**& argv) {
		auto options = cxxopts::Options { "injen", "Source code generation tool" };
		options.add_options()
			("h,help", "Print usage information and exit.")
			("p,print", "Print the results to stdout.")
			("j,json", "Path to the data json data.", cxxopts::value<std::string>())
			("o,outdir", "Specify the output directory.", cxxopts::value<std::string>())
			("f,files", "A list of files to render.", cxxopts::value<std::vector<std::string>>())
		;
		options.parse_positional({ "files" });
		return { argc, argv, std::move(options) };
	}
}

#endif // INJEN_OPTIONS_HPP
