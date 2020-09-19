---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

local ServiceManager = {}

ServiceManager._cachedService = {}
ServiceManager._cachedModel = {}

function ServiceManager:registerService(sService)
    if not sService or not sService.type then 
        error("function 'ServiceManager.registerService' error!") 
    end

    self._cachedService[sService.type] = sService

    local tModel = sService:getModel()
    if tModel then self._cachedModel[tModel.type] = tModel end
end

-- 重置所有模块数据
function ServiceManager:resetAllModelData()
    for _, item in pairs(self._cachedModel) do
        if item.resetData then
            item:resetData()
        else
            if IsLogDebug then
                error("[ServiceManager]>> " .. item.type .. " has no function resetData")
            end
        end
    end
end

--- 获取数据模块
function ServiceManager:getService(sName)
    local result = self._cachedService[sName]
    if not result then 
        if IsLogDebug then
            error("[ServiceManager]>> getService error! name: " .. tostring(sName)) 
        end
    end

    return result
end

--- 获取数据模块
-- @params sName
function ServiceManager:getModel(sName)
    local result = self._cachedModel[sName]
    if not result then 
        if IsLogDebug then
            error("[ServiceManager]>> getModel error! name: " .. tostring(sName))
        end
    end

    return result
end

return ServiceManager
