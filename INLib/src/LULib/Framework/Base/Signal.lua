-- Created by IntelliJ IDEA.
-- Date: 14/11/3
-- Time: 下午1:27
-- To change this template use File | Settings | File Templates.

local Signal = class("Signal")

function Signal:ctor()
    self.listeners = {}
end

function Signal:addToTable(theTable, func, scope)
    if func == nil then
        error("Function passed to signal:add() must not non-nil.")
    end

    for _, listener in ipairs(theTable) do
        if listener.func == func and listener.scope == scope then
            return false
        end
    end
    
    table.insert(theTable, {func=func,scope = scope})
end

function Signal:add(func, scope)
    self:addToTable(self.listeners, func, scope)   
end

function Signal:dispatch(...)
    --local arg = { ... }
    for _, listener in ipairs(self.listeners) do
        if listener.scope then
            listener.func(listener.scope, ...)
        else
            listener.func(...)
        end
    end
end

function Signal:remove(func, scope)
    for i, listener in ipairs(self.listeners) do
        if listener.func == func and listener.scope == scope then
            table.remove(self.listeners, i)
        end
    end
end

function Signal:removeByContext(scope)
    for i = #self.listeners,1,-1 do
        local listener = self.listeners[i]
        if listener.scope == scope then
            table.remove(self.listeners, i)
        end
    end
end

return Signal