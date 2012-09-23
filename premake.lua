-- These are only used on Windows + Visual Studio ------------------------------
ASSIMP_ROOT          = "/dev/assimp"
--------------------------------------------------------------------------------
-- For Mac OSX having installed assimp via Homebrew ----------------------------
ASSIMP_OSX_INCLUDE_DIR = "/usr/local/Cellar/assimp/2.0.863/include"
ASSIMP_OSX_LIB_DIR     = "/usr/local/Cellar/assimp/2.0.863/lib"
--------------------------------------------------------------------------------

BUILD_DIR         = "build"

if _ACTION == "clean" then
	os.rmdir(BUILD_DIR)
end

solution "AssimpToMesh"
	configurations { "Debug", "Release" }
	location (BUILD_DIR .. "/" .. _ACTION)
	
project "AssimpToMesh"
	kind "ConsoleApp"
	language "C++"
	location (BUILD_DIR .. "/" .. _ACTION)
	files {
		"./src/**.c*",
		"./src/**.h",
	}
	debugdir "."
	
	---- PLATFORM SPECIFICS ----------------------------------------------------
	configuration "vs*"
		flags {
			"NoPCH",
			"NoMinimalRebuild"
		}
		buildoptions { "/MP" }
		links {
			"assimp",
		}
		defines {
			"_CRT_SECURE_NO_WARNINGS",
			"_CRT_NONSTDC_NO_WARNINGS"
		}
		includedirs {
			(ASSIMP_ROOT .. "/include")
		}
		
	configuration { "vs*", "Debug" }
		libdirs {
			(ASSIMP_ROOT .. "/lib/assimp_debug-dll_win32"),
		}
		
	configuration { "vs*", "Release" }
		libdirs {
			(ASSIMP_ROOT .. "/lib/assimp_debug-dll_win32"),
		}
		
	configuration "gmake"
		kind "ConsoleApp"
		buildoptions { "-Wall" }
		
	configuration { "windows", "gmake" }
		kind "ConsoleApp"
		defines {
			"_GNU_SOURCE=1",
		}
		links {
			"mingw32",
			"assimp",
		}
		linkoptions {
			"-static-libgcc",
			"-static-libstdc++",
		}
		
	configuration "macosx"
		links {
			"assimp",
		}
		includedirs {
			ASSIMP_OSX_INCLUDE_DIR,
		}
		libdirs {
			ASSIMP_OSX_LIB_DIR,
		}
		
	configuration "linux"
		links {
			"assimp",
		}
	----------------------------------------------------------------------------
			
	configuration "Debug"
		defines {
			"DEBUG",
			"DEBUG_ASSERT_BREAK",
		}
		flags { "Symbols" }
	
	configuration "Release"
		defines {
			"NDEBUG",
		}
		flags { "Optimize" }
