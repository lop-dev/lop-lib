---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

PTMap1 = class("PTMap1")

function PTMap1:ctor()
    self.m_hashMap = {}
end

function PTMap1:addObjectUsePE(key1, peList)
    if key1 == nil then
        error("key1 == nil")
        return false
    end
    if peList == nil then
        error("peList == nil")
        return false
    end
    if self.m_hashMap[key1] == nil then
        self.m_hashMap[key1] = peList
        return true
    end
    
    local mapPeList = self.m_hashMap[key1]
    local size = peList:getSize()
    local tmpObj = nil
    for i = 1, size do
        tmpObj = peList:getObjectByIndex(i)
        if tmpObj and mapPeList:getObject(tmpObj:getBuf()) == nil then
            mapPeList:addObject(tmpObj:getBuf())
        end
    end
    return true
end

function PTMap1:setObjectUsePE(key1, peList)
    if key1 == nil then
        error("key1 == nil")
        return false
    end
    if peList == nil then
        error("peList == nil")
        return false
    end
    self.m_hashMap[key1] = peList
    return true
end

function PTMap1:getObject(key1)
    if key1 == nil then
        error("key1 == nil")
        return nil
    end
    return self.m_hashMap[key1]
end

function PTMap1:delObject(key1)
    if key1 == nil then
        error("key1 == nil")
        return false
    end
    if self.m_hashMap[key1] == nil then
        warning("self.m_hashMap[key1] == nil")
        return false
    end
    self.m_hashMap[key1] = nil
    -- table.remove(self.m_hashMap, key1)
    return true
end

function PTMap1:dumpInfo(key1)
    if key1 == nil then
        error("key1 == nil")
        return false
    end
    if self.m_hashMap[key1] == nil then
        warning("self.m_hashMap[key1] == nil")
        return false
    end
    self.m_hashMap[key1]:dumpInfo()
    return true
end

function PTMap1:getMap()
    return self.m_hashMap
end

function PTMap1:clrMap()
    self.m_hashMap = {}
end

--------------------------------------------------------------------------

PTMap2 = class("PTMap2")

function PTMap2:ctor()
    self.m_hashMap = {}
end

function PTMap2:addObjectUsePE(key1, key2, peList)
    if key1 == nil then
        error("key1 == nil")
        return false
    end
    if key2 == nil then
        error("key2 == nil")
        return false
    end
    if peList == nil then
        error("peList == nil")
        return false
    end
    if self.m_hashMap[key1] == nil then
        self.m_hashMap[key1] = self.m_hashMap[key1] or {}
        self.m_hashMap[key1][key2] = peList
        return true
    elseif self.m_hashMap[key1] ~= nil and self.m_hashMap[key1][key2] == nil then
        self.m_hashMap[key1][key2] = peList
        return true
    end
    
    local mapPeList = self.m_hashMap[key1][key2]
    local size = peList:getSize()
    local tmpObj = nil
    for i = 1, size do
        tmpObj = peList:getObjectByIndex(i)
        if tmpObj and mapPeList:getObject(tmpObj:getBuf()) == nil then
            mapPeList:addObject(tmpObj:getBuf())
        end
    end
    return true
end

function PTMap2:setObjectUsePE(key1, key2, peList)
    if key1 == nil then
        error("key1 == nil")
        return false
    end
    if key2 == nil then
        error("key2 == nil")
        return false
    end
    if peList == nil then
        error("peList == nil")
        return false
    end
    self.m_hashMap[key1] = self.m_hashMap[key1] or {}
    self.m_hashMap[key1][key2] = peList
    return true
end

function PTMap2:getObject(key1, key2)
    if key1 == nil then
        error("key1 == nil")
        return nil
    end
    if key2 == nil then
        return self.m_hashMap[key1]
    end
    if self.m_hashMap[key1] == nil then
        error("self.m_hashMap[key1] == nil")
        return nil
    end
    return self.m_hashMap[key1][key2]
end

function PTMap2:delObject(key1, key2)
    if key1 == nil then
        error("key1 == nil")
        return false
    end
    if key2 == nil then
        if self.m_hashMap[key1] == nil then
            warning("self.m_hashMap[key1] == nil")
            return false
        else
            self.m_hashMap[key1] = nil
            return true
        end
    end
    if self.m_hashMap[key1] == nil then
        warning("self.m_hashMap[key1] == nil")
        return false
    end
    if self.m_hashMap[key1][key2] == nil then
        warning("self.m_hashMap[key1][key2] == nil")
        return false
    end
    self.m_hashMap[key1][key2] = nil
    -- table.remove(self.m_hashMap[key1], key2)
    return true
end

function PTMap2:dumpInfo(key1, key2)
    if key1 == nil then
        error("key1 == nil")
        return false
    end
    if key2 == nil then
        error("key2 == nil")
        return false
    end
    if self.m_hashMap[key1] == nil then
        warning("self.m_hashMap[key1] == nil")
        return false
    end
    if self.m_hashMap[key1][key2] == nil then
        warning("self.m_hashMap[key1][key2] == nil")
        return false
    end
    self.m_hashMap[key1][key2]:dumpInfo()
    return true
end

function PTMap2:getMap()
    return self.m_hashMap
end

function PTMap2:clrMap()
    self.m_hashMap = {}
end
