/*************************************************************************
* Copyright 2018 Reegan Troy Layzell
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*************************************************************************/
#include "nlohmann/json.hpp"
#include "inja/inja.hpp"
#include "options.hpp"

#include <iostream>
#include <string>
#include <string_view>

using json = inja::json;

template <typename RanIter>
std::string join(std::string_view delim, RanIter first, RanIter last) {
   std::string res;
   for (; first != last; ++first) {
      res += *first;
      if (first != last - 1)
        res += delim;
   }
   return res;
}


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

   env.add_callback("concat", 2, [](inja::Arguments& args) {
      auto lhs = args.at(0)->get<inja::json>();
      auto rhs = args.at(1)->get<inja::json>();
      lhs.insert(lhs.end(), rhs.begin(), rhs.end());
      return lhs;
   });

   env.add_callback("join", 2, [](inja::Arguments& args) {
      auto delim = args.at(0)->get<std::string>();
      auto jval = args.at(1)->get<inja::json>();

      if (!jval.is_array()
      || std::any_of(jval.begin(), jval.end(), [](auto& x) { return !x.is_string(); }))
         throw std::invalid_argument("expected an array of strings.");

      return join(delim, jval.begin(), jval.end());
   });

   env.add_callback("prepend", 2, [](inja::Arguments& args) {
      auto prep = args.at(0)->get<std::string>();
      auto jval = args.at(1)->get<inja::json>();

      if (jval.is_string())
         return json::parse(prep + jval.get<std::string>());

      if (!jval.is_array()
      || std::any_of(jval.begin(), jval.end(), [](auto& x) { return !x.is_string(); }))
         throw std::invalid_argument("expected a string or array of strings.");

      auto res = json::array();
      std::for_each(jval.begin(), jval.end(), [&res, &prep](auto& x) {
         res.push_back(prep + x.template get<std::string>());
      });
      return std::move(res);
   });

   env.add_callback("ltrim", 2, [](inja::Arguments& args) {
      auto s = args.at(0)->get<std::string>();
      auto ts = args.at(1)->get<std::string>();

      if (ts.empty()) return s;
      s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&ts](int ch) {
         return ts.front() == ch;
      }));

      return s;
   });

   env.add_callback("rtrim", 2, [](inja::Arguments& args) {
      auto s = args.at(0)->get<std::string>();
      auto ts = args.at(1)->get<std::string>();

      if (ts.empty()) return s;
      s.erase(std::find_if(s.rbegin(), s.rend(), [&ts](int ch) {
         return ts.front() != ch;
      }).base(), s.end());

      return s;
   });

   // env.add_callback("not", [](inja::Arguments& args) {
   //    return !args.at(0)->get<bool>();
   // });

   env.add_callback("contains", 2, [](inja::Arguments& args) {
      auto jval = args.at(0)->get<inja::json>();
      auto elem = args.at(1)->get<std::string>();

      switch (jval.type()) {
      default: return false;
      case inja::json::value_t::object:
         return jval.find(elem) != jval.end();
      case inja::json::value_t::array:
         return std::find(jval.begin(), jval.end(), elem) != jval.end();
      }
   });

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

