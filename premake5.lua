project "Converter"
    kind "StaticLib"
    language "C"
    
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "Convert.cpp",
        "Convert.h",
		"deps/glm/**.hpp",
		"deps/glm/**.inl",
    }
	
	includedirs
	{
		"deps/Include"
	}

	libdirs 
	{ 
		"deps/Lib"
	}
	
	links 
	{ 
		"assimp-vc140-mt.lib"
	}
    
	filter "system:windows"
        systemversion "latest"
        staticruntime "On"
        
    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"
		
