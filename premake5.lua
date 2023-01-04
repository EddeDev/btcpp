workspace "btcpp"
    configurations { "Debug", "Release", "Shipping" }
    architecture "x64"
    flags "MultiProcessorCompile"
    startproject "btcpp"

    targetdir ("build/bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("build/obj/%{cfg.buildcfg}/%{prj.name}")

VulkanSDKPath = os.getenv("VULKAN_SDK")

project "btcpp"
    language "C++"
    cppdialect "C++latest"
    location ""
    staticruntime "Off"

    pchheader "btcppPCH.h"
    pchsource "src/btcppPCH.cpp"

    files
    {
        "src/**.cpp",
        "src/**.h"
    }

    includedirs
    {
        "src",
        "thirdparty/spdlog/include",
        "thirdparty/json/include"
    }

    filter "system:windows"
        systemversion "latest"
        defines "BTCPP_PLATFORM_WINDOWS"

        defines
        {
            "BTCPP_PLATFORM_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS"
        }

        links
        {
            "Winmm.lib",
            "Ws2_32.lib",
            "Mswsock.lib",
            "AdvApi32.lib",
            "Version.lib",
            "Bcrypt.lib",
            "Urlmon.lib"
        }
        
    filter "system:macosx"
        systemversion "latest"
        defines "BTCPP_PLATFORM_MAC"

    filter "system:linux"
        systemversion "latest"
        defines "BTCPP_PLATFORM_LINUX"

    filter "configurations:Debug"
        kind "ConsoleApp"
        defines "BTCPP_BUILD_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        kind "ConsoleApp"
        defines { "BTCPP_BUILD_RELEASE", "NDEBUG" }
        runtime "Release"
        optimize "On"
    
    filter "configurations:Shipping"
        kind "ConsoleApp"
        defines { "BTCPP_BUILD_SHIPPING", "NDEBUG" }
        runtime "Release"
        optimize "On"
        symbols "Off"