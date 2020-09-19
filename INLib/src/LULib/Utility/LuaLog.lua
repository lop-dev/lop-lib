---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

--IsDebug
local mLogDebug = nil
local function logDebug()
    if mLogDebug == nil then
        mLogDebug = CLog.IsDebug
    end

    return mLogDebug
end
--log关闭时使用字符串拼接会产生额外消耗，需在输出log前加入此判断
IsLogDebug = logDebug()

function error(str)
	if IsLogDebug then
		Debugger.LogError(string.format("%s\n%s", tostring(str), debug.traceback("", 2)));
	end
end

-- local _print = print
function warning(str)
    if IsLogDebug then
        Debugger.LogWarning(string.format("%s%s", tostring(str), debug.traceback("", 2)));
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
        Debugger.Log(string.format("<color=#%s>%s%s</color>%s", _color, tag, str, debug.traceback("", 2)))
    end
end

-- local _print = print
function print(str)
	if IsLogDebug then
		Debugger.Log(string.format("%s%s", tostring(str), debug.traceback("", 2)));
	end
end
