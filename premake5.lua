-- premake5.lua
workspace "rayTracing"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "rayTracing"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

include "rayTracing"