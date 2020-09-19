---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

local string = string

--将字符串中特定符号替换为相应args中的类容
--@function [parent=#string] substitute
--@param msg string 原字符串
--@param args table 参数列表
function string.substitute(msg , ...)
    local tArgs = {...}
    if #tArgs < 1 then
        return msg
    end

    if type(tArgs[1]) == "table" then tArgs = tArgs[1] end

    for i , item in ipairs(tArgs) do
        msg = string.gsub(msg , "{".. (i-1) .."}" , item)
    end

    return msg
end

function string.trim(input)
    input = string.gsub(input, "^[ \t\n\r]+", "")
    return string.gsub(input, "[ \t\n\r]+$", "")
end

---filterSpecificChars 过滤掉除中文英文数字下划线意外的其他字符
---@param srcChars string
function string.filterSpecificChars(srcChars)
    if not srcChars or type(srcChars) ~= "string" then
        return nil
    end

    local resultChars = {}
    for k = 1, #srcChars do
        local c = string.byte(srcChars, k)
        if not c then break end
        if c == 95 or (c>=48 and c<=57) or (c>= 65 and c<=90) or (c>=97 and c<=122) then
            table.insert(resultChars, string.char(c))
        elseif c>=228 and c<=233 then
            local c1 = string.byte(srcChars, k+1)
            local c2 = string.byte(srcChars, k+2)
            if c1 and c2 then
                local c1Min, c1Max = 128, 191
                local c2Min, c2Max = 128, 191
                if c == 228 then c1Min = 184
                elseif c == 233 then c1Max, c2Max = 190, c1 ~= 190 and 191 or 165
                end
                if c1>=c1Min and c1<=c1Max and c2>=c2Min and c2<=c2Max then
                    table.insert(resultChars, string.char(c, c1, c2))
                    k = k + 2
                end
            end
        end
    end
    return table.concat(resultChars)
end

function string.split(input, delimiter)
    input = tostring(input)
    delimiter = tostring(delimiter)
    if (delimiter=='') then return false end
    local pos,arr = 0, {}
    for st,sp in function() return string.find(input, delimiter, pos, true) end do
        table.insert(arr, string.sub(input, pos, st - 1))
        pos = sp + 1
    end
    table.insert(arr, string.sub(input, pos))
    return arr
end

function string.starts(String,Start)
    return string.sub(String,1,string.len(Start))==Start
end

function string.ends(String,End)
    return End=='' or string.sub(String,-string.len(End))==End
end

function string.tolstring(str)
    return tolua.tolstring(str)
end

--去除首尾空格
function string.trim(str)
    return (string.gsub(str, "^%s*(.-)%s*$", "%1"))
end

--是否空字符串
function string.isEmpty(str)
    if str == nil then
        return true
    end

    if #str == 0 then
        return true
    end

    return false
end

--是否有效字符串
function string.isValid(str)
    if nil == str then
        return false
    end

    if string:trim(str) == "" then
        return false
    end

    return true
end