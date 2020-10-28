---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

--判断平台
local isAndroid = nil
function isAndroidPlatform()
    if isAndroid == nil then
        isAndroid = (Application.platform == RuntimePlatform.Android)
    end
    return isAndroid
end

local isIphone = nil
function isIPhonePlatform()
    if isIphone == nil then
        isIphone = (Application.platform == RuntimePlatform.IPhonePlayer)
    end
    return isIphone
end

function LuaGC()
    local c = collectgarbage("count")
    --warning(string.format("Begin gc count = {%d} kb", c))
    collectgarbage("collect")
    c = collectgarbage("count")
    --warning(string.format("End gc count = {%d} kb", c))
end

function PrintLuaGarbageInKB()
    local c = collectgarbage("count")
    return c
end

-- 判断unity对象是否为空
function isNil(uObj)
    return uObj == nil or uObj:Equals(nil)
end

function handler(obj, method)
    return function(...)
        return method(obj, ...)
    end
end

function checkNumber(value, base)
    return tonumber(value, base) or 0
end

function checkInt(value)
    return math.round(checkNumber(value))
end

function checkBool(value)
    return (value ~= nil and value ~= false)
end

local function dump_value_(v)
    if type(v) == "string" then
        v = "\"" .. v .. "\""
    end
    return tostring(v)
end

-- 打印出table详细信息
function dump(value, description, nesting)
    if IsLogDebug then
        return
    end
    if type(nesting) ~= "number" then nesting = 3 end

    local lookupTable = {}
    local result = {}

    local traceback = string.split(debug.traceback("", 2), "\n")
    -- print("dump from: " .. string.trim(traceback[3]))
    result[#result +1] = "dump from: " .. string.trim(traceback[3])

    local function dump_(value, description, indent, nest, keylen)
        description = description or "<var>"
        local spc = ""
        if type(keylen) == "number" then
            spc = string.rep(" ", keylen - string.len(dump_value_(description)))
        end
        if type(value) ~= "table" then
            result[#result +1 ] = string.format("%s%s%s = %s", indent, dump_value_(description), spc, dump_value_(value))
        elseif lookupTable[tostring(value)] then
            result[#result +1 ] = string.format("%s%s%s = *REF*", indent, dump_value_(description), spc)
        else
            lookupTable[tostring(value) or "nil"] = true
            if nest > nesting then
                result[#result +1 ] = string.format("%s%s = *MAX NESTING*", indent, dump_value_(description))
            else
                result[#result +1 ] = string.format("%s%s = {", indent, dump_value_(description))
                local indent2 = indent.."    "
                local keys = {}
                local keylength = 0
                local values = {}
                for k, v in pairs(value) do
                    keys[#keys + 1] = k
                    local vk = dump_value_(k)
                    local vkl = string.len(vk)
                    if vkl > keylength then keylength = vkl end
                    values[k] = v
                end
                table.sort(keys, function(a, b)
                    if type(a) == "number" and type(b) == "number" then
                        return a < b
                    else
                        return tostring(a) < tostring(b)
                    end
                end)
                for _, k in ipairs(keys) do
                    dump_(values[k], k, indent2, nest + 1, keylength)
                end
                result[#result +1] = string.format("%s}", indent)
            end
        end
    end

    dump_(value, description, "- ", 1)
    print(table.concat(result, "\n"))
end


