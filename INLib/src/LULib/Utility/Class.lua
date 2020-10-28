---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

function clone(object)
    local lookup_table = {}
    local function _copy(obj)
        if type(obj) ~= "table" then
            return obj
        elseif lookup_table[obj] then
            return lookup_table[obj]
        end
        local new_table = {}
        lookup_table[obj] = new_table
        for key, value in pairs(obj) do
            new_table[_copy(key)] = _copy(value)
        end
        return setmetatable(new_table, getmetatable(obj))
    end
    return _copy(object)
end

--Create an class.
function class(classname, super)
    local cls = {type = classname}
    cls.__cname = classname

    local superType = type(super)
    if superType ~= "function" and superType ~= "table" then
        superType = nil
        super = nil
    end

    -- inherited from Lua Object
    if super then
        -- cls = clone(super)
        cls.super = super
        setmetatable(cls, {__index = super})
    end

    cls.__index = cls
    if not cls.ctor then
        -- add default constructor
        cls.ctor = function() end
    end
    function cls.New(...)
        -- local instance = clone(cls)
        -- local create
        -- create = function(c, ...)
        --      if c.super then -- 递归向上调用create
        --           create(c.super, ...)
        --      end
        --      if c.ctor then
        --           c.ctor(instance, ...)
        --      end
        -- end
        -- create(instance, ...)

        local instance = setmetatable({}, cls)
        instance:ctor(...)
        instance.class = cls
        return instance
    end

    --引入行为树机制引入的问题，暂时没有想到好的解决方案，先用办法解决
    function cls.NewNode(...)
        local instance = cls.New()
        if instance.initialize then
            instance:initialize(...)
        end
        return instance
    end

    return cls
end

local isKindOf_
isKindOf_ = function(cls, name)
    local __index = rawget(cls, "__index")
    if type(__index) == "table" and rawget(__index, "type") == name then 
        return true
    end

    --    if rawget(cls, "type") == name then return true end
    if cls.type == name then
        return true
    end

    local super = rawget(cls.__index, "super")
    if not super then
        return false
    end
    if isKindOf_(super, name) then
        return true
    end
    return false
end
--判断类型
function isKindOf(obj, classname)
    local t = type(obj)
    if t ~= "table" and t ~= "userdata" then
        return false
    end

    local mt
    if t == "userdata" then -- not support temporary
        return false
    else
        mt = getmetatable(obj)
    end
    if mt then
        return isKindOf_(mt, classname)
    end
    return false
end

-- 调用父类方法（避免.:使用错误）
-- @param obj Table 调用者，一般为self
-- @param funcName String 函数名
-- @param ... 任意参数
function callSuperFunc(obj, funcName, ...)
    if not obj or type(funcName) ~= "string" then
        error("callSuperFunc the input params are invalid")
        return
    end

    local super = obj.super
    local result = nil
    if (super and super[funcName]) then
        result = super[funcName](obj, ...)
    else
        if IsLogDebug then
            error("super or super funcName " .. funcName .. "not exist")
        end
    end

    return result
end
