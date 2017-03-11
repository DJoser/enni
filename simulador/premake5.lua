include "script"

PROJECT_NAME = path.getname(os.getcwd())

minko.project.solution(PROJECT_NAME)

	minko.project.application(PROJECT_NAME)

		files { "src/**.cpp", "src/**.hpp", "asset/**", "src/**.h" }
		includedirs { "src" }
		links {"Setupapi"}

		-- plugin
		minko.plugin.enable("sdl")
		minko.plugin.enable("bullet")
		--minko.plugin.enable("jpeg")
		--minko.plugin.enable("serializer")
		--minko.plugin.enable("particles")
		--minko.plugin.enable("png")
		minko.plugin.enable("html-overlay")
		minko.plugin.enable("assimp")
		minko.plugin.enable("jpeg")
		minko.plugin.enable("png")

		minko.package.assets {
		    ['**'] = { 'copy', 'embed' }
		}
