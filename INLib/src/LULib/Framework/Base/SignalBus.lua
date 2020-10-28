-- Created by IntelliJ IDEA.
-- Date: 14/11/4
-- Time: 上午10:30
-- To change this template use File | Settings | File Templates.

local CURRENT_MODULE_NAME = ...
local SignalBus = class("SignalBus")

SignalBus.APP_RECOVER = "SignalBus.APP_RECOVER"

function SignalBus:ctor()
	self.signalMap = {}
    self._objEvents = {}
end

function SignalBus:getInstance()
    if self._instance == nil then
        self._instance = self:New()
    end

    return self._instance
end

function SignalBus:getSignal(type)
    if self.signalMap[type] == nil then
        self.signalMap[type] = import(".Signal", CURRENT_MODULE_NAME).New()
    end
    return self.signalMap[type]
end

function SignalBus:dispatch(type, ...)
    self:getSignal(type):dispatch(...)
end

function SignalBus:addContextListener(type, listener, listenerContext)
    if nil == type then
        error("addContextListener error: type is nil")
        return
    end
    if nil == listener then
        error("addContextListener error: listener is nil")
        return
    end
    if nil == listenerContext then
        error("addContextListener error: context is nil")
        return
    end

    self:getSignal(type):add(listener, listenerContext)
    local key = nil
    if listenerContext.type ~= nil then
        key = listenerContext.type
    else
        key = listenerContext
    end
    self._objEvents[key] = self._objEvents[key] or {}
    self._objEvents[key][type] = true
end

function SignalBus:delContextListener(type, listener, listenerContext)
    self:getSignal(type):remove(listener, listenerContext);
end

-- 使用该接口是 addContextListener 务必传入 listenerContext
function SignalBus:delContextListeners(listenerContext)
    local key = nil
    if listenerContext.type ~= nil then
        key = listenerContext.type
    else
        key = listenerContext
    end
    local objEvents = self._objEvents[key]
    if objEvents then
        for type, v in pairs(objEvents) do
            self:getSignal(type):removeByContext(listenerContext)
        end
        self._objEvents[key] = nil
    end
end

return SignalBus
