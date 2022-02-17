#include "EmbindProxy/moduleFile.hpp"
#include <fmt/format.h>
#include <string>

namespace EmbindProxy {

namespace {
std::string joinRegisterCommands(std::set<std::string> const& commands) {
	std::string out;

	for (auto const& cmd : commands) {
		out += fmt::format("\t{};\n", cmd);
	}
	return out;
}

std::string joinExtraFunctions(std::set<std::string> const& functions,
                               std::string const& ns) {
	if (functions.empty()) {
		return "";
	}

	std::string out;

	for (auto const& f : functions) {
		out += fmt::format("\t{}\n", f);
	}

	// Add the namespace
	out = fmt::format(R"(
namespace {} {{
{}
}})",
	                  ns,
	                  out);

	return out;
}

std::string joinPreJSModules(std::vector<Module> const& modules) {
	std::string out;

	for (auto const& m : modules) {
		out += fmt::format("{}", m.getPreJS());
	}

	return out;
}
}    // namespace

ModuleFile::ModuleFile(Module const& rootModule, std::string const& libraryName)
    : m_rootModuleName(rootModule.getPrefix()), m_libraryName(libraryName),
      m_modules({rootModule}), m_typeInfo() {}

void ModuleFile::addModule(Module const& m) {
	m_modules.push_back(m);
}

std::filesystem::path ModuleFile::getCppFilepath() const {
	return m_libraryName + ".cpp";
}

std::filesystem::path ModuleFile::getPreJSFilepath() const {
	return "pre.js";
}

void ModuleFile::setTypeInfo(EmbindProxy::TypeInfo const& info) {
	m_typeInfo = info;
}

std::string ModuleFile::getEmbind() const {
	std::string out = fmt::format(
	    R"(
#include <emscripten/bind.h>

namespace em = emscripten;

{joinedFunctions}

EMSCRIPTEN_BINDINGS({libraryName}) {{

{registerCommands}
)",
	    fmt::arg("joinedFunctions",
	             joinExtraFunctions(m_typeInfo.m_extraFunctions,
	                                m_typeInfo.m_functionsNamespace)),
	    fmt::arg("libraryName", m_libraryName),
	    fmt::arg("registerCommands",
	             joinRegisterCommands(m_typeInfo.m_registerCommands)));

	for (auto const& m : m_modules) {
		out += m.getEmbind();
	}
	out += '}';

	return out;
}

std::string ModuleFile::getPreJS() const {
	std::string out =
	    fmt::format(R"(
var Module = {{
	postRun: [() => {{
		{modules}
	}}],
}};)",
	                fmt::arg("modules", joinPreJSModules(m_modules)));

	return out;
}

}    // namespace EmbindProxy
