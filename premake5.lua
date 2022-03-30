workspace " 8-Puzzle-Solver" 
    architecture "x86_64" 
   
    configurations
    {
        "Debug",
        "Release"
    } 

project "8-Puzzle-Solver"  
    location "8-Puzzle-Solver"
    kind "ConsoleApp"   
    language "C++"   
    
    targetdir "bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" 
    objdir "bin/int/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" 

    files 
    {
        "**.h",
        "**.cpp"
    } 


    filter "configurations:Debug"
        defines { "DEBUG" }  
        symbols "On" 

    filter "configurations:Release"  
        defines { "NDEBUG" }    
        optimize "On" 