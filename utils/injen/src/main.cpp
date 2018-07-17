#include "nlohmann/json.hpp"
#include "inja/inja.hpp"

int main(int, char**) {
	inja::json j;
	j["name"] = "world";

	auto result = inja::render("Hello {{ name }}!", j); // Returns "Hello world!"
	std::cout << result << std::endl;
}