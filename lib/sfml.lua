SFML = {
	basepath = "sfml",

	OptFind = function (name, required)
		local check = function(option, settings)
			option.value = false
			option.lib_path = nil

			--[[if IsNegativeTerm(ScriptArgs[name .. ".use_pkgconfig"]) then
				option.use_pkgconfig = false
			elseif IsPositiveTerm(ScriptArgs[name .. ".use_pkgconfig"]) then
				option.use_pkgconfig = true
			end

			if family ~= "windows" and ExecuteSilent("pkg-config foolib") == 0 then
				option.value = true
				if option.use_pkgconfig == nil then
					option.use_pkgconfig = true
				end
			end

			if option.use_pkgconfig == nil then
				option.use_pkgconfig = false
			end]]

			if exists(SFML.basepath .. "/build/lib") then -- confirmation that SFML was compiled
				option.value = true
			end
		end

		local apply = function(option, settings)
			settings.cc.includes:Add(SFML.basepath .. "/include")
			settings.link.libpath:Add(SFML.basepath .. "/build/lib")

			settings.cc.defines:Add("SFML_STATIC")
			settings.link.libs:Add("sfml-graphics-s")
			settings.link.libs:Add("sfml-window-s")
			settings.link.libs:Add("sfml-system-s")
			settings.link.libs:Add("freetype")
		end

		local save = function(option, output)
			output:option(option, "value")
		end

		local display = function(option)
			if option.value == true then
				return "found"
			else
				if option.required then
					return "not found (required)"
				else
					return "not found (optional)"
				end
			end
		end

		local o = MakeOption(name, 0, check, save, display)
		o.Apply = apply
		o.include_path = nil
		o.lib_path = nil
		o.required = required
		return o
	end
}
