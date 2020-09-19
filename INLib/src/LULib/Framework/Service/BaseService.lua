---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

local SignalBus = require("LULib.Framework.Base.SignalBus")

--local BaseService = class("BaseService", import("..Base.Notifier"))
local BaseService = class("BaseService")

BaseService._enableMockData = false
BaseService._model           = nil

-- @params isAddModel 是否注册XxxModel.lua类。默认true
function BaseService:ctor(isAddModel)
    -- isAddModel = isAddModel ~= false and true or false
    self:onRegister()
end


-- 模拟服务器数据，有需要的service才写，只是在服务器没有准备好的时候，客户端用于开发调试
function BaseService:mockServer()
end

function BaseService:onRegister()
end

function BaseService:getModel()
    return self._model
end

-----------------------------------------------------------------------------------------------------------------
--
-- protected function
--
-----------------------------------------------------------------------------------------------------------------

function BaseService:dispatch(uniqueId, ...)
    SignalBus:getInstance():dispatch(uniqueId, ...)
end

function BaseService:addListeners()

end

-- function BaseService:delListeners()

-- end

function BaseService:delContextListeners(listenerContext)
	SignalBus:getInstance():delContextListeners(listenerContext)
end

function BaseService:addContextListener(uniqueId, callback, context)
    SignalBus:getInstance():addContextListener(uniqueId, callback, context)
end

function BaseService:delContextListener(uniqueId, callback, context)
    SignalBus:getInstance():delContextListener(uniqueId, callback, context)
end

-- 阻塞式网络发送接口，在没有收到请求的回报时，timeout时间内不能再次发送相同的协议，处理如购买，抽卡等需要保证安全的协议
function BaseService:sendMessageSafely(reqMsg, resMsg, timeout)
    return NetworkManager:getInstance():sendMessageSafely(self._systemName, reqMsg, resMsg, timeout)
end

function BaseService:processError(sys_pb, info, data)
    if IsLogDebug and data == nil then
        error("[BaseService]>> file: ".. info.short_src .. ", line: " .. info.currentline .. ", Error : " .. "data is nil")
    end
    local result = data.Result
    if result == 0 then
        -- 没有错误
        return
    end

    if result == message_pb.EEID_DBTASK_NO_RECORD then
        return
    end

    if result <= message_pb.EERROR_RANGE_VALUE_BEGIN then
        --  基础段错误
        local eID = self:getKeyByValue(message_pb, result)
        if IsLogDebug then
            error("[BaseService]>> file: ".. info.short_src .. ", line: " .. info.currentline  .. ", ErrorID : " ..tostring(eID))
        end
    else
        -- 系统段错误
        local eID = self:getKeyByValue(sys_pb, result)
        if IsLogDebug then
            error("[BaseService]>> file: ".. info.short_src .. ", line: " .. info.currentline .. ", ErrorID : " .. tostring(eID))
        end
    end
end

-- 获取相应的错误码，注意value的重复，只取EEID_开头的
function BaseService:getKeyByValue(table, value)
    for k, v in pairs(table) do
        if value == v and string.find(k, "EEID_") then
            return k
        end
    end

    return nil
end

return BaseService
