---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

function table.isEmpty(tab)
    return _G.next(tab) == nil
end

-- 获取排序函数
--@attrName 属性名
--@isAscend 是否升序。默认true
function table.getSortFunction(attrName, isAscend)
    isAscend = isAscend ~= false and true or false

    return function(a1, a2)
        if a1[attrName] < a2[attrName] then
            return isAscend
        elseif a1[attrName] > a2[attrName] then
            return not isAscend
        else
            return false
        end
    end
end

function table.keys(hashtable)
    local keys = {}
    for k, v in pairs(hashtable) do
        keys[#keys + 1] = k
    end
    return keys
end

function table.values(hashtable)
    local values = {}
    for k, v in pairs(hashtable) do
        values[#values + 1] = v
    end
    return values
end

--- 仅用于数组
function table.indexOf(array, value, begin)
    for i = begin or 1, #array do
        if array[i] == value then return i end
    end
    return -1
end

--- 适用于字典（亦可用于数组，但不推荐适用。此时可用indexof
function table.hasValue(table, value)
    for key, item in pairs(table) do
        if item == value then
            return true
        end
    end

    return false
end

function table.insertTo(dest, src, begin)
    begin = checkInt(begin)
    if begin <= 0 then
        begin = #dest + 1
    end

    local len = #src
    for i = 0, len - 1 do
        dest[i + begin] = src[i + 1]
    end
end

function table.removeByValue(array, value, removeAll)
    local c, i, max = 0, 1, #array
    while i <= max do
        if array[i] == value then
            table.remove(array, i)
            c = c + 1
            i = i - 1
            max = max - 1
            if not removeAll then break end
        end
        i = i + 1
    end
    return c
end

function table.removeByKey(tab, key, removeAll)
    local c = 0
    if nil == tab then
        return
    end

    for k,v in pairs(tab) do
        if k == key then
            tab[k] = nil
            c = c + 1
            if not removeAll then break end
        end
    end

    return c
end

function table.removeValue(tab, value, removeAll)
    local c = 0
    if nil == tab then
        return
    end

    for k,v in pairs(tab) do
        if v == value then
            tab[k] = nil
            c = c + 1
            if not removeAll then break end
        end
    end

    return c
end

function table.nums(t)
    local count = 0
    for k, v in pairs(t) do
        count = count + 1
    end
    return count
end

function table.merge(dest, src)
    for k, v in pairs(src) do
        dest[k] = v
    end
end

function table.createWeakTable(mode)
    local result = {}
    setmetatable(result, {__mode = mode})
    return result
end

-- table copy
-- 浅拷贝
function table.shallowCopy(orig)
    local orig_type = type(orig)
    local copy
    if orig_type == 'table' then
        copy = {}
        for orig_key, orig_value in pairs(orig) do
            copy[orig_key] = orig_value
        end
    else -- number, string, boolean, etc
        copy = orig
    end

    return copy
end

function table.deepCopy(object)
    local lookup_table = {}
    local function _copy(object)
        if type(object) ~= "table" then
            return object
        elseif lookup_table[object] then
            return lookup_table[object]
        end
        local new_table = {}
        lookup_table[object] = new_table
        for index, value in pairs(object) do
            new_table[_copy(index)] = _copy(value)
        end
        return setmetatable(new_table, getmetatable(object))
    end
    return _copy(object)
end