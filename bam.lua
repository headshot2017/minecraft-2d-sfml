target_family = os.getenv("TARGET_FAMILY")
if target_family then
	family = target_family
end
target_platform = os.getenv("TARGET_PLATFORM")
if target_platform then
	platform = target_platform
end
target_arch = os.getenv("TARGET_ARCH")
if target_arch then
	arch = target_arch
end

function ResCompile(scriptfile)
	windres = os.getenv("WINDRES")
	if not windres then
		windres = "windres"
	end

	scriptfile = Path(scriptfile)
	if config.compiler.driver == "cl" then
		output = "objs/" .. PathBase(scriptfile) .. ".res"
		AddJob(output, "rc " .. scriptfile, "rc /fo " .. output .. " " .. scriptfile)
	elseif config.compiler.driver == "gcc" then
		output = "objs/" .. PathBase(scriptfile) .. ".coff"
		AddJob(output, windres .. " " .. scriptfile, windres .. " -i " .. scriptfile .. " -o " .. output)
	end

	AddDependency(output, scriptfile)
	return output
end

function CopyGameData()
	input = Path("data")
	if family == "windows" then
		output = Path("out\\data")
		AddJob(output, "copy "..input, "xcopy /E /I /Y "..input.." "..output)
	else
		output = Path("out/data")
		AddJob(output, "copy "..input, "cp -r "..input.." "..output)
	end

	AddDependency(output, input)
	return output
end

function CopyGameDLL()
	if family == "windows" then
		input = Path("lib\\windows\\bass.dll")
		output = Path("out\\bass.dll")
		AddJob(output, "copy "..Path(output), "copy /Y "..input.." "..output)
	else
		if platform == "linux" then
			input = Path("lib/linux/libbass.so")
			output = Path("out/libbass.so")
		elseif platform == "macosx" then
			input = Path("lib/linux/libbass.dylib")
			output = Path("out/libbass.dylib")
		end
		AddJob(output, "copy "..Path(output), "cp "..input.." "..output)
	end

	AddDependency(output, input)
	return output
end

function Intermediate_Output(settings, input)
	return "objs/" .. string.sub(PathBase(input), string.len("./")-1) .. settings.config_ext
end

function exists(file)
	local ok, err, code = os.rename(file, file)
	if not ok then
		if code == 13 then -- permission denied but it exists
			return true
		end
	end
	return ok, err
end

Import("configure.lua")
Import("lib/sfml.lua")

config = NewConfig()
config:Add(OptCCompiler("compiler"))
config:Add(OptTestCompileC("stackprotector", "int main(){return 0;}", "-fstack-protector -fstack-protector-all"))
config:Add(OptTestCompileC("minmacosxsdk", "int main(){return 0;}", "-mmacosx-version-min=10.7 -isysroot /Developer/SDKs/MacOSX10.7.sdk"))
config:Add(OptTestCompileC("macosxppc", "int main(){return 0;}", "-arch ppc"))
config:Add(SFML.OptFind("sfml", true))
config:Finalize("config.lua")


settings = NewSettings()
settings.cc.Output = Intermediate_Output

-- apply compiler settings
config.compiler:Apply(settings)
config.sfml:Apply(settings)
settings.link.flags:Add("-static")
settings.link.flags:Add("-static-libgcc")
settings.link.flags:Add("-static-libstdc++")
settings.link.libs:Add("bass")

if family == "windows" then
	settings.link.libpath:Add("lib/windows")
	settings.link.libs:Add("opengl32")
	settings.link.libs:Add("gdi32")
	settings.link.libs:Add("winmm")

elseif family == "unix" then
	settings.link.libpath:Add("lib/linux")
	settings.link.flags:Add("-Wl,-rpath,.")
	settings.link.libs:Add("dl")
	settings.link.libs:Add("pthread")
	settings.link.libs:Add("udev")
	settings.link.libs:Add("X11")
	settings.link.libs:Add("Xrandr")
	settings.link.libs:Add("Xcursor")

end

src = Collect(
	"*.cpp",
	"gui/*.cpp",
	"submenus/*.cpp",
	"game/*.cpp",
	"game/entities/*.cpp",
	"game/entities/particles/*.cpp"
)

objs = Compile(settings, src)
if family == "windows" then
	icon_rc = {ResCompile("icon.rc")}
end

gamedata = CopyGameData()
gamedll = CopyGameDLL()

exe = Link(settings, "out/minecrap.exe", objs, icon_rc)
