#pragma once

#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <string>
#include <vector>

namespace TestUtil {
	struct Code {
		std::string language;
		std::string code;
	};

    /**
	* Output as Markdown example
	*/
    void exportAsExample(std::string const& name,
                         std::vector<Code> const& exports,
                         std::filesystem::path const& examplesDir) {
	    std::filesystem::path fileName =
	        examplesDir / fmt::format("{}.md", name);
	    std::string content = fmt::format(R"(
## {} ##

)",
	                                      name);
	    for (auto const& [language, code] : exports) {
		    content += fmt::format(R"(
```{language}
{code}
```
)",
		                           fmt::arg("language", language),
		                           fmt::arg("code", code));
	    }

	    std::ofstream example(fileName);
	    example << content;
	    example.close();
    }
}
