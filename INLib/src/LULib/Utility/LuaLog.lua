---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------
local DebugError, DebugWarning, DebugLog = Debugger.LogError, Debugger.LogWarning, Debugger.Log

--log关闭时使用字符串拼接会产生额外消耗，需在输出log前加入此判断
IsLogDebug = LOP.CLog.IsDebug

function error(str)
	if IsLogDebug then
		DebugError(string.format("%s\n%s", tostring(str), debug.traceback("", 2)));
	end
end

-- local _print = print
function warning(str)
    if IsLogDebug then
        DebugWarning(string.format("%s%s", tostring(str), debug.traceback("", 2)));
    end
end

-- local _log = log
-- tag 可以自己定一个自己的模块名字,方便筛选
function log(str, tag, _color)
    if IsLogDebug then
        if not _color then 
            _color = "008BDB" 
        end
        if not tag then 
            tag = "★★★ " 
        end
        DebugLog(string.format("<color=#%s>%s%s</color>%s", _color, tag, str, debug.traceback("", 2)))
    end
end

-- local _print = print
function print(str)
	if IsLogDebug then
		DebugLog(string.format("%s%s", tostring(str), debug.traceback("", 2)));
	end
end

--多参数log输出
function prints(...)
    if IsLogDebug then
        local params = {...}
        --need convert when params contain 'nil' value
        local tb = table.keys(params)
        for i = 1, tb[#tb] do
            tb[i] = tostring(params[i])
        end

        local str = table.concat(tb, " ")
        DebugLog(string.format("%s%s", tostring(str), debug.traceback("", 2)));
    end
end
