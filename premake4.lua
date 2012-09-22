solution "RayDungeon"
    configurations { "debug", "release" }
    location "build"

    project "RayDungeon"
        kind "WindowedApp"
        language "C++"
        includedirs { "include" }
        files { "src/*.cpp" }
        links { "SDL" }

        configuration "debug"
            defines { "DEBUG" }
            flags { "Symbols", "ExtraWarnings" }

        configuration "release"
            defines { "NDEBUG" }
            flags { "OptimizeSpeed", "ExtraWarnings", "FatalWarnings" }
